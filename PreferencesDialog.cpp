// PreferencesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "PreferencesDialog.h"

#include "julian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDialog dialog


CPreferencesDialog::CPreferencesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPreferencesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferencesDialog)
	m_iCarLength = 0;
	m_iLoadEmptyDays = 0;
	m_sReportingMarks = _T("");
	m_sRRName = _T("");
	m_iMethod = -1;
	m_iOffLayoutTime = 0;
	m_sLogoFile = _T("");
	m_bRealisticPrintout = FALSE;
	//}}AFX_DATA_INIT
}


void CPreferencesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesDialog)
	DDX_Control(pDX, IDC_DATE, m_ctlRRDate);
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Text(pDX, IDC_CARLENGTH, m_iCarLength);
	DDV_MinMaxInt(pDX, m_iCarLength, 5, 36);
	DDX_Text(pDX, IDC_LOADEMPTY, m_iLoadEmptyDays);
	DDX_Text(pDX, IDC_REPORTINGMARKS, m_sReportingMarks);
	DDX_Text(pDX, IDC_RRNAME, m_sRRName);
	DDX_Radio(pDX, IDC_METHOD, m_iMethod);
	DDX_Text(pDX, IDC_OFFLAYOUTTIME, m_iOffLayoutTime);
	DDV_MinMaxInt(pDX, m_iOffLayoutTime, 0, 365);
	DDX_Text(pDX, IDC_LOGO, m_sLogoFile);
	DDX_Check(pDX, IDC_REALISTICPRINT, m_bRealisticPrintout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreferencesDialog, CDialog)
	//{{AFX_MSG_MAP(CPreferencesDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDialog message handlers

BOOL CPreferencesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL = "SELECT rrname,reporting_marks,LoadEmptyDays,car_length,car_method,rrdate,offlayouttime,id,date,logo,realistic_reports FROM _Config WHERE id=1;";
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    if (q.eof() || (q.getIntField("id") == 1 && q.getIntField("rrdate") == -1))
    {
        m_sRRName = "<railroad>";
        m_sReportingMarks = "<rr>";
        m_iLoadEmptyDays = 1;
        m_iCarLength = 18;
        m_iMethod = METHOD_CARS;
        m_ctlRRDate.SetDate(1920,1,1);
        m_iOffLayoutTime = 1;
        m_sLogoFile = "";
        m_bRealisticPrintout = TRUE;
    }
    else
    {
        m_sRRName = q.fieldValue("rrname");
        m_sReportingMarks = q.fieldValue("reporting_marks");
        m_iLoadEmptyDays = q.getIntField("LoadEmptyDays");
        m_iCarLength = q.getIntField("car_length");
        m_iMethod = q.getIntField("car_method");
        int y,m,d;
        JulianDate::JdToYmd( q.getIntField("rrdate"), &y,&m,&d);
        m_ctlRRDate.SetDate(y,m,d);
        m_iOffLayoutTime = q.getIntField("offlayouttime");
        m_sLogoFile = q.getStringField("logo");
        m_bRealisticPrintout = q.getIntField("realistic_reports")==1?TRUE:FALSE;
    }
#ifndef _DEBUG
    GetDlgItem(IDC_RADIO2)->EnableWindow(false);
#endif
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesDialog::OnOK() 
{
	// TODO: Add extra validation here

    UpdateData(TRUE);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    long iJulDate = JulianDate::YmdToJd( m_ctlRRDate.GetYear(), m_ctlRRDate.GetMonth(), m_ctlRRDate.GetDay() );
    sSQL.Format("UPDATE _Config SET rrname=\"%s\", reporting_marks=\"%s\", LoadEmptyDays=%d,car_length=%d, car_method=%d, rrdate=%d, offlayouttime=%d , logo=\"%s\", realistic_reports=%d WHERE id=1;",m_sRRName,m_sReportingMarks,m_iLoadEmptyDays,m_iCarLength,m_iMethod,iJulDate,m_iOffLayoutTime,m_sLogoFile,m_bRealisticPrintout?1:0);
    pDB->execDML(sSQL);
    if( pDB->lastRowId() < 0 )
    {
        sSQL.Format("INSERT INTO _Config (id,rrname,reporting_marks,LoadEmptyDays,car_length,car_method,rrdate,offlayouttime,logo,realistic_reports) VALUES (1,\"%s\",\"%s\",%d,%d,%d,%d,%d,\"%s\",%d);",m_sRRName,m_sReportingMarks,m_iLoadEmptyDays,m_iCarLength,m_iMethod,iJulDate,m_iOffLayoutTime,m_sLogoFile,m_bRealisticPrintout?1:0);
        pDB->execDML(sSQL);
    }
    //
	g_iCarLength = m_iCarLength;
	g_iLoadEmptyDays = m_iLoadEmptyDays;
	g_sReportingMarks = m_sReportingMarks;
	g_sRRName = m_sRRName;
	g_iMethod = m_iMethod;
    g_iOffLayoutTime = m_iOffLayoutTime;
    g_sLogo = m_sLogoFile;
    g_bRealisticReports = m_bRealisticPrintout;
    //
	CDialog::OnOK();
}

void CPreferencesDialog::OnBrowse() 
{
	// TODO: Add your control notification handler code here
    CFileDialog pDlg(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,"PNG Images (*.png)|*.png|JPG Images (*.jpg)|*.jpg|");
    if(pDlg.DoModal() != IDOK)
        return;
    m_sLogoFile = pDlg.GetPathName();
    UpdateData(FALSE);
}
