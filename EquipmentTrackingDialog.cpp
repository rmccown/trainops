// EquipmentTrackingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "EquipmentTrackingDialog.h"

#include "julian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipmentTrackingDialog dialog


CEquipmentTrackingDialog::CEquipmentTrackingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEquipmentTrackingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipmentTrackingDialog)
	//}}AFX_DATA_INIT
}


void CEquipmentTrackingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipmentTrackingDialog)
	DDX_Control(pDX, IDC_EQUIPMENTTREE, m_ctlEquipmentTree);
	DDX_Control(pDX, IDC_CARDATA, m_ctlCarData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquipmentTrackingDialog, CDialog)
	//{{AFX_MSG_MAP(CEquipmentTrackingDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_EQUIPMENTTREE, OnSelchangedEquipmenttree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipmentTrackingDialog message handlers

BOOL CEquipmentTrackingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_ctlCarData.InsertColumn(0,"Date");
    m_ctlCarData.InsertColumn(1,"Starting Location");
    m_ctlCarData.InsertColumn(2,"Ending Location");
    m_ctlCarData.InsertColumn(3,"E/L");
    m_ctlCarData.InsertColumn(4,"Train");
    m_ctlCarData.InsertColumn(5,"Off-Layout");
    
    m_ctlCarData.SetColumnWidth(0,70);
    m_ctlCarData.SetColumnWidth(1,120);
    m_ctlCarData.SetColumnWidth(2,120);
    m_ctlCarData.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarData.SetColumnWidth(4,45);
    m_ctlCarData.SetColumnWidth(5,70);

    ReadDB();

	m_ctlCarData.RestoreState("State","EquipmentTraclomgDialog_m_ctlCarData");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEquipmentTrackingDialog::ReadDB() 
{
    //  read the database into tree
    m_ctlEquipmentTree.DeleteAllItems();

    HTREEITEM hLocationRoot = m_ctlEquipmentTree.InsertItem("Cars");
    //
    //  Read all cars
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,description,type_id FROM CarTypes WHERE active=1 ORDER BY description;");
    //
    while (!q.eof())
    {
        CString sCarType;
        sCarType.Format("%s %s",q.getStringField("description"),q.getStringField("type_id"));
        HTREEITEM pCarType = m_ctlEquipmentTree.InsertItem(sCarType,hLocationRoot);
        m_ctlEquipmentTree.SetItemData(pCarType,-1);
        //
        //  get cars of this type
        //
        CString sSQL;
        sSQL.Format("SELECT * FROM Cars WHERE Cars.CarTypes_FK=%d ORDER BY car_number;",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while( !q2.eof() )
        {
            CString sCar;
            CString sOOS("");
            if( q2.getIntField("out_of_service") == 1 )
                sOOS = "(Out of Service)";
            sCar.Format("%s %s %s",q2.getStringField("reporting_marks"),q2.getStringField("car_number"),sOOS);
            HTREEITEM pCar = m_ctlEquipmentTree.InsertItem(sCar,pCarType);
            if( !sOOS.IsEmpty() )
                m_ctlEquipmentTree.SetItemColor(pCar,RGB(255,0,0));
            m_ctlEquipmentTree.SetItemData(pCar,q2.getIntField("id"));
            q2.nextRow();
        }
        q2.finalize();
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlEquipmentTree.Expand(hLocationRoot,TVE_EXPAND);
    //
    m_ctlEquipmentTree.EnsureVisible(hLocationRoot);
    //
    TBTreeStateMgr::LoadTreeState(_T("EquipmentTracking"),&m_ctlEquipmentTree);
}

void CEquipmentTrackingDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("EquipmentTracking"),&m_ctlEquipmentTree);    

    CleanupTree();

	m_ctlCarData.SaveState("State","EquipmentTraclomgDialog_m_ctlCarData");
    
	CDialog::OnOK();
}

void CEquipmentTrackingDialog::CleanupTree()
{
    TVITEM item;
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    HTREEITEM lastItem;
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlEquipmentTree.m_hWnd, &item );
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlEquipmentTree.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlEquipmentTree.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlEquipmentTree.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
}

void CEquipmentTrackingDialog::OnSelchangedEquipmenttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    //  get cars for this location/siding/etc
    m_ctlCarData.DeleteAllItems();
    //
    HTREEITEM hItem = TreeView_GetSelection(m_ctlEquipmentTree);
    
    if( m_ctlEquipmentTree.GetItemData(hItem) == -1 )
        return;
	
    GetCars(m_ctlEquipmentTree.GetItemData(hItem));
	
	*pResult = 0;
}

void CEquipmentTrackingDialog::GetCars(int iCarFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    CString sSQL;
    sSQL.Format("SELECT * FROM Trains_Scheduled_Cars WHERE Cars_FK=%d ORDER BY Trains_Scheduled_FK DESC;",iCarFK);
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
                int nIndex = m_ctlCarData.InsertItem(m_ctlCarData.GetItemCount(),sDate);
                //
                //  check location/siding/industry FROM
                //
                CString sFrom(""),sTo("");
                sSQL.Format("SELECT name FROM Locations WHERE id=%d;",q.getIntField("Location_from_FK"));
                CppSQLite3Query q2c = pDB->execQuery(sSQL);
                if( !q2c.eof() )
                {
                    sFrom = q2c.getStringField("name");
                }
                q2c.finalize();
                //
                if( q.getIntField("Industry_from_FK") == -1 )
                    sSQL.Format("SELECT name AS name FROM Sidings WHERE id=%d;",q.getIntField("Siding_from_FK"));
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
                //
                sSQL.Format("SELECT name FROM Locations WHERE id=%d;",q.getIntField("Location_to_FK"));
                q2c = pDB->execQuery(sSQL);
                if( !q2c.eof() )
                {
                    sTo = q2c.getStringField("name");
                }
                q2c.finalize();
                //
                if( q.getIntField("Industry_to_FK") == -1 )
                    sSQL.Format("SELECT name AS name FROM Sidings WHERE id=%d;",q.getIntField("Siding_to_FK"));
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
                //
                CString sOffLayoutDate("");
                if( q.getIntField("offlayout") == 1 )
                {
                    CppSQLite3Query q3c = pDB->execQuery("SELECT rrdate FROM _Config WHERE id=1;");
                    if( !q3c.eof() )
                    {
                        //
                        int y,m,d;
                        JulianDate::JdToYmd( q3c.getIntField("rrdate")+q2.getIntField("run_date"), &y,&m,&d);
                        sOffLayoutDate.Format("%d/%d/%d",m,d,y);
                    }
                    q3c.finalize();
                }

                //
                m_ctlCarData.SetItemText(nIndex,1,sFrom);
                m_ctlCarData.SetItemText(nIndex,2,sTo);
                m_ctlCarData.SetItemText(nIndex,3,q.getIntField("loadempty")==1?"E":"L");
                m_ctlCarData.SetItemText(nIndex,4,q2.getStringField("trainid"));
                m_ctlCarData.SetItemText(nIndex,5,sOffLayoutDate);
            }
            q2b.finalize();
        //
        }
        q2.finalize();
        q.nextRow();
    }
    q.finalize();
}
