// TrainsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "TrainsDialog.h"

#include "EditTrainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainsDialog dialog


CTrainsDialog::CTrainsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainsDialog)
	//}}AFX_DATA_INIT
}


void CTrainsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainsDialog)
	DDX_Control(pDX, IDC_NEWTRAIN, m_ctlNewTrain);
	DDX_Control(pDX, IDC_EDITTRAIN, m_ctlEditTrain);
	DDX_Control(pDX, IDC_DELETETRAIN, m_ctlDeleteTrain);
	DDX_Control(pDX, IDC_TRAINS, m_ctlTrains);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainsDialog, CDialog)
	//{{AFX_MSG_MAP(CTrainsDialog)
	ON_BN_CLICKED(IDC_EDITTRAIN, OnEditTrain)
	ON_BN_CLICKED(IDC_DELETETRAIN, OnDeleteTrain)
	ON_BN_CLICKED(IDC_NEWTRAIN, OnNewTrain)
	ON_NOTIFY(NM_CLICK, IDC_TRAINS, OnClickTrains)
	ON_NOTIFY(NM_DBLCLK, IDC_TRAINS, OnDblclkTrains)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainsDialog message handlers

BOOL CTrainsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    ReadDB();

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrainsDialog::ReadDB()
{
    m_ctlTrains.DeleteAllItems();
    HTREEITEM pRoot = m_ctlTrains.InsertItem(g_sRRName);
    //
    //  Read data
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL = "SELECT Trains.id,Trains.trainid,Trains.name FROM Trains ORDER BY trainid;";
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    while (!q.eof())
    {
        //
        CString sText;
        sText.Format("%s (%s)",q.getStringField(2),q.getStringField(1));
        HTREEITEM pItem = m_ctlTrains.InsertItem(sText,pRoot);
        m_ctlTrains.SetItemData(pItem,q.getIntField(0));
        //
        //  locations
        //
//        sSQL.Format("SELECT Trains_Location.id,Locations.name FROM Trains_Location, Locations WHERE Train_FK=%d AND Locations.id = Trains_Location.Location_FK ORDER BY route_order",q.getIntField(0));
        sSQL.Format("SELECT Trains_Location.id,Locations.name,Locations.id FROM Trains_Location, Locations WHERE Train_FK=%d AND Locations.id = Trains_Location.Location_FK ORDER BY route_order",q.getIntField(0));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( q2.eof() )
        {
            m_ctlTrains.InsertItem("No locations",pItem);
        }
        else
        {
            while (!q2.eof())
            {
                HTREEITEM pSubItem = m_ctlTrains.InsertItem(q2.getStringField(1),pItem);
                //m_ctlTrains.SetItemData(pSubItem,q2.getIntField(0));
                //
                //  Industries for this locatoin
                //
                sSQL.Format("SELECT name FROM Trains_Industry,Industries WHERE Trains_Industry.Industry_FK=Industries.id AND Train_FK=%d AND Location_FK=%d;",q.getIntField(0),q2.getIntField(2));
                CppSQLite3Query q3 = pDB->execQuery(sSQL);
                if( q3.eof() )
                {
                    m_ctlTrains.InsertItem("No industries switched here",pSubItem);
                }
                else
                {
                    while (!q3.eof())
                    {
                        m_ctlTrains.InsertItem(q3.getStringField(0),pSubItem);
                        q3.nextRow();
                    }
                }
                q3.finalize();
                //
                q2.nextRow();
                //m_ctlTrains.Expand(pSubItem,TVE_EXPAND);
            }
        }
        //m_ctlTrains.Expand(pItem,TVE_EXPAND);
        q2.finalize();
        //
        q.nextRow();
    }
    q.finalize();
    //
    //m_ctlTrains.Expand(pRoot,TVE_EXPAND);
    //
	m_ctlNewTrain.EnableWindow(FALSE);
	m_ctlEditTrain.EnableWindow(FALSE);
	m_ctlDeleteTrain.EnableWindow(FALSE);

    TBTreeStateMgr::LoadTreeState(_T("TrainsDialog"),&m_ctlTrains);
}


