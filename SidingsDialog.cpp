// SidingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "SidingsDialog.h"
#include "SidingDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSidingsDialog dialog

#define TIMER_SIDINGS 12345
#define TIMER_SIDINGS_TIME 100

CSidingsDialog::CSidingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSidingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSidingsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSidingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSidingsDialog)
	DDX_Control(pDX, IDC_EDITBUTTON, m_ctlEditButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_ADDBUTTON, m_ctlAddButton);
	DDX_Control(pDX, IDC_SIDINGSTREE, m_ctlSidingsTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSidingsDialog, CDialog)
	//{{AFX_MSG_MAP(CSidingsDialog)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_EDITBUTTON, OnEditbutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeletebutton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SIDINGSTREE, OnSelchangedSidingstree)
	ON_NOTIFY(NM_DBLCLK, IDC_SIDINGSTREE, OnDblclkSidingstree)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSidingsDialog message handlers

BOOL CSidingsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    ReadDB();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSidingsDialog::ReadDB()
{
    m_ctlSidingsTree.SetRedraw(FALSE);
    //
    m_ctlSidingsTree.DeleteAllItems();
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,name from Locations;");
    HTREEITEM pFirstItem=0;
    //
    while (!q.eof())
    {
        CString sTxt("Location: ");
        sTxt += q.fieldValue(1);
        HTREEITEM pItem = m_ctlSidingsTree.InsertItem(sTxt,TVI_ROOT, TVI_LAST);
        if( !pFirstItem )
            pFirstItem = pItem;
        m_ctlSidingsTree.SetItemData(pItem,atoi(q.fieldValue(0)));
        //
        CString sSQL;
        sSQL.Format("SELECT id,name FROM Sidings WHERE Locations_FK=%d;",atoi(q.fieldValue(0)));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while (!q2.eof())
        {
            HTREEITEM pSubItem = m_ctlSidingsTree.InsertItem(q2.fieldValue(1),pItem, TVI_LAST);
            m_ctlSidingsTree.SetItemData(pSubItem,atoi(q2.fieldValue(0)));
            q2.nextRow();
        }
        q2.finalize();
        //
        q.nextRow();
    }
    q.finalize();
    TBTreeStateMgr::LoadTreeState(_T("SidingsDialog"),&m_ctlSidingsTree);
    m_ctlSidingsTree.SelectItem(pFirstItem);
    m_ctlAddButton.EnableWindow(TRUE);
    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
    //
    m_ctlSidingsTree.SetRedraw(TRUE);
}

void CSidingsDialog::OnAddbutton() 
{
    //
    CSidingDetailDialog pDlg;	
    pDlg.SetSidingDefault();
    HTREEITEM pItem = m_ctlSidingsTree.GetSelectedItem();
    if( !pItem )
        return;
    CString sName = m_ctlSidingsTree.GetItemText(pItem);
    if( sName.Left(9) == "Location:" )
    {
        pDlg.SetSidingLocation(sName.Mid(10));
        pDlg.DoModal();
        CString sSiding = "Location: " + pDlg.GetSidingLocation();
        HTREEITEM hRootItem = m_ctlSidingsTree.GetRootItem();
        HTREEITEM hItem = hRootItem;
        while (hItem)
        {
            if (m_ctlSidingsTree.GetItemText(hItem) == sSiding)
            {
                m_ctlSidingsTree.Expand(hItem, TVE_EXPAND);
                break;
            }
            hItem = m_ctlSidingsTree.GetNextItem(hItem,TVGN_NEXT);
        }
        //
        TBTreeStateMgr::SaveTreeState(_T("SidingsDialog"),&m_ctlSidingsTree);
        ReadDB();
    }
}

void CSidingsDialog::OnEditbutton() 
{
    HTREEITEM pItem = m_ctlSidingsTree.GetSelectedItem();
    if( !pItem )
        return;
    CString sName = m_ctlSidingsTree.GetItemText(pItem);
    if( sName.Left(9) == "Location:" )
        return;
    int iID = m_ctlSidingsTree.GetItemData(pItem);
    //
    CSidingDetailDialog pDlg;
    pDlg.SetSidingID(iID);
    pDlg.DoModal();
    //
    TBTreeStateMgr::SaveTreeState(_T("SidingsDialog"),&m_ctlSidingsTree);
    //
    SetTimer(TIMER_SIDINGS,TIMER_SIDINGS_TIME,NULL);
}

void CSidingsDialog::OnDeletebutton() 
{
	// TODO: Add your control notification handler code here
    if( MessageBox("Deleting a siding may have adverse effects on industries, trains, and.  Are you sure you want to do this?","Delete Location?",MB_YESNO) == IDNO )
        return;
    HTREEITEM pItem = m_ctlSidingsTree.GetSelectedItem();
    if( !pItem )
        return;
    int iID = m_ctlSidingsTree.GetItemData(pItem);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Locations WHERE id=%d;",iID);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Industries SET Sidings_FK=-1 WHERE Sidings_FK=%d;",iID);
    pDB->execDML(sSQL);
    ReadDB();
    
}

void CSidingsDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("SidingsDialog"),&m_ctlSidingsTree);
	
	CDialog::OnOK();
}

void CSidingsDialog::OnSelchangedSidingstree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    HTREEITEM pItem = m_ctlSidingsTree.GetSelectedItem();
    if( !pItem )
        return;
    CString sName = m_ctlSidingsTree.GetItemText(pItem);
    m_ctlAddButton.EnableWindow(FALSE);
    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
    if( sName.Left(9) == "Location:" )
        m_ctlAddButton.EnableWindow(TRUE);
    else
    {
        m_ctlEditButton.EnableWindow(TRUE);
        m_ctlDeleteButton.EnableWindow(TRUE);
    }

	*pResult = 0;
}

void CSidingsDialog::OnDblclkSidingstree(NMHDR* pNMHDR, LRESULT* pResult) 
{
    OnEditbutton();
    //
	*pResult = 0;
}

void CSidingsDialog::OnTimer(UINT nIDEvent) 
{
    if( nIDEvent == TIMER_SIDINGS )
    {
        KillTimer(nIDEvent);
        ReadDB();
    }
	CDialog::OnTimer(nIDEvent);
}
