// IndustryDetailDialog_Commodities.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustryDetailDialog_Commodities.h"
#include "IndustryCarDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog_Commodities dialog


CIndustryDetailDialog_Commodities::CIndustryDetailDialog_Commodities(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustryDetailDialog_Commodities::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryDetailDialog_Commodities)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndustryDetailDialog_Commodities::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryDetailDialog_Commodities)
	DDX_Control(pDX, IDC_COMMODITIESOUTLIST, m_ctlCommodityListOut);
	DDX_Control(pDX, IDC_COMMODITIESINLIST, m_ctlCommodityListIn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryDetailDialog_Commodities, CDialog)
	//{{AFX_MSG_MAP(CIndustryDetailDialog_Commodities)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COMMODITIESINLIST, OnItemchangedCommoditiesinlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COMMODITIESOUTLIST, OnItemchangedCommoditiesoutlist)
	ON_NOTIFY(NM_DBLCLK, IDC_COMMODITIESINLIST, OnDblclkCommoditiesinlist)
	ON_NOTIFY(NM_DBLCLK, IDC_COMMODITIESOUTLIST, OnDblclkCommoditiesoutlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog_Commodities message handlers

void CIndustryDetailDialog_Commodities::Load()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  load commodities
    //
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Commodities;");
    //
    m_ctlCommodityListIn.SetExtendedStyle(m_ctlCommodityListIn.GetStyle()|LVS_EX_CHECKBOXES|LVM_SETEXTENDEDLISTVIEWSTYLE);
    m_ctlCommodityListIn.InsertColumn(0,"Commodity",LVCFMT_LEFT,100);
    m_ctlCommodityListIn.InsertColumn(1,"Time",LVCFMT_LEFT,0);
    m_ctlCommodityListIn.InsertColumn(2,"High",LVCFMT_LEFT,0);
    m_ctlCommodityListIn.InsertColumn(3,"Low",LVCFMT_LEFT,0);
    m_ctlCommodityListIn.InsertColumn(4,"Pct",LVCFMT_LEFT,0);
    m_ctlCommodityListIn.InsertColumn(5,"Pass",LVCFMT_LEFT,0);
    //m_ctlCommodityListIn.EnableWindow(FALSE);
    m_ctlCommodityListOut.SetExtendedStyle(m_ctlCommodityListOut.GetStyle()|LVS_EX_CHECKBOXES|LVM_SETEXTENDEDLISTVIEWSTYLE);
    m_ctlCommodityListOut.InsertColumn(0,"Commodity",LVCFMT_LEFT,100);
    m_ctlCommodityListOut.InsertColumn(1,"Time",LVCFMT_LEFT,0);
    m_ctlCommodityListOut.InsertColumn(2,"High",LVCFMT_LEFT,0);
    m_ctlCommodityListOut.InsertColumn(3,"Low",LVCFMT_LEFT,0);
    m_ctlCommodityListOut.InsertColumn(4,"Pct",LVCFMT_LEFT,0);
    m_ctlCommodityListOut.InsertColumn(5,"Pass",LVCFMT_LEFT,0);
    //m_ctlCommodityListOut.EnableWindow(FALSE);
    //
    while (!q.eof())
    {
        int nIndex = m_ctlCommodityListIn.InsertItem(0,q.fieldValue(1));
        m_ctlCommodityListIn.SetItemData(nIndex,atoi(q.fieldValue(0)));
        int nIndex2 = m_ctlCommodityListOut.InsertItem(0,q.fieldValue(1));
        m_ctlCommodityListOut.SetItemData(nIndex2,atoi(q.fieldValue(0)));
        
        //  check if we have info for this car type
        if( m_iIndustryID != -1 )
        {
            CString sSQL;
            sSQL.Format("SELECT LoadEmptyDays,InOut,Quantity_high,Quantity_low,Quantity_percentage FROM Industries_Commodities WHERE Industries_FK=%d AND Commodities_FK=%s;",m_iIndustryID,q.fieldValue(0));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( q2.eof() )
            {
                m_ctlCommodityListIn.SetItemText(nIndex,1,"1");
                m_ctlCommodityListIn.SetItemText(nIndex,2,"1");
                m_ctlCommodityListIn.SetItemText(nIndex,3,"0");
                m_ctlCommodityListIn.SetItemText(nIndex,4,"100");//100
                //
                m_ctlCommodityListOut.SetItemText(nIndex2,1,"1");
                m_ctlCommodityListOut.SetItemText(nIndex2,2,"1");
                m_ctlCommodityListOut.SetItemText(nIndex2,3,"0");
                m_ctlCommodityListOut.SetItemText(nIndex2,4,"100");//100
            }
            else
            {
                //
                //  may be 2 entries, one in and one out
                //
                while (!q2.eof())
                {
                    if( strcmp(q2.fieldValue(1),"0") == 0 )
                    {
                        m_ctlCommodityListIn.SetCheck(nIndex,TRUE);
                        m_ctlCommodityListIn.SetItemText(nIndex,1,q2.fieldValue(0));
                        m_ctlCommodityListIn.SetItemText(nIndex,2,q2.fieldValue(2));
                        m_ctlCommodityListIn.SetItemText(nIndex,3,q2.fieldValue(3));
                        m_ctlCommodityListIn.SetItemText(nIndex,4,q2.fieldValue(4));
                    }
                    else
                    {
                        m_ctlCommodityListOut.SetCheck(nIndex2,TRUE);
                        m_ctlCommodityListOut.SetItemText(nIndex2,1,q2.fieldValue(0));
                        m_ctlCommodityListOut.SetItemText(nIndex2,2,q2.fieldValue(2));
                        m_ctlCommodityListOut.SetItemText(nIndex2,3,q2.fieldValue(3));
                        m_ctlCommodityListOut.SetItemText(nIndex2,4,q2.fieldValue(4));
                    }
                    q2.nextRow();
                }
            }
            q2.finalize();
        }
        else
        {
            m_ctlCommodityListIn.SetItemText(nIndex,1,"1");
            m_ctlCommodityListIn.SetItemText(nIndex,2,"1");
            m_ctlCommodityListIn.SetItemText(nIndex,3,"0");
            m_ctlCommodityListIn.SetItemText(nIndex,4,"100");//100
            //
            m_ctlCommodityListOut.SetItemText(nIndex2,1,"1");
            m_ctlCommodityListOut.SetItemText(nIndex2,2,"1");
            m_ctlCommodityListOut.SetItemText(nIndex2,3,"0");
            m_ctlCommodityListOut.SetItemText(nIndex2,4,"100");//100
        }
        q.nextRow();
    }
    q.finalize();

}

