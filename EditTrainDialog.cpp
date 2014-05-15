// EditTrainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "EditTrainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTrainDialog dialog


CEditTrainDialog::CEditTrainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTrainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditTrainDialog)
	m_bLocalSwitcher = FALSE;
	m_sTrainCode = _T("");
	m_sTrainName = _T("");
	m_bSwitchInterchanges = FALSE;
	m_iMaxMoves = 0;
	//}}AFX_DATA_INIT
}


void CEditTrainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditTrainDialog)
	DDX_Control(pDX, IDC_MAXMOVESTEXT, m_ctlMaxMovesText);
	DDX_Control(pDX, IDC_MAXMOVES, m_ctlMaxMoves);
	DDX_Control(pDX, IDC_INTERCHANGECHECK, m_ctlSwitchInterchanges);
	DDX_Control(pDX, IDC_LOCALSWITCHER, m_ctlLocalSwitcher);
	DDX_Control(pDX, IDC_ADDBUTTON, m_ctlAddButton);
	DDX_Control(pDX, IDC_REMOVEBUTTON, m_ctlRemoveButton);
	DDX_Control(pDX, IDC_INDUSTRYLIST, m_ctlIndustryList);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_ctlDown);
	DDX_Control(pDX, IDC_UPBUTTON, m_ctlUp);
	DDX_Control(pDX, IDC_ROUTELOCATIONLIST, m_ctlRouteLocationList);
	DDX_Control(pDX, IDC_AVAILABLELOCATIONLIST, m_ctlLocationList);
	DDX_Check(pDX, IDC_LOCALSWITCHER, m_bLocalSwitcher);
	DDX_Text(pDX, IDC_ROUTECODE, m_sTrainCode);
	DDV_MaxChars(pDX, m_sTrainCode, 4);
	DDX_Text(pDX, IDC_ROUTENAME, m_sTrainName);
	DDX_Check(pDX, IDC_INTERCHANGECHECK, m_bSwitchInterchanges);
	DDX_Text(pDX, IDC_MAXMOVES, m_iMaxMoves);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditTrainDialog, CDialog)
	//{{AFX_MSG_MAP(CEditTrainDialog)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnAddbutton)
	ON_BN_CLICKED(IDC_REMOVEBUTTON, OnRemovebutton)
	ON_LBN_SELCHANGE(IDC_ROUTELOCATIONLIST, OnSelchangeRoutelocationlist)
	ON_BN_CLICKED(IDC_UPBUTTON, OnUpbutton)
	ON_BN_CLICKED(IDC_DOWNBUTTON, OnDownbutton)
	ON_LBN_SELCHANGE(IDC_AVAILABLELOCATIONLIST, OnSelchangeAvailablelocationlist)
	ON_LBN_DBLCLK(IDC_AVAILABLELOCATIONLIST, OnDblclkAvailablelocationlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_INDUSTRYLIST, OnItemchangedIndustrylist)
	ON_BN_CLICKED(IDC_LOCALSWITCHER, OnLocalswitcher)
	ON_EN_CHANGE(IDC_ROUTECODE, OnChangeRoutecode)
	ON_EN_CHANGE(IDC_ROUTENAME, OnChangeRoutename)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_INTERCHANGECHECK, OnInterchangeCheck)
	ON_EN_CHANGE(IDC_MAXMOVES, OnChangeMaxMoves)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTrainDialog message handlers

