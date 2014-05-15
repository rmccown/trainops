// EditCarDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "EditCarDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCarDialog dialog


CEditCarDialog::CEditCarDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCarDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCarDialog)
	m_sCarNumber = _T("");
	m_sReportingMarks = _T("");
	m_sCarLength = _T("");
	m_sWeight = _T("");
	m_bForeign = FALSE;
	m_pMaintDate = 0;
	m_sNotes = _T("");
	m_bOutOfService = FALSE;
	m_bChangeLastMaint = FALSE;
	m_sReturnTo = _T("");
	m_sInterchangeVia = _T("");
	//}}AFX_DATA_INIT
    m_iCarID = -1;
}


void CEditCarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCarDialog)
	DDX_Control(pDX, IDC_INTERCHANGEVIATEXT, m_ctlInterchangeViaText);
	DDX_Control(pDX, IDC_INTERCHANGEVIACOMBO, m_ctlInterchangeVia);
	DDX_Control(pDX, IDC_RETURNTOCOMBO, m_ctlReturnTo);
	DDX_Control(pDX, IDC_MAINTDATE, m_ctlMaintDate);
	DDX_Text(pDX, IDC_CARNUMBER, m_sCarNumber);
	DDX_Text(pDX, IDC_REPORTINGMARKS, m_sReportingMarks);
	DDX_Text(pDX, IDC_CARLENGTH, m_sCarLength);
	DDV_MaxChars(pDX, m_sCarLength, 2);
	DDX_Text(pDX, IDC_WEIGHT, m_sWeight);
	DDX_Check(pDX, IDC_FOREIGN, m_bForeign);
	DDX_DateTimeCtrl(pDX, IDC_MAINTDATE, m_pMaintDate);
	DDX_Text(pDX, IDC_NOTES, m_sNotes);
	DDX_Check(pDX, IDC_OUTOFSERVICE, m_bOutOfService);
	DDX_Check(pDX, IDC_LASTMAINT, m_bChangeLastMaint);
	DDX_CBString(pDX, IDC_RETURNTOCOMBO, m_sReturnTo);
	DDX_CBString(pDX, IDC_INTERCHANGEVIACOMBO, m_sInterchangeVia);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditCarDialog, CDialog)
	//{{AFX_MSG_MAP(CEditCarDialog)
	ON_BN_CLICKED(IDC_LASTMAINT, OnLastmaint)
	ON_BN_CLICKED(IDC_FOREIGN, OnForeign)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCarDialog message handlers