void CIndustryDetailDialog_Commodities::OnItemchangedCommoditiesinlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if( !m_ctlCommodityListIn.IsWindowVisible() )
        return;

	*pResult = 0;

    if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
        return;    // No change

    // Old check box state
    BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);  
    if (bPrevState < 0)     // On startup there's no previous state 
        bPrevState = 0;     // so assign as false (unchecked)

    // New check box state
    BOOL bChecked = (BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
    if (bChecked < 0)       // On non-checkbox notifications assume false
        bChecked = 0; 

    if (bPrevState == bChecked) // No change in check box
        return;

    // Now bChecked holds the new check box state

    if( bChecked )
    {
        CIndustryCarDetailDialog pDlg;
        pDlg.SetIn();
        pDlg.SetLoadEmpty(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,1)));
        pDlg.SetQuanHigh(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,2)));
        pDlg.SetQuanLow(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,3)));
        pDlg.SetPercent(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,4)));
        pDlg.SetPassenger(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,5)=="Y"?TRUE:FALSE);
        //
        if( pDlg.DoModal() == IDOK )
        {
            CString sWork;
            sWork.Format("%d",pDlg.GetLoadEmpty());
            m_ctlCommodityListIn.SetItemText(pNMListView->iItem,1,sWork);
            sWork.Format("%d",pDlg.GetQuanHigh());
            m_ctlCommodityListIn.SetItemText(pNMListView->iItem,2,sWork);
            sWork.Format("%d",pDlg.GetQuanLow());
            m_ctlCommodityListIn.SetItemText(pNMListView->iItem,3,sWork);
            sWork.Format("%d",pDlg.GetPercent());
            m_ctlCommodityListIn.SetItemText(pNMListView->iItem,4,sWork);
            m_ctlCommodityListIn.UpdateData();
        }
    }}

void CIndustryDetailDialog_Commodities::OnItemchangedCommoditiesoutlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if( !m_ctlCommodityListOut.IsWindowVisible() )
        return;

	*pResult = 0;

    if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
        return;    // No change

    // Old check box state
    BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);  
    if (bPrevState < 0)     // On startup there's no previous state 
        bPrevState = 0;     // so assign as false (unchecked)

    // New check box state
    BOOL bChecked = (BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
    if (bChecked < 0)       // On non-checkbox notifications assume false
        bChecked = 0; 

    if (bPrevState == bChecked) // No change in check box
        return;

    // Now bChecked holds the new check box state

    if( bChecked )
    {
        CIndustryCarDetailDialog pDlg;
        pDlg.SetOut();
        pDlg.SetLoadEmpty(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,1)));
        pDlg.SetQuanHigh(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,2)));
        pDlg.SetQuanLow(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,3)));
        pDlg.SetPercent(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,4)));
        pDlg.SetPassenger(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,5)=="Y"?TRUE:FALSE);
        //
        if( pDlg.DoModal() == IDOK )
        {
            CString sWork;
            sWork.Format("%d",pDlg.GetLoadEmpty());
            m_ctlCommodityListOut.SetItemText(pNMListView->iItem,1,sWork);
            sWork.Format("%d",pDlg.GetQuanHigh());
            m_ctlCommodityListOut.SetItemText(pNMListView->iItem,2,sWork);
            sWork.Format("%d",pDlg.GetQuanLow());
            m_ctlCommodityListOut.SetItemText(pNMListView->iItem,3,sWork);
            sWork.Format("%d",pDlg.GetPercent());
            m_ctlCommodityListOut.SetItemText(pNMListView->iItem,4,sWork);
            m_ctlCommodityListOut.UpdateData();
        }
    }
}

