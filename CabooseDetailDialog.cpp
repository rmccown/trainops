// CabooseDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CabooseDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCabooseDetailDialog dialog


CCabooseDetailDialog::CCabooseDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCabooseDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCabooseDetailDialog)
	m_sCarNumber = _T("00000");
	m_sReportingMarks = g_sReportingMarks;
	m_iLength = g_iCarLength;
	m_sNotes = _T("");
	m_bOutOfService = FALSE;
	m_bChangeLastMaint = FALSE;
	//}}AFX_DATA_INIT
    m_iCabooseFK=-1;
}


void CCabooseDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCabooseDetailDialog)
	DDX_Control(pDX, IDC_MAINTDATE, m_ctlMaintDate);
	DDX_Text(pDX, IDC_CARNUMBER, m_sCarNumber);
	DDX_Text(pDX, IDC_REPORTINGMARKS, m_sReportingMarks);
	DDX_Text(pDX, IDC_LENGTH, m_iLength);
	DDX_Text(pDX, IDC_NOTES, m_sNotes);
	DDX_Check(pDX, IDC_OUTOFSERVICE, m_bOutOfService);
	DDX_Check(pDX, IDC_LASTMAINT, m_bChangeLastMaint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCabooseDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CCabooseDetailDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCabooseDetailDialog message handlers

BOOL CCabooseDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    if( m_iCabooseFK != -1 )
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        sSQL.Format("SELECT * FROM Caboose WHERE id=%d;",m_iCabooseFK);
        CppSQLite3Query q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            m_sCarNumber = q.getStringField("car_number");
            m_iLength = q.getIntField("car_length");
            m_sReportingMarks = q.getStringField("reporting_marks");
            m_sNotes = q.getStringField("notes");
            if( q.getIntField("maintdate") > 0 )
            {
                CTime t(q.getIntField("maintdate"));
                m_ctlMaintDate.SetTime(&t);
            }
            else
            {
                CTime t = CTime::GetCurrentTime();
                m_ctlMaintDate.SetTime(&t);
            }
            if( q.getIntField("out_of_service") == 1 )
                m_bOutOfService=TRUE;

        }
        else
        {
            m_iCabooseFK=-1;
            CTime t = CTime::GetCurrentTime();
            m_ctlMaintDate.SetTime(&t);
        }
        q.finalize();
    }
    UpdateData(FALSE);
    //
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCabooseDetailDialog::OnOK() 
{
	// TODO: Add extra validation here
    UpdateData();
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    if( m_iCabooseFK == -1 )
    {
        //  insert
        sSQL.Format("INSERT INTO Caboose (reporting_marks,car_number,car_length) VALUES (\"%s\",\"%s\",%d);",m_sReportingMarks,m_sCarNumber,m_iLength);
        pDB->execDML(sSQL);
        sSQL = "SELECT last_insert_rowid()";
        CppSQLite3Query q = pDB->execQuery(sSQL);   
        m_iCabooseFK = atoi(q.fieldValue(0));
    }
    else
    {
        sSQL.Format("UPDATE Caboose SET reporting_marks=\"%s\",car_number=\"%s\",car_length=%d WHERE id=%d;",m_sReportingMarks,m_sCarNumber,m_iLength,m_iCabooseFK);
        pDB->execDML(sSQL);
        //  update
    }
    //
    sSQL.Format("UPDATE Caboose SET notes=\"%s\" WHERE id=%d",m_sNotes,m_iCabooseFK);
    pDB->execDML(sSQL);
    //
    sSQL.Format("UPDATE Caboose SET out_of_service=%d WHERE id=%d;",m_bOutOfService?1:0,m_iCabooseFK);
    pDB->execDML(sSQL);
    //
    if( m_bChangeLastMaint )
    {
        CTime pMaintDate;
        m_ctlMaintDate.GetTime(pMaintDate);
        int iTime = pMaintDate.GetTime();
        sSQL.Format("UPDATE Caboose SET maintdate=%d WHERE id=%d;",iTime,m_iCabooseFK);
        pDB->execDML(sSQL);
    }
    
	CDialog::OnOK();
}
