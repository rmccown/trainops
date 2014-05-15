// LocationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "LocationDialog.h"
#include "LocationDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocationDialog dialog


CLocationDialog::CLocationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLocationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocationDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLocationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocationDialog)
	DDX_Control(pDX, IDC_EDITBUTTON, m_ctlEditButton);
	DDX_Control(pDX, IDC_DELBUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_LOCATIONLIST, m_ctlLocationList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocationDialog, CDialog)
	//{{AFX_MSG_MAP(CLocationDialog)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOCATIONLIST, OnItemchangedLocationlist)
	ON_BN_CLICKED(IDC_EDITBUTTON, OnEditbutton)
	ON_NOTIFY(NM_DBLCLK, IDC_LOCATIONLIST, OnDblclkLocationlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocationDialog message handlers

BOOL CLocationDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlLocationList.InsertColumn(0,"Location Name");
    m_ctlLocationList.SetColumnWidth(0,275);

    //m_ctlLocationList.ModifyStyle(0 ,LVS_EDITLABELS );

    ReadDB();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLocationDialog::ReadDB()
{
    m_ctlLocationList.DeleteAllItems();
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,name from Locations;");
    //
    while (!q.eof())
    {
        TRACE("%s %s\n",q.fieldValue(0),q.fieldValue(1));
        int nIndex = m_ctlLocationList.InsertItem(1,q.fieldValue(1));
        m_ctlLocationList.SetItemData(nIndex,atoi(q.fieldValue(0)));
        q.nextRow();
    }
    q.finalize();

    m_ctlEditButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);
}

void CLocationDialog::OnAddbutton() 
{
    CLocationDetailDialog pDlg;
    if( pDlg.DoModal() != IDOK )
        return;
    CString sLocationName = pDlg.GetLocationName();
    m_ctlLocationList.InsertItem(m_ctlLocationList.GetItemCount(),sLocationName);
    //
    //  insert into database
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("INSERT INTO Locations (name) VALUES (\"%s\")",sLocationName);
    pDB->execQuery(sSQL);
    //
    ReadDB();
}

void CLocationDialog::OnDelbutton() 
{
    if( MessageBox("Deleting a Location may have adverse effects on sidings, industries, trains, and.  Are you sure you want to do this?","Delete Location?",MB_YESNO) == IDNO )
        return;
    int iSel = m_ctlLocationList.GetSelectionMark();
    int iID = m_ctlLocationList.GetItemData(iSel);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Locations WHERE id=%d;",iID);
    pDB->execDML(sSQL);
    ReadDB();
}

void CLocationDialog::OnItemchangedLocationlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
    if( m_ctlLocationList.GetSelectedCount() == 0 )
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

void CLocationDialog::OnEditbutton() 
{
	// TODO: Add your control notification handler code here
    LRESULT pResult;
    OnDblclkLocationlist(0,&pResult);
}

void CLocationDialog::OnDblclkLocationlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int iSel = m_ctlLocationList.GetSelectionMark();
    if( iSel >= 0 )
    {
        CLocationDetailDialog pDlg;
        int iID = m_ctlLocationList.GetItemData(iSel);
//        pDlg.SetLocationName(m_ctlLocationList.GetItemText(iSel,0));
        if( pDlg.DoModal() != IDOK )
            return;
        CString sLocationName = pDlg.GetLocationName();
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        sSQL.Format("update Locations set name=\"%s\" where id=%d",sLocationName,iID,0);
        pDB->execQuery(sSQL);
        //
        ReadDB();
    }
	
	*pResult = 0;
}
