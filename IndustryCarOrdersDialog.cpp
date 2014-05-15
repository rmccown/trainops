// IndustryCarOrdersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "IndustryCarOrdersDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryCarOrdersDialog dialog


CIndustryCarOrdersDialog::CIndustryCarOrdersDialog(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CIndustryCarOrdersDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryCarOrdersDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndustryCarOrdersDialog::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryCarOrdersDialog)
	DDX_Control(pDX, IDC_INDUSTRYCARORDERTREE, m_ctlIndustryCarOrderTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryCarOrdersDialog, CResizableDialog)
	//{{AFX_MSG_MAP(CIndustryCarOrdersDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryCarOrdersDialog message handlers

BOOL CIndustryCarOrdersDialog::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

    AddAnchor(IDOK, TOP_RIGHT);
    AddAnchor(IDC_INDUSTRYCARORDERTREE, TOP_LEFT, BOTTOM_RIGHT);

    CRect rc;
	GetDesktopWindow()->GetClientRect(&rc);
    SetMaxTrackSize(CSize(275, rc.Height()-100));
	
    HTREEITEM hLocationRoot = m_ctlIndustryCarOrderTree.InsertItem("Car Orders for "+((CTrainOpsApp*)AfxGetApp())->GetDateString());

    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT * FROM Industries ORDER BY name;");
    //
    while (!q.eof())
    {
        HTREEITEM pIndustry = m_ctlIndustryCarOrderTree.InsertItem(q.getStringField("name"),hLocationRoot);
        //
        CString sSQL;
        sSQL.Format("SELECT * FROM Industries_Car_Orders WHERE Industries_FK=%d;",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while( !q2.eof() )
        {
            sSQL.Format("SELECT description FROM CarTypes WHERE id=%d;",q2.getIntField("CarTypes_FK"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            if( !q3.eof() )
            {
                CString sText;
                sText.Format("%s (%s)",q3.getStringField("description"),q2.getIntField("Trains_Scheduled_FK")<0?"Unfilled":"On Train");
                HTREEITEM pCarOrder = m_ctlIndustryCarOrderTree.InsertItem(sText,pIndustry);
                if( q2.getIntField("Trains_Scheduled_FK")<0 )
                    m_ctlIndustryCarOrderTree.SetItemColor(pCarOrder,RGB(255,0,0));
            }
            q2.nextRow();
        }
        q2.finalize();
        //
        m_ctlIndustryCarOrderTree.Expand(pIndustry,TVE_EXPAND);
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlIndustryCarOrderTree.Expand(hLocationRoot,TVE_EXPAND);
    //
    m_ctlIndustryCarOrderTree.EnsureVisible(hLocationRoot);

    TBTreeStateMgr::LoadTreeState(_T("IndustryCarOrders"),&m_ctlIndustryCarOrderTree);

    EnableSaveRestore(_T("IndustryCarOrdersDialog"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIndustryCarOrdersDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("IndustryCarOrders"),&m_ctlIndustryCarOrderTree);    
	
	CResizableDialog::OnOK();
}
