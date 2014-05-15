// CarTypesDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CarTypesDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarTypesDetailDialog dialog


CCarTypesDetailDialog::CCarTypesDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCarTypesDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCarTypesDetailDialog)
	m_bPassenger = FALSE;
	m_sCarTypeDescription = _T("");
	m_sCarTypeID = _T("");
	//}}AFX_DATA_INIT
    m_iCarTypeFK=-1;
}


void CCarTypesDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarTypesDetailDialog)
	DDX_Check(pDX, IDC_PASSENGER, m_bPassenger);
	DDX_Text(pDX, IDC_CARTYPE_DESCRIPTION, m_sCarTypeDescription);
	DDX_Text(pDX, IDC_CARTYPE_ID, m_sCarTypeID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCarTypesDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CCarTypesDetailDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarTypesDetailDialog message handlers

BOOL CCarTypesDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    if( m_iCarTypeFK != -1 )
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        sSQL.Format("SELECT * FROM CarTypes WHERE id=%d",m_iCarTypeFK);
        CppSQLite3Query q = pDB->execQuery((LPCTSTR)sSQL);
        if( !q.eof() )
        {
            m_sCarTypeDescription = q.getStringField("description");
            m_sCarTypeID = q.getStringField("type_id");
            m_bPassenger = q.getIntField("passenger")==1?TRUE:FALSE;
        }
        q.finalize();
    }


    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCarTypesDetailDialog::OnOK() 
{
	// TODO: Add extra validation here

    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    //  add?
    //
    if( m_iCarTypeFK == -1 )
    {
        sSQL.Format("SELECT 1 FROM CarTypes WHERE type_id=\"%s\"",m_sCarTypeID);
        CppSQLite3Query q = pDB->execQuery((LPCTSTR)sSQL);
        //
        if(!q.eof())
        {
            MessageBox("Duplcate Type ID!","LSC TrainOps Error",MB_ICONSTOP|MB_OK);
            return;
        }
        sSQL.Format("INSERT INTO CarTypes (id,type_id,description,active,passenger) VALUES (NULL,\"%s\",\"%s\",1,%d)",m_sCarTypeID,m_sCarTypeDescription,m_bPassenger?1:0);
    }
    else
    {
        sSQL.Format("UPDATE CarTypes SET type_id=\"%s\",description=\"%s\",passenger=%d WHERE id=%d",m_sCarTypeID,m_sCarTypeDescription,m_bPassenger?1:0,m_iCarTypeFK);
    }
    pDB->execDML((LPCTSTR)sSQL);
    
	CDialog::OnOK();
}