BOOL CEditTrainDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    m_ctlIndustryList.InsertColumn(0,"");
    m_ctlIndustryList.SetColumnWidth(0,186);
    m_ctlIndustryList.SetExtendedStyle(m_ctlIndustryList.GetExtendedStyle()|LVS_EX_CHECKBOXES);

    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  create and preload _TL and _TI
    //
    //try { pDB->execDML("DROP TABLE _TL;"); } catch (...) {}
    //try { pDB->execDML("DROP TABLE _TI;"); } catch (...) {}
    CString sSQL;
    sSQL.Format("CREATE TEMP TABLE _TL AS SELECT * FROM Trains_Location WHERE Train_FK=%d;",m_iTrainID);
    pDB->execDML(sSQL);
    sSQL.Format("CREATE TEMP TABLE _TI AS SELECT * FROM Trains_Industry WHERE Train_FK=%d;",m_iTrainID);
    pDB->execDML(sSQL);
    //
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Locations;");
    //
    while (!q.eof())
    {
        int nIndex = m_ctlLocationList.AddString(q.fieldValue(1));
        m_ctlLocationList.SetItemData(nIndex,atoi(q.fieldValue(0)));
        q.nextRow();
    }
    q.finalize();
    sSQL.Format("SELECT name,trainid FROM Trains WHERE id=%d",m_iTrainID);
    q = pDB->execQuery(sSQL);
    if (!q.eof())
    {
        m_sTrainName = q.getStringField(0);
        m_sTrainCode = q.getStringField(1);
    }
    else
    {
        m_sTrainName = "Un-named Train";
        m_sTrainCode = "ZZZZ";
    }
    q.finalize();
    //
    //  get current locations
    //
    sSQL.Format("SELECT Trains_Location.Location_FK,Locations.name FROM Trains_Location, Locations WHERE Train_FK=%d AND Locations.id = Trains_Location.Location_FK ORDER BY route_order",m_iTrainID);
    q = pDB->execQuery(sSQL);
    while (!q.eof())
    {
        int nIndex = m_ctlRouteLocationList.AddString(q.getStringField("name"));
        m_ctlRouteLocationList.SetItemData(nIndex,q.getIntField("Location_FK"));
        //
        q.nextRow();
    }
    q.finalize();
    //
	m_ctlRemoveButton.EnableWindow(FALSE);
	m_ctlIndustryList.EnableWindow(FALSE);
	m_ctlLocalSwitcher.EnableWindow(FALSE);
	m_ctlDown.EnableWindow(FALSE);
	m_ctlUp.EnableWindow(FALSE);
    m_ctlAddButton.EnableWindow(FALSE);
    m_ctlMaxMovesText.EnableWindow(FALSE);
    m_ctlMaxMoves.EnableWindow(FALSE);
    //
    UpdateData(FALSE);
    //
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditTrainDialog::OnAddbutton() 
{
    CString sLocation;
    m_ctlLocationList.GetText(m_ctlLocationList.GetCurSel(),sLocation);
    if( sLocation.IsEmpty() )
        return;
    if( m_ctlRouteLocationList.FindString(-1,sLocation) != LB_ERR )
        return;
    int iLocationID = m_ctlLocationList.GetItemData(m_ctlLocationList.GetCurSel());
    int nIndex = m_ctlRouteLocationList.AddString(sLocation);
    m_ctlRouteLocationList.SetItemData(nIndex,iLocationID);
    //
    CString sSQL;
    sSQL.Format("INSERT INTO _TL (Location_FK) VALUES (%d);",iLocationID);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    try {
        pDB->execDML(sSQL);
    }
    catch(...){}
}

void CEditTrainDialog::OnDblclkAvailablelocationlist() 
{
    OnAddbutton();	
}

void CEditTrainDialog::OnRemovebutton() 
{
    int iLocationID = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
    m_ctlRouteLocationList.DeleteString(m_ctlRouteLocationList.GetCurSel());
    OnSelchangeRoutelocationlist();
    //
    CString sSQL;
    sSQL.Format("DELETE FROM _TL WHERE Location_FK=%d;",iLocationID);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    try {
        pDB->execDML(sSQL);
    }
    catch(...){}
    //
}

