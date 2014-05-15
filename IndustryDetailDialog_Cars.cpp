// IndustryDetailDialog_Cars.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustryDetailDialog_Cars.h"
#include "IndustryCarDetailDialog.h"
#include "IndustryDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog_Cars dialog

CIndustryDetailDialog_Cars::CIndustryDetailDialog_Cars(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustryDetailDialog_Cars::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryDetailDialog_Cars)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndustryDetailDialog_Cars::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryDetailDialog_Cars)
	DDX_Control(pDX, IDC_CARLISTOUT, m_ctlCarListOut);
	DDX_Control(pDX, IDC_CARLISTIN, m_ctlCarListIn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryDetailDialog_Cars, CDialog)
	//{{AFX_MSG_MAP(CIndustryDetailDialog_Cars)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CARLISTIN, OnItemchangedCarlistin)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CARLISTOUT, OnItemchangedCarlistout)
	ON_NOTIFY(NM_DBLCLK, IDC_CARLISTIN, OnDblclkCarlistin)
	ON_NOTIFY(NM_DBLCLK, IDC_CARLISTOUT, OnDblclkCarlistout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog_Cars message handlers

BOOL CIndustryDetailDialog_Cars::OnInitDialog() 
{
	CDialog::OnInitDialog();
    return TRUE;
}

void CIndustryDetailDialog_Cars::Load()
{
    //	
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  load car types
    //
    CppSQLite3Query q = pDB->execQuery("select id,description,passenger from CarTypes WHERE active=1;");
    //
    m_ctlCarListIn.SetExtendedStyle(m_ctlCarListIn.GetStyle()|LVS_EX_CHECKBOXES|LVM_SETEXTENDEDLISTVIEWSTYLE);
    m_ctlCarListIn.InsertColumn(IDD_C_TYPE,"Car Type",LVCFMT_LEFT,115);
    m_ctlCarListIn.InsertColumn(IDD_C_TEXT,"",LVCFMT_LEFT,65);
    m_ctlCarListIn.InsertColumn(IDD_C_TIME,"Time",LVCFMT_LEFT,0);
    m_ctlCarListIn.InsertColumn(IDD_C_HIGH,"High",LVCFMT_LEFT,0);
    m_ctlCarListIn.InsertColumn(IDD_C_LOW,"Low",LVCFMT_LEFT,0);
    m_ctlCarListIn.InsertColumn(IDD_C_PCT,"Pct",LVCFMT_LEFT,0);
    m_ctlCarListIn.InsertColumn(IDD_C_PASS,"Pass",LVCFMT_LEFT,0);
    //
    m_ctlCarListOut.SetExtendedStyle(m_ctlCarListOut.GetStyle()|LVS_EX_CHECKBOXES|LVM_SETEXTENDEDLISTVIEWSTYLE);
    m_ctlCarListOut.InsertColumn(IDD_C_TYPE,"Car Type",LVCFMT_LEFT,115);
    m_ctlCarListOut.InsertColumn(IDD_C_TEXT,"",LVCFMT_LEFT,65);
    m_ctlCarListOut.InsertColumn(IDD_C_TIME,"Time",LVCFMT_LEFT,0);
    m_ctlCarListOut.InsertColumn(IDD_C_HIGH,"High",LVCFMT_LEFT,0);
    m_ctlCarListOut.InsertColumn(IDD_C_LOW,"Low",LVCFMT_LEFT,0);
    m_ctlCarListOut.InsertColumn(IDD_C_PCT,"Pct",LVCFMT_LEFT,0);
    m_ctlCarListOut.InsertColumn(IDD_C_PASS,"Pass",LVCFMT_LEFT,0);
    //
    while (!q.eof())
    {
        //
        //  Add them to both lists
        //
        int nIndex = m_ctlCarListIn.InsertItem(IDD_C_TYPE,q.getStringField("description"));
        m_ctlCarListIn.SetItemData(nIndex,q.getIntField("id"));
        int nIndex2 = m_ctlCarListOut.InsertItem(IDD_C_TYPE,q.getStringField("description"));
        m_ctlCarListOut.SetItemData(nIndex2,q.getIntField("id"));
        //m_ctlCarListIn.SetItemText(nIndex,IDD_C_PASS,q.getIntField("passenger")==1?"Y":"N");
        //m_ctlCarListOut.SetItemText(nIndex2,IDD_C_PASS,q.getIntField("passenger")==1?"Y":"N");
        //
        //  defaults
        //
        m_ctlCarListIn.SetItemText(nIndex,IDD_C_TIME,"1");
        m_ctlCarListIn.SetItemText(nIndex,IDD_C_TEXT,"1-1(100%)");
        m_ctlCarListIn.SetItemText(nIndex,IDD_C_HIGH,"1");
        m_ctlCarListIn.SetItemText(nIndex,IDD_C_LOW,"0");
        m_ctlCarListIn.SetItemText(nIndex,IDD_C_PCT,"100");
        //
        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_TIME,"1");
        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_TEXT,"1-1(100%)");
        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_HIGH,"1");
        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_LOW,"0");
        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_PCT,"100");
        //  check if we have info for this car type
        if( m_iIndustryID != -1 )
        {
            CString sSQL;
            sSQL.Format("SELECT LoadEmptyDays,InOut,Quantity_high,Quantity_low,Quantity_percentage FROM Industries_Cars WHERE Industries_FK=%d AND CarTypes_FK=%d;",m_iIndustryID,q.getIntField("id"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
            {
                //
                //  may be 2 entries, one in and one out
                //
                while (!q2.eof())
                {
                    CString sText;
                    sText.Format("%d-%d (%d%%)",q2.getIntField("Quantity_low"),q2.getIntField("Quantity_high"),q2.getIntField("Quantity_percentage"));
                    //
                    if( q2.getIntField("InOut") == 0 )
                    {
                        m_ctlCarListIn.SetCheck(nIndex,TRUE);
                        m_ctlCarListIn.SetItemText(nIndex,IDD_C_TIME,q2.fieldValue(0));
                        m_ctlCarListIn.SetItemText(nIndex,IDD_C_TEXT,sText);
                        m_ctlCarListIn.SetItemText(nIndex,IDD_C_HIGH,q2.fieldValue(2));
                        m_ctlCarListIn.SetItemText(nIndex,IDD_C_LOW,q2.fieldValue(3));
                        m_ctlCarListIn.SetItemText(nIndex,IDD_C_PCT,q2.fieldValue(4));
                    }
                    else
                    {
                        m_ctlCarListOut.SetCheck(nIndex2,TRUE);
                        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_TIME,q2.fieldValue(0));
                        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_TEXT,sText);
                        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_HIGH,q2.fieldValue(2));
                        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_LOW,q2.fieldValue(3));
                        m_ctlCarListOut.SetItemText(nIndex2,IDD_C_PCT,q2.fieldValue(4));
                    }
                    q2.nextRow();
                }
            }
            q2.finalize();
        }
        //
        q.nextRow();
    }
    q.finalize();

	//m_ctlCarListIn.RestoreState("State","IndustryDetailDialog_Cars_m_ctlCarListIn");
	//m_ctlCarListOut.RestoreState("State","IndustryDetailDialog_Cars_m_ctlCarListOut");
}