BOOL CEditCarDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    m_ctlMaintDate.EnableWindow(FALSE);
    //
    int nItem = m_ctlReturnTo.AddString("");
    m_ctlReturnTo.SetItemData(nItem,0);
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Industries ORDER BY name");
    while( !q.eof() )
    {
        nItem = m_ctlReturnTo.AddString(q.getStringField("name"));
        m_ctlReturnTo.SetItemData(nItem,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
    //
    q = pDB->execQuery("SELECT id,name FROM Sidings WHERE interchange=1 ORDER BY name;");
    nItem = m_ctlInterchangeVia.AddString("-Any Interchange-");
    m_ctlInterchangeVia.SetItemData(nItem,-1);
    while( !q.eof() )
    {
        nItem = m_ctlInterchangeVia.AddString(q.getStringField("name"));
        m_ctlInterchangeVia.SetItemData(nItem,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
    //
    if( m_iCarID >= 0 )
    {
        CString sSQL;
        //
        //  get car data from Cars
        //
        sSQL.Format("SELECT * FROM Cars WHERE id=%d;",m_iCarID);
        q = pDB->execQuery(sSQL);
        int iTrainScheduledFK = 0;
        //
        if (!q.eof() )
        {
            m_sReportingMarks = q.getStringField("reporting_marks");
            m_sCarNumber = q.getStringField("car_number");
            if( q.getIntField("car_length") < 1 )
                m_sCarLength.Format("%d",g_iCarLength);
            else
                m_sCarLength = q.getStringField("car_length");
            if( q.getIntField("tonnage") < 1 )
                m_sWeight.Format("%d",35);
            else
                m_sWeight = q.getStringField("tonnage");
            if( q.getIntField("foreign_road") == 1 )
            {
                m_bForeign = TRUE;
                m_ctlInterchangeViaText.EnableWindow(TRUE);
                m_ctlInterchangeVia.EnableWindow(TRUE);
                sSQL.Format("SELECT name FROM Sidings WHERE id=%d",q.getIntField("interchange_via"));
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if( !q2.eof() )
                    m_sInterchangeVia = q2.getStringField("name");
                else
                    m_sInterchangeVia = "-Any Interchange-";
                q2.finalize();
            }
            else
            {
                m_ctlInterchangeViaText.EnableWindow(FALSE);
                m_ctlInterchangeVia.EnableWindow(FALSE);
            }
            if( q.getIntField("out_of_service") == 1 )
                m_bOutOfService = TRUE;
            m_sNotes = q.getStringField("notes");
            UpdateData(FALSE);
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
            if( q.getIntField("returnto") > 0 )
            {
                for( int i=0;i<m_ctlReturnTo.GetCount();i++ )
                {
                    if( m_ctlReturnTo.GetItemData(i) == q.getIntField("returnto") )
                    {
                        m_ctlReturnTo.SetCurSel(i);
                        break;
                    }
                }
            }
        }
    }
    //
    m_ctlMaintDate.SetFormat(_T("MM-dd-yyyy"));

    //UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditCarDialog::OnOK() 
{
    UpdateData();

    if( !m_sReportingMarks.IsEmpty() && !m_sCarNumber.IsEmpty() )
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        //
        m_sNotes.Replace("\"","");
        //
        sSQL.Format("UPDATE cars SET reporting_marks=\"%s\" , car_number=\"%s\", car_length=%d, tonnage=%d, foreign_road=%d, out_of_service=%d, notes=\"%s\", returnto=%d WHERE id=%d;",
            m_sReportingMarks,
            m_sCarNumber,
            atoi((LPCTSTR)m_sCarLength),
            atoi((LPCTSTR)m_sWeight),
            m_bForeign?1:0,
            m_bOutOfService?1:0,
            m_sNotes,
            m_ctlReturnTo.GetItemData(m_ctlReturnTo.GetCurSel()),
            m_iCarID);
        pDB->execDML(sSQL);
        //
        if( m_iCarID >= 0 )
            sSQL.Format("UPDATE cars SET out_of_service=%d WHERE id=%d;",m_bOutOfService?1:0,m_iCarID);
        else
            sSQL.Format("UPDATE cars SET out_of_service=%d,Location_FK=-1,Siding_FK=-1,Industry_FK=-1 WHERE id=%d;",m_bOutOfService?1:0,m_iCarID);
        pDB->execDML(sSQL);
        //
        if( m_bChangeLastMaint )
        {
            CTime pMaintDate;
            m_ctlMaintDate.GetTime(pMaintDate);
            int iTime = pMaintDate.GetTime();
            sSQL.Format("UPDATE cars SET maintdate=%d WHERE id=%d;",iTime,m_iCarID);
            pDB->execDML(sSQL);
        }
        if( m_bForeign )
        {
            int iInterchangeVia(0);
            if( m_ctlInterchangeVia.GetItemData(m_ctlInterchangeVia.GetCurSel()) > 0 )
                iInterchangeVia = m_ctlInterchangeVia.GetItemData(m_ctlInterchangeVia.GetCurSel());
            sSQL.Format("UPDATE cars SET interchange_via=%d WHERE id=%d",iInterchangeVia,m_iCarID);
            pDB->execDML(sSQL);
        }
        else
        {
            sSQL.Format("UPDATE cars SET interchange_via=0 WHERE id=%d",m_iCarID);
            pDB->execDML(sSQL);
        }
    }

	CDialog::OnOK();
}

void CEditCarDialog::OnLastmaint() 
{
    UpdateData(TRUE);
    if( m_bChangeLastMaint )
        m_ctlMaintDate.EnableWindow(TRUE);
    else
        m_ctlMaintDate.EnableWindow(FALSE);
}

void CEditCarDialog::OnForeign() 
{
    UpdateData();
    if( m_bForeign )
    {
        m_ctlInterchangeViaText.EnableWindow(TRUE);
        m_ctlInterchangeVia.EnableWindow(TRUE);
        m_sInterchangeVia = "-Any Interchange-";
    }
    else
    {
        m_ctlInterchangeViaText.EnableWindow(FALSE);
        m_ctlInterchangeVia.EnableWindow(FALSE);
        m_sInterchangeVia = "";
    }
    UpdateData(FALSE);
}