void CTrainsDialog::OnEditTrain() 
{
	// TODO: Add your control notification handler code here
    //
    //  get currently selected train, and edit it
    //
    HTREEITEM pItem = m_ctlTrains.GetSelectedItem();
    if( !pItem )
        return;
    //
	CEditTrainDialog pDlg;
    pDlg.SetTrainID(m_ctlTrains.GetItemData(pItem));
    pDlg.DoModal();
    //
    TBTreeStateMgr::SaveTreeState(_T("TrainsDialog"),&m_ctlTrains); 
    //
    ReadDB();
}

void CTrainsDialog::OnDeleteTrain() 
{
    HTREEITEM pItem = m_ctlTrains.GetSelectedItem();
    if( !pItem )
        return;
    CString sTrainName = m_ctlTrains.GetItemText(pItem);
    if( sTrainName.IsEmpty() )
        return;
	// TODO: Add your control notification handler code here
    CString sMsg;
    sMsg.Format("Are you sure you want to delete train '%s'?",sTrainName);
	// TODO: Add your control notification handler code here
	if( MessageBox(sMsg,"LSC TrainOps",MB_YESNO) == IDNO )
        return;
    int iTrainFK = m_ctlTrains.GetItemData(pItem);
    //
    //  delete industries, locations, and train record
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Trains_Location WHERE Train_FK=%d;",iTrainFK);
    pDB->execDML(sSQL);
    sSQL.Format("DELETE FROM Trains_Industry WHERE Train_FK=%d;",iTrainFK);
    pDB->execDML(sSQL);
    sSQL.Format("DELETE FROM Trains WHERE id=%d",iTrainFK);
    pDB->execDML(sSQL);
    MessageBox("Train Deleted!","LSC TrainOps");
    TBTreeStateMgr::SaveTreeState(_T("TrainsDialog"),&m_ctlTrains); 
    //
    ReadDB();
}

void CTrainsDialog::OnNewTrain() 
{
	// TODO: Add your control notification handler code here
	CEditTrainDialog pDlg;
    pDlg.SetTrainID(-1);
    pDlg.DoModal();
    //
    TBTreeStateMgr::SaveTreeState(_T("TrainsDialog"),&m_ctlTrains); 
    //
    ReadDB();
}

void CTrainsDialog::OnClickTrains(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_ctlNewTrain.EnableWindow(FALSE);
	m_ctlEditTrain.EnableWindow(FALSE);
	m_ctlDeleteTrain.EnableWindow(FALSE);
	
    UINT flags;
    CPoint ptScreen;
    if (!GetCursorPos(&ptScreen))
        return;
    CPoint ptClient(ptScreen);
    m_ctlTrains.ScreenToClient(&ptClient);
    HTREEITEM pItem = m_ctlTrains.HitTest(ptClient, &flags);
    if( !pItem )
        return;

    m_ctlTrains.SelectItem(pItem);
    
    HTREEITEM pParent = m_ctlTrains.GetParentItem(pItem);
    if( pParent == NULL )
    {
        m_ctlNewTrain.EnableWindow(TRUE);
    }
    else
    {
        HTREEITEM pParent2 = m_ctlTrains.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
            m_ctlEditTrain.EnableWindow(TRUE);
            m_ctlDeleteTrain.EnableWindow(TRUE);
        }
    }

	*pResult = 0;
}

void CTrainsDialog::OnDblclkTrains(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    UINT flags;
    CPoint ptScreen;
    if (!GetCursorPos(&ptScreen))
        return;
    CPoint ptClient(ptScreen);
    m_ctlTrains.ScreenToClient(&ptClient);
    HTREEITEM pItem = m_ctlTrains.HitTest(ptClient, &flags);
    if( !pItem )
        return;

    m_ctlTrains.SelectItem(pItem);
    
    HTREEITEM pParent = m_ctlTrains.GetParentItem(pItem);
    if( pParent == NULL )
    {
        ;
    }
    else
    {
        HTREEITEM pParent2 = m_ctlTrains.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
        	CEditTrainDialog pDlg;
            pDlg.SetTrainID(m_ctlTrains.GetItemData(pItem));
            pDlg.DoModal();
            //
            *pResult = 1;
            TBTreeStateMgr::SaveTreeState(_T("TrainsDialog"),&m_ctlTrains); 
            //
            ReadDB();
            return;
        }
    }
	
	*pResult = 0;
}

void CTrainsDialog::OnOK() 
{
	// TODO: Add extra validation here
    TBTreeStateMgr::SaveTreeState(_T("TrainsDialog"),&m_ctlTrains); 
    //
	CDialog::OnOK();
}
