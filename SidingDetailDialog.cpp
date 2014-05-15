// SidingDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "SidingDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSidingDetailDialog dialog


CSidingDetailDialog::CSidingDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSidingDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSidingDetailDialog)
	m_iCapacity = 0;
	m_sLocation = _T("");
	m_sSidingName = _T("");
	m_bGetEmpties = FALSE;
	m_bInterchange = FALSE;
	//}}AFX_DATA_INIT
    m_iSidingID = -1;
    m_sSidingLocation = "";
}


void CSidingDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSidingDetailDialog)
	DDX_Control(pDX, IDC_ISYARD, m_ctlIsYard);
	DDX_Control(pDX, IDC_LOCATIONCOMBO, m_ctlLocationCombo);
	DDX_Text(pDX, IDC_CAPACITY, m_iCapacity);
	DDX_CBString(pDX, IDC_LOCATIONCOMBO, m_sLocation);
	DDX_Text(pDX, IDC_SIDINGNAME, m_sSidingName);
	DDX_Check(pDX, IDC_EMPTIES, m_bGetEmpties);
	DDX_Check(pDX, IDC_ISYARD, m_bIsYard);
	DDX_Check(pDX, IDC_INTERCHANGE, m_bInterchange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSidingDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CSidingDetailDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSidingDetailDialog message handlers


BOOL CSidingDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctlIsYard.SetTitleStyle(BS_AUTOCHECKBOX);
	m_ctlIsYard.SetGroupID(0);

    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,name from Locations;");
    //
    while (!q.eof())
    {
        int nIndex = m_ctlLocationCombo.AddString(q.fieldValue(1));
        m_ctlLocationCombo.SetItemData(nIndex,atoi(q.fieldValue(0)));
        q.nextRow();
    }
    q.finalize();
    
    if( m_iSidingID != -1 )
    {
        //
        //  load siding from db
        //
        CString sSQL;
        sSQL.Format("SELECT * FROM Sidings WHERE id=%d;",m_iSidingID);
        CppSQLite3Query q = pDB->execQuery(sSQL);
        //
        if (!q.eof())
        {
            m_sSidingName = q.getStringField("name");
            m_iCapacity = q.getIntField("capacity");
            m_bIsYard = (q.getIntField("yard")==1);
            m_bGetEmpties = (q.getIntField("getempties")==1);
            m_bInterchange = (q.getIntField("interchange")==1);

            sSQL.Format("SELECT name FROM Locations WHERE id=%d",q.getIntField("Locations_FK"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if (!q2.eof())
            {
                m_sLocation = q2.getStringField("name");
            }
            q2.finalize();
        }
        q.finalize();
    }
    else
    {
        m_sSidingName = "New Siding";
        if( !m_sSidingLocation.IsEmpty() )
        {
            m_sSidingName += " at ";
            m_sSidingName += m_sSidingLocation;
        }
        m_iCapacity = 100;
        if( m_sSidingLocation.IsEmpty() )
            m_ctlLocationCombo.SetCurSel(0);
        else
            m_ctlLocationCombo.SelectString(0,m_sSidingLocation);
    }

    UpdateData(FALSE);
        
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSidingDetailDialog::SetSidingID(int iSidingID)
{
    m_iSidingID = iSidingID;
}

void CSidingDetailDialog::SetSidingDefault()
{
    m_iSidingID = -1;
    //UpdateData(FALSE);
}

void CSidingDetailDialog::OnOK() 
{
    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    int nIndex = m_ctlLocationCombo.GetCurSel();
    int iLocationFK = m_ctlLocationCombo.GetItemData(nIndex);
    m_ctlLocationCombo.GetLBText(nIndex,m_sSidingLocation);
    if( iLocationFK == -1 )
    {
    }
    //
    //  save back into the database
    //
    if( m_iSidingID == -1 )
    {
        //  new
        CString sSQL;
        sSQL.Format("INSERT INTO Sidings (Locations_FK,name,capacity,yard,getempties,interchange) VALUES (%d,\"%s\",%d,%d,%d,%d);",iLocationFK,m_sSidingName,m_iCapacity,m_bIsYard?1:0,m_bGetEmpties?1:0,m_bInterchange?1:0);
        //MessageBox(sSQL);
        pDB->execQuery(sSQL);
    }
    else
    {
        //  update
        CString sSQL;
        sSQL.Format("UPDATE sidings SET Locations_FK=%d,name=\"%s\",capacity=%d,yard=%d,getempties=%d,interchange=%d WHERE id=%d;",iLocationFK,m_sSidingName,m_iCapacity,m_bIsYard?1:0,m_bGetEmpties?1:0,m_bInterchange?1:0,m_iSidingID);
        //MessageBox(sSQL);
        pDB->execQuery(sSQL);
    }
	CDialog::OnOK();
}
