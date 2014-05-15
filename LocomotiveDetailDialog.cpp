// LocomotiveDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "LocomotiveDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocomotiveDetailDialog dialog


CLocomotiveDetailDialog::CLocomotiveDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLocomotiveDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocomotiveDetailDialog)
	m_sRoadName = _T("");
	m_sRoadNumber = _T("");
	m_iCarCapacity = 1;
	m_sDescription = _T("");
	m_bActive = FALSE;
	m_sNotes = _T("");
	m_bChangeLastMaint = FALSE;
	//}}AFX_DATA_INIT
    m_iLocoFK =0;
}


void CLocomotiveDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocomotiveDetailDialog)
	DDX_Control(pDX, IDOK, m_ctlOKButton);
	DDX_Control(pDX, IDC_MAINTDATE, m_ctlMaintDate);
	DDX_Text(pDX, IDC_ROADNAME, m_sRoadName);
	DDX_Text(pDX, IDC_NUMBER, m_sRoadNumber);
	DDX_Text(pDX, IDC_CARCAPACITY, m_iCarCapacity);
	DDV_MinMaxInt(pDX, m_iCarCapacity, 1, 100);
	DDX_Text(pDX, IDC_DESCRIPTION, m_sDescription);
	DDX_Check(pDX, IDC_ACTIVE, m_bActive);
	DDX_Text(pDX, IDC_NOTES, m_sNotes);
	DDX_DateTimeCtrl(pDX, IDC_MAINTDATE, m_pMaintDate);
	DDX_Check(pDX, IDC_LASTMAINT, m_bChangeLastMaint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocomotiveDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CLocomotiveDetailDialog)
	ON_EN_CHANGE(IDC_CARCAPACITY, OnChangeCarcapacity)
	ON_EN_CHANGE(IDC_DESCRIPTION, OnChangeDescription)
	ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
	ON_EN_CHANGE(IDC_ROADNAME, OnChangeRoadname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocomotiveDetailDialog message handlers

BOOL CLocomotiveDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    if( m_iLocoFK != 0 )
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        sSQL.Format("SELECT * FROM Locomotives WHERE id=%d;",m_iLocoFK);
        CppSQLite3Query q = pDB->execQuery(sSQL);
        //
        if (!q.eof())
        {
            m_sRoadNumber = q.getStringField("car_number");
            m_sRoadName = q.getStringField("road_name");
            m_sDescription = q.getStringField("description");
            m_iCarCapacity = q.getIntField("capacity");
            m_bActive = q.getIntField("active") == 1?TRUE:FALSE;
            m_sNotes = q.getStringField("notes");
            //
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

        }
        q.finalize();
    }
    else
    {
    }

    UpdateData(FALSE);

    CheckButtons();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLocomotiveDetailDialog::OnOK() 
{
    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    if( m_iLocoFK == 0 )
    {
        //  insert
        sSQL.Format("INSERT INTO Locomotives (road_name,car_number,capacity) VALUES (\"%s\",\"%s\",%d);",m_sRoadName,m_sRoadNumber,m_iCarCapacity);
        pDB->execDML(sSQL);
        sSQL = "SELECT last_insert_rowid()";
        CppSQLite3Query q = pDB->execQuery(sSQL);   
        m_iLocoFK = atoi(q.fieldValue(0));
    }
    sSQL.Format("UPDATE Locomotives SET car_number=\"%s\" WHERE id=%d;",m_sRoadNumber,m_iLocoFK);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Locomotives SET road_name=\"%s\" WHERE id=%d;",m_sRoadName,m_iLocoFK);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Locomotives SET description=\"%s\" WHERE id=%d;",m_sDescription,m_iLocoFK);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Locomotives SET capacity=%d WHERE id=%d;",m_iCarCapacity,m_iLocoFK);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Locomotives SET active=%d WHERE id=%d;",m_bActive?1:0,m_iLocoFK);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Locomotives SET notes=\"%s\" WHERE id=%d;",m_sNotes,m_iLocoFK);
    pDB->execDML(sSQL);
    if( m_bChangeLastMaint )
    {
        CTime pMaintDate;
        m_ctlMaintDate.GetTime(pMaintDate);
        int iTime = pMaintDate.GetTime();
        sSQL.Format("UPDATE Locomotives SET maintdate=%d WHERE id=%d;",iTime,m_iLocoFK);
        pDB->execDML(sSQL);
    }
	
	CDialog::OnOK();
}


void CLocomotiveDetailDialog::CheckButtons()
{
    UpdateData();
    //
    if( m_sRoadNumber.IsEmpty() ||
        m_sRoadName.IsEmpty() ||
        m_sDescription.IsEmpty() ||
        m_iCarCapacity == 0 )
    {
        m_ctlOKButton.EnableWindow(FALSE);
    }
    else
    {
        m_ctlOKButton.EnableWindow(TRUE);
    }
}

void CLocomotiveDetailDialog::OnChangeCarcapacity() 
{
    CheckButtons();
}

void CLocomotiveDetailDialog::OnChangeDescription() 
{
    CheckButtons();
}

void CLocomotiveDetailDialog::OnChangeNumber() 
{
    CheckButtons();
}

void CLocomotiveDetailDialog::OnChangeRoadname() 
{
    CheckButtons();
}