void CIndustryDetailDialog_Cars::OnItemchangedCarlistin(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if( !m_ctlCarListIn.IsWindowVisible() )
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
        pDlg.SetLoadEmpty(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_TIME)));
        pDlg.SetQuanHigh(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_HIGH)));
        pDlg.SetQuanLow(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_LOW)));
        pDlg.SetPercent(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_PCT)));
        pDlg.SetPassenger(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_PASS)=="Y"?TRUE:FALSE);
        //
        if( pDlg.DoModal() == IDOK )
        {
            CString sText;
            sText.Format("%d-%d (%d%%)",pDlg.GetQuanLow(),pDlg.GetQuanHigh(),pDlg.GetPercent());
            m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_TEXT,sText);
            //
            CString sWork;
            sWork.Format("%d",pDlg.GetLoadEmpty());
            m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_TIME,sWork);
            sWork.Format("%d",pDlg.GetQuanHigh());
            m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_HIGH,sWork);
            sWork.Format("%d",pDlg.GetQuanLow());
            m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_LOW,sWork);
            sWork.Format("%d",pDlg.GetPercent());
            m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_PCT,sWork);
            m_ctlCarListIn.UpdateData();
        }
    }
    //
    CIndustryDetailDialog* pWnd = (CIndustryDetailDialog*)GetParent();
    pWnd->RecalcRatios();
    //
}

