// TerminateTrainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "TerminateTrainDialog.h"

#include "TerminateNewLocationDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerminateTrainDialog dialog


CTerminateTrainDialog::CTerminateTrainDialog(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CTerminateTrainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerminateTrainDialog)
	m_sTrainDescription = _T("");
	//}}AFX_DATA_INIT
    m_iTrainID = -1;
//    m_sSelectedIndustry = _T("");
//    m_sSelectedSiding = _T("");
}


void CTerminateTrainDialog::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerminateTrainDialog)
	DDX_Control(pDX, IDC_TRAINLIST, m_ctlTrainList);
	DDX_Text(pDX, IDC_TRAINDESCRIPTION, m_sTrainDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTerminateTrainDialog, CResizableDialog)
	//{{AFX_MSG_MAP(CTerminateTrainDialog)
	ON_NOTIFY(NM_RCLICK, IDC_TRAINLIST, OnRclickTrainlist)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	//ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerminateTrainDialog message handlers

enum {
    TT_CARNUMBER=0,
    TT_REPORTINGMARKS,
    TT_DESTLOCATION,
    TT_DESTINDUSTRY,
    TT_DESTSIDING,
    TT_LOAD
};

BOOL CTerminateTrainDialog::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctlTrainList.InsertColumn(TT_CARNUMBER, "Car Number", LVCFMT_LEFT, 75);
	m_ctlTrainList.InsertColumn(TT_REPORTINGMARKS, "Reporting Marks", LVCFMT_LEFT, 95);
	m_ctlTrainList.InsertColumn(TT_DESTLOCATION, "Dest. Location", LVCFMT_LEFT, 100);
	m_ctlTrainList.InsertColumn(TT_DESTSIDING, "Dest. Siding", LVCFMT_LEFT, 100);
	m_ctlTrainList.InsertColumn(TT_DESTINDUSTRY, "Dest. Industry", LVCFMT_LEFT, 100);
	m_ctlTrainList.InsertColumn(TT_LOAD, "Load", LVCFMT_LEFT, 50);
	m_ctlTrainList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    //
    if( m_iTrainID == -1 )
        return FALSE;
    //
    //  read train
    //  SELECT Trains_Scheduled_Cars.id, Cars.car_number, Cars.reporting_marks from Trains_Scheduled_Cars,Cars WHERE Trains_Scheduled_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    sSQL.Format("SELECT Trains_Scheduled_Cars.Cars_FK,Cars.car_number,Cars.reporting_marks,Industry_to_FK,Location_to_FK,Siding_to_FK,Trains_Scheduled_Cars.loadempty FROM Trains_Scheduled_Cars,Cars WHERE Trains_Scheduled_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK ORDER BY Trains_Scheduled_Cars.Location_to_FK;",m_iTrainID);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while(!q.eof())
    {
        int iSidingToFK = q.getIntField("Siding_to_FK");
        int iLocationToFK = q.getIntField("Location_to_FK");
        int iIndustryToFK = q.getIntField("Industry_to_FK");
        bool bEmpty = q.getIntField("loadempty")==1;
        //
		int nItem = m_ctlTrainList.InsertItem(LVIF_TEXT|LVIF_STATE, m_ctlTrainList.GetItemCount(), 
			q.getStringField("car_number"), LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
        m_ctlTrainList.SetItemText(nItem,TT_REPORTINGMARKS,q.getStringField("reporting_marks"));
        m_ctlTrainList.SetItemData(nItem,q.getIntField("Cars_FK"));
        m_ctlTrainList.SetItemText(nItem,TT_LOAD,"Load");
        //
        sSQL.Format("SELECT name FROM Locations WHERE id=%d;",iLocationToFK);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            m_ctlTrainList.SetItemText(nItem,TT_DESTLOCATION,q2.getStringField("name"));
            //m_sSelectedSiding = q2.getStringField("name");
        }
        q2.finalize();
        //
        sSQL.Format("SELECT name FROM Sidings WHERE id=%d;",iSidingToFK);
         q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            m_ctlTrainList.SetItemText(nItem,TT_DESTSIDING,q2.getStringField("name"));
            //m_sSelectedSiding = q2.getStringField("name");
        }
        q2.finalize();
        //
        sSQL.Format("SELECT name FROM Industries WHERE id=%d;",iIndustryToFK);
        q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            m_ctlTrainList.SetItemText(nItem,TT_DESTINDUSTRY,q2.getStringField("name"));
            //m_sSelectedIndustry = q2.getStringField("name");
        }
        //
        q2.finalize();
        //
        if( bEmpty )
        {
            m_ctlTrainList.SetItemText(nItem,TT_LOAD,"Empty");
        }
        //
        q.nextRow();
    }
    q.finalize();
    //
    //  get train description
    //
    sSQL.Format("SELECT Trains.name,Trains.id FROM Trains,Trains_Scheduled WHERE Trains_Scheduled.Trains_FK=Trains.id AND Trains_Scheduled.id=%d ;",m_iTrainID);
    q = pDB->execQuery(sSQL);
    if(!q.eof())
        m_sTrainDescription.Format("Train '%s'",q.fieldValue(0));
    else
        m_sTrainDescription = "WARNING: Unable to query train description";
    int iTrainFK = q.getIntField("id");
    sSQL.Format("SELECT Locations.name FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order",iTrainFK);
    q = pDB->execQuery(sSQL);
    CString sWork,sWork2;
    int i=0;
    while (!q.eof())
    {
        if( i++ == 0 )
        {
            sWork.Format(" (%s to ",q.getStringField("name"));
        }
        sWork2 = q.getStringField("name");
        q.nextRow();
    }
    q.finalize();
    sWork += sWork2;
    sWork += ")";
    m_sTrainDescription += sWork;

    UpdateData(FALSE);


	AddAnchor(IDC_TRAINDESCRIPTION, TOP_LEFT);
	AddAnchor(IDC_TRAINLIST, TOP_LEFT,BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC, TOP_LEFT);

	AddAnchor(IDOK, TOP_RIGHT);
	AddAnchor(IDCANCEL, TOP_RIGHT);
    
    CRect rc;
	GetDesktopWindow()->GetClientRect(&rc);
    SetMaxTrackSize(CSize(rc.Width()-150,191));

    EnableSaveRestore(_T("TerminateTrainDialog"));

    m_ctlTrainList.RestoreState("State","TerminateTrainDialog_m_ctlTrainList");

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*

  Functional overview of terminating a train:

  For each Car in the train
    If ending location is an industry
      Set deliverydate = CurrentDate
      set car to that industry
    end if
    If End location is a siding
      mark car for delivery to that siding
    end if
  End for

  Set locomotive as availabe
  Set caboose as available 

