// RunTrainsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "RunTrainsDialog.h"

#include "TrainDetailDialog.h"
#include "TerminateTrainDialog.h"
#include "ScheduledTrainsReportsDialog.h"
#include "Reports.h"

#include "julian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INDCAR_DROPOFF 0
#define INDCAR_PICKUP 1

//
//  Its a global so the TRACE macro can find it
//
extern CIGDebugDialog* m_pDebugDialog;



#ifdef TRACE
#undef TRACE

#define TRACEMAXSTRING	1024

char szBuffer[TRACEMAXSTRING];
//extern char szBuffer[];
inline void TRACE(const char* format,...)
{
    va_list args;
    va_start(args,format);
    int nBuf;
    nBuf = _vsnprintf(szBuffer,
        TRACEMAXSTRING,
        format,
        args);
    va_end(args);
    
    _RPT0(_CRT_WARN,szBuffer);
    //m_pDebugDialog->AddDebug(szBuffer);
    FILE *fh = fopen("./debug.txt","a");
    if( fh )
    {
        fprintf(fh,szBuffer);
        fclose(fh);
    }
}

#endif




/////////////////////////////////////////////////////////////////////////////
// CRunTrainsDialog dialog


CRunTrainsDialog::CRunTrainsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRunTrainsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunTrainsDialog)
	//}}AFX_DATA_INIT
}

void CRunTrainsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunTrainsDialog)
	DDX_Control(pDX, IDC_REPORTSBUTTON, m_ctlReportsButton);
	DDX_Control(pDX, IDC_TRAINDETAILBUTTON, m_ctlDetailButton);
	DDX_Control(pDX, IDC_CURRENTDATE, m_ctlCurrentDate);
	DDX_Control(pDX, IDC_TERMINATEBUTTON, m_ctlTerminateButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_BUILDTRAINBUTTON, m_ctlBuildTrain);
	DDX_Control(pDX, IDC_TRAINLIST, m_ctlTrainList);
	DDX_Control(pDX, IDC_RUNNINGTRAINSLIST, m_ctlRunningTrainsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunTrainsDialog, CDialog)
	//{{AFX_MSG_MAP(CRunTrainsDialog)
	ON_BN_CLICKED(IDC_BUILDTRAINBUTTON, OnRuntrainbutton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRAINLIST, OnItemchangedTrainlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOCOLIST, OnItemchangedLocolist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RUNNINGTRAINSLIST, OnItemchangedRunningtrainslist)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeletebutton)
	ON_BN_CLICKED(IDC_TERMINATEBUTTON, OnTerminatebutton)
	ON_BN_CLICKED(IDC_ADVANCE, OnAdvance)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_TRAINLIST, OnItemchangingTrainlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CABOOSELIST, OnItemchangedCabooselist)
	ON_BN_CLICKED(IDC_TRAINDETAILBUTTON, OnTrainDetailButton)
	ON_BN_CLICKED(IDC_FORMS, OnForms)
	ON_BN_CLICKED(IDC_REPORTSBUTTON, OnReports)
	ON_BN_CLICKED(IDC_RECALC, OnRecalc)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SWITCHLISTBUTTON, OnSwitchlist)
	ON_BN_CLICKED(IDC_FORM19BUTTON, OnForm19)
	ON_BN_CLICKED(IDC_BADORDERBUTTON, OnBadOrder)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunTrainsDialog message handlers

