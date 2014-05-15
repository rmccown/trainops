// IndustryUsageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "IndustryUsageDialog.h"

#include "julian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryUsageDialog dialog


CIndustryUsageDialog::CIndustryUsageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustryUsageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryUsageDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndustryUsageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryUsageDialog)
	DDX_Control(pDX, IDC_INDUSTRYTREE, m_ctlIndustryTree);
	DDX_Control(pDX, IDC_INDUSTRYDETAIL, m_ctlIndustryDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryUsageDialog, CDialog)
	//{{AFX_MSG_MAP(CIndustryUsageDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_INDUSTRYTREE, OnSelchangedIndustrytree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryUsageDialog message handlers

BOOL CIndustryUsageDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ctlIndustryDetail.InsertColumn(0,"Date");
    m_ctlIndustryDetail.InsertColumn(1,"Train #");
    m_ctlIndustryDetail.InsertColumn(2,"E/L");
    m_ctlIndustryDetail.InsertColumn(3,"Car Type");
    m_ctlIndustryDetail.InsertColumn(4,"Car #");
    m_ctlIndustryDetail.InsertColumn(5,"To");
    m_ctlIndustryDetail.InsertColumn(6,"From");
    
    m_ctlIndustryDetail.SetColumnWidth(0,70);
    m_ctlIndustryDetail.SetColumnWidth(1,50);
    m_ctlIndustryDetail.SetColumnWidth(2,40);
    m_ctlIndustryDetail.SetColumnWidth(3,70);
    m_ctlIndustryDetail.SetColumnWidth(4,70);
    m_ctlIndustryDetail.SetColumnWidth(5,120);
    m_ctlIndustryDetail.SetColumnWidth(6,120);

    ReadDB();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIndustryUsageDialog::ReadDB() 
{
    //  read the database into tree
    m_ctlIndustryTree.DeleteAllItems();

    HTREEITEM hRoot = m_ctlIndustryTree.InsertItem("Industries");
    //
    //  Read locations
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Industries;");
    //
    while (!q.eof())
    {
        //  SELECT COUNT(*) AS inbound FROM Trains_Scheduled_Cars WHERE Industry_to_FK=26;
        //  SELECT COUNT(*) AS outbound FROM Trains_Scheduled_Cars WHERE Industry_from_FK=26;
        CString sSQL;
        sSQL.Format("SELECT COUNT(*) AS inbound FROM Trains_Scheduled_Cars WHERE Industry_to_FK=%d;",q.getIntField("id"));
        int iInbound(0),iOutbound(0);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
            iInbound = q2.getIntField("inbound");
        q2.finalize();
        sSQL.Format("SELECT COUNT(*) AS outbound FROM Trains_Scheduled_Cars WHERE Industry_from_FK=%d;",q.getIntField("id"));
        q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
            iOutbound = q2.getIntField("outbound");
        q2.finalize();
        //
        CString sText;
        sText.Format("%s (In: %d / Out: %d)",q.getStringField("name"),iInbound,iOutbound);
        HTREEITEM pIndustry = m_ctlIndustryTree.InsertItem(sText,hRoot);
        m_ctlIndustryTree.SetItemData(pIndustry,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlIndustryTree.Expand(hRoot,TVE_EXPAND);
    //
    m_ctlIndustryTree.EnsureVisible(hRoot);

    TBTreeStateMgr::LoadTreeState(_T("IndustryUsage"),&m_ctlIndustryTree);
	m_ctlIndustryDetail.RestoreState("State","IndustryUsageDialog_m_ctlIndustryDetail");
}


void CIndustryUsageDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("IndustryUsage"),&m_ctlIndustryTree);    
	
	m_ctlIndustryDetail.SaveState("State","IndustryUsageDialog_m_ctlIndustryDetail");

	CDialog::OnOK();
}

void CIndustryUsageDialog::OnSelchangedIndustrytree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    //  Get info for this industry
    m_ctlIndustryDetail.DeleteAllItems();
    //  DATE Train # Load/Empty Cartype Car # From or To Location
    HTREEITEM hItem = TreeView_GetSelection(m_ctlIndustryTree);
    
    if( m_ctlIndustryTree.GetItemData(hItem) == -1 )
        return;
	
    GetCars(m_ctlIndustryTree.GetItemData(hItem));
	
	*pResult = 0;
}