void CEditTrainDialog::OnSelchangeRoutelocationlist() 
{
	// TODO: Add your control notification handler code here
    if( m_ctlRouteLocationList.GetCurSel() == -1 )
    {
    	m_ctlRemoveButton.EnableWindow(FALSE);
	    m_ctlIndustryList.EnableWindow(FALSE);
    	m_ctlLocalSwitcher.EnableWindow(FALSE);
    	m_ctlDown.EnableWindow(FALSE);
	    m_ctlUp.EnableWindow(FALSE);
        m_ctlMaxMovesText.EnableWindow(FALSE);
        m_ctlMaxMoves.EnableWindow(FALSE);
    }
    else
    {
    	m_ctlRemoveButton.EnableWindow(TRUE);
	    m_ctlIndustryList.EnableWindow(TRUE);
	    m_ctlLocalSwitcher.EnableWindow(TRUE);
    	m_ctlDown.EnableWindow(TRUE);
	    m_ctlUp.EnableWindow(TRUE);
        m_ctlMaxMovesText.EnableWindow(TRUE);
        m_ctlMaxMoves.EnableWindow(TRUE);
        //
        //  read DB for this item
        //
        int iLocationID = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
        CString sSQL;
        sSQL.Format("SELECT Industries.id,Industries.name FROM Industries,Sidings WHERE Industries.Sidings_FK=Sidings.id AND Sidings.Locations_FK=%d",iLocationID);
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CppSQLite3Query q = pDB->execQuery(sSQL);
        //
        m_ctlIndustryList.DeleteAllItems();
      	m_bLocalSwitcher = FALSE;
        while (!q.eof())
        {
            int nIndex = m_ctlIndustryList.InsertItem(1,q.getStringField("name"));
            m_ctlIndustryList.SetItemData(nIndex,q.getIntField("id"));
            //
            //  see if this industry is served by this train
            //
            sSQL.Format("SELECT 1 FROM _TI WHERE Industry_FK=%d",q.getIntField("id"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            BOOL bCheck = FALSE;
            if( !q2.eof() )
                bCheck = TRUE;
            q2.finalize();
            if( bCheck )
                m_ctlIndustryList.SetCheck(nIndex,TRUE);
            //
            q.nextRow();
        }
        q.finalize();
        //
        //  check for local switcher for this location
        //
        sSQL.Format("SELECT localswitcher FROM _TL WHERE Location_FK=%d;",iLocationID);
        q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            if( q.getIntField("localswitcher") == 1 )
                m_bLocalSwitcher = TRUE;
        }
        q.finalize();
        //
        //  check for limited moves
        //
        sSQL.Format("SELECT max_moves FROM _TL WHERE Location_FK=%d;",iLocationID);
        q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            if( q.getIntField("max_moves") > 0 )
                m_iMaxMoves = q.getIntField("max_moves");
            else
                m_iMaxMoves = 0;
        }
        q.finalize();
        //
        sSQL.Format("SELECT 1 FROM Sidings WHERE Sidings.Locations_FK=%d AND interchange=1",iLocationID);
        q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            m_ctlSwitchInterchanges.EnableWindow(TRUE);
            sSQL.Format("SELECT switch_interchanges FROM _TL WHERE Location_FK=%d;",iLocationID);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
            {
                int iSwitch = q2.getIntField("switch_interchanges");
                m_bSwitchInterchanges = iSwitch==1?TRUE:FALSE;
            }
            q2.finalize();
        }
        else
        {
            m_ctlSwitchInterchanges.EnableWindow(FALSE);
            m_bSwitchInterchanges = FALSE;
        }
        q.finalize();
    }
    UpdateData(FALSE);
    if( m_bLocalSwitcher )
        m_ctlIndustryList.EnableWindow(FALSE);
    else
        m_ctlIndustryList.EnableWindow(TRUE);
}

void CEditTrainDialog::OnSelchangeAvailablelocationlist() 
{
	// TODO: Add your control notification handler code here
    if( m_ctlLocationList.GetCurSel() == -1 )
    {
        m_ctlAddButton.EnableWindow(FALSE);
    }
    else
    {
        m_ctlAddButton.EnableWindow(TRUE);
    }
}

void CEditTrainDialog::OnUpbutton() 
{
	// TODO: Add your control notification handler code here
    if( m_ctlRouteLocationList.GetCount() < 2 ||
        m_ctlRouteLocationList.GetCurSel() == 0
        )
        return;

    int iOrig = m_ctlRouteLocationList.GetCurSel();
    CString sLocation;
    m_ctlRouteLocationList.GetText(m_ctlRouteLocationList.GetCurSel(),sLocation);
    int iData = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
    m_ctlRouteLocationList.DeleteString(iOrig);
    int nIndex = m_ctlRouteLocationList.InsertString(iOrig-1,sLocation);
    m_ctlRouteLocationList.SetItemData(nIndex,iData);
    m_ctlRouteLocationList.SetCurSel(nIndex);
}