BOOL CRunTrainsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    FILE *fh = fopen("./debug.txt","w");
    fclose(fh);
	
    m_ctlTrainList.InsertColumn(0,"Train");
    m_ctlTrainList.InsertColumn(1,"Train Name");
    m_ctlTrainList.InsertColumn(2,"Last Run");
    m_ctlTrainList.SetColumnWidth(0,50);
    m_ctlTrainList.SetColumnWidth(1,190);
    m_ctlTrainList.SetColumnWidth(2,75);
    LVCOLUMN lvCol;
    memset(&lvCol,0,sizeof(lvCol));
    lvCol.mask = LVCF_FMT;
    lvCol.fmt = LVCFMT_CENTER;
    m_ctlTrainList.SetColumn(3, &lvCol);

    m_ctlTrainList.SetExtendedStyle(m_ctlTrainList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    //
    m_ctlRunningTrainsList.SetExtendedStyle(m_ctlRunningTrainsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    m_ctlRunningTrainsList.InsertColumn(0,"Train ID");
    m_ctlRunningTrainsList.InsertColumn(1,"Name");
    m_ctlRunningTrainsList.SetColumnWidth(0,65);
    m_ctlRunningTrainsList.SetColumnWidth(1,190);
    //
    m_ctlFormsBtn.Create("Forms", WS_TABSTOP | WS_CHILD|WS_VISIBLE,CRect(606,236,606+74,236+23),this,IDC_FORMS);
    m_ctlFormsBtn.AddMenuItem(IDC_SWITCHLISTBUTTON,"Switchlist",0);
    m_ctlFormsBtn.AddMenuItem(IDC_FORM19BUTTON,"Form 19s",0);
    m_ctlFormsBtn.AddMenuItem(IDC_BADORDERBUTTON,"Bad Orders",0);
    m_ctlFormsBtn.EnableWindow(FALSE);
//    m_ctlFormsButton.ShowWindow(SW_HIDE);
    //
    ReadDB();
    //
    m_ctlBuildTrain.EnableWindow(FALSE);
    //
    //  get RRDate, add 'date', and put in control
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    if( !q.eof() )
    {
        int y,m,d;
        JulianDate::JdToYmd( q.getIntField(0)+q.getIntField(1), &y,&m,&d);
        m_ctlCurrentDate.SetDate(y,m,d);
    }
    else
        m_ctlCurrentDate.SetDate(1950,10,10);
    //
	m_ctlRunningTrainsList.RestoreState("State","RunTrainsDialog_m_ctlRunningTrainsList");
    //
    //  see if we're running trains on the 0 date, and, if so,
    //  prompt them to advance the date to prime the pump
    //
    if( q.getIntField(1) == 1 )
    {
        if( MessageBox("You seem to have just set up your railroad.\nTrainOps needs to advance the date to make\ncars and industries start working\n\nDo you wish to do this now?","LSC TrainOps",MB_YESNO|MB_ICONQUESTION) != IDNO )
            return TRUE;
        AdvanceTheDate();
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunTrainsDialog::ReadDB() 
{
    m_ctlRunningTrainsList.DeleteAllItems();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT Trains_Scheduled.id,name,trainid FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.Trains_FK=Trains.id AND train_run=0;");
    //
    m_ctlReportsButton.EnableWindow(FALSE);
    while (!q.eof() )
    {
        int nItem = m_ctlRunningTrainsList.InsertItem(1,q.getStringField("trainid"));
        m_ctlRunningTrainsList.SetItemText(nItem,1,q.getStringField("name"));
        m_ctlRunningTrainsList.SetItemData(nItem,q.getIntField("id"));
        //
        q.nextRow();
        m_ctlReportsButton.EnableWindow(TRUE);
    }
    q.finalize();
    m_ctlTrainList.DeleteAllItems();
    CString sSQL = "SELECT Trains.id,Trains.trainid,Trains.name FROM Trains WHERE Trains.id NOT IN (SELECT Trains_FK FROM Trains_Scheduled WHERE train_run=0);";
    q = pDB->execQuery(sSQL);
    //
    while (!q.eof())
    {
        int nIndex = m_ctlTrainList.InsertItem(1,q.getStringField("trainid"));
        m_ctlTrainList.SetItemData(nIndex,q.getIntField("id"));
        m_ctlTrainList.SetItemText(nIndex,1,q.getStringField("name"));
        //
        //  get last run date for this train from Trains_Scheduled
        //
        sSQL.Format("SELECT MAX(run_date) FROM Trains_Scheduled WHERE Trains_FK=%d AND train_run=1;",q.getIntField(0));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() && q2.getIntField(0) != 0 )
        {
            CppSQLite3Query q3 = pDB->execQuery("SELECT rrdate FROM _Config WHERE id=1;");
            int y=1950,m=1,d=1;
            if( !q3.eof() )
            {
                JulianDate::JdToYmd( q3.getIntField(0)+q2.getIntField(0), &y,&m,&d);
            }
            long lRunDate = q3.getIntField(0)+q2.getIntField(0);
            q3.finalize();
            CString sDate;
            sDate.Format("%02d/%02d/%4d",m,d,y);
            q3 = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
            long lCurDate=0;
            if( !q3.eof() )
            {
                lCurDate = q3.getIntField(0)+q3.getIntField(1);
            }
            q3.finalize();
            m_ctlTrainList.SetItemText(nIndex,2,sDate);
            if( lCurDate <= lRunDate )
                m_ctlTrainList.SetItemText(nIndex,2,NULL,RGB(255,255,255),RGB(240,0,0));
        }
        else
            m_ctlTrainList.SetItemText(nIndex,2,"");
        //
        q2.finalize();
        //
        q.nextRow();
    }
    q.finalize();
}

void CRunTrainsDialog::OnRuntrainbutton() 
{
    if( g_iMethod == METHOD_CARS )
    {
        BuildByCars();
    }
    else
    {
        BuildByCommodities();
    }
}

/*

Functional overview of building a train by car

  For each Trains_Location

    For each Trains_Industries for this Location

      For each entry in Industries_Car_Orders for this Industry
        If load
          Get car from yard
            Mark as load
            Mark Industries_Car_Orders with Trains_Scheduled_FK
        If empty
          Get Empty car on-layout, or from yard
          Mark Industries_Car_Orders with Trains_Scheduled_FK
      End for each Industries_Car_Orders

      For each Car already at industry (outbound empty or full)
        If Car needs pickup (CurrentDate vs delivery date+loadempty time)
          find any 'yard' siding
          set delivery to that siding
          Add to switchlist
        end if
      End for each car

    End each Trains_Industry

  End each Trains_Location
        
*/
void CRunTrainsDialog::BuildByCars() 
{
//    m_pDebugDialog->ClearDebug();
    //
    TRACE("\n*********** BEGIN BUILD TRAIN *******************************\n\n");
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  get train
    //
    int iSel = m_ctlTrainList.GetSelectionMark();
    if( iSel < 0 )
        return;
    int iTrainFK = m_ctlTrainList.GetItemData(iSel);
    //
    m_pCarTable.clear();

    SetCursor(LoadCursor(NULL,IDC_WAIT));

    CString sSQL;
    //
    //  Save off car orders
    //
    try { pDB->execDML("DROP TABLE _ICO;"); } catch (...) {}
    sSQL.Format("CREATE TEMP TABLE _ICO AS SELECT * FROM Industries_Car_Orders WHERE Trains_Scheduled_FK!=99;");
    //sSQL.Format("CREATE TABLE _ICO AS SELECT * FROM Industries_Car_Orders WHERE Trains_Scheduled_FK!=99;");
    pDB->execDML(sSQL);
    //
    //  For each Trains_Location
    //
    sSQL.Format("SELECT Location_FK, name FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iFirstLocationFK = 0;
    while (!q.eof())
    {
        int iLocationFK = q.getIntField("Location_FK");
        //  Save off first route location
        if( iFirstLocationFK == 0 )
            iFirstLocationFK = iLocationFK;
        TRACE("Location: %s\n",q.getStringField("name"));
        //
        //  Read the industries for this location this into a list, so 
        //  that the GetCar() routine can use the Trains_Industry table as well.
        //
        CArray <int,int> pIndustryArray;
        CArray <CString,CString> pIndustryNameArray;
        sSQL.Format("SELECT Industry_FK,name FROM Trains_Industry,Industries WHERE Train_FK=%d AND Location_FK=%d AND Industries.id=Trains_Industry.Industry_FK;",iTrainFK,iLocationFK);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            pIndustryArray.Add(q2.getIntField("Industry_FK"));
            pIndustryNameArray.Add(q2.getStringField("name"));
            q2.nextRow();
        }
        q2.finalize();
        //
        //  pIndustryArray/pIndustryNameArray now contain industries for this location
        //
        //  For each Train_Industry for this Location
        //
        for( int i=0;i<pIndustryArray.GetSize();i++ )
        {
            int iIndustryFK = pIndustryArray.GetAt(i);
            //
            TRACE("\n-Industry: %s\n",pIndustryNameArray.GetAt(i));
            //
            //  For each entry in Industries_Car_Orders for this Industry
            //
            sSQL.Format("SELECT * FROM _ICO WHERE Industries_FK=%d;",iIndustryFK);
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            while(!q3.eof())
            {
                int iCarTypeFK = q3.getIntField("CarTypes_FK");
                TRACE("-- Wants one car type '%s' ",GetCarType(iCarTypeFK));
                //
                //  If it is a load
                //
                if( q3.getIntField("loadempty") == 0 )
                {
                    TRACE("with an inbound load.\n");
                    //
                    //  Get a car, and mark it as a load
                    //  Loaded first, then empty
                    //
                    CFoundCar* pFoundCar = GetLoadedCar(iCarTypeFK,iTrainFK,iLocationFK);
                    if( !pFoundCar )
                        pFoundCar = GetEmptyCar(iCarTypeFK,iTrainFK,iLocationFK);
                    if( pFoundCar ) //  found a car, fixup the car table, put the car
                        //  in the first route location, unless its a foreign road car,
                    {
                        /*sSQL.Format("SELECT reporting_marks, car_number FROM Cars WHERE id=%d",pFoundCar->m_iCar_FK);
                        CppSQLite3Query q3a = pDB->execQuery(sSQL);
                        TRACE("--Found (id: %d) %s %s off-layout\n",pFoundCar->m_iCar_FK,q3a.getStringField("reporting_marks"),q3a.getStringField("car_number"));
                        q3a.finalize();*/
                        //
                        CCar pCar;
                        //
                        //  Build car record, filling in pick-up and drop-off info
                        //  If location = industry/location needed, we just
                        //  pick up the car and move it to the last location, to be
                        //  grabbed by the next train (possibly)
                        //
                        pCar.m_iCar_FK = pFoundCar->m_iCar_FK;
                        pCar.m_iIndustryFromFK = pFoundCar->m_iCarIndustry;
                        pCar.m_iLocationFromFK = pFoundCar->m_iCarLocation==-1?iFirstLocationFK:pFoundCar->m_iCarLocation;
                        pCar.m_iSidingFromFK = pFoundCar->m_iCarSiding;
                        pCar.m_iLocationToFK = iLocationFK;
                        pCar.m_iIndustryToFK = iIndustryFK;
                        pCar.m_bEmpty = FALSE;
                        //
                        m_pCarTable.push_back(pCar);
                        TRACE("-- Pick up car %d at location %d, industry %d\n",pCar.m_iCar_FK, pCar.m_iLocationFromFK, pCar.m_iIndustryFromFK);
                        TRACE("-- Deliver car %d to location %d, industry %d\n",pCar.m_iCar_FK, pCar.m_iLocationToFK, pCar.m_iIndustryToFK);
                        //
                        sSQL.Format("UPDATE Industries_Car_Orders SET Trains_Scheduled_FK=-99 WHERE id=%d;",q3.getIntField("id"));
                        pDB->execDML(sSQL);
                        //
                        delete pFoundCar;
                    }
                    else
                    {
                        TRACE("** Unable to find car\n");// off-layout\n");
                    }
                }   //  end if load
                else
                {
                    TRACE("as an inbound empty.\n");
                    //  is an empty (inbound needed)
                    //
                    //  Search from the beginning of the route, to this location for this car type
                    //
                    CFoundCar* pFoundCar = GetEmptyCar(iCarTypeFK,iTrainFK,iLocationFK);
                    //
                    if( pFoundCar )
                    {
                        /*sSQL.Format("SELECT reporting_marks,car_number FROM Cars WHERE id=%d;",pFoundCar->m_iCar_FK);
                        CppSQLite3Query q3a = pDB->execQuery(sSQL);
                        if( !q3a.eof() )
                            TRACE("--Found empty car %s %s at industry %d\n",q3a.getStringField("reporting_marks"),q3a.getStringField("car_number"),pFoundCar->m_iCarIndustry);
                        else
                            TRACE("--Found car type %d at industry\n",iCarTypeFK);
                        q3a.finalize();*/
                        //
                        CCar pCar;
                        //
                        //  Build car record, filling in pick-up and drop-off info
                        //  If location = industry/location needed, we just
                        //  pick up the car and move it to the last location, to be
                        //  grabbed by the next train (possibly)
                        //
                        pCar.m_iCar_FK = pFoundCar->m_iCar_FK;
                        pCar.m_iIndustryFromFK = pFoundCar->m_iCarIndustry;
                        pCar.m_iLocationFromFK = pFoundCar->m_iCarLocation;
                        pCar.m_iSidingFromFK = pFoundCar->m_iCarSiding;
                        pCar.m_iLocationToFK = iLocationFK;
                        pCar.m_iIndustryToFK = iIndustryFK;
                        pCar.m_bEmpty = TRUE;
                        m_pCarTable.push_back(pCar);
                        TRACE("-- Pick up car %d at location %d, industry%d \n",pFoundCar->m_iCar_FK,pFoundCar->m_iCarLocation,pFoundCar->m_iCarIndustry);
                        TRACE("-- Deliver car %d to industry %d\n",pFoundCar->m_iCar_FK,iIndustryFK);
                        //
                        sSQL.Format("UPDATE Industries_Car_Orders SET Trains_Scheduled_FK=-99 WHERE id=%d;",q3.getIntField("id"));
                        pDB->execDML(sSQL);
                        //
                        delete pFoundCar;
                    }
                    else
                    {
                        TRACE("** Did not find empty car type %s\n",GetCarType(iCarTypeFK));
                    }

                }
                q3.nextRow();
            }   //  end cars in
            q3.finalize();
            //
            //  Pickups (full or empty)
            //
            /*
            For each Car already at industry (outbound empty or full)
              If Car needs pickup (CurrentDate vs delivery date+loadempty time)
                If full
                  Search from this location, to end, for a 'yard' siding
                  if found
                    set delivery to that siding
                    Add to switchlist
                  end if
                end if
              end if
            End for each car
            */
            
            int iDate = 0;
            CppSQLite3Query qq = pDB->execQuery("SELECT date FROM _Config WHERE id=1;");
            if( !qq.eof() )
                iDate = qq.getIntField("date");
            qq.finalize();
            TRACE("## Checking for pickups\n");
            sSQL.Format("SELECT * FROM Cars where Industry_FK=%d;",iIndustryFK);
            q3 = pDB->execQuery(sSQL);
            while( !q3.eof() )
            {
                if( !CarOnTrain(q3.getIntField("id")) )
                {
                    TRACE("--Car %d %s %s at %s in %s\n",q3.getIntField("id"),q3.getStringField("reporting_marks"),q3.getStringField("car_number"),pIndustryNameArray.GetAt(i),q.getStringField("name"));
                    int iDeliveryDate = q3.getIntField("delivery_date");
                    TRACE("   delivery date: %d\n",iDeliveryDate);
                    TRACE("   load empty days: %d\n",g_iLoadEmptyDays);
                    TRACE("   date: %d\n",iDate);
                    if( iDeliveryDate + g_iLoadEmptyDays <= iDate )
                    {
                        TRACE("    SCHEDULE PICK UP\n");
                        //  if its a foreign road car, find an interchange point (if applicable)
                        //  between this location and the end of the route

                        //  otherwise, find a yard location

                        //  otherwise, bring it to the end of the route

                        int iToLocation(iLocationFK);
                        int iToSiding(0);
                        BOOL bPickup(TRUE);
                        if( q3.getIntField("foreign_road") == 1 )
                        {
                            TRACE("---It's a foreign road car\n");
                            //
                            //  see if its a specific 'interchange via', and if we're switching this siding
                            //
                            if( q3.getIntField("interchange_via") > 0 )
                            {
                                if( !SwitchingSiding(iTrainFK,q3.getIntField("interchange_via"))) 
                                {
                                    TRACE("----No Switching Siding\n");
                                    bPickup=FALSE;
                                }
                            }
                            else
                            {
                                //
                                if( !GetInterchangeSiding(iTrainFK, iLocationFK, -1, &iToSiding,&iToLocation) )
                                {
                                    TRACE("----No Interchange Siding\n");
                                    bPickup=FALSE;  //  no interchange siding, don't pick car up
                                }
                            }
                        }
                        else
                        {
                            TRACE("---It's a local road car\n");
                            //  
                            //  1.  Is the car empty?  Does this car have a return to?  Is it on our route?  If so, mark it to go to that siding.
                            //  2.  Is there a "Return Empties" to yard (one or more)?   If so, return to closest yard
                            //  3.  Else, mark it for pickup;
                            //
                            if( q3.getIntField("loadempty")==1 && CarHasReturnTo(q3.getIntField("id"),&iToSiding,&iToLocation) )
                            {
                                TRACE("----Empty with return to\n");
                                //  is iToLocation on our route?
                                if( !LocationOnRoute(iTrainFK,iLocationFK,iToLocation) )
                                {
                                    TRACE("---To location not in our route\n");
                                    iToSiding = 0;
                                    iToLocation = 0;
                                }
                            }
                            else
                            {
                                //
                                //  Car is an empty, so where should it go?
                                //  Do we have a yard for empties in this direction?
                                //  
                                //  FIXME:  Move empties correctly
                                //
                                if( q3.getIntField("loadempty")==1 )
                                {
                                    TRACE("----Empty\n");
                                    if( GetYardSidingForEmpties(iTrainFK,iLocationFK,&iToSiding,&iToLocation) )
                                        ;   //  empty with 'return to'
                                    else
                                    {
                                        if( AnyYardSidings() )
                                        {
                                            iToSiding = 0;
                                            iToLocation = 0;
                                        }
                                    }
                                }
                                else
                                {
                                    TRACE("----Load\n");
                                    GetYardSiding(iTrainFK,iLocationFK,&iToSiding,&iToLocation);
                                }
                            }
                        }
                        if( bPickup )
                        {
                            TRACE("--- WANT to pick up the car\n");
                            if( iToSiding )
                            {
                                TRACE("--- Deliver to a siding\n");
                                //
                                //  Mark car for delivery
                                //  set to deliver to that siding
                                //  add to switchlist
                                //
                                CCar pCar;
                                memset(&pCar,sizeof(CCar),0);
                                pCar.m_iCar_FK = q3.getIntField("id");
                                pCar.m_iIndustryFromFK = iIndustryFK;
                                pCar.m_iSidingFromFK = -1;
                                pCar.m_iLocationFromFK = iLocationFK;
                                pCar.m_iIndustryToFK = -1;
                                pCar.m_iSidingToFK = iToSiding;
                                pCar.m_iLocationToFK = iToLocation;
                                pCar.m_bEmpty = q3.getIntField("loadempty")==1;
                                m_pCarTable.push_back(pCar);
                            }
                            else
                            {
                                TRACE("--- Deliver to last location\n");
                                // drop at last location
                                int iLastLocation = GetLastTrainLocation(iTrainFK);
                                int iToSiding = -1;
                                sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d ORDER BY RANDOM();",iLastLocation);
                                CppSQLite3Query q4 = pDB->execQuery(sSQL);
                                if( !q4.eof() )
                                    iToSiding = q4.getIntField("id");
                                q2.finalize();
                                //
                                CCar pCar;
                                memset(&pCar,sizeof(CCar),0);
                                pCar.m_iCar_FK = q3.getIntField("id");
                                pCar.m_iIndustryFromFK = iIndustryFK;
                                pCar.m_iSidingFromFK = -1;
                                pCar.m_iLocationFromFK = iLocationFK;
                                pCar.m_iIndustryToFK = -1;
                                pCar.m_iSidingToFK = iToSiding;
                                pCar.m_iLocationToFK = iLastLocation;
                                pCar.m_bEmpty = q3.getIntField("loadempty")==1;
                                m_pCarTable.push_back(pCar);
                                //
                                //TRACE("Unable to find down-route yard/interchange siding for car\n");
                            }
                        }
                        else
                        {
                            TRACE("--- Don't want to pick up the car\n");
                        }

                    }
                }
                else
                    TRACE("    NOT READY FOR PICKUP\n");
                q3.nextRow();
            }
            q3.finalize();
            
//    sSQL.Format("SELECT LastDelivery + LoadEmptyDays FROM Industries_Cars WHERE Industries_FK=%d AND CarTypes_FK=%d;",iIndustryFK,iCarTypeFK);
//            sSQL.Format("SELECT Cars.id FROM Cars,Industries_Cars WHERE loadempty = 0 AND Industries_Cars.CarTypes_FK = Cars.CarTypes_FK AND Industry_FK=%d AND Industries_FK=Industry_FK AND delivery_date+Industries_Cars.LoadEmptyDays<%d;",iIndustryFK,((CTrainOpsApp*)::AfxGetApp())->GetDate());
            /*
            sSQL.Format("SELECT DISTINCT Cars.id,loadempty,foreign_road,reporting_marks,car_number,interchange_via FROM Cars,Industries_Cars WHERE Industries_Cars.CarTypes_FK = Cars.CarTypes_FK AND Industry_FK=%d AND Industries_FK=Industry_FK AND delivery_date+Industries_Cars.LoadEmptyDays<=%d AND Cars.id NOT IN (%s);",iIndustryFK,((CTrainOpsApp*)::AfxGetApp())->GetDate(),GetScheduledCars());
            q3 = pDB->execQuery(sSQL);
            TRACE("## Checking for pickups\n");
            while( !q3.eof() )
            {
                if( !CarOnTrain(q3.getIntField("id")) )
                {
                    TRACE("--Car %d %s %s waiting for pickup at %s in %s\n",q3.getIntField("id"),q3.getStringField("reporting_marks"),q3.getStringField("car_number"),pIndustryNameArray.GetAt(i),q.getStringField("name"));
                    //
                    //  Get yard/interchange siding between this location, and end of route
                    //
                    int iToLocation(iLocationFK);
                    int iToSiding(0);
                    BOOL bPickup(TRUE);
                    if( q3.getIntField("foreign_road") == 1 )
                    {
                        TRACE("---It's a foreign road car\n");
                        //
                        //  see if its a specific 'interchange via', and if we're switching this siding
                        //
                        if( q3.getIntField("interchange_via") > 0 )
                        {
                            if( !SwitchingSiding(iTrainFK,q3.getIntField("interchange_via"))) 
                            {
                                TRACE("----No Switching Siding\n");
                                bPickup=FALSE;
                            }
                        }
                        else
                        {
                            //
                            if( !GetInterchangeSiding(iTrainFK, iLocationFK, -1, &iToSiding,&iToLocation) )
                            {
                                TRACE("----No Interchange Siding\n");
                                bPickup=FALSE;  //  no interchange siding, don't pick car up
                            }
                        }
                    }
                    else
                    {
                        TRACE("---It's a local road car\n");
                        //  
                        //  1.  Is the car empty?  Does this car have a return to?  Is it on our route?  If so, mark it to go to that siding.
                        //  2.  Is there a "Return Empties" to yard (one or more)?   If so, return to closest yard
                        //  3.  Else, mark it for pickup;
                        //
                        if( q3.getIntField("loadempty")==1 && CarHasReturnTo(q3.getIntField("id"),&iToSiding,&iToLocation) )
                        {
                            TRACE("----Empty with return to\n");
                            //  is iToLocation on our route?
                            if( !LocationOnRoute(iTrainFK,iLocationFK,iToLocation) )
                            {
                                TRACE("---To location not in our route\n");
                                iToSiding = 0;
                                iToLocation = 0;
                            }
                        }
                        else
                        {
                            //
                            //  Car is an empty, so where should it go?
                            //  Do we have a yard for empties in this direction?
                            //  
                            //  FIXME:  Move empties correctly
                            //
                            if( q3.getIntField("loadempty")==1 )
                            {
                                TRACE("----Empty\n");
                                if( GetYardSidingForEmpties(iTrainFK,iLocationFK,&iToSiding,&iToLocation) )
                                    ;   //  empty with 'return to'
                                else
                                {
                                    if( AnyYardSidings() )
                                    {
                                        iToSiding = 0;
                                        iToLocation = 0;
                                    }
                                }
                            }
                            else
                            {
                                TRACE("----Load\n");
                                GetYardSiding(iTrainFK,iLocationFK,&iToSiding,&iToLocation);
                            }
                        }
                    }
                    if( bPickup )
                    {
                        TRACE("--- WANT to pick up the car\n");
                        if( iToSiding )
                        {
                            TRACE("--- Deliver to a siding\n");
                            //
                            //  Mark car for delivery
                            //  set to deliver to that siding
                            //  add to switchlist
                            //
                            CCar pCar;
                            memset(&pCar,sizeof(CCar),0);
                            pCar.m_iCar_FK = q3.getIntField("id");
                            pCar.m_iIndustryFromFK = iIndustryFK;
                            pCar.m_iSidingFromFK = -1;
                            pCar.m_iLocationFromFK = iLocationFK;
                            pCar.m_iIndustryToFK = -1;
                            pCar.m_iSidingToFK = iToSiding;
                            pCar.m_iLocationToFK = iToLocation;
                            pCar.m_bEmpty = q3.getIntField("loadempty")==1;
                            m_pCarTable.push_back(pCar);
                        }
                        else
                        {
                            TRACE("--- Deliver to last location\n");
                            // drop at last location
                            int iLastLocation = GetLastTrainLocation(iTrainFK);
                            int iToSiding = -1;
                            sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d ORDER BY RANDOM();",iLastLocation);
                            CppSQLite3Query q4 = pDB->execQuery(sSQL);
                            if( !q4.eof() )
                                iToSiding = q4.getIntField("id");
                            q2.finalize();
                            //
                            CCar pCar;
                            memset(&pCar,sizeof(CCar),0);
                            pCar.m_iCar_FK = q3.getIntField("id");
                            pCar.m_iIndustryFromFK = iIndustryFK;
                            pCar.m_iSidingFromFK = -1;
                            pCar.m_iLocationFromFK = iLocationFK;
                            pCar.m_iIndustryToFK = -1;
                            pCar.m_iSidingToFK = iToSiding;
                            pCar.m_iLocationToFK = iLastLocation;
                            pCar.m_bEmpty = q3.getIntField("loadempty")==1;
                            m_pCarTable.push_back(pCar);
                            //
                            //TRACE("Unable to find down-route yard/interchange siding for car\n");
                        }
                    }
                    else
                    {
                        TRACE("--- Don't want to pick up the car\n");
                    }
                }
                q3.nextRow();
            }
            */
            TRACE("## Done checking for pickups\n");
            //q3.finalize();

        }   //   end Route_Industry for
        //
        q.nextRow();
    }
    q.finalize();
    TRACE("\n*********** END BUILD TRAIN *******************************\n\n");
    //
    //  End of first pass.
    //
    TRACE("Car table has %d entries\n",m_pCarTable.size());
    //
    //  Copy over m_pCarTable into _TSC temp table in the database
    //  so we can pass it over to the Train Detail Dialog
    //
    try { pDB->execDML("DROP TABLE _TSC;"); } catch (...) {}
#ifdef _DEBUG
    sSQL.Format("CREATE TABLE _TSC AS SELECT * FROM Trains_Scheduled_Cars WHERE id=-100;");
#else
    sSQL.Format("CREATE TEMP TABLE _TSC AS SELECT * FROM Trains_Scheduled_Cars WHERE id=-100;");
#endif

    pDB->execDML(sSQL);
    for( int i=0;i<m_pCarTable.size();i++)
    {
        CCar pCar;
        pCar = m_pCarTable.at(i);
        sSQL.Format("INSERT INTO _TSC (Trains_Scheduled_FK,Cars_FK,Industry_from_FK,Industry_to_FK,Location_from_FK,Location_to_FK,Siding_from_FK,Siding_to_FK,loadempty) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d)",
            -1,                                         //  Trains_Scheduled_FK
            pCar.m_iCar_FK,                             //  Cars_FK
            pCar.m_iIndustryFromFK,                     //  Industry_from_FK
            pCar.m_iIndustryToFK,                       //  Industry_to_FK
            pCar.m_iLocationFromFK,                     //  Location_from_FK
            pCar.m_iLocationToFK,                       //  Location_to_FK
            pCar.m_iSidingFromFK,                       //  Siding from FK
            pCar.m_iSidingToFK,                         //  Siding to FK
            pCar.m_bEmpty?1:0                           //  Load/Empty
            );
        pDB->execDML(sSQL);
        //TRACE("%s\n",sSQL);
    }
    //
    ValidateTrain(iTrainFK);
}

/*

Functional overview of building a train by commodities

  For each Trains_Location

    For each Trains_Industries for this Location

      For each Commodity in Industries_Commodities for this Industry

        If Commodity/In needed (LastDelivery vs CurrentDate + loademptytime)
          Use high/how/percent to figure out how many cars
          For each Commodity needed
            Check for on-layout industry providing this Commodity

            Add to switchlist
          End for
        End if needed In

        If Commodity/Out needed (LastDelivery vs CurrentDate + loademptytime)
          Use high/how/percent to figure out how many cars
          For each Commodity needed
            Locate empty car somewhere before Industry (using Commodities_Cars for x-ref)
            Add to switchlist
          End for
        End if needed Out

      End each Commodity

      For each Car already at industry (outbound empty or full)
        If Car needs pickup (CurrentDate vs delivery date+loadempty time)
          If full
            Search from this location, to end, for a 'yard' siding
            if found
              set delivery to that siding
            else
              find any 'yard' siding
              set delivery to that siding
            end if
          end if
          Add to switchlist
        end if
      End for each car

    End each Route_Industry

  End each Route_Location
        
*/
void CRunTrainsDialog::BuildByCommodities()
{
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//  post-processing for built train
//
/////////////////////////////////////////////////////////////////////////////////////////

void CRunTrainsDialog::ValidateTrain(int iTrainFK)
{
    //
    //  _TSC now contains the scheduled cars for this train
    //
    //  If we have no cars, we couldnt build train
    //
    if( m_pCarTable.size() == 0 )
    {
        MessageBox("Unable to build the train.  No cars were found.\n\nThis may be due to a number of factors, such as insufficient rolling stock.\nCheck the car location report for where your cars are.\nAlso, verify your train is switching the correct industries on its route.","TrainOps");
        return;
    }
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  If we're a turn, any cars going to the last location, and have no siding or industry
    //  on them, end up going BACK to the start location.
    //
#if 0
    int iFirstLocation = GetFirstTrainLocation(iTrainFK);
    int iLastLocation = GetLastTrainLocation(iTrainFK);
    CString sSQL;
    sSQL.Format("CREATE TEMP TABLE __TSC AS SELECT * FROM _TSC;");
    pDB->execDML(sSQL);
    sSQL.Format("SELECT * FROM __TSC");
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while( !q.eof() )
    {
        int iIndustryToFK = q.getIntField("Industry_to_FK");
        int iLocationToFK = q.getIntField("Location_to_FK");
        int iSidingToFK = q.getIntField("Siding_to_FK");
        //
        if( iLocationToFK == iLastLocation )
        {
            //  If we're going to the last location, and not an industry or siding
            //  set to first location
            if( iIndustryToFK == -1 && iSidingToFK == -1 )
            {
                sSQL.Format("UPDATE _TSC SET Location_to_FK=%d WHERE id=%d;",iFirstLocation,q.getIntField("id"));
                pDB->execDML(sSQL);
            }
        }
        //
        q.nextRow();
    }
    q.finalize();
    //
    sSQL.Format("DROP TABLE __TSC;");
    pDB->execDML(sSQL);
#endif
    //
    //  Bring up the "Train Detail Dialog" if we need to fart around with capacities
    //
    CTrainDetailDialog pTrainDetailDialog;
    pTrainDetailDialog.SetTrainInfo(iTrainFK);
    if( pTrainDetailDialog.DoModal() == IDCANCEL )
    {
        //  un-flag marked cars
        pDB->execDML("UPDATE Industries_Car_Orders SET Trains_Scheduled_FK=-1 WHERE Trains_Scheduled_FK=-99;");
        //
        return;
    }
    //
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    //
    //  move _TSC (possibly edited) back into m_pCarTable
    //
    m_pCarTable.clear();
    CString sSQL;
    sSQL.Format("SELECT * FROM _TSC");
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while (!q.eof())
    {
        CCar pCar;
        pCar.m_iCar_FK = q.getIntField("Cars_FK");
        pCar.m_iIndustryFromFK = q.getIntField("Industry_from_FK");
        pCar.m_iIndustryToFK = q.getIntField("Industry_to_FK");
        pCar.m_iLocationFromFK = q.getIntField("Location_from_FK");
        pCar.m_iLocationToFK = q.getIntField("Location_to_FK");
        pCar.m_iSidingFromFK = q.getIntField("Siding_from_FK");
        pCar.m_iSidingToFK = q.getIntField("Siding_to_FK");
        pCar.m_bEmpty = q.getIntField("loadempty") == 1;
        m_pCarTable.push_back(pCar);
        //
        q.nextRow();
    }
    q.finalize();
    //
    //  Do we have a valid train?
    //
    if( m_pCarTable.size() < 1 )
    {
        MessageBox("Unable to build the train.  No cars found.","TrainOps");
//        if( MessageBox("Unable to build the train.\n\nDo you want to view the Not Found Cars report?","TrainOps",MB_YESNO) == IDYES )
//            DoCarsNotFoundReport(iTrainFK,((CTrainOpsApp*)::AfxGetApp())->GetDate());
        return;
    }
    //
    //  Add train, locomotive, and caboose to Trains_Scheduled
    //
    int iLocomotiveFK = pTrainDetailDialog.GetLocomotive();
    int iCabooseFK = pTrainDetailDialog.GetCaboose();

    sSQL.Format("INSERT INTO Trains_Scheduled (Trains_FK,Locomotive_FK,Caboose_FK,run_date,notes) VALUES (%d,%d,%d,%d,\"%s\");",iTrainFK,iLocomotiveFK,iCabooseFK,((CTrainOpsApp*)::AfxGetApp())->GetDate(),pTrainDetailDialog.GetTrainNotes());
    pDB->execDML(sSQL);
    q = pDB->execQuery("SELECT last_insert_rowid()");
    int iScheduledTrainFK = atoi(q.fieldValue(0));
    q.finalize();
    //
    //  Add cars to Trains_Scheduled_Cars (Pickups need location, too)
    //
    for( int i=0;i<m_pCarTable.size();i++)
    {
        CCar pCar;
        pCar = m_pCarTable.at(i);
        sSQL.Format("INSERT INTO Trains_Scheduled_Cars (Trains_Scheduled_FK,Cars_FK,Industry_from_FK,Industry_to_FK,Location_from_FK,Location_to_FK,Siding_from_FK,Siding_to_FK,loadempty) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d)",
            iScheduledTrainFK,                          //  Trains_Scheduled_FK
            pCar.m_iCar_FK,                             //  Cars_FK
            pCar.m_iIndustryFromFK,                     //  Industry_from_FK
            pCar.m_iIndustryToFK,                       //  Industry_to_FK
            pCar.m_iLocationFromFK,                     //  Location_from_FK
            pCar.m_iLocationToFK,                       //  Location_to_FK
            pCar.m_iSidingFromFK,                       //  Siding from FK
            pCar.m_iSidingToFK,                         //  Siding to FK
            pCar.m_bEmpty?1:0                           //  Load/Empty
            );
        pDB->execDML(sSQL);
        //
        //  For each car, update its position to reflect where its supposed to start from
        //
        //sSQL.Format("UPDATE Cars SET Location_FK=%d WHERE id=%d",pCar.m_iLocationFromFK,pCar.m_iCar_FK);
        //pDB->execDML(sSQL);
        //
        //  For each entry in the industry car table, get location, and build
        //  Trains_Scheduled_Locations / Trains_Scheduled_Locations_Cars tables,
        //  so we know what the train is like after each location
        //
    }
    //
    //  Update cars in Industries_Car_Orders where Trains_Scheduled_FK=-99 to iScheduledTrainFK
    //
    sSQL.Format("UPDATE Industries_Car_Orders SET Trains_Scheduled_FK=%d WHERE Trains_Scheduled_FK=-99;",iScheduledTrainFK);
    pDB->execDML(sSQL);
    //
    //  Cleanup
    //
    m_pCarTable.clear();
#ifndef _DEBUG
    pDB->execDML("DROP TABLE _TSC");
#endif
    //
    ReadDB();
    //
    //PrintSwitchlist(iScheduledTrainFK);
    //
    //PrintForm19s(iScheduledTrainFK);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//          Support functions
//
/////////////////////////////////////////////////////////////////////////////////////////


CString CRunTrainsDialog::GetCarType(int iCarTypeFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT description FROM CarTypes WHERE id=%d;",iCarTypeFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    CString sName("Unknown");
    if( !q.eof() )
        sName = q.getStringField("description");
    q.finalize();
    return sName;
}

int CRunTrainsDialog::GetFirstTrainLocation(int iTrainFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iFirstLocation(0);
    if( !q.eof() )
        iFirstLocation = q.getIntField("Location_FK");
    q.finalize();
    return iFirstLocation;
}

int CRunTrainsDialog::GetLastTrainLocation(int iTrainFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order DESC;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iLastLocation(0);
    if( !q.eof() )
        iLastLocation = q.getIntField("Location_FK");
    q.finalize();
    return iLastLocation;
}


/*

  If CarType/In needed (LastDelivery vs CurrentDate + loademptytime)

*/

BOOL CRunTrainsDialog::NeedThisCar(int iIndustryFK,int iCarTypeFK)
{
    //
    //  check current 'date' (_Config/date) against Industries_Cars/LastDelivery + LoadEmptyDays for this industry/cartype
    //
    //  if date > LastDelivery + LoadTime, deliver another car.
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT LastDelivery + LoadEmptyDays FROM Industries_Cars WHERE Industries_FK=%d AND CarTypes_FK=%d;",iIndustryFK,iCarTypeFK);
//    TRACE("Need this car: %s",(LPCTSTR)sSQL);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iLastDate = q.getIntField(0);
    q.finalize();
//    TRACE("GetDate: %d / iLastDate: %d\n",((CTrainOpsApp*)::AfxGetApp())->GetDate(),iLastDate);
    if( ((CTrainOpsApp*)::AfxGetApp())->GetDate() > iLastDate )
        return TRUE;
    else
        return FALSE;
}

BOOL CRunTrainsDialog::CarReadyForPickup(int iCarFK)
{
    //
    //  check current 'date' (_Config/date) against Cars/delivery_date + LoadEmptyDays for this industry/cartype
    //
    //  if Car/DeliveryDate + LoadTime < date, car is ready for pickup
    //
    //  Select all of this car type from this industry
    //      see if it is ready for pickup
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //sSQL.Format("SELECT delivery_date,LoadEmptyDays FROM Cars,Industries_Cars WHERE Cars.id=%d AND Cars.Industry_FK=Industries_Cars.Industries_FK AND Cars.CarTypes_FK=Industries_Cars.CarTypes_FK;",iCarFK);
    sSQL.Format("SELECT delivery_date FROM Cars WHERE Cars.id=%d;",iCarFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    BOOL bRet = FALSE;
    if(!q.eof())
    {
        int iDeliveryDate = q.getIntField(0);
        //int iLoadEmptyDays = q.getIntField(1);
        //if( iDeliveryDate + iLoadEmptyDays < ((CTrainOpsApp*)::AfxGetApp())->GetDate() )
        if( iDeliveryDate != ((CTrainOpsApp*)::AfxGetApp())->GetDate() )
            bRet = TRUE;
    }
    q.finalize();
    return bRet;
}

BOOL CRunTrainsDialog::NeedThisCommodity(int iIndustryFK,int iCommodityFK)
{
    //
    //  check current 'date' (_Config/date) against LastDelivery for this industry/commodity
    //
    return TRUE;
}

int CRunTrainsDialog::GetCarQuantity(int iHigh, int iLow, int iPct)
{
    //
    //  calculate the number of cars based on High, Low, and Pct values
    //
    //  Pct is the percentage that High will be sent.  If its < 100%,
    //  each car from iLow+1 to iHigh gets a iPct chance to be sent.
    //
    if( iPct == 100 || iHigh<=iLow )
        return iHigh;
    //
    int iCarCount=iLow;
    int iDiff = iHigh - iLow;
    for( int i=0;i<iDiff;i++)
    {
        int iRand = (rand() % 100);
        if( iRand <= iPct )
            iCarCount++;
    }
    return iCarCount;
}

CFoundCar* CRunTrainsDialog::GetCarFromOffLayout(int iCarType_FK, int iTrainFK, int iEndLocationFK)
{
    //
    //  Search for cars marked as 'off-layout', and that have passed their off-layout timestamp
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    //  get cars
    //
    sSQL.Format("SELECT id,reporting_marks,car_number,foreign_road,interchange_via FROM Cars WHERE CarTypes_FK=%d AND offlayout=1 AND out_of_service=0 AND delivery_date+%d<=%d AND id NOT IN (%s);",iCarType_FK,g_iOffLayoutTime,((CTrainOpsApp*)::AfxGetApp())->GetDate(),GetScheduledCars());
    CppSQLite3Query q = pDB->execQuery(sSQL);
    if( !q.eof() )
    {
        CFoundCar* pCar = new CFoundCar();
        pCar->m_iCar_FK = q.getIntField("id");
        CString sReportingMarks = q.getStringField("reporting_marks");
        CString sCarNumber = q.getStringField("car_number");
        pCar->m_bForeignRoad = (q.getIntField("foreign_road")==1);//FALSE to disable foreign road code
        int iInterchangeVia = q.getIntField("interchange_via");
        q.finalize();
        pCar->m_iCarLocation = -1;
        pCar->m_iCarIndustry = -1;
        pCar->m_iCarSiding = -1;
        //
        //  if it is a foreign road car, then bring it in on an interchange track
        //
        if( pCar->m_bForeignRoad )
        {
            if( iInterchangeVia > 0 )
            {
                if( !SwitchingSiding(iTrainFK,iInterchangeVia) ) 
                {
                    TRACE("Found a foreign road car (%s %s), but there is no appropriate interchange track to bring it in on (not switching)\n",sReportingMarks,sCarNumber);
                    delete pCar;
                    return NULL;
                }
            }
            int iInterchangeSidingLocation(0);
            int iInterchangeSiding(0);
            if( GetInterchangeSiding(iTrainFK,-1,iEndLocationFK,&iInterchangeSiding,&iInterchangeSidingLocation))
            {
                pCar->m_iCarLocation = iInterchangeSidingLocation;
                pCar->m_iCarSiding = iInterchangeSiding;
            }
            else
            {
                TRACE("Found a foreign road car (%s %s), but there is no appropriate interchange track to bring it in on\n",sReportingMarks,sCarNumber);
                delete pCar;
                return NULL;
            }

        }
        else
        {
            //
            //  If we fail to get a siding, then we return -1/-1 for siding/location.
            //  The calling routine will then set the location to the first location on the route
            //
            int iSidingFK = GetFirstYardSiding(iTrainFK,iEndLocationFK);
            if( iSidingFK != 0 )
            {
                TRACE("Found a car (%s %s), dropping on siding %d\n",sReportingMarks,sCarNumber,iSidingFK);
                pCar->m_iCarSiding = iSidingFK;
            }
            else
                TRACE("Found a car (%s %s), but there is no appropriate yard siding to bring it in on.  Defaulting to first location\n",sReportingMarks,sCarNumber);

        }
        //
        return pCar;
    }
    else
    {
        q.finalize();
        TRACE("Unable to find car off-layout\n");
        return NULL;
    }
}

CString CRunTrainsDialog::GetScheduledCars()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    //  Build scheduled trains cars IDs string
    //
    CString sScheduledCarsIDs("");;
    sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars,Trains_Scheduled WHERE Trains_Scheduled.id=Trains_Scheduled_Cars.Trains_Scheduled_FK AND train_run=0;");
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while( !q.eof() )
    {
        CString sWork;
        sWork.Format("%d,%s",q.getIntField("Cars_FK"),sScheduledCarsIDs);
        sScheduledCarsIDs = sWork;
        q.nextRow();
    }
    q.finalize();
    //
    for( int i=0;i<m_pCarTable.size();i++)
    {
        CString sWork;
        sWork.Format("%d,%s",m_pCarTable.at(i).m_iCar_FK,sScheduledCarsIDs);
        sScheduledCarsIDs = sWork;
    }
    int iLen = sScheduledCarsIDs.GetLength();
    if( iLen > 0 )
    {
        if( sScheduledCarsIDs.GetAt(iLen-1) == ',' )
        {
            sScheduledCarsIDs = sScheduledCarsIDs.Left(iLen-1);
        }
    }
    return sScheduledCarsIDs;
}

//
//  search for an interchange siding between these two locations
//
BOOL CRunTrainsDialog::GetInterchangeSiding(int iTrainFK, int iStartLocation_FK, int iEndLocation_FK, int* iInterchangeSiding,int* iInterchangeSidingLocation)
{
    TRACE("%%%%%%%%%% Get Interchange Siding %%%%%%%%%%%%%%%%%%%%%%\n");
    //TRACE("trainfk: %d, StartLocationFK: %d, EndLocationFK: %d\n",iTrainFK,iStartLocation_FK,iEndLocation_FK);

    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    sSQL.Format("SELECT Location_FK,name,switch_interchanges FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order;",iTrainFK);
    TRACE(sSQL);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bStart(false);
    bool bFound(false);
    //
    while( !q.eof() && !bFound )
    {
        TRACE("Looking at %s\n",q.getStringField("name"));
        //  Should we start?
        if( iStartLocation_FK == q.getIntField("Location_FK") || iStartLocation_FK == -1 )
            bStart = true;
        //
        if( bStart )
        {
            if( q.getIntField("switch_interchanges") == 1 )
            {
                TRACE("Looking for an interchange siding here\n");
                sSQL.Format("SELECT id,name FROM Sidings WHERE Locations_FK=%d AND interchange=1;",q.getIntField("Location_FK"));
                TRACE(sSQL);
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if( !q2.eof() )
                {
                    TRACE("Found a siding here (%s)\n",q2.getStringField("name"));
                    //iRet = q2.getIntField("id");
                    *iInterchangeSiding = q2.getIntField("id");
                    *iInterchangeSidingLocation = q.getIntField("Location_FK");
                    bFound = true;
                }
                if( !bFound )
                    TRACE("No interchange sidings found at this location\n");
                q2.finalize();
            }
            else
            {
                TRACE("No interchange sidings are being switched here (option is off)\n");
            }
            //break;
        }
        else
        {
            TRACE("Haven't started looking yet\n");
        }
        //  Should we stop?
        if( iEndLocation_FK == q.getIntField("Location_FK") )
            break;
        q.nextRow();
    }
    q.finalize();
    //TRACE("Returning with bFound=%s...\n",bFound?"TRUE":"FALSE");
    TRACE("%%%%%%%%%% Exiting Get Interchange Siding %%%%%%%%%%%%%%%%%%%%%%\n");
    return bFound;
}

CFoundCar* CRunTrainsDialog::GetEmptyCar(int iCarType_FK, int iTrain_FK, int iEndLocationFK)
{
    //
    //  Search from the beginning of the trains route, to this location for this car type
    //  Get a car ID of this type.  Start at the beginning of the route, and search
    //  until we find one, or get to iLocation_FK
    //  If we don't find an available car, return NULL;
    //
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    TRACE("### in GetEmptyCar ###\n");
    //
    //  Location loop
    //
    sSQL.Format("SELECT Location_FK,name FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order;",iTrain_FK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    BOOL bSearching = TRUE;
    while( !q.eof() && bSearching ) 
    {
        int iLocationFK = q.getIntField("Location_FK");
        TRACE("---'%s'\n",q.getStringField("name"));
        //
        if( iLocationFK == iEndLocationFK )   //  we only search up to where we are, DONT search same location
            bSearching = FALSE;
        //
        if( bSearching )
        {
            TRACE("---- Searching '%s' for empty %s\n",q.getStringField("name"),GetCarType(iCarType_FK));
            //
            //  Get industries at this location, served by this train
            //
            sSQL.Format("SELECT Industry_FK,name FROM Trains_Industry,Industries WHERE Train_FK=%d AND Location_FK=%d AND Industries.id=Trains_Industry.Industry_FK;",iTrain_FK,iLocationFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            while(!q2.eof())
            {
                
                TRACE("---- Industry '%s'\n",q2.getStringField("name"));
                int iIndustryFK = q2.getIntField("Industry_FK");
                sSQL.Format("SELECT id,reporting_marks,car_number,Siding_FK FROM Cars WHERE Industry_FK=%d AND loadempty=1 AND CarTypes_FK=%d AND out_of_service=0 AND id NOT IN (%s)",q2.getStringField("Industry_FK"),iCarType_FK,GetScheduledCars());
                CppSQLite3Query q3 = pDB->execQuery(sSQL);
                if( !q3.eof() )
                {
                    TRACE("---- Found %s %s\n",q3.getStringField("reporting_marks"),q3.getStringField("car_number"));
                    int iCarFK = q3.getIntField("id");
                    int iSidingFK = q3.getIntField("Siding_FK");
                    q3.finalize();
                    //
                    CFoundCar* pCar = new CFoundCar();
                    pCar->m_iCar_FK = iCarFK;
                    pCar->m_iCarLocation = iLocationFK;
                    pCar->m_iCarIndustry = iIndustryFK;
                    pCar->m_iCarSiding = iSidingFK;
                    q2.finalize();
                    q.finalize();
                    return pCar;
                    //q3.nextRow();
                }
                else
                {
                    TRACE("---- No empty car found at industry\n");
                }
                q3.finalize();
                //
                q2.nextRow();
            }
            q2.finalize();
            //
            //  If we get here, then we haven't found a car at an industry.
            //  Check for cars NOT at an industry at this location
            //
            //sSQL.Format("SELECT Cars.id, Siding_FK FROM Cars WHERE Cars.Location_FK=%d AND Industry_FK=-1 AND out_of_service=0 AND loadempty=1 AND CarTypes_FK=%d AND Cars.id NOT IN (%s);",iLocationFK,iCarType_FK,GetScheduledCars());
            sSQL.Format("SELECT Cars.id, Siding_FK FROM Cars WHERE Cars.Location_FK=%d AND out_of_service=0 AND loadempty=1 AND CarTypes_FK=%d AND Cars.id NOT IN (%s);",iLocationFK,iCarType_FK,GetScheduledCars());
            q2 = pDB->execQuery(sSQL);
            while( !q2.eof() )
            {
                TRACE("---- Found non-industry car here\n");
                int iCarFK = q2.getIntField("id");
                int iSidingFK = q2.getIntField("Siding_FK");
                //int iIndustryFK = q2.getIntField("Industry_FK");
                q2.finalize();
                CFoundCar* pCar = new CFoundCar();
                pCar->m_iCar_FK = iCarFK;
                pCar->m_iCarLocation = iLocationFK;
                pCar->m_iCarIndustry = -1;//iIndustryFK;
                pCar->m_iCarSiding = iSidingFK;
                q.finalize();
                return pCar;
                //q2.nextRow();
            }
            q2.finalize();
        }
        //
        q.nextRow();
        //
        if( iEndLocationFK == iLocationFK )   //  we only search up to where we are
            bSearching = FALSE;
    }
    q.finalize();

    return NULL;
}

CFoundCar* CRunTrainsDialog::GetLoadedCar(int iCarType_FK,int iTrain_FK,int iEndLocationFK)
{
    //
    //  Search from the beginning of the trains route, to this location for this car type
    //  Get a car ID of this type.  Start at the beginning of the route, and search
    //  until we find one, or get to iLocation_FK
    //  If we don't find an available car, return NULL;
    //
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    //  Location loop
    //
    sSQL.Format("SELECT Location_FK,name FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order;",iTrain_FK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    BOOL bSearching = TRUE;
    while( !q.eof() && bSearching ) 
    {
        int iLocationFK = q.getIntField("Location_FK");
        TRACE("---'%s'\n",q.getStringField("name"));
        //
        if( iLocationFK == iEndLocationFK )   //  we only search up to where we are, DONT search same location
            bSearching = FALSE;
        //
        if( bSearching )
        {
            TRACE("--- Searching '%s' for loaded %s\n",q.getStringField("name"),GetCarType(iCarType_FK));

            /*
            //
            //  Get industries at this location, served by this train
            //
            sSQL.Format("SELECT Industry_FK,name FROM Trains_Industry,Industries WHERE Train_FK=%d AND Location_FK=%d AND Industries.id=Trains_Industry.Industry_FK;",iTrain_FK,iLocationFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            while(!q2.eof())
            {
                
                TRACE("---- Industry '%s'\n",q2.getStringField("name"));
                int iIndustryFK = q2.getIntField("Industry_FK");
                sSQL.Format("SELECT id,reporting_marks,car_number,Siding_FK FROM Cars WHERE Industry_FK=%d AND loadempty=0 AND CarTypes_FK=%d AND out_of_service=0 AND id NOT IN (%s)",q2.getStringField("Industry_FK"),iCarType_FK,GetScheduledCars());
                CppSQLite3Query q3 = pDB->execQuery(sSQL);
                while( !q3.eof() )
                {
                    TRACE("---- Found %s %s\n",q3.getStringField("reporting_marks"),q3.getStringField("car_number"));
                    int iCarFK = q3.getIntField("id");
                    int iSidingFK = q3.getIntField("Siding_FK");
                    q3.finalize();
                    //
                    CFoundCar* pCar = new CFoundCar();
                    pCar->m_iCar_FK = iCarFK;
                    pCar->m_iCarLocation = iLocationFK;
                    pCar->m_iCarIndustry = iIndustryFK;
                    pCar->m_iCarSiding = iSidingFK;
                    q2.finalize();
                    q.finalize();
                    return pCar;
                    //q3.nextRow();
                }
                q3.finalize();
                //
                q2.nextRow();
            }
            q2.finalize();
            */
            //
            //  search for any car here, even if we dont switch the industry inbound
            //
            sSQL.Format("SELECT Cars.id, Siding_FK, Industry_FK FROM Cars WHERE Cars.Location_FK=%d AND out_of_service=0 AND CarTypes_FK=%d AND Cars.id NOT IN (%s);",iLocationFK,iCarType_FK,GetScheduledCars());
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            while( !q2.eof() )
            {
                int iCarFK = q2.getIntField("id");
                int iSidingFK = q2.getIntField("Siding_FK");
                int iIndustryFK = q2.getIntField("Industry_FK");
                q2.finalize();
                CFoundCar* pCar = new CFoundCar();
                pCar->m_iCar_FK = iCarFK;
                pCar->m_iCarLocation = iLocationFK;
                pCar->m_iCarIndustry = iIndustryFK;
                pCar->m_iCarSiding = iSidingFK;
                q.finalize();
                return pCar;
            }
            q2.finalize();

            //
            //  If we get here, then we haven't found a car at an industry.
            //  Check for cars NOT at an industry at this location
            //
            /*
            sSQL.Format("SELECT Cars.id, Siding_FK FROM Cars WHERE Cars.Location_FK=%d AND out_of_service=0 AND loadempty=0 AND CarTypes_FK=%d AND Cars.id NOT IN (%s);",iLocationFK,iCarType_FK,GetScheduledCars());
            q2 = pDB->execQuery(sSQL);
            while( !q2.eof() )
            {
                int iCarFK = q2.getIntField("id");
                int iSidingFK = q2.getIntField("Siding_FK");
                //int iIndustryFK = q2.getIntField("Industry_FK");
                q2.finalize();
                CFoundCar* pCar = new CFoundCar();
                pCar->m_iCar_FK = iCarFK;
                pCar->m_iCarLocation = iLocationFK;
                pCar->m_iCarIndustry = -1;//iIndustryFK;
                pCar->m_iCarSiding = iSidingFK;
                q.finalize();
                return pCar;
                //q2.nextRow();
            }
            q2.finalize();
            */
        }
        //
        q.nextRow();
        //
        if( iEndLocationFK == iLocationFK )   //  we only search up to where we are
            bSearching = FALSE;
    }
    q.finalize();

    return NULL;
}

//
//  Find a siding marked 'yard' between the beginning of the route and iLocationFK
//
int CRunTrainsDialog::GetFirstYardSiding(int iTrainFK,int iLocationFK)
{
    TRACE("trainfk: %d, LocationFK: %d\n",iTrainFK,iLocationFK);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    int iFirstLocation(0);
    int iFirstSiding(0);

    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bFound=false;
    int iRet = 0;
    CString sName("");
    //
    while( !q.eof() && !bFound )
    {
        sSQL.Format("SELECT id,name FROM Sidings WHERE Locations_FK=%d AND yard=1;",q.getIntField("Location_FK"));
        //TRACE(sSQL);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            iRet = q2.getIntField("id");
            sName = q2.getStringField("name");
            q2.finalize();
            bFound = true;
            break;
        }
        q2.finalize();
        //
        if( iLocationFK == q.getIntField("Location_FK") )
            break;
        q.nextRow();
    }
    q.finalize();
    if( iRet == 0 )
        TRACE("Unable to find yard siding\n");
    else
        TRACE("Found yard siding: %s\n",sName);
    return iRet;
}

//
//  Find a siding marked 'yard' between (but not including) iLocationFK and the end of the route of iTrainFK
//
//  Read Trains_Locations ordered by route_order.  Find iLocationFK, and then search for a 'yard'
//  between there and the end of the route.
//  If there are multiple yard sidings at that location, pick one randomly
//
bool CRunTrainsDialog::GetYardSiding(int iTrainFK,int iLocationFK,int* iSiding,int* iSidingLocation)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

//    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order;",iTrainFK);
    sSQL.Format("SELECT Location_FK,name FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bStart(false);
    bool bFound(false);
    //
    while( !q.eof() )
    {
        TRACE("Looking at %s\n",q.getStringField("name"));
        //  Should we start?
        if( iLocationFK == q.getIntField("Location_FK") )
            bStart = true;
        //
        if( bStart )
        {
            TRACE("Searching...\n");
            //  pick one siding randomly if there are more than one (see end of select)
            sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d AND yard=1 ORDER BY RANDOM();",q.getIntField("Location_FK"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
            {
                TRACE("Found yard siding\n");
                *iSiding = q2.getIntField("id");
                *iSidingLocation = q.getIntField("Location_FK");
                bFound = true;
            }
            q2.finalize();
        }
        else
        {
            TRACE("Haven't started looking yet\n");
        }
        q.nextRow();
    }
    q.finalize();
    return bFound;
}

//
//  Find a siding marked 'yard' that takes empties, 
//  between (but not including) iLocationFK and the end of the route of iTrainFK
//
bool CRunTrainsDialog::GetYardSidingForEmpties(int iTrainFK,int iLocationFK,int* iSiding,int* iSidingLocation)
{
    TRACE("trainfk: %d, LocationFK: %d\n",iTrainFK,iLocationFK);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bFound=false;
    //
    while( !q.eof() )
    {
        if( bFound )
        {
            //  pick one siding randomly if there are more than one (see end of select)
            sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d AND yard=1 AND getempties=1 ORDER BY RANDOM();",q.getIntField("Location_FK"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
            {
                *iSiding = q2.getIntField("id");
                *iSidingLocation = q.getIntField("Location_FK");
            }
            q2.finalize();
            break;
        }
        if( iLocationFK == q.getIntField("Location_FK") )
            bFound = true;
        q.nextRow();
    }
    q.finalize();
    return true;
}

//  are there ANY sidings marked yard in the system?
//
bool CRunTrainsDialog::AnyYardSidings()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT 1 FROM Sidings WHERE yard=1");
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bRet(false);
    if( !q.eof() )
        bRet = true;
    q.finalize();
    return bRet;
}

BOOL CRunTrainsDialog::CarOnTrain(int iCarFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT 1 FROM Trains_Scheduled_Cars,Trains_Scheduled WHERE Cars_FK=%d AND Trains_Scheduled.id=Trains_Scheduled_Cars.Trains_Scheduled_FK AND train_run=0;",iCarFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    BOOL bRet=FALSE;
    if( !q.eof()) 
        bRet=TRUE;
    q.finalize();
    return bRet;
}

//
//  Is this train switching this interchange siding?
//
bool CRunTrainsDialog::SwitchingSiding(int iTrainFK,int iSidingFK) 
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Locations_FK FROM Sidings WHERE id=%d;",iSidingFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bRet(false);
    if( !q.eof()) 
    {
        sSQL.Format("SELECT switch_interchanges FROM Trains_Location WHERE Train_FK=%d AND Location_FK=%d;",iTrainFK,q.getIntField("Locations_FK"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            if( q2.getIntField("switch_interchanges") == 1 )
                bRet = true;
        }
        q2.finalize();
    }
    q.finalize();

    return bRet;
}

//
//  does the car have a return to, and is it on our way?
//

bool CRunTrainsDialog::CarHasReturnTo(int iCarFK,int* iSiding,int* iSidingLocation)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //sSQL.Format("SELECT Sidings_FK,Locations_FK FROM Cars,Industries,Sidings WHERE Cars.id=%d AND Cars.returnto == Industries.id AND Sidings.id=Sidings_FK;",iCarFK);
    sSQL.Format("SELECT returnto FROM Cars WHERE Cars.id=%d AND returnto>0;",iCarFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bRet(false);
    if( !q.eof()) 
    {
        sSQL.Format("SELECT Sidings_FK,Locations_FK FROM Industries,Sidings WHERE Industries.id=%d AND Sidings.id=Sidings_FK;",q.getIntField("returnto"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            *iSiding=q2.getIntField("Sidings_FK");
            *iSidingLocation=q2.getIntField("Locations_FK");
            bRet = true;
        }
        q2.finalize();
    }
    q.finalize();
    return bRet;
}

//
//  search route from iLocationFK to the end, and see if iToLocation is there
//
bool CRunTrainsDialog::LocationOnRoute(int iTrainFK,int iLocationFK,int iToLocation)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order;",iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    bool bFound=false;
    bool bStart=false;
    //
    while( !q.eof() )
    {
        if( q.getIntField("Location_FK") == iLocationFK )
            bStart=true;
        if( bStart )
        {
            if( q.getIntField("Location_FK") == iToLocation )
                bFound = true;
        }
        q.nextRow();
    }
    q.finalize();
    //
    return bFound;
}

void CRunTrainsDialog::OnItemchangedTrainlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CheckButtons();
	
	*pResult = 0;
}

void CRunTrainsDialog::CheckButtons()
{
    if( m_ctlTrainList.GetSelectedCount() == 1 )//&& m_ctlLocoList.GetSelectedCount() == 1 )
        m_ctlBuildTrain.EnableWindow(TRUE);
    else
        m_ctlBuildTrain.EnableWindow(FALSE);
    //
    if( m_ctlRunningTrainsList.GetSelectedCount() == 1 )
    {
        //m_ctlFormsButton.EnableWindow(TRUE);
        m_ctlFormsBtn.EnableWindow(TRUE);
        m_ctlDeleteButton.EnableWindow(TRUE);
        m_ctlTerminateButton.EnableWindow(TRUE);
        m_ctlDetailButton.EnableWindow(TRUE);
    }
    else
    {
        //m_ctlFormsButton.EnableWindow(FALSE);
        m_ctlFormsBtn.EnableWindow(FALSE);
        m_ctlDeleteButton.EnableWindow(FALSE);
        m_ctlTerminateButton.EnableWindow(FALSE);
        m_ctlDetailButton.EnableWindow(FALSE);
    }
}

void CRunTrainsDialog::OnItemchangedLocolist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
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

    CheckButtons();
	
	*pResult = 0;
}

void CRunTrainsDialog::OnItemchangedRunningtrainslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CheckButtons();
	
	*pResult = 0;
}

void CRunTrainsDialog::OnDeletebutton() 
{
    CString sTrainName = m_ctlRunningTrainsList.GetItemText(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED),1);
    if( sTrainName.IsEmpty() )
        return;
    CString sMsg;
    sMsg.Format("Are you sure you want to delete train '%s'?",sTrainName);
    if( MessageBox(sMsg,"LSC TrainOps",MB_YESNO) != IDYES )
        return;
    //
    //  delete entries for this train out of Trains_Scheduled, and Trains_Scheduled_Cars
    //
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Trains_Scheduled WHERE id=%d;",iTrains_Scheduled_FK);	
    pDB->execDML(sSQL);
    sSQL.Format("DELETE FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d;",iTrains_Scheduled_FK);
    pDB->execDML(sSQL);
    sSQL.Format("UPDATE Industries_Car_Orders SET Trains_Scheduled_FK=-1 WHERE Trains_Scheduled_FK=%d;",iTrains_Scheduled_FK);
    pDB->execDML(sSQL);
    //
    ReadDB();
    MessageBox("Train has been deleted!");
}

void CRunTrainsDialog::OnTerminatebutton() 
{
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    //
    //  Bring up terminate train dialog (to get final location of cars)
    //
    CTerminateTrainDialog pDlg;	
    pDlg.SetTrainID(iTrains_Scheduled_FK);
    if( pDlg.DoModal() != IDOK )
        return;
    //
    ReadDB();
}

/*
When advancing the date:

Terminate all running trains (force user to do this)

Increment date in database
For each Car
  If Car at industry
    If 'filling' and deliverytime+loadempty time >= CurrentDate (car arrived empty)
      Mark car as Full
    If 'emptying' and deliverytime+loadempty time >= CurrentDate (car arrived full)
      Mark car as Empty
  End if
  If Car off-line
    If deliverytime+offsite load/empty time >= CurrentDate
      Make car available
  End if
End For 

Clear out 'filled' orders in Industry_Car_Orders table
    id
    Industry_FK
    CarType_FK
    loadempty
    Date
    Train_Scheduled_FK

For each Industry
  Calculate cars IN needed
  Add record to Industry_Car_Orders table for each car IN needed
  Calculate cars OUT needed
  Add record to Industry_Car_Orders table for each car OUT needed
End For

*/
void CRunTrainsDialog::OnAdvance() 
{
    if( MessageBox("Are you sure you want to advance the date?","LSC TrainOps",MB_YESNO) != IDYES )
        return;
    //
    //  Check for running trains
    //
    for( int i=0 ; i<m_ctlRunningTrainsList.GetItemCount();i++ )
    {
        int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(i);
        CTerminateTrainDialog pDlg;	
        pDlg.SetTrainID(iTrains_Scheduled_FK);
        if( pDlg.DoModal() != IDOK )
        {
            MessageBox("You must terminate all trains before when you advance the date!","LSC TrainOps",MB_ICONSTOP);
            return;
        }
    }
    //
    AdvanceTheDate();
}

void CRunTrainsDialog::AdvanceTheDate()
{
    //
    long lJulDate = JulianDate::YmdToJd( m_ctlCurrentDate.GetYear(), m_ctlCurrentDate.GetMonth(), m_ctlCurrentDate.GetDay() );
    int y,m,d;
    JulianDate::JdToYmd( ++lJulDate, &y,&m,&d);
    m_ctlCurrentDate.SetDate(y,m,d);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    pDB->execDML("UPDATE _Config SET date=date+1 WHERE id=1;");
    CppSQLite3Query q = pDB->execQuery("SELECT date FROM _Config WHERE id=1");
    int iCurrentDate = q.getIntField(0);
    q.finalize();
    //
    CString sSQL;
    try {
        pDB->execDML("DROP TABLE _cars");
    }
    catch(...) {}
    sSQL.Format("CREATE TEMP TABLE _cars AS SELECT id,Industry_FK,delivery_date,loadempty,CarTypes_FK FROM Cars");
    q = pDB->execQuery(sSQL);
    //
    sSQL.Format("SELECT id,Industry_FK,delivery_date,loadempty,CarTypes_FK FROM _cars");
    q = pDB->execQuery(sSQL);
    while (!q.eof())
    {
        int iCarFK = q.getIntField("id");
        int iIndustryFK = q.getIntField("Industry_FK");
        int iDeliveryDate = q.getIntField("delivery_date");
        int iLoadEmpty = q.getIntField("loadempty");
        int iCarTypeFK = q.getIntField("CarTypes_FK");

        if( iIndustryFK > 0 )
        {
            sSQL.Format("SELECT LoadEmptyDays FROM Industries_Cars WHERE Industries_FK = %d AND CarTypes_FK=%d;",iIndustryFK,iCarTypeFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            int iLoadEmptyDays = 1;
            if( !q2.eof() )
                iLoadEmptyDays = q2.getIntField("LoadEmptyDays");
            q2.finalize();
            if( iLoadEmpty == 0 )       //  is loaded now?
            {
                if( iDeliveryDate + iLoadEmptyDays >= iCurrentDate )
                {
                    sSQL.Format("UPDATE Cars SET loadempty=1 WHERE id=%d",iCarFK);
                    //TRACE(sSQL);
                    pDB->execDML(sSQL);
                }
            }
            else                        //  is empty now?
            {
                if( iDeliveryDate + iLoadEmptyDays >= iCurrentDate )
                {
                    sSQL.Format("UPDATE Cars SET loadempty=0 WHERE id=%d",iCarFK);
                    //TRACE(sSQL);
                    pDB->execDML(sSQL);
                }
            }
        }
        q.nextRow();
    }
    q.finalize();
    //
    pDB->execDML("DROP TABLE _cars");
    //
    //  anything still in Industries_Car_Orders without a Trains_Scheduled_FK gets moved to Cars_Not_Found
    //
    sSQL.Format("SELECT * FROM Industries_Car_Orders WHERE Trains_Scheduled_FK==-1");
    q = pDB->execQuery(sSQL);
    while (!q.eof())
    {
        sSQL.Format("INSERT INTO Cars_Not_Found (Industries_FK,CarTypes_FK,train_date) VALUES (%d,%d,%d);",q.getIntField("Industries_FK"),q.getIntField("CarTypes_FK"),((CTrainOpsApp*)::AfxGetApp())->GetDate()-1);
        pDB->execDML(sSQL);
        q.nextRow();
    }
    q.finalize();
    //
    pDB->execDML("DELETE FROM Industries_Car_Orders");
    //
    if( g_iMethod == METHOD_CARS )
    {
        sSQL.Format("SELECT * FROM Industries");
        q = pDB->execQuery(sSQL);
        while (!q.eof())
        {
            //
            //  See what cars this industry needs:
            //      For each CarType in Industries_Cars for this Industry
            //      See if we need this car (check dates)
            //      Get car quantity
            //      insert record into Industries_Car_Orders
            //
            sSQL.Format("SELECT * FROM Industries_Cars WHERE Industries_FK=%d;",q.getIntField("id"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            while (!q2.eof())
            {
                if( NeedThisCar(q.getIntField("id"),q2.getIntField("CarTypes_FK")) )
                {
                    int iNumCars = GetCarQuantity(q2.getIntField("Quantity_high"),q2.getIntField("Quantity_low"),q2.getIntField("Quantity_percentage"));
                    for( int i=0;i<iNumCars;i++ )
                    {
                        sSQL.Format("INSERT INTO Industries_Car_Orders (Industries_FK,CarTypes_FK,loadempty) VALUES (%d,%d,%d);",
                            q.getIntField("id"),q2.getIntField("CarTypes_FK"),q2.getIntField("InOut")==0?0:1);
                        pDB->execDML(sSQL);
                    }
                }
                q2.nextRow();
            }
            q2.finalize();
            q.nextRow();
        }
        q.finalize();
        //
        //  Check industry sidings for cars that are already there, and needed,
        //  but not spotted at the industry.  Move them to the industry if they are.
        //  (When Empty Return To) implementation
        //
        sSQL.Format("SELECT * FROM Industries_Car_Orders");
        q = pDB->execQuery(sSQL);
        while (!q.eof())
        {
            //
            //  get industry key, and check its siding for cars already there,
            //  but not spotted at any industry
            //
            int iIndustryFK = q.getIntField("Industries_FK");
            sSQL.Format("SELECT Sidings_FK FROM Industries WHERE id=%d;",iIndustryFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
            {
                sSQL.Format("SELECT * FROM Cars WHERE returnto > 0 AND loadempty=1 AND Industry_FK=-1 AND Siding_FK=%d AND CarTypes_FK=%d AND id NOT IN (%s);",q2.getIntField("Sidings_FK"),q.getIntField("CarTypes_FK"),GetScheduledCars());
                CppSQLite3Query q3 = pDB->execQuery(sSQL);
                if( !q3.eof() )
                {
                    //  got a car, move it to the industry
                    int iCarFK = q3.getIntField("id");
                    q3.finalize();
                    sSQL.Format("UPDATE Cars SET Trains_FK=0,Industries_FK=%d,delivery_date=%d WHERE id=%d;",iIndustryFK,((CTrainOpsApp*)::AfxGetApp())->GetDate(),iCarFK);
                    pDB->execDML(sSQL);
                }
                else
                    q3.finalize();
            }
            q2.finalize();
            //
            q.nextRow();
        }
        q.finalize();
    }
    else
    {
        ASSERT("Commodities not implemented!!!");
    }
    //
    ReadDB();
    ((CTrainOpsApp*)AfxGetApp())->SetTitle();
    //
}

void CRunTrainsDialog::OnItemchangingTrainlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//
    //  check the current date against the last run date
    //
    CString sDate = m_ctlTrainList.GetItemText(pNMListView->iItem,4);
    if( sDate.IsEmpty() )
    {
        *pResult=0;
        return;
    }
    int y,m,d;  //  01/01/1950
    m=atoi((LPCTSTR)sDate);
    d=atoi((LPCTSTR)sDate.Mid(3));
    y=atoi((LPCTSTR)sDate.Mid(6));
    long lRunDate = JulianDate::YmdToJd(y,m,d);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    if( !q.eof() )
    {
        long lCurDate = q.getIntField(0)+q.getIntField(1);
        q.finalize();
//        TRACE("CurDate: %ld, RunDate: %ld\n",lCurDate,lRunDate);
        if( lCurDate > lRunDate )
            *pResult=0;
        else
        {
            *pResult=TRUE;
        }
    }
    else
    {
        q.finalize();
	    *pResult = 0;
    }
}

void CRunTrainsDialog::OnItemchangedCabooselist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
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

    CheckButtons();
	
	*pResult = 0;
}

BOOL CRunTrainsDialog::DestroyWindow() 
{
//    if( m_pDebugDialog )
//        delete m_pDebugDialog;
	
	return CDialog::DestroyWindow();
}

int CRunTrainsDialog::GetTrainFK(int iTrains_Scheduled_FK)
{
    CString sSQL;
    sSQL.Format("SELECT Trains_FK FROM Trains_Scheduled WHERE id=%d",iTrains_Scheduled_FK);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery(sSQL);
    if( q.eof() )
    {
        return 0;
    }
    int iTrainFK = q.getIntField(0);
    q.finalize();
    return iTrainFK;
}

void CRunTrainsDialog::OnTrainDetailButton() 
{
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    //
    int iTrainFK = GetTrainFK(iTrains_Scheduled_FK);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    try { pDB->execDML("DROP TABLE _TSC;"); } catch (...) {}
    CString sSQL;
#ifdef _DEBUG
    sSQL.Format("CREATE TABLE _TSC AS SELECT * FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d",iTrains_Scheduled_FK);
#else
    sSQL.Format("CREATE TEMP TABLE _TSC AS SELECT * FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d",iTrains_Scheduled_FK);
#endif
    pDB->execDML(sSQL);
    //
    CTrainDetailDialog pTrainDetailDialog;
    pTrainDetailDialog.SetTrainInfo(iTrainFK,iTrains_Scheduled_FK);
    if( pTrainDetailDialog.DoModal() == IDCANCEL )
        return;
    //
    sSQL.Format("UPDATE Trains_Scheduled SET notes=\"%s\" WHERE id=%d",pTrainDetailDialog.GetTrainNotes(),iTrains_Scheduled_FK);
    pDB->execDML(sSQL);
    //
    //  update Trains_Scheduled_Cars table from TSC
    //
    m_pCarTable.clear();
    //
    sSQL.Format("SELECT * FROM _TSC");
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while (!q.eof())
    {
        CCar pCar;
        pCar.m_iCar_FK = q.getIntField("Cars_FK");
        pCar.m_iIndustryFromFK = q.getIntField("Industry_from_FK");
        pCar.m_iIndustryToFK = q.getIntField("Industry_to_FK");
        pCar.m_iLocationFromFK = q.getIntField("Location_from_FK");
        pCar.m_iLocationToFK = q.getIntField("Location_to_FK");
        pCar.m_iSidingFromFK = q.getIntField("Siding_from_FK");
        pCar.m_iSidingToFK = q.getIntField("Siding_to_FK");
        pCar.m_bEmpty = q.getIntField("loadempty") == 1;
        m_pCarTable.push_back(pCar);
        //
        q.nextRow();
    }
    q.finalize();
    //
    sSQL.Format("DELETE FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d",iTrains_Scheduled_FK);
    pDB->execDML(sSQL);
    //
    //  Add cars to Trains_Scheduled_Cars (Pickups need location, too)
    //
    for( int i=0;i<m_pCarTable.size();i++)
    {
        CCar pCar;
        pCar = m_pCarTable.at(i);
        sSQL.Format("INSERT INTO Trains_Scheduled_Cars (Trains_Scheduled_FK,Cars_FK,Industry_from_FK,Industry_to_FK,Location_from_FK,Location_to_FK,Siding_from_FK,Siding_to_FK,loadempty) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d)",
            iTrains_Scheduled_FK,                       //  Trains_Scheduled_FK
            pCar.m_iCar_FK,                             //  Cars_FK
            pCar.m_iIndustryFromFK,                     //  Industry_from_FK
            pCar.m_iIndustryToFK,                       //  Industry_to_FK
            pCar.m_iLocationFromFK,                     //  Location_from_FK
            pCar.m_iLocationToFK,                       //  Location_to_FK
            pCar.m_iSidingFromFK,                       //  Siding from FK
            pCar.m_iSidingToFK,                         //  Siding to FK
            pCar.m_bEmpty?1:0                           //  Load/Empty
            );
        pDB->execDML(sSQL);
    }
}

void CRunTrainsDialog::OnForms() 
{
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    //
    PrintSwitchlist(iTrains_Scheduled_FK);
    //
    PrintForm19s(iTrains_Scheduled_FK);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars, Cars WHERE Trains_Scheduled_Cars.Trains_Scheduled_FK=%d AND Trains_Scheduled_Cars.Cars_FK=Cars.id AND out_of_service=1",iTrains_Scheduled_FK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    BOOL bBadOrder(FALSE);
    if( !q.eof() )
        bBadOrder = TRUE;
    q.finalize();
    if( bBadOrder )
	    PrintBadOrderCards(iTrains_Scheduled_FK);
    //	
    SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void CRunTrainsDialog::OnSwitchlist()
{
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    //
    PrintSwitchlist(iTrains_Scheduled_FK);
    //	
    SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void CRunTrainsDialog::OnForm19()
{
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    //
    PrintForm19s(iTrains_Scheduled_FK);
    //	
    SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void CRunTrainsDialog::OnBadOrder()
{
    int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(m_ctlRunningTrainsList.GetNextItem(-1, LVNI_SELECTED));
    if( iTrains_Scheduled_FK == -1 )
        return;
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars, Cars WHERE Trains_Scheduled_Cars.Trains_Scheduled_FK=%d AND Trains_Scheduled_Cars.Cars_FK=Cars.id AND out_of_service=1",iTrains_Scheduled_FK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    BOOL bBadOrder(FALSE);
    if( !q.eof() )
        bBadOrder = TRUE;
    q.finalize();
    if( bBadOrder )
	    PrintBadOrderCards(iTrains_Scheduled_FK);
    else
        MessageBox("No bad order cars on this train");
    //	
    SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void CRunTrainsDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_ctlRunningTrainsList.SaveState("State","RunTrainsDialog_m_ctlRunningTrainsList");
	CDialog::OnOK();
}

void CRunTrainsDialog::OnReports() 
{
    CScheduledTrainsReportsDialog pDlg;	
    pDlg.DoModal();
}

void CRunTrainsDialog::OnRecalc() 
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  Check for running trains
    //
    for( int i=0 ; i<m_ctlRunningTrainsList.GetItemCount();i++ )
    {
        int iTrains_Scheduled_FK = m_ctlRunningTrainsList.GetItemData(i);
        CTerminateTrainDialog pDlg;	
        pDlg.SetTrainID(iTrains_Scheduled_FK);
        if( pDlg.DoModal() != IDOK )
        {
            MessageBox("You cannot recalculate industry demands when there are scheduled trains!","LSC TrainOps",MB_ICONSTOP);
            return;
        }
    }
    //
    if( MessageBox("Are you sure you want to recalculate industry demands?","LSC TrainOps",MB_YESNO) != IDYES )
        return;
    //
    pDB->execDML("DELETE FROM Industries_Car_Orders");
    //
    if( g_iMethod == METHOD_CARS )
    {
        CString sSQL = "SELECT * FROM Industries";
        CppSQLite3Query q = pDB->execQuery(sSQL);
        while (!q.eof())
        {
            //
            //  See what cars this industry needs:
            //      For each CarType in Industries_Cars for this Industry
            //      See if we need this car (check dates)
            //      Get car quantity
            //      insert record into Industries_Car_Orders
            //
            sSQL.Format("SELECT * FROM Industries_Cars WHERE Industries_FK=%d;",q.getIntField("id"));
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            while (!q2.eof())
            {
                if( NeedThisCar(q.getIntField("id"),q2.getIntField("CarTypes_FK")) )
                {
                    int iNumCars = GetCarQuantity(q2.getIntField("Quantity_high"),q2.getIntField("Quantity_low"),q2.getIntField("Quantity_percentage"));
                    for( int i=0;i<iNumCars;i++ )
                    {
                        sSQL.Format("INSERT INTO Industries_Car_Orders (Industries_FK,CarTypes_FK,loadempty) VALUES (%d,%d,%d);",
                            q.getIntField("id"),q2.getIntField("CarTypes_FK"),q2.getIntField("InOut")==0?0:1);
                        pDB->execDML(sSQL);
                    }
                }
                q2.nextRow();
            }
            q2.finalize();
            q.nextRow();
        }
        q.finalize();
        //
        //  Check industry sidings for cars that are already there, and needed,
        //  but not spotted at the industry.  Move them to the industry if they are.
        //  (When Empty Return To) implementation
        //
        sSQL.Format("SELECT * FROM Industries_Car_Orders");
        q = pDB->execQuery(sSQL);
        while (!q.eof())
        {
            //
            //  get industry key, and check its siding for cars already there,
            //  but not spotted at any industry
            //
            int iIndustryFK = q.getIntField("Industries_FK");
            sSQL.Format("SELECT Sidings_FK FROM Industries WHERE id=%d;",iIndustryFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if( !q2.eof() )
            {
                sSQL.Format("SELECT * FROM Cars WHERE returnto > 0 AND loadempty=1 AND Industry_FK=-1 AND Siding_FK=%d AND CarTypes_FK=%d AND id NOT IN (%s);",q2.getIntField("Sidings_FK"),q.getIntField("CarTypes_FK"),GetScheduledCars());
                CppSQLite3Query q3 = pDB->execQuery(sSQL);
                if( !q3.eof() )
                {
                    //  got a car, move it to the industry
                    int iCarFK = q3.getIntField("id");
                    q3.finalize();
                    sSQL.Format("UPDATE Cars SET Trains_FK=0,Industries_FK=%d,delivery_date=%d WHERE id=%d;",iIndustryFK,((CTrainOpsApp*)::AfxGetApp())->GetDate(),iCarFK);
                    pDB->execDML(sSQL);
                }
                else
                    q3.finalize();
            }
            q2.finalize();
            //
            q.nextRow();
        }
        q.finalize();
    }
    else
    {
        ASSERT("Commodities not implemented!!!");
    }
}
