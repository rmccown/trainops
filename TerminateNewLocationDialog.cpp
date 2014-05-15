// TerminateNewLocationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "TerminateNewLocationDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerminateNewLocationDialog dialog


CTerminateNewLocationDialog::CTerminateNewLocationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerminateNewLocationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerminateNewLocationDialog)
	m_sIndustry = _T("");
	m_sLocation = _T("");
	m_sSiding = _T("");
	//}}AFX_DATA_INIT
}


void CTerminateNewLocationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerminateNewLocationDialog)
	DDX_Control(pDX, IDC_SIDINGLIST, m_ctlSidingList);
	DDX_Control(pDX, IDC_INDUSTRYLIST, m_ctlIndustryList);
	DDX_Control(pDX, IDC_LOCATIONLIST, m_ctlLocationList);
	DDX_LBString(pDX, IDC_INDUSTRYLIST, m_sIndustry);
	DDX_LBString(pDX, IDC_LOCATIONLIST, m_sLocation);
	DDX_LBString(pDX, IDC_SIDINGLIST, m_sSiding);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTerminateNewLocationDialog, CDialog)
	//{{AFX_MSG_MAP(CTerminateNewLocationDialog)
	ON_LBN_SELCHANGE(IDC_LOCATIONLIST, OnSelchangeLocationlist)
	ON_LBN_SELCHANGE(IDC_INDUSTRYLIST, OnSelchangeIndustrylist)
	ON_LBN_SELCHANGE(IDC_SIDINGLIST, OnSelchangeSidinglist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerminateNewLocationDialog message handlers

BOOL CTerminateNewLocationDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Locations;");
    //
    while (!q.eof())
    {
        int nIndex = m_ctlLocationList.AddString(q.getStringField("name"));
        m_ctlLocationList.SetItemData(nIndex,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTerminateNewLocationDialog::OnSelchangeLocationlist() 
{
    int iLocationID = m_ctlLocationList.GetItemData(m_ctlLocationList.GetCurSel());
    CString sSQL;
    sSQL.Format("SELECT Industries.id,Industries.name FROM Industries,Sidings WHERE Industries.Sidings_FK=Sidings.id AND Sidings.Locations_FK=%d",iLocationID);
    TRACE(sSQL);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    while( m_ctlIndustryList.GetCount() > 0 )
        m_ctlIndustryList.DeleteString(0);
    //
    while (!q.eof())
    {
        int nIndex = m_ctlIndustryList.AddString(q.getStringField("name"));
        m_ctlIndustryList.SetItemData(nIndex,q.getIntField("id"));
        q.nextRow();
    }
    //
    sSQL.Format("SELECT id,name FROM Sidings WHERE Locations_FK=%d;",iLocationID);
    q = pDB->execQuery(sSQL);
    //
    while( m_ctlSidingList.GetCount() > 0 )
        m_ctlSidingList.DeleteString(0);
    //
    while (!q.eof())
    {
        int nIndex = m_ctlSidingList.AddString(q.getStringField("name"));
        m_ctlSidingList.SetItemData(nIndex,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
}

void CTerminateNewLocationDialog::OnOK() 
{
	// TODO: Add extra validation here

    m_iLocation = m_ctlLocationList.GetItemData(m_ctlLocationList.GetCurSel());
    m_iIndustry = m_ctlIndustryList.GetItemData(m_ctlIndustryList.GetCurSel()); 
    m_iSiding = m_ctlSidingList.GetItemData(m_ctlSidingList.GetCurSel()); 

	CDialog::OnOK();
}

void CTerminateNewLocationDialog::OnSelchangeIndustrylist() 
{
    m_ctlSidingList.SetCurSel(-1);
}

void CTerminateNewLocationDialog::OnSelchangeSidinglist() 
{
    m_ctlIndustryList.SetCurSel(-1);
}