void CEditTrainDialog::OnDownbutton() 
{
	// TODO: Add your control notification handler code here
    if( m_ctlRouteLocationList.GetCount() < 2 ||
        m_ctlRouteLocationList.GetCurSel() == (m_ctlRouteLocationList.GetCount()-1)
        )
        return;
    int iOrig = m_ctlRouteLocationList.GetCurSel();
    CString sLocation;
    m_ctlRouteLocationList.GetText(m_ctlRouteLocationList.GetCurSel(),sLocation);
    int iData = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
    m_ctlRouteLocationList.DeleteString(iOrig);
    int nIndex = m_ctlRouteLocationList.InsertString(iOrig+1,sLocation);
    m_ctlRouteLocationList.SetItemData(nIndex,iData);
    m_ctlRouteLocationList.SetCurSel(nIndex);
}

void CEditTrainDialog::OnItemchangedIndustrylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;

    if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
        return;    // No change

    // Old check box state
    BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);  
    if (bPrevState < 0)     // On startup there's no previous state 
        bPrevState = 0;     // so assign as false (unchecked)

    // New check box state
    BOOL bChecked = (BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
    if (bChecked < 0)       // On non-checkbox notifications assume false
        bChecked = 0; 

    if (bPrevState == bChecked) // No change in check box
        return;

    // Now bChecked holds the new check box state
    //
    //  save this industry state temporarily into the _TI table:
    //
    int iIndustryFK = m_ctlIndustryList.GetItemData(pNMListView->iItem);
    CString sSQL;
    sSQL.Format("DELETE FROM _TI WHERE Industry_FK=%d;",iIndustryFK);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    try {
        pDB->execDML(sSQL);
    }
    catch(...){}
    //
    if( bChecked )
    {
        //
        int iLocationID = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
        sSQL.Format("INSERT INTO _TI (Train_FK,Location_FK,Industry_FK) VALUES (%d,%d,%d);",m_iTrainID,iLocationID,iIndustryFK);
        pDB->execDML(sSQL);
    }
}