void CIndustryDetailDialog_Commodities::OnDblclkCommoditiesinlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    //
    CIndustryCarDetailDialog pDlg;
    pDlg.SetIn();
    pDlg.SetLoadEmpty(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,1)));
    pDlg.SetQuanHigh(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,2)));
    pDlg.SetQuanLow(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,3)));
    pDlg.SetPercent(atoi(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,4)));
    pDlg.SetPassenger(m_ctlCommodityListIn.GetItemText(pNMListView->iItem,5)=="Y"?TRUE:FALSE);
    //
    if( pDlg.DoModal() == IDOK )
    {
        CString sWork;
        sWork.Format("%d",pDlg.GetLoadEmpty());
        m_ctlCommodityListIn.SetItemText(pNMListView->iItem,1,sWork);
        sWork.Format("%d",pDlg.GetQuanHigh());
        m_ctlCommodityListIn.SetItemText(pNMListView->iItem,2,sWork);
        sWork.Format("%d",pDlg.GetQuanLow());
        m_ctlCommodityListIn.SetItemText(pNMListView->iItem,3,sWork);
        sWork.Format("%d",pDlg.GetPercent());
        m_ctlCommodityListIn.SetItemText(pNMListView->iItem,4,sWork);
        m_ctlCommodityListIn.UpdateData();
    }
    //
    *pResult = 0;
}

void CIndustryDetailDialog_Commodities::OnDblclkCommoditiesoutlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    //
    CIndustryCarDetailDialog pDlg;
    pDlg.SetOut();
    pDlg.SetLoadEmpty(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,1)));
    pDlg.SetQuanHigh(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,2)));
    pDlg.SetQuanLow(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,3)));
    pDlg.SetPercent(atoi(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,4)));
    pDlg.SetPassenger(m_ctlCommodityListOut.GetItemText(pNMListView->iItem,5)=="Y"?TRUE:FALSE);
    //
    if( pDlg.DoModal() == IDOK )
    {
        CString sWork;
        sWork.Format("%d",pDlg.GetLoadEmpty());
        m_ctlCommodityListOut.SetItemText(pNMListView->iItem,1,sWork);
        sWork.Format("%d",pDlg.GetQuanHigh());
        m_ctlCommodityListOut.SetItemText(pNMListView->iItem,2,sWork);
        sWork.Format("%d",pDlg.GetQuanLow());
        m_ctlCommodityListOut.SetItemText(pNMListView->iItem,3,sWork);
        sWork.Format("%d",pDlg.GetPercent());
        m_ctlCommodityListOut.SetItemText(pNMListView->iItem,4,sWork);
        m_ctlCommodityListOut.UpdateData();
    }
    //
	*pResult = 0;
}

void CIndustryDetailDialog_Commodities::Save()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    for( int i=0;i<m_ctlCommodityListIn.GetItemCount();i++ )
    {
        if( m_ctlCommodityListIn.GetCheck(i) )
        {
            int iCommodityFK = m_ctlCommodityListIn.GetItemData(i);
            int iLoadEmpty = atoi(m_ctlCommodityListIn.GetItemText(i,1));
            int iQuanHigh = atoi(m_ctlCommodityListIn.GetItemText(i,2));
            int iQuanLow = atoi(m_ctlCommodityListIn.GetItemText(i,3));
            int iPercent = atoi(m_ctlCommodityListIn.GetItemText(i,4));
            CString sSQL;
            sSQL.Format("INSERT INTO Industries_Commodities (Industries_FK,Commodities_FK,InOut,LoadEmptyDays,Quantity_low,Quantity_high,Quantity_percentage) VALUES (%d,%d,0,%d,%d,%d,%d);",m_iIndustryID,iCommodityFK,iLoadEmpty,iQuanLow,iQuanHigh,iPercent);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            q.finalize();
        }
    }
    for( i=0;i<m_ctlCommodityListOut.GetItemCount();i++ )
    {
        if( m_ctlCommodityListOut.GetCheck(i) )
        {
            int iCommodityFK = m_ctlCommodityListOut.GetItemData(i);
            int iLoadEmpty = atoi(m_ctlCommodityListOut.GetItemText(i,1));
            int iQuanHigh = atoi(m_ctlCommodityListOut.GetItemText(i,2));
            int iQuanLow = atoi(m_ctlCommodityListOut.GetItemText(i,3));
            int iPercent = atoi(m_ctlCommodityListOut.GetItemText(i,4));
            CString sSQL;
            sSQL.Format("INSERT INTO Industries_Commodities (Industries_FK,Commodities_FK,InOut,LoadEmptyDays,Quantity_low,Quantity_high,Quantity_percentage) VALUES (%d,%d,1,%d,%d,%d,%d);",m_iIndustryID,iCommodityFK,iLoadEmpty,iQuanLow,iQuanHigh,iPercent);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            q.finalize();
        }
    }
}
