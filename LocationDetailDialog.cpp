// LocationDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "LocationDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocationDetailDialog dialog


CLocationDetailDialog::CLocationDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLocationDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocationDetailDialog)
	m_sLocationName = _T("");
	m_sLocalInstructions = _T("");
	//}}AFX_DATA_INIT
    m_iLocationID = -1;
}


void CLocationDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocationDetailDialog)
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Text(pDX, IDC_LOCATIONNAME, m_sLocationName);
	DDX_Text(pDX, IDC_LOCALINSTRUCTIONS, m_sLocalInstructions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocationDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CLocationDetailDialog)
	ON_EN_CHANGE(IDC_LOCATIONNAME, OnChangeLocationname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocationDetailDialog message handlers

BOOL CLocationDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    if( m_iLocationID == -1 )
        m_ctlOK.EnableWindow(FALSE);
    else
    {
        CString sSQL;
        sSQL.Format("SELECT name,notes FROM Locations WHERE id=%d;",m_iLocationID);
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CppSQLite3Query q = pDB->execQuery(sSQL);
        //
        if (!q.eof())
        {
            m_sLocationName = q.getStringField("name");
            m_sLocalInstructions = q.getStringField("notes");
        }
        q.finalize();
        UpdateData(FALSE);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLocationDetailDialog::OnChangeLocationname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    UpdateData();
    if( m_sLocationName.IsEmpty() )
        m_ctlOK.EnableWindow(FALSE);
    else
        m_ctlOK.EnableWindow(TRUE);
}

void CLocationDetailDialog::OnOK() 
{
	// TODO: Add extra validation here
    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    if( m_iLocationID == -1 )
        sSQL.Format("INSERT INTO Locations (name,notes) VALUES (\"%s\",\"%s\")",m_sLocationName,m_sLocalInstructions);
    else
        sSQL.Format("UPDATE Locations SET name=\"%s\", notes=\"%s\" WHERE id=%d",m_sLocationName,m_sLocalInstructions,m_iLocationID);
    pDB->execQuery(sSQL);
	
	CDialog::OnOK();
}