void CEditTrainDialog::OnOK() 
{
	// TODO: Add extra validation here
    //
    //  insert into Trains_Location / Trains_Industry
    //  (see code in OnCreateButton below)
    //

    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    if( m_iTrainID == -1 )
    {
        //
        //  Convert to Trains, Train_Location, and Train_Industry tables
        //
        //  If there are not any industries checked, AND there isnt a local switcher checked,
        //  we make a location/location/location route.  If there are industries checked, its
        //  an industry-based checklist.  All the same to the program, different for the user
        //
        //  First, create Train table entry, and get the ID
        //
        CString sSQL;
        sSQL.Format("INSERT INTO Trains (trainid,name,Route_FK) VALUES (\"%s\",\"%s\",-1);",m_sTrainCode,m_sTrainName);
        try {
            pDB->execDML(sSQL);
        }
        catch (...)
        {
            sSQL.Format("INSERT INTO Trains (trainid,name) VALUES (\"%s\",\"%s\");",m_sTrainCode,m_sTrainName);
            pDB->execDML(sSQL);
        }
        //
        sSQL = "SELECT last_insert_rowid()";
        CppSQLite3Query q = pDB->execQuery(sSQL);   
        int iTrainID = atoi(q.fieldValue(0));
        q.finalize();
        //
        //  update _TL and _TI with train ID
        //
        sSQL.Format("UPDATE _TI SET Train_FK=%d;",iTrainID);
        pDB->execDML(sSQL);
        pDB->execDML("INSERT INTO Trains_Industry SELECT * FROM _TI");
        //
        //  Put entries into Train_Location table
        //
        for( int i=0;i<m_ctlRouteLocationList.GetCount();i++ )
        {
            int iLocationID = m_ctlRouteLocationList.GetItemData(i);
            //
            //  local switcher and switch_interchanges for this location
            //
            sSQL.Format("SELECT localswitcher,switch_interchanges,max_moves FROM _TL WHERE Location_FK=%d;",iLocationID);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            int iLocalSwitcher(0);
            int iSwitchInterchanges(1);
            int iMaxMoves(0);
            if( !q.eof())
            {
                iLocalSwitcher = q.getIntField("localswitcher");
                iSwitchInterchanges = q.getIntField("switch_interchanges");
                iMaxMoves = q.getIntField("max_moves");
            }
            q.finalize();
            //
            sSQL.Format("INSERT INTO Trains_Location (Train_FK,route_order,Location_FK,localswitcher,switch_interchanges,max_moves) VALUES (%d,%d,%d,%d,%d,%d);",iTrainID,i+1,iLocationID,iLocalSwitcher,iSwitchInterchanges,iMaxMoves);
            pDB->execDML(sSQL);
        }
        //
        //MessageBox("Train Created!");
    }
    else
    {
        //
        //  Delete existing Train_Location and Train_Industry entries for this train
        //
        CString sSQL;
        sSQL.Format("DELETE FROM Trains_Location WHERE Train_FK=%d;",m_iTrainID);
        pDB->execDML(sSQL);
        sSQL.Format("DELETE FROM Trains_Industry WHERE Train_FK=%d;",m_iTrainID);
        pDB->execDML(sSQL);
        //
        //  Update Routes name and code
        //
        sSQL.Format("UPDATE Trains SET trainid=\"%s\" ,name=\"%s\" WHERE id=%d;",m_sTrainCode,m_sTrainName,m_iTrainID);
        pDB->execDML(sSQL);
        //
        //  Put entries into Trains_Location and Trains_Industry tables
        //
        pDB->execDML("INSERT INTO Trains_Industry SELECT * FROM _TI");
        //
        for( int i=0;i<m_ctlRouteLocationList.GetCount();i++ )
        {
            int iLocationID = m_ctlRouteLocationList.GetItemData(i);
            //
            //  local switcher and switch interchanges for this location?
            //
            sSQL.Format("SELECT localswitcher,switch_interchanges,max_moves FROM _TL WHERE Location_FK=%d;",iLocationID);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            int iLocalSwitcher(0);
            int iSwitchInterchanges(1);
            int iMaxMoves(0);
            if( !q.eof())
            {
                iLocalSwitcher = q.getIntField("localswitcher");
                iSwitchInterchanges = q.getIntField("switch_interchanges");
                iMaxMoves = q.getIntField("max_moves");
            }
            q.finalize();
            //
            sSQL.Format("INSERT INTO Trains_Location (Train_FK,route_order,Location_FK,localswitcher,switch_interchanges,max_moves) VALUES (%d,%d,%d,%d,%d,%d);",m_iTrainID,i+1,iLocationID,iLocalSwitcher,iSwitchInterchanges,iMaxMoves);
            pDB->execDML(sSQL);
        }
        //
        MessageBox("Train Updated!");
    }
    //
    try { pDB->execDML("DROP TABLE _TL;"); } catch (...) {}
    try { pDB->execDML("DROP TABLE _TI;"); } catch (...) {}
	CDialog::OnOK();
}

void CEditTrainDialog::OnLocalswitcher() 
{
    UpdateData();
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    int iLocationID = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
    sSQL.Format("UPDATE _TL SET localswitcher=%d WHERE Location_FK=%d;",m_bLocalSwitcher?1:0,iLocationID);
    pDB->execDML(sSQL);
}

void CEditTrainDialog::OnChangeRoutecode() 
{
    UpdateData(TRUE);
}

void CEditTrainDialog::OnChangeRoutename() 
{
    UpdateData(TRUE);
}

void CEditTrainDialog::OnClose() 
{
	CDialog::OnClose();
}

void CEditTrainDialog::OnInterchangeCheck() 
{
    UpdateData();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    int iLocationID = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
    sSQL.Format("UPDATE _TL SET switch_interchanges=%d WHERE Location_FK=%d;",m_bSwitchInterchanges?1:0,iLocationID);
    pDB->execDML(sSQL);
}

void CEditTrainDialog::OnCancel() 
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    try { pDB->execDML("DROP TABLE _TL;"); } catch (...) {}
    try { pDB->execDML("DROP TABLE _TI;"); } catch (...) {}
	CDialog::OnCancel();
}

void CEditTrainDialog::OnChangeMaxMoves() 
{
    UpdateData();
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    int iLocationID = m_ctlRouteLocationList.GetItemData(m_ctlRouteLocationList.GetCurSel());
    CString sSQL;
    sSQL.Format("UPDATE _TL SET max_moves=%d WHERE Location_FK=%d;",m_iMaxMoves,iLocationID);
    pDB->execDML(sSQL);
}
