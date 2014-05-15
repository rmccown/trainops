// CarTypesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CarTypesDialog.h"

#include "CarTypesDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarTypesDialog dialog


CCarTypesDialog::CCarTypesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCarTypesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCarTypesDialog)
	//}}AFX_DATA_INIT
}


void CCarTypesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarTypesDialog)
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_ctlEditButton);
	DDX_Control(pDX, IDC_ADD_BUTTON, m_ctlAddButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_CARTYPE_LIST, m_ctlCarTypes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCarTypesDialog, CDialog)
	//{{AFX_MSG_MAP(CCarTypesDialog)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_NOTIFY(NM_CLICK, IDC_CARTYPE_LIST, OnClickCartypeList)
	ON_NOTIFY(NM_DBLCLK, IDC_CARTYPE_LIST, OnDblclkCartypeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarTypesDialog message handlers

BOOL CCarTypesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
    //
    m_ctlCarTypes.InsertColumn(0, "Car Type",LVCFMT_LEFT, 65);
    m_ctlCarTypes.InsertColumn(1, "Car Description",LVCFMT_LEFT, 242);   
    m_ctlCarTypes.SetExtendedStyle(m_ctlCarTypes.GetStyle()|LVS_EX_FULLROWSELECT);
    m_ctlCarTypes.SetExtendedStyle(m_ctlCarTypes.GetStyle()|LVS_EX_CHECKBOXES);
    //
    DWORD dwStyle = ::SendMessage(m_ctlCarTypes.m_hWnd,LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
    dwStyle |= LVS_EX_FULLROWSELECT;
    ::SendMessage(m_ctlCarTypes.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,0,dwStyle);
    //
    ReadDB();
    //
	return TRUE;
}

void CCarTypesDialog::ReadDB()
{
    m_ctlCarTypes.DeleteAllItems();
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,type_id,description,active from CarTypes;");
    //
    while (!q.eof())
    {
        int nIndex = m_ctlCarTypes.InsertItem(0,q.fieldValue(1));
        m_ctlCarTypes.SetItemText(nIndex,1,q.fieldValue(2));
        m_ctlCarTypes.SetItemData(nIndex,q.getIntField(0));
        if( q.getIntField(3) == 1 )
            m_ctlCarTypes.SetCheck(nIndex,TRUE);
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
}

void CCarTypesDialog::OnAddButton() 
{
    CCarTypesDetailDialog pDlg;
    if( pDlg.DoModal() == IDOK )
        ReadDB();
}

void CCarTypesDialog::OnDeleteButton() 
{
    int iSel = m_ctlCarTypes.GetSelectionMark();
    if( iSel < 0 )
        return;
    if( MessageBox("Are you sure you want to delete this car type?","LSC TrainOps",MB_YESNO|MB_ICONQUESTION) != IDYES )
        return;
    int iCarType = m_ctlCarTypes.GetItemData(iSel);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT 1 FROM Cars WHERE CarTypes_FK=%d;",iCarType);
    CppSQLite3Query q = pDB->execQuery((LPCTSTR)sSQL);
    //
    if(!q.eof())
    {
        MessageBox("There are still cars using this car type!\nYou must delete them before removing this car type.","TrainOps Error",MB_ICONSTOP);
        return;
    }
    q.finalize();
    sSQL.Format("DELETE FROM CarTypes WHERE id=%d;",iCarType);
    pDB->execDML((LPCTSTR)sSQL);
    //
    ReadDB();
}

void CCarTypesDialog::OnOK() 
{
	// TODO: Add extra validation here
    //  set active flags
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    for( int i=0;i<m_ctlCarTypes.GetItemCount();i++ )
    {
        CString sSQL;
        CString sTypeID = m_ctlCarTypes.GetItemText(i,0);
        if( m_ctlCarTypes.GetCheck(i) )
            sSQL.Format("UPDATE CarTypes SET active=1 WHERE type_id=\"%s\"",sTypeID);
        else
            sSQL.Format("UPDATE CarTypes SET active=0 WHERE type_id=\"%s\"",sTypeID);
        pDB->execDML(sSQL);
    }
	CDialog::OnOK();
}

void CCarTypesDialog::OnEditButton() 
{
    int iSel = m_ctlCarTypes.GetSelectionMark();
    if( iSel < 0 )
        return;
    int iCarType = m_ctlCarTypes.GetItemData(iSel);
	// TODO: Add your control notification handler code here
    CCarTypesDetailDialog pDlg;
    pDlg.SetCarType(iCarType);
    //
    if( pDlg.DoModal() == IDOK )
        ReadDB();
}

void CCarTypesDialog::OnClickCartypeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int iSel = m_ctlCarTypes.GetSelectionMark();
    if( iSel < 0 )
    {
        m_ctlEditButton.EnableWindow(FALSE);
        m_ctlDeleteButton.EnableWindow(FALSE);
    }
    else
    {
        m_ctlEditButton.EnableWindow(TRUE);
        m_ctlDeleteButton.EnableWindow(TRUE);
    }
	
	*pResult = 0;
}

void CCarTypesDialog::OnDblclkCartypeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int iSel = m_ctlCarTypes.GetSelectionMark();
    if( iSel < 0 )
        return;
    int iCarType = m_ctlCarTypes.GetItemData(iSel);
	// TODO: Add your control notification handler code here
    CCarTypesDetailDialog pDlg;
    pDlg.SetCarType(iCarType);
    //
    if( pDlg.DoModal() == IDOK )
    {
    	*pResult = 1;
        ReadDB();
        return;
    }
	*pResult = 0;
}
