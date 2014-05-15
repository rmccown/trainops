// EditCommodityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "EditCommodityDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCommodityDialog dialog


CEditCommodityDialog::CEditCommodityDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCommodityDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCommodityDialog)
	m_sCommodityName = _T("");
	//}}AFX_DATA_INIT
    m_iCommodityID = -1;
}


void CEditCommodityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCommodityDialog)
	DDX_Control(pDX, IDC_CARTYPES, m_ctlCarTypes);
	DDX_Text(pDX, IDC_COMMODITYNAME, m_sCommodityName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditCommodityDialog, CDialog)
	//{{AFX_MSG_MAP(CEditCommodityDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCommodityDialog message handlers

BOOL CEditCommodityDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlCarTypes.InsertColumn(0,"Car Types");
    m_ctlCarTypes.SetColumnWidth(0,130);
    m_ctlCarTypes.SetExtendedStyle(m_ctlCarTypes.GetStyle()|LVS_EX_CHECKBOXES);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,description from CarTypes WHERE active=1;");
    //
    while (!q.eof())
    {
        int nIndex = m_ctlCarTypes.InsertItem(0,q.getStringField("description"));
        m_ctlCarTypes.SetItemData(nIndex,q.getIntField("id"));
        //
        //  Get entries from Commodities_Cars
        //
        if( m_iCommodityID >= 0 )
        {
            CString sSQL;
            sSQL.Format("SELECT 1 FROM Commodities_Cars WHERE CarTypes_FK=%s AND Commodities_FK=%d;",q.getIntField("id"),m_iCommodityID);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
                m_ctlCarTypes.SetCheck(nIndex,TRUE);
        }
        q.nextRow();
    }
    q.finalize();

    if( m_iCommodityID >= 0 )
    {
        CString sSQL;
        sSQL.Format("SELECT name FROM Commodities WHERE id=%d;",m_iCommodityID);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
            m_sCommodityName = q2.getStringField("name");
        else
            m_sCommodityName = "?Error Reading Commodity?";
    }
    else
        m_sCommodityName = "New Commodity";

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditCommodityDialog::OnOK() 
{
	// TODO: Add extra validation here
    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    if( m_iCommodityID == -1 )
    {
        CString sSQL;
        sSQL.Format("INSERT INTO Commodities (name) VALUES (\"%s\");",m_sCommodityName);
        CppSQLite3Query q = pDB->execQuery(sSQL);
        q.finalize();
        sSQL = "SELECT last_insert_rowid()";
        q = pDB->execQuery(sSQL);   
        m_iCommodityID = atoi(q.fieldValue(0));
        q.finalize();
    }
    else
    {
        CString sSQL;
        sSQL.Format("UPDATE Commodities SET name=\"%s\" WHERE id=%d;",m_sCommodityName,m_iCommodityID);
        pDB->execDML(sSQL);
        sSQL.Format("DELETE FROM Commodities_Cars WHERE Commodities_FK=%d;",m_iCommodityID);
        pDB->execDML(sSQL);
    }
    for( int i=0;i<m_ctlCarTypes.GetItemCount();i++ )
    {
        if( m_ctlCarTypes.GetCheck(i) )
        {
            int iCarTypeFK = m_ctlCarTypes.GetItemData(i);
            CString sSQL;
            sSQL.Format("INSERT INTO Commodities_Cars (Commodities_FK,CarTypes_FK) VALUES (%d,%d);",m_iCommodityID,iCarTypeFK);
            pDB->execDML(sSQL);
        }
    }
    //
	CDialog::OnOK();
}