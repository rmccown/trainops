// CabooseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CabooseDialog.h"
#include "CabooseDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCabooseDialog dialog


CCabooseDialog::CCabooseDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCabooseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCabooseDialog)
	//}}AFX_DATA_INIT
}


void CCabooseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCabooseDialog)
	DDX_Control(pDX, IDC_CABOOSELIST, m_ctlCabooseList);
	DDX_Control(pDX, IDC_ADDBUTTON, m_ctlAddButton);
	DDX_Control(pDX, IDC_EDITBUTTON, m_ctlEditButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_ctlDeleteButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCabooseDialog, CDialog)
	//{{AFX_MSG_MAP(CCabooseDialog)
	ON_BN_CLICKED(IDC_EDITBUTTON, OnEditbutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeletebutton)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_NOTIFY(NM_CLICK, IDC_CABOOSELIST, OnClickCabooselist)
	ON_NOTIFY(NM_DBLCLK, IDC_CABOOSELIST, OnDblclkCabooselist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCabooseDialog message handlers

BOOL CCabooseDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlCabooseList.InsertColumn(0,"Reporting Marks/Number");
    m_ctlCabooseList.SetColumnWidth(0,260);

    ReadDB();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCabooseDialog::ReadDB()
{
    m_ctlCabooseList.DeleteAllItems();
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,reporting_marks,car_number FROM Caboose ORDER BY reporting_marks,car_number;");
    //
    while (!q.eof())
    {
        //TRACE("%s %s %s\n",q.fieldValue(0),q.fieldValue(1),q.fieldValue(1));
        CString sWork;
        sWork.Format("%s %s",q.fieldValue(1),q.fieldValue(2));
        int nIndex = m_ctlCabooseList.InsertItem(1,sWork);
        m_ctlCabooseList.SetItemData(nIndex,atoi(q.fieldValue(0)));
        q.nextRow();
    }
    q.finalize();

    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
}

void CCabooseDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CCabooseDialog::OnEditbutton() 
{
	// TODO: Add your control notification handler code here
    int iSel = m_ctlCabooseList.GetSelectionMark();
    if( iSel < 0 )
        return;
    int iCabooseFK = m_ctlCabooseList.GetItemData(iSel);
    CCabooseDetailDialog pDlg;
    pDlg.SetCabooseFK(iCabooseFK);
    pDlg.DoModal();
    //
    ReadDB();
}

void CCabooseDialog::OnDeletebutton() 
{
	// TODO: Add your control notification handler code here
    if( MessageBox("Are you sure you want to delete this caboose?","TrainOps Delete Caboose?",MB_YESNO) == IDNO )
        return;
    int iSel = m_ctlCabooseList.GetSelectionMark();
    if( iSel < 0 )
        return;
    int iCabooseFK = m_ctlCabooseList.GetItemData(iSel);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Caboose WHERE id=%d;",iCabooseFK);
    pDB->execDML(sSQL);
    //
    ReadDB();
    //
    MessageBox("Caboose deleted!");
}

void CCabooseDialog::OnAddbutton() 
{
	// TODO: Add your control notification handler code here
    CCabooseDetailDialog pDlg;
    pDlg.DoModal();
    //
    ReadDB();
}

void CCabooseDialog::OnClickCabooselist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if( pNMListView->iItem == -1 )
        return;
    //
    m_ctlDeleteButton.EnableWindow(TRUE);
    m_ctlEditButton.EnableWindow(TRUE);
	
	*pResult = 0;
}

void CCabooseDialog::OnDblclkCabooselist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if( pNMListView->iItem == -1 )
        return;
    //
    int iCabooseFK = m_ctlCabooseList.GetItemData(pNMListView->iItem);
    CCabooseDetailDialog pDlg;
    pDlg.SetCabooseFK(iCabooseFK);
    pDlg.DoModal();
    //
    ReadDB();
	
	*pResult = 0;
}