*/

void CTerminateTrainDialog::OnOK() 
{
	// TODO: Add extra validation here
    if( MessageBox("Are you sure you want to terminate this train?","TrainOps",MB_YESNO) != IDYES )
        return;
    //
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    for( int i=0; i< m_ctlTrainList.GetItemCount();i++ )
    {
        int iCarFK = m_ctlTrainList.GetItemData(i);
        CString sIndustryName = m_ctlTrainList.GetItemText(i,TT_DESTINDUSTRY);
        CString sSidingName = m_ctlTrainList.GetItemText(i,TT_DESTSIDING);
        //
        int iEmpty = m_ctlTrainList.GetItemText(i,TT_LOAD)=="Empty"?1:0;
        //
        //  delivery to an industry
        //
        if( !sIndustryName.IsEmpty() )
        {
            //  get industry id, siding_fk, location_fk
            sSQL.Format("SELECT Locations_FK,Sidings_FK,Industries.id AS Industries_FK FROM Industries,Sidings WHERE Industries.name=\"%s\" AND Industries.Sidings_FK=Sidings.id",sIndustryName);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            if(!q.eof())
            {
                int iLocationFK = q.getIntField("Locations_FK");
                int iSidingFK = q.getIntField("Sidings_FK");
                int iIndustryFK = q.getIntField("Industries_FK");
                sSQL.Format("UPDATE Cars SET Location_FK=%d, Siding_FK=%d, Trains_FK=0, Industry_FK=%d, delivery_date=%d, loadempty=%d WHERE id=%d",
                    iLocationFK,iSidingFK,iIndustryFK,((CTrainOpsApp*)::AfxGetApp())->GetDate(),iEmpty,iCarFK);
                pDB->execDML(sSQL);
            }
            q.finalize();
            continue;
        }
        //
        //  siding delivery 
        //
        if( sIndustryName.IsEmpty() && !sSidingName.IsEmpty() )
        {
            sSQL.Format("SELECT id,Locations_FK FROM Sidings WHERE name=\"%s\"",sSidingName);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            if(!q.eof())
            {
                int iSidingFK = q.getIntField("id");
                int iLocationFK = q.getIntField("Locations_FK");
                sSQL.Format("UPDATE Cars SET Location_FK=%d, Siding_FK=%d, Trains_FK=0, Industry_FK=0, delivery_date=%d, loadempty=%d WHERE id=%d",
                    iLocationFK,iSidingFK,((CTrainOpsApp*)::AfxGetApp())->GetDate(),iEmpty,iCarFK);
                pDB->execDML(sSQL);
            }
            q.finalize();
            continue;
        }
    }
    //
    //  set Trains_Scheduled/run_date and Trains_Scheduled/train_run
    //
    sSQL.Format("UPDATE Trains_Scheduled SET train_run=1, run_date=%d WHERE id=%d",((CTrainOpsApp*)::AfxGetApp())->GetDate(),m_iTrainID);
    pDB->execDML(sSQL);

    m_ctlTrainList.SaveState("State","TerminateTrainDialog_m_ctlTrainList");

	CResizableDialog::OnOK();
}

