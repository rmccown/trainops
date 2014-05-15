// IndustryCarDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustryCarDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryCarDetailDialog dialog


CIndustryCarDetailDialog::CIndustryCarDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustryCarDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryCarDetailDialog)
	m_iLoadEmpty = 1;
	m_iPercent = 100;
	m_iQuanHigh = 1;
	m_iQuanLow = 0;
	m_sLoadEmptyText = _T("");
	//}}AFX_DATA_INIT
    m_bIn = TRUE;
    m_bPassenger = FALSE;
}


void CIndustryCarDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryCarDetailDialog)
	DDX_Control(pDX, IDC_PERCENT, m_ctlPercent);
	DDX_Control(pDX, IDC_QUANHIGH, m_ctlQuanHigh);
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Text(pDX, IDC_LOADEMPTY, m_iLoadEmpty);
	DDX_Text(pDX, IDC_PERCENT, m_iPercent);
	DDX_Text(pDX, IDC_QUANHIGH, m_iQuanHigh);
	DDX_Text(pDX, IDC_QUANLOW, m_iQuanLow);
	DDX_Text(pDX, IDC_LOADEMPTYTEXT, m_sLoadEmptyText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryCarDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CIndustryCarDetailDialog)
	ON_EN_CHANGE(IDC_LOADEMPTY, OnChangeLoadempty)
	ON_EN_CHANGE(IDC_PERCENT, OnChangePercent)
	ON_EN_CHANGE(IDC_QUANHIGH, OnChangeQuanhigh)
	ON_EN_CHANGE(IDC_QUANLOW, OnChangeQuanlow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryCarDetailDialog message handlers

BOOL CIndustryCarDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    if( m_bIn )
        m_sLoadEmptyText = "Time to empty car (days):";
    else
        m_sLoadEmptyText = "Time to load car (days):";

    UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIndustryCarDetailDialog::OnOK() 
{
    UpdateData();
    //
	CDialog::OnOK();
}

void CIndustryCarDetailDialog::OnChangeLoadempty() 
{
	// TODO: Add your control notification handler code here
    UpdateData();
    int iLow = 1;
    if( m_bPassenger )
        iLow = 0;
    if(m_iLoadEmpty < iLow || m_iLoadEmpty > 14 )
    {
        m_iLoadEmpty = 1;
        m_ctlOK.EnableWindow(TRUE);
    }
    else
        m_ctlOK.EnableWindow(TRUE);
    //
    UpdateData(FALSE);
}

void CIndustryCarDetailDialog::OnChangePercent() 
{
	// TODO: Add your control notification handler code here
    UpdateData();	
    if(m_iPercent < 1 || m_iPercent > 100 )
    {
        m_iPercent = 100;
        m_ctlOK.EnableWindow(TRUE);
    }
    else
        m_ctlOK.EnableWindow(TRUE);
    //
    UpdateData(FALSE);
}

void CIndustryCarDetailDialog::OnChangeQuanhigh() 
{
	// TODO: Add your control notification handler code here
    UpdateData();	
    if(m_iQuanHigh < m_iQuanLow || m_iQuanHigh < 1 )
    {
        m_iQuanHigh = m_iQuanLow + 1;
        m_ctlOK.EnableWindow(TRUE);
    }
    else
        m_ctlOK.EnableWindow(TRUE);
    //
    UpdateData(FALSE);
}

void CIndustryCarDetailDialog::OnChangeQuanlow() 
{
	// TODO: Add your control notification handler code here
    UpdateData();	
    if(m_iQuanHigh < m_iQuanLow || m_iQuanLow < 1 )
    {
        m_iQuanHigh = m_iQuanLow + 1;
        m_ctlOK.EnableWindow(TRUE);
    }
    else
        m_ctlOK.EnableWindow(TRUE);
    //
    UpdateData(FALSE);
}