void CIndustryDetailDialog_Cars::OnItemchangedCarlistout(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if( !m_ctlCarListIn.IsWindowVisible() )
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
        pDlg.SetLoadEmpty(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_TIME)));
        pDlg.SetQuanHigh(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_HIGH)));
        pDlg.SetQuanLow(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_LOW)));
        pDlg.SetPercent(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_PCT)));
        pDlg.SetPassenger(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_PASS)=="Y"?TRUE:FALSE);
        //
        if( pDlg.DoModal() == IDOK )
        {
            CString sText;
            sText.Format("%d-%d (%d%%)",pDlg.GetQuanLow(),pDlg.GetQuanHigh(),pDlg.GetPercent());
            m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_TEXT,sText);
            CString sWork;
            sWork.Format("%d",pDlg.GetLoadEmpty());
            m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_TIME,sWork);
            sWork.Format("%d",pDlg.GetQuanHigh());
            m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_HIGH,sWork);
            sWork.Format("%d",pDlg.GetQuanLow());
            m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_LOW,sWork);
            sWork.Format("%d",pDlg.GetPercent());
            m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_PCT,sWork);
            m_ctlCarListOut.UpdateData();
        }
    }
    //
    CIndustryDetailDialog* pWnd = (CIndustryDetailDialog*)GetParent();
    pWnd->RecalcRatios();
    //
}

void CIndustryDetailDialog_Cars::OnDblclkCarlistin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    //
    CIndustryCarDetailDialog pDlg;
    pDlg.SetIn();
    pDlg.SetLoadEmpty(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_TIME)));
    pDlg.SetQuanHigh(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_HIGH)));
    pDlg.SetQuanLow(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_LOW)));
    pDlg.SetPercent(atoi(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_PCT)));
    pDlg.SetPassenger(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_PASS)=="Y"?TRUE:FALSE);
    //
    if( pDlg.DoModal() == IDOK )
    {
        CString sText;
        sText.Format("%d-%d (%d%%)",pDlg.GetQuanLow(),pDlg.GetQuanHigh(),pDlg.GetPercent());
        m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_TEXT,sText);
        //
        CString sWork;
        sWork.Format("%d",pDlg.GetLoadEmpty());
        m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_TIME,sWork);
        sWork.Format("%d",pDlg.GetQuanHigh());
        m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_HIGH,sWork);
        sWork.Format("%d",pDlg.GetQuanLow());
        m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_LOW,sWork);
        sWork.Format("%d",pDlg.GetPercent());
        m_ctlCarListIn.SetItemText(pNMListView->iItem,IDD_C_PCT,sWork);
        m_ctlCarListIn.UpdateData();
    }
    //
    CIndustryDetailDialog* pWnd = (CIndustryDetailDialog*)GetParent();
    pWnd->RecalcRatios();
    //
    *pResult = 0;
}

