// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "AboutDialog.h"

#include "afxinet.h"

#include "_BldNum.h"

#include "hpdf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog

CAboutDialog::CAboutDialog() : CDialog(CAboutDialog::IDD)

{
	//{{AFX_DATA_INIT(CAboutDialog)
	m_sLicense = _T("");
	m_sVersionText = _T("");
	m_sScrollText = _T("");
	m_sCopyrightText = _T("");
	//}}AFX_DATA_INIT
}


void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDialog)
	DDX_Control(pDX, IDC_SCROLLTEXT, m_ctlScrollText);
	DDX_Text(pDX, IDC_LICENSING, m_sLicense);
	DDX_Text(pDX, IDC_VERSIONTEXT, m_sVersionText);
	DDX_Text(pDX, IDC_SCROLLTEXT, m_sScrollText);
	DDX_Text(pDX, IDC_COPYRIGHTTEXT, m_sCopyrightText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	//{{AFX_MSG_MAP(CAboutDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog message handlers

BOOL CAboutDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_sVersionText.Format("LSC TrainOps Version %d.%d (Build %d)",
        FILE_VERSION0,FILE_VERSION1,BUILD_NUMBER);

    CTime t = CTime::GetCurrentTime();
    m_sCopyrightText.Format("Copyright © 2006 - %d - Large Scale Central",t.GetYear());

    CStringArray pText;
	pText.Add("TrainOps");
	pText.Add(" ");
    pText.Add(m_sCopyrightText);
	pText.Add(" ");
	pText.Add("Thanks to Bruce Chandler for all his suggestions, his tireless debugging sessions, and assistance in writing the help file.");
	pText.Add(" ");
    pText.Add("This program uses the following software libraries:");
    pText.Add("SQLite 3 Library Version ");
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    pText.Add(pDB->SQLiteVersion());
    pText.Add("LibHaru PDF Library Version ");
    pText.Add(HPDF_GetVersion());

    m_ctlScrollText.AddLine(pText);

    m_ctlScrollText.SetScrollTimer(750);

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
