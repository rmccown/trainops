// LocomotiveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "LocomotiveDialog.h"

#include "LocomotiveDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocomotiveDialog dialog


CLocomotiveDialog::CLocomotiveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLocomotiveDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocomotiveDialog)
	//}}AFX_DATA_INIT
}


void CLocomotiveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocomotiveDialog)
	DDX_Control(pDX, IDC_EDITBUTTON, m_ctlEditButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_LOCOLIST, m_ctlLocoList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocomotiveDialog, CDialog)
	//{{AFX_MSG_MAP(CLocomotiveDialog)
	ON_NOTIFY(NM_CLICK, IDC_LOCOLIST, OnClickLocolist)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeletebutton)
	ON_BN_CLICKED(IDC_EDITBUTTON, OnEditbutton)
	ON_BN_CLICKED(IDC_NEWBUTTON, OnNewbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocomotiveDialog message handlers

BOOL CLocomotiveDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlLocoList.SetExtendedStyle(m_ctlLocoList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

    m_ctlLocoList.InsertColumn(0,"Number");
    m_ctlLocoList.InsertColumn(1,"Road Name");
    m_ctlLocoList.InsertColumn(2,"Description");
    m_ctlLocoList.InsertColumn(3,"Capacity");
    m_ctlLocoList.SetColumnWidth(0,70);
    m_ctlLocoList.SetColumnWidth(1,192);
    m_ctlLocoList.SetColumnWidth(2,192);
    m_ctlLocoList.SetColumnWidth(3,60);

    ReadDB();

    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLocomotiveDialog::ReadDB()
{
    m_ctlLocoList.DeleteAllItems();
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,active,road_name,car_number,capacity,description FROM Locomotives;");
    //
    while (!q.eof())
    {
        int nIndex = m_ctlLocoList.InsertItem(1,q.fieldValue(3));
        m_ctlLocoList.SetItemData(nIndex,atoi(q.fieldValue(0)));
        m_ctlLocoList.SetItemText(nIndex,1,q.fieldValue(2));
        m_ctlLocoList.SetItemText(nIndex,2,q.fieldValue(5));
        m_ctlLocoList.SetItemText(nIndex,3,q.fieldValue(4));
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
}

void CLocomotiveDialog::OnDeletebutton() 
{
	// TODO: Add your control notification handler code here
    if( MessageBox("Are you sure you want to delete this locomotive?","TrainOps Question",MB_YESNO) == IDNO )
        return;
    int iLocoFK = m_ctlLocoList.GetItemData(m_ctlLocoList.GetNextItem(-1, LVNI_SELECTED));
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Locomotives WHERE id=%d;",iLocoFK);
    pDB->execQuery(sSQL);
    //
    ReadDB();
}

void CLocomotiveDialog::OnClickLocolist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    m_ctlEditButton.EnableWindow(TRUE);
    m_ctlDeleteButton.EnableWindow(TRUE);
    
	*pResult = 0;
}

void CLocomotiveDialog::OnEditbutton() 
{
    int iLocoFK = m_ctlLocoList.GetItemData(m_ctlLocoList.GetNextItem(-1, LVNI_SELECTED));
    CLocomotiveDetailDialog pDlg;
	pDlg.SetLocoFK(iLocoFK);
    pDlg.DoModal();	
    //
    ReadDB();
}

void CLocomotiveDialog::OnNewbutton() 
{
    CLocomotiveDetailDialog pDlg;
    pDlg.DoModal();	
    //
    ReadDB();
}