void CIndustryUsageDialog::GetCars(int iIndustryFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    CString sSQL;
    sSQL.Format("SELECT * FROM Trains_Scheduled_Cars WHERE Industry_to_FK=%d OR Industry_from_FK=%d ORDER BY Trains_Scheduled_FK DESC;",iIndustryFK,iIndustryFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while( !q.eof() )
    {
        sSQL.Format("SELECT run_date,trainid FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.id=%d AND Trains.id=Trains_Scheduled.Trains_FK",q.getIntField("Trains_Scheduled_FK"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            CppSQLite3Query q2b = pDB->execQuery("SELECT rrdate FROM _Config WHERE id=1;");
            if( !q2b.eof() )
            {
                int y,m,d;
                JulianDate::JdToYmd( q2b.getIntField("rrdate")+q2.getIntField("run_date"), &y,&m,&d);
                CString sDate;
                sDate.Format("%02d/%02d/%4d",m,d,y);
                int nIndex = m_ctlIndustryDetail.InsertItem(m_ctlIndustryDetail.GetItemCount(),sDate);
                //
                //  get car info
                //
                sSQL.Format("SELECT reporting_marks, car_number, description FROM Cars,CarTypes WHERE Cars.id=%d AND Cars.CarTypes_FK=CarTypes.id;",q.getIntField("Cars_FK"));
                CppSQLite3Query q2c = pDB->execQuery(sSQL);
                CString sCarInfo("");
                CString sCarType("");
                if( !q2c.eof() )
                {
                    sCarInfo.Format("%s %s",q2c.getStringField("reporting_marks"),q2c.getStringField("car_number"));
                    sCarType = q2c.getStringField("description");
                }
                q2c.finalize();
                //
                //  check location/siding/industry FROM
                //
                CString sFrom(""),sTo("");
                if( iIndustryFK != q.getIntField("Industry_from_FK") )
                {
                    sSQL.Format("SELECT name FROM Locations WHERE id=%d;",q.getIntField("Location_from_FK"));
                    q2c = pDB->execQuery(sSQL);
                    if( !q2c.eof() )
                    {
                        sFrom = q2c.getStringField("name");
                    }
                    q2c.finalize();
                    //
                    if( q.getIntField("Industry_from_FK") == -1 )
                        sSQL.Format("SELECT name ||' Siding' AS name FROM Sidings WHERE id=%d;",q.getIntField("Siding_from_FK"));
                    else
                        sSQL.Format("SELECT name FROM Industries WHERE id=%d;",q.getIntField("Industry_from_FK"));
                    q2c = pDB->execQuery(sSQL);
                    if( !q2c.eof() )
                    {
                        CString sWork;
                        if( sFrom.IsEmpty() )
                            sFrom = q2c.getStringField("name");
                        else
                        {
                            sWork.Format("%s / %s",sFrom,q2c.getStringField("name"));
                            sFrom = sWork;
                        }
                    }
                    q2c.finalize();
                }
                //
                if( iIndustryFK != q.getIntField("Industry_to_FK") )
                {
                    sSQL.Format("SELECT name FROM Locations WHERE id=%d;",q.getIntField("Location_to_FK"));
                    q2c = pDB->execQuery(sSQL);
                    if( !q2c.eof() )
                    {
                        sTo = q2c.getStringField("name");
                    }
                    q2c.finalize();
                    //
                    if( q.getIntField("Industry_to_FK") == -1 )
                        sSQL.Format("SELECT name ||' Siding' AS name FROM Sidings WHERE id=%d;",q.getIntField("Siding_to_FK"));
                    else
                        sSQL.Format("SELECT name FROM Industries WHERE id=%d;",q.getIntField("Industry_to_FK"));
                    q2c = pDB->execQuery(sSQL);
                    if( !q2c.eof() )
                    {
                        CString sWork;
                        if( sTo.IsEmpty() )
                            sTo = q2c.getStringField("name");
                        else
                        {
                            sWork.Format("%s / %s",sTo,q2c.getStringField("name"));
                            sTo = sWork;
                        }
                    }
                    q2c.finalize();
                }
                //
                m_ctlIndustryDetail.SetItemText(nIndex,1,q2.getStringField("trainid"));
                m_ctlIndustryDetail.SetItemText(nIndex,2,q.getIntField("loadempty")==1?"E":"L");
                m_ctlIndustryDetail.SetItemText(nIndex,3,sCarType);
                m_ctlIndustryDetail.SetItemText(nIndex,4,sCarInfo);
                m_ctlIndustryDetail.SetItemText(nIndex,5,sTo);
                m_ctlIndustryDetail.SetItemText(nIndex,6,sFrom);
            }
            q2b.finalize();
//        m_ctlCarData.SetItemText(nIndex,1,q.getStringField("reporting_marks"));
//        m_ctlCarData.SetItemText(nIndex,2,GetCarType(q.getIntField("CarTypes_FK")));
        //
        }
        q2.finalize();
        q.nextRow();
    }
    q.finalize();
}
