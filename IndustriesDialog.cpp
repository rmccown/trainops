// IndustriesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustriesDialog.h"
#include "IndustryDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INDUSTRY_TIMER 12345
#define INDUSTRY_TIMER_TIME 100

/////////////////////////////////////////////////////////////////////////////
// CIndustriesDialog dialog


CIndustriesDialog::CIndustriesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustriesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustriesDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndustriesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustriesDialog)
	DDX_Control(pDX, IDC_EDITBUTTON, m_ctlEditButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_ADDBUTTON, m_ctlAddButton);
	DDX_Control(pDX, IDC_INDUSTRYTREE, m_ctlIndustryTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustriesDialog, CDialog)
	//{{AFX_MSG_MAP(CIndustriesDialog)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_EDITBUTTON, OnEditbutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeletebutton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_INDUSTRYTREE, OnSelchangedIndustrytree)
	ON_NOTIFY(NM_DBLCLK, IDC_INDUSTRYTREE, OnDblclkIndustrytree)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustriesDialog message handlers

BOOL CIndustriesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    ReadDB();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIndustriesDialog::ReadDB()
{
    m_ctlIndustryTree.DeleteAllItems();
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,name from Sidings;");
    HTREEITEM pFirstItem=0;
    //
    while (!q.eof())
    {
        CString sTxt("Siding: ");
        sTxt += q.fieldValue(1);
        HTREEITEM pItem = m_ctlIndustryTree.InsertItem(sTxt,TVI_ROOT, TVI_LAST);
        if( !pFirstItem )
            pFirstItem = pItem;
        m_ctlIndustryTree.SetItemData(pItem,atoi(q.fieldValue(0)));
        //
        CString sSQL;
        sSQL.Format("SELECT id,name FROM Industries WHERE Sidings_FK=%d;",atoi(q.fieldValue(0)));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while (!q2.eof())
        {
            HTREEITEM pSubItem = m_ctlIndustryTree.InsertItem(q2.fieldValue(1),pItem, TVI_LAST);
            m_ctlIndustryTree.SetItemData(pSubItem,atoi(q2.fieldValue(0)));
            q2.nextRow();
        }
        q2.finalize();
        //
        q.nextRow();
    }
    q.finalize();
    TBTreeStateMgr::LoadTreeState(_T("IndustriesDialog"),&m_ctlIndustryTree);
    m_ctlIndustryTree.SelectItem(pFirstItem);
    m_ctlAddButton.EnableWindow(TRUE);
    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
}

void CIndustriesDialog::OnAddbutton() 
{
    HTREEITEM pItem = m_ctlIndustryTree.GetSelectedItem();
    CString sName = m_ctlIndustryTree.GetItemText(pItem);
    if( sName.Left(7) == "Siding:" )
    {
        m_ctlIndustryTree.Expand(pItem,TVE_EXPAND);
        CIndustryDetailDialog pDlg;	
        pDlg.SetIndustryDefault();
        int iID = m_ctlIndustryTree.GetItemData(pItem);
        //pDlg.SetIndustrySiding(sName.Mid(8));
        pDlg.SetIndustrySiding(iID);
        pDlg.DoModal();
        TBTreeStateMgr::SaveTreeState(_T("IndustriesDialog"),&m_ctlIndustryTree);
        ReadDB();
    }
    else
        return;
}

void CIndustriesDialog::OnEditbutton() 
{
    HTREEITEM pItem = m_ctlIndustryTree.GetSelectedItem();
    if( !pItem )
        return;
    CString sName = m_ctlIndustryTree.GetItemText(pItem);
    if( sName.Left(7) == "Siding:" )
        return;
    int iID = m_ctlIndustryTree.GetItemData(pItem);
    //
    //  get siding...
    //
    sName = m_ctlIndustryTree.GetItemText(m_ctlIndustryTree.GetNextItem(pItem,TVGN_PARENT));
    //
    CIndustryDetailDialog pDlg;
    pDlg.SetIndustryID(iID);
//    pDlg.SetIndustrySiding(sName.Mid(8));
    pDlg.SetIndustrySiding(iID);
    pDlg.DoModal();
    //
    TBTreeStateMgr::SaveTreeState(_T("IndustriesDialog"),&m_ctlIndustryTree);

    SetTimer(INDUSTRY_TIMER,INDUSTRY_TIMER_TIME,NULL);
}

void CIndustriesDialog::OnDeletebutton() 
{
	// TODO: Add your control notification handler code here
    HTREEITEM pItem = m_ctlIndustryTree.GetSelectedItem();
    if( !pItem )
        return;
    if( MessageBox("Are you sure you want to delete this industry?","LSC TrainOps",MB_YESNO|MB_ICONQUESTION) != IDYES )
        return;
    CString sName = m_ctlIndustryTree.GetItemText(pItem);
    if( sName.Left(7) == "Siding:" )
        return;
    int iID = m_ctlIndustryTree.GetItemData(pItem);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Industries WHERE id=\"%d\"",iID);
    pDB->execDML((LPCTSTR)sSQL);
    //
    ReadDB();
}

void CIndustriesDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("IndustriesDialog"),&m_ctlIndustryTree);
	
	CDialog::OnOK();
}

void CIndustriesDialog::OnSelchangedIndustrytree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    HTREEITEM pItem = m_ctlIndustryTree.GetSelectedItem();
    if( !pItem )
        return;
    CString sName = m_ctlIndustryTree.GetItemText(pItem);
    m_ctlAddButton.EnableWindow(FALSE);
    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
    if( sName.Left(7) == "Siding:" )
        m_ctlAddButton.EnableWindow(TRUE);
    else
    {
        m_ctlEditButton.EnableWindow(TRUE);
        m_ctlDeleteButton.EnableWindow(TRUE);
    }
	
	*pResult = 0;
}

void CIndustriesDialog::OnDblclkIndustrytree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditbutton();
	*pResult = 0;
}

void CIndustriesDialog::OnTimer(UINT nIDEvent) 
{
    if( nIDEvent == INDUSTRY_TIMER )
    {
        KillTimer(nIDEvent);
        ReadDB();
    }
	CDialog::OnTimer(nIDEvent);
}