void CTerminateTrainDialog::OnRclickTrainlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CPoint pt(pNMListView->ptAction);
    CMenu menu;
    menu.LoadMenu(IDM_TERMINATETRAINMENU);
    CMenu* pContextMenu = menu.GetSubMenu(0);
    POINT p;
    p.x = pt.x;
    p.y = pt.y;
    ::ClientToScreen(pNMHDR->hwndFrom, &p);
    int nID = pContextMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
        p.x, p.y, this);

    if( nID == ID__CHANGELOCATION )
    {
        //MessageBox("TODO: Change location dialog");
        CTerminateNewLocationDialog pDlg;
        if( pDlg.DoModal() == IDOK )
        {
            m_ctlTrainList.SetItemText(pNMListView->iItem,TT_DESTLOCATION,"");
            m_ctlTrainList.SetItemText(pNMListView->iItem,TT_DESTINDUSTRY,"");
            m_ctlTrainList.SetItemText(pNMListView->iItem,TT_DESTSIDING,"");
            if( pDlg.m_iLocation > 0 )
            {
                m_ctlTrainList.SetItemText(pNMListView->iItem,TT_DESTLOCATION,pDlg.m_sLocation);
            }
            if( pDlg.m_iIndustry > 0 )
            {
                m_ctlTrainList.SetItemText(pNMListView->iItem,TT_DESTINDUSTRY,pDlg.m_sIndustry);
            }
            if( pDlg.m_iSiding > 0 )
            {
                m_ctlTrainList.SetItemText(pNMListView->iItem,TT_DESTSIDING,pDlg.m_sSiding);
            }
        }
    }
    *pResult = 0;
}

void CTerminateTrainDialog::OnCancel() 
{
    m_ctlTrainList.SaveState("State","TerminateTrainDialog_m_ctlTrainList");
	
	CResizableDialog::OnCancel();
}

void CTerminateTrainDialog::OnClose() 
{
    m_ctlTrainList.SaveState("State","TerminateTrainDialog_m_ctlTrainList");
	
	CResizableDialog::OnClose();
}
