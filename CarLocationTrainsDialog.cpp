// CarLocationTrainsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CarLocationTrainsDialog.h"

#include "Reports.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarLocationTrainsDialog dialog


CCarLocationTrainsDialog::CCarLocationTrainsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCarLocationTrainsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCarLocationTrainsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCarLocationTrainsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarLocationTrainsDialog)
	DDX_Control(pDX, IDC_REPORTSORTEDBUTTON, m_ctlReportSortedButton);
	DDX_Control(pDX, IDC_REPORTBUTTON, m_ctlReportButton);
	DDX_Control(pDX, IDC_TRAINLIST, m_ctlTrainList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCarLocationTrainsDialog, CDialog)
	//{{AFX_MSG_MAP(CCarLocationTrainsDialog)
	ON_BN_CLICKED(IDC_REPORTBUTTON, OnReportbutton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRAINLIST, OnItemchangedTrainlist)
	ON_BN_CLICKED(IDC_REPORTSORTEDBUTTON, OnReportSortedButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarLocationTrainsDialog message handlers

BOOL CCarLocationTrainsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlTrainList.SetExtendedStyle(m_ctlTrainList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    //m_ctlTrainList.SetExtendedStyle(m_ctlTrainList.GetExtendedStyle()|LVS_EX_CHECKBOXES);
    m_ctlTrainList.InsertColumn(0,"Train ID");
    m_ctlTrainList.InsertColumn(1,"Name");
    m_ctlTrainList.SetColumnWidth(0,50);
    m_ctlTrainList.SetColumnWidth(1,150);
    //
    m_ctlTrainList.DeleteAllItems();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT Trains_Scheduled.id,name,trainid FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.Trains_FK=Trains.id AND train_run=0;");
    //
    while (!q.eof() )
    {
        int nItem = m_ctlTrainList.InsertItem(1,q.fieldValue(2));
        m_ctlTrainList.SetItemText(nItem,1,q.fieldValue(1));
        m_ctlTrainList.SetItemData(nItem,q.getIntField(0));
        //
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlReportButton.EnableWindow(FALSE);
    m_ctlReportSortedButton.EnableWindow(TRUE);
    //
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCarLocationTrainsDialog::OnItemchangedTrainlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    m_ctlReportButton.EnableWindow(TRUE);
    m_ctlReportSortedButton.EnableWindow(TRUE);
	*pResult = 0;
}

void CCarLocationTrainsDialog::OnReportbutton() 
{
    int iTrainFK = m_ctlTrainList.GetItemData(m_ctlTrainList.GetNextItem(-1, LVNI_SELECTED));
    //m_ctlReportButton.EnableWindow(FALSE);
    //m_ctlReportSortedButton.EnableWindow(FALSE);
    DoCarLocationReport(iTrainFK);
}

void CCarLocationTrainsDialog::OnReportSortedButton() 
{
    int iTrainFK = m_ctlTrainList.GetItemData(m_ctlTrainList.GetNextItem(-1, LVNI_SELECTED));
    //m_ctlReportButton.EnableWindow(FALSE);
    //m_ctlReportSortedButton.EnableWindow(FALSE);
    DoCarLocationSortedReport(iTrainFK);
}