void CIndustryDetailDialog_Cars::OnDblclkCarlistout(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    //
    CIndustryCarDetailDialog pDlg;
    pDlg.SetOut();
    pDlg.SetLoadEmpty(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_TIME)));
    pDlg.SetQuanHigh(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_HIGH)));
    pDlg.SetQuanLow(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_LOW)));
    pDlg.SetPercent(atoi(m_ctlCarListOut.GetItemText(pNMListView->iItem,IDD_C_PCT)));
    pDlg.SetPassenger(m_ctlCarListIn.GetItemText(pNMListView->iItem,IDD_C_PASS)=="Y"?TRUE:FALSE);
    //
    if( pDlg.DoModal() == IDOK )
    {
        CString sText;
        sText.Format("%d-%d (%d%%)",pDlg.GetQuanLow(),pDlg.GetQuanHigh(),pDlg.GetPercent());
        m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_TEXT,sText);
        //
        CString sWork;
        sWork.Format("%d",pDlg.GetLoadEmpty());
        m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_TIME,sWork);
        sWork.Format("%d",pDlg.GetQuanHigh());
        m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_HIGH,sWork);
        sWork.Format("%d",pDlg.GetQuanLow());
        m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_LOW,sWork);
        sWork.Format("%d",pDlg.GetPercent());
        m_ctlCarListOut.SetItemText(pNMListView->iItem,IDD_C_PCT,sWork);
        m_ctlCarListOut.UpdateData();
    }
    //
    CIndustryDetailDialog* pWnd = (CIndustryDetailDialog*)GetParent();
    pWnd->RecalcRatios();
    //
	*pResult = 0;
}

void CIndustryDetailDialog_Cars::Save(int iIndustryFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    for( int i=0;i<m_ctlCarListIn.GetItemCount();i++ )
    {
        if( m_ctlCarListIn.GetCheck(i) )
        {
            int iCarTypeFK = m_ctlCarListIn.GetItemData(i);
            int iLoadEmpty = atoi(m_ctlCarListIn.GetItemText(i,IDD_C_TIME));
            int iQuanHigh = atoi(m_ctlCarListIn.GetItemText(i,IDD_C_HIGH));
            int iQuanLow = atoi(m_ctlCarListIn.GetItemText(i,IDD_C_LOW));
            int iPercent = atoi(m_ctlCarListIn.GetItemText(i,IDD_C_PCT));
            CString sSQL;
            sSQL.Format("INSERT INTO Industries_Cars (Industries_FK,CarTypes_FK,InOut,LoadEmptyDays,Quantity_low,Quantity_high,Quantity_percentage) VALUES (%d,%d,0,%d,%d,%d,%d);",iIndustryFK,iCarTypeFK,iLoadEmpty,iQuanLow,iQuanHigh,iPercent);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            q.finalize();
        }
    }
    for( i=0;i<m_ctlCarListOut.GetItemCount();i++ )
    {
        if( m_ctlCarListOut.GetCheck(i) )
        {
            int iCarTypeFK = m_ctlCarListOut.GetItemData(i);
            int iLoadEmpty = atoi(m_ctlCarListOut.GetItemText(i,IDD_C_TIME));
            int iQuanHigh = atoi(m_ctlCarListOut.GetItemText(i,IDD_C_HIGH));
            int iQuanLow = atoi(m_ctlCarListOut.GetItemText(i,IDD_C_LOW));
            int iPercent = atoi(m_ctlCarListOut.GetItemText(i,IDD_C_PCT));
            CString sSQL;
            sSQL.Format("INSERT INTO Industries_Cars (Industries_FK,CarTypes_FK,InOut,LoadEmptyDays,Quantity_low,Quantity_high,Quantity_percentage) VALUES (%d,%d,1,%d,%d,%d,%d);",iIndustryFK,iCarTypeFK,iLoadEmpty,iQuanLow,iQuanHigh,iPercent);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            q.finalize();
        }
    }
	m_ctlCarListIn.SaveState("State","IndustryDetailDialog_Cars_m_ctlCarListIn");
	m_ctlCarListOut.SaveState("State","IndustryDetailDialog_Cars_m_ctlCarListOut");

}
