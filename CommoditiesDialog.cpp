// CommoditiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CommoditiesDialog.h"
#include "EditCommodityDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommoditiesDialog dialog


CCommoditiesDialog::CCommoditiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCommoditiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommoditiesDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCommoditiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommoditiesDialog)
	DDX_Control(pDX, IDC_COMMODITYLIST, m_ctlCommodityList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommoditiesDialog, CDialog)
	//{{AFX_MSG_MAP(CCommoditiesDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_COMMODITYLIST, OnDblclkCommoditylist)
	ON_BN_CLICKED(IDC_EDITBUTTON, OnEditbutton)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeletebutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommoditiesDialog message handlers

BOOL CCommoditiesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlCommodityList.InsertColumn(0,"Commodity",LVCFMT_LEFT,100);
    //
    ReadDB();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommoditiesDialog::ReadDB()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Commodities;");
    //
    m_ctlCommodityList.DeleteAllItems();
    //
    while (!q.eof())
    {
        int nIndex = m_ctlCommodityList.InsertItem(0,q.getStringField("name"));
        m_ctlCommodityList.SetItemData(nIndex,q.getIntField("id"));
        q.nextRow();
    }
}

void CCommoditiesDialog::OnDblclkCommoditylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditbutton();

	*pResult = 0;
}

void CCommoditiesDialog::OnEditbutton() 
{
    int iSel = m_ctlCommodityList.GetSelectionMark();
    if( iSel >= 0 )
    {
        CEditCommodityDialog pDlg;
        pDlg.SetCommodityID(m_ctlCommodityList.GetItemData(iSel));
        pDlg.DoModal();	
    }
    ReadDB();
}

void CCommoditiesDialog::OnAddbutton() 
{
    CEditCommodityDialog pDlg;	
    pDlg.DoModal();	
    ReadDB();
}

void CCommoditiesDialog::OnDeletebutton() 
{
	// TODO: Add your control notification handler code here
    if( MessageBox("Are you sure you want to delete this commodity?","TrainOps Question",MB_YESNO) != IDYES )
        return;
    int iSel = m_ctlCommodityList.GetSelectionMark();
    if( iSel >= 0 )
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        sSQL.Format("DELETE FROM Commodities WHERE id=%d;",m_ctlCommodityList.GetItemData(iSel));
        pDB->execDML(sSQL);
        ReadDB();
        MessageBox("Commodity Deleted!");
    }
}
