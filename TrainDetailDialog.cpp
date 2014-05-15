// TrainDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "TrainDetailDialog.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainDetailDialog dialog

#define UM_CHECKSTATECHANGE (WM_USER + 100)

#define NODETYPE_LOCATION 0
#define NODETYPE_CAR 1

struct node_data
{
    int iType;          //  0=location, 1=car
    std::string sText;  //  text
    int iKey;           //  key based on type (iLocationFK or iCarFK)
};

//////////////////////

CTrainDetailDialog::CTrainDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainDetailDialog)
	m_bIgnoreRestrictions = FALSE;
	m_sTrainNotes = _T("");
	//}}AFX_DATA_INIT
    m_iLocomotive = -1;
    m_iCaboose = -1;
}


void CTrainDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainDetailDialog)
	DDX_Control(pDX, IDC_TRAINNOTES, m_ctlTrainNotes);
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Control(pDX, IDC_IGNORERESTRICTIONS, m_ctlIgnoreRestrictions);
	DDX_Control(pDX, IDC_LOCOMOTIVELIST, m_ctlLocomotiveList);
	DDX_Control(pDX, IDC_CABOOSELIST, m_ctlCabooseList);
	DDX_Control(pDX, IDC_TRAINDETAIL, m_ctlTrainDetail);
	DDX_Check(pDX, IDC_IGNORERESTRICTIONS, m_bIgnoreRestrictions);
	DDX_Text(pDX, IDC_TRAINNOTES, m_sTrainNotes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CTrainDetailDialog)
	ON_NOTIFY(NM_CLICK, IDC_TRAINDETAIL, OnClickTraindetail)
	ON_BN_CLICKED(IDC_IGNORERESTRICTIONS, OnIgnorerestrictions)
	ON_CBN_SELCHANGE(IDC_LOCOMOTIVELIST, OnSelChangeLocomotiveList)
	//}}AFX_MSG_MAP
    ON_MESSAGE(UM_CHECKSTATECHANGE, OnCheckStateChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainDetailDialog message handlers

BOOL CTrainDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_ctlTrainDetail.ModifyStyle( TVS_CHECKBOXES, 0 );
    m_ctlTrainDetail.ModifyStyle( 0, TVS_CHECKBOXES );
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT name, trainid FROM Trains WHERE id=%d",m_iTrainFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    if( q.eof() )
    {
        MessageBox("Unable to verify train information!");
        return FALSE;
    }
    CString sTitle;
    sTitle.Format("Train Detail for '%s', Train # %s",q.getStringField("name"),q.getStringField("trainid"));
    SetWindowText(sTitle);
    //
    q.finalize();
    //
    m_ctlTrainNotes.SetInvalidCharacters("\"");
    //
//    sSQL.Format("SELECT Location_FK FROM Trains_Location WHERE Train_FK=%d ORDER BY route_order;",m_iTrainFK);
    sSQL.Format("SELECT Locations.name,Locations.id,Locations.notes FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order;",m_iTrainFK);
    //
    CppSQLite3Query qRoute = pDB->execQuery(sSQL);
    int iLastLocation=0;
    int iCars=0;
    HTREEITEM hParent(0);
    CArray <int,int> pDeliveredCarsArray;
    while (!qRoute.eof())
    {
        int iLocationID = qRoute.getIntField("id");
        if( iLastLocation != iLocationID)
        {
            iLastLocation = iLocationID;
            //CString sSQL;
            //sSQL.Format("SELECT name FROM Locations WHERE id=%d",iLocationID);
            //CppSQLite3Query qLocation = pDB->execQuery(sSQL);
            //if( !qLocation.eof() )
            {
                //
                //  FIXME: industries for this location, THEN cars for this industry (see reports.cpp)
                //
                CString sText;
                sText.Format("%s - _ setouts / _ pickups",qRoute.getStringField("name"));
                hParent = m_ctlTrainDetail.InsertItem(sText,TVI_ROOT);

                node_data* pNodeData = new node_data();
                pNodeData->iType = NODETYPE_LOCATION;
                pNodeData->sText = qRoute.getStringField("name");
                pNodeData->iKey = iLocationID;

                m_ctlTrainDetail.SetItemData(hParent,(DWORD)pNodeData);
                //
                //  get cars from this location
                //
                sSQL.Format("SELECT * FROM _TSC WHERE Location_from_FK=%d",iLocationID);
                CppSQLite3Query qCarsFrom = pDB->execQuery(sSQL);
                int iPickups=0;
                while(!qCarsFrom.eof())
                {
                    iPickups++;
                    //
                    //  get car info
                    //
                    sSQL.Format("SELECT car_number,reporting_marks,description FROM Cars,CarTypes WHERE Cars.id=%d AND CarTypes.id=Cars.CarTypes_FK",qCarsFrom.getIntField("Cars_FK"));
                    CppSQLite3Query qCar = pDB->execQuery(sSQL);
                    if( !qCar.eof() )
                    {
                        TRACE("From %s %s %s\n",qCar.getStringField("description"),qCar.getStringField("reporting_marks"),qCar.getStringField("car_number"));
                        //
                        //  get industry or siding at this location
                        //
                        //  if empty and offlayout, then we're going to a siding
                        //
                        CString sFrom;
                        //  temp un-commented
                        /*
                        if( qCarsFrom.getIntField("loadempty") == 1 && qCarsFrom.getIntField("offlayout") == 1 )
                        {
                            TRACE("Empty from offlayout\n");
                            sSQL.Format("SELECT name FROM Sidings WHERE id=%d",qCarsFrom.getIntField("Siding_from_FK"));
                            //sSQL.Format("SELECT name FROM Sidings WHERE id=%d",qCarsFrom.getIntField("Siding_to_FK"));
                            CppSQLite3Query qSiding = pDB->execQuery(sSQL);
                            //CString sDest("");
                            if( !qSiding.eof() )
                            {
                                CString sWork = qSiding.getStringField("name");
                                sWork.MakeUpper();
                                if( sWork.Find("SIDING") >= 0 )
                                    sFrom.Format("%s",qSiding.getStringField("name"));
                                else
                                    sFrom.Format("'%s' siding",qSiding.getStringField("name"));
                            }
                            qSiding.finalize();
                        }
                        //
                        else*/
                        {
                            sSQL.Format("SELECT Industries.name,Locations.name FROM Industries,Sidings,Locations WHERE Industries.id=%d AND Industries.Sidings_FK=Sidings.id AND Sidings.Locations_FK=Locations.id",qCarsFrom.getIntField("Industry_from_FK"));
                            CppSQLite3Query qIndustry = pDB->execQuery(sSQL);
                            CString sDest("");
                            if( !qIndustry.eof() )
                            {
                                //  Pickup from industry
                                sFrom.Format("%s at %s",qIndustry.getStringField(0),qIndustry.getStringField(1));
                            }
                            else
                            {
                                //  Not an industry, so see if its a siding or what?
                                //int iSidingFrom = qCarsFrom.getIntField("Siding_from_FK");
                                //sSQL.Format("SELECT name,interchange FROM Sidings WHERE id=%d;",iSidingFrom);
                                //CppSQLite3Query qSiding = pDB->execQuery(sSQL);
                                //if( qSiding.eof() )
                                {
                                    sFrom.Format("%s yardmaster",qRoute.getStringField("name"));
                                }
                                /*else
                                {
                                    if( qSiding.getIntField("interchange") == 1)
                                        //sFrom.Format("%s Interchange on %s",qRoute.getStringField("name"),qSiding.getStringField("name"));
                                        sFrom.Format("%s Interchange",qRoute.getStringField("name"));
                                    else
                                        sFrom.Format("%s",qSiding.getStringField("name"));
                                }
                                qSiding.finalize();*/
                            }
                        }
                        CString sWork;
                        sWork.Format("%s %s %s (%s) pickup from %s",qCar.getStringField("description"),qCar.getStringField("reporting_marks"),qCar.getStringField("car_number"),qCarsFrom.getIntField("loadempty")==1?"Empty":"Load",(LPCTSTR)sFrom);
                        HTREEITEM pItem = m_ctlTrainDetail.InsertItem(sWork,hParent,TVI_LAST);

                        node_data* pNodeData = new node_data();
                        pNodeData->iType = NODETYPE_CAR;
                        CString sText;
                        sText.Format("%s %s %d",qCar.getStringField("description"),qCar.getStringField("reporting_marks"),qCar.getIntField("car_number"));
                        pNodeData->sText = (LPCTSTR)sText;
                        pNodeData->iKey = qCarsFrom.getIntField("Cars_FK");

                        m_ctlTrainDetail.SetItemData(pItem,(DWORD)pNodeData);
                        m_ctlTrainDetail.SetCheck(pItem,TRUE);
                        m_ctlTrainDetail.Expand(hParent,TVE_EXPAND);
                    }
                    qCar.finalize();
                    //
                    qCarsFrom.nextRow();
                }
                qCarsFrom.finalize();
                //
                //  get cars bound for this location
                //
                sSQL.Format("SELECT * FROM _TSC WHERE Location_to_FK=%d",iLocationID);
                CppSQLite3Query qCarsTo = pDB->execQuery(sSQL);
                int iDeliveries=0;
                while(!qCarsTo.eof())
                {
                    iDeliveries++;
                    pDeliveredCarsArray.Add(qCarsTo.getIntField("Cars_FK"));

                    //
                    //  get car info
                    //
                    //sSQL.Format("SELECT car_number,reporting_marks,description FROM Cars,CarTypes WHERE Cars.id=%d AND CarTypes.id=Cars.CarTypes_FK",qCarsTo.getIntField("Cars_FK"));
                    sSQL.Format("SELECT car_number,reporting_marks,CarTypes_FK FROM Cars WHERE Cars.id=%d",qCarsTo.getIntField("Cars_FK"));
                    CppSQLite3Query q3 = pDB->execQuery(sSQL);
                    if( !q3.eof() )
                    {
                        sSQL.Format("SELECT description FROM CarTypes WHERE id=%d",q3.getIntField("CarTypes_FK"));
                        CppSQLite3Query q4 = pDB->execQuery(sSQL);
                        CString sDescription("*unknown*");
                        if( !q4.eof() )
                            sDescription = q4.getStringField("description");
                        q4.finalize();
                        //
                        //  get industry or siding at this location
                        //
                        int iSidingCap=0;
                        CString sTo;
                        if( qCarsTo.getIntField("offlayout") == 1 )
                        {
                            sSQL.Format("SELECT * FROM Sidings WHERE id=%d",qCarsTo.getIntField("Siding_to_FK"));
                            CppSQLite3Query q4 = pDB->execQuery(sSQL);
                            CString sDest("");
                            if( !q4.eof() )
                            {
                                CString sWork = q4.getStringField("name");
                                sWork.MakeUpper();
                                if( q4.getIntField("yard") == 1 )
                                {
                                    sTo.Format("%s yardmaster",qRoute.getStringField("name"));
                                }
                                else 
                                {
                                    if( sWork.Find("SIDING") >= 0 )
                                        sTo.Format("%s",q4.getStringField("name"));
                                    else
                                        sTo.Format("'%s' siding",q4.getStringField("name"));
                                }
                                iSidingCap = q4.getIntField("capacity");
                            }
                            q4.finalize();
                        }
                        else
                        {
                            if( qCarsTo.getIntField("Industry_to_FK") > 0 )
                            {
                                sSQL.Format("SELECT Industries.name,Locations.name,Sidings.capacity FROM Industries,Sidings,Locations WHERE Industries.id=%d AND Industries.Sidings_FK=Sidings.id AND Sidings.Locations_FK=Locations.id",qCarsTo.getIntField("Industry_to_FK"));
                                CppSQLite3Query q4 = pDB->execQuery(sSQL);
                                CString sDest("");
                                if( !q4.eof() )
                                    sTo.Format("%s at %s",q4.getStringField(0),q4.getStringField(1));
                                iSidingCap = q4.getIntField("capacity");
                                q4.finalize();
                            }
                            else
                            {
                                if( qCarsTo.getIntField("Siding_to_FK") > 0 )
                                {
                                    sSQL.Format("SELECT * FROM Sidings WHERE id=%d",qCarsTo.getIntField("Siding_to_FK"));
                                    CppSQLite3Query q4 = pDB->execQuery(sSQL);
                                    CString sDest("");
                                    if( !q4.eof() )
                                    {
                                        CString sWork = q4.getStringField("name");
                                        sWork.MakeUpper();
                                        if( q4.getIntField("yard") == 1 )
                                        {
                                            sTo.Format("%s yardmaster",qRoute.getStringField("name"));
                                        }
                                        else 
                                        {
                                            if( sWork.Find("SIDING") >= 0 )
                                                sTo.Format("%s",q4.getStringField("name"));
                                            else
                                                sTo.Format("'%s' siding",q4.getStringField("name"));
                                        }
                                        iSidingCap = q4.getIntField("capacity");
                                    }
                                    q4.finalize();
                                }
                            }
                        }
                        //  fix for empty siding?
                        if( sTo.IsEmpty() )
                        {
                            int iSidingFK = GetSiding(iLocationID);
                            //
                            sSQL.Format("SELECT * FROM Sidings WHERE id=%d",iSidingFK);
                            CppSQLite3Query q4 = pDB->execQuery(sSQL);
                            CString sDest("");
                            if( !q4.eof() )
                            {
                                CString sWork = q4.getStringField("name");
                                sWork.MakeUpper();
                                if( q4.getIntField("yard") == 1 )
                                {
                                    sTo.Format("%s yardmaster",qRoute.getStringField("name"));
                                }
                                else
                                {
                                    if( sWork.Find("SIDING") >= 0 )
                                        sTo.Format("%s",q4.getStringField("name"));
                                    else
                                        sTo.Format("'%s' siding",q4.getStringField("name"));
                                }
                            }
                            q4.finalize();
                        }
                        //
                        //  see if delivery will exceed siding capacity
                        //  (iDeliveries + cars on siding > siding capacity)
                        //
                        sSQL.Format("SELECT count(*) AS carcount FROM Cars WHERE Siding_FK=%d",qCarsTo.getIntField("Siding_to_FK"));
                        CppSQLite3Query q5 = pDB->execQuery(sSQL);
                        int iCarsOnSiding=0;
                        if( !q5.eof())
                        {
                            iCarsOnSiding = q5.getIntField("carcount");
                        }
                        q5.finalize();
                        //
                        if( iCarsOnSiding + iDeliveries > iSidingCap )
                        {
                            // TODO: finish what to do in this situation
                        }
                        //
                        CString sWork;
                        sWork.Format("%s %s %s (%s) deliver to %s",sDescription,q3.getStringField("reporting_marks"),q3.getStringField("car_number"),qCarsTo.getIntField("loadempty")==1?"Empty":"Load",(LPCTSTR)sTo);
                        HTREEITEM pItem = m_ctlTrainDetail.InsertItem(sWork,hParent,TVI_LAST);

                        node_data* pNodeData = new node_data();
                        pNodeData->iType = NODETYPE_CAR;
                        CString sText;
                        sText.Format("%s %s %s",sTo,q3.getStringField("reporting_marks"),q3.getStringField("car_number"));
                        pNodeData->sText = (LPCTSTR)sText;
                        pNodeData->iKey = qCarsTo.getIntField("Cars_FK");

                        m_ctlTrainDetail.SetItemData(pItem,(DWORD)pNodeData);
                        m_ctlTrainDetail.SetCheck(pItem,TRUE);
                        m_ctlTrainDetail.Expand(hParent,TVE_EXPAND);
                    }
                    q3.finalize();
                    //
                    qCarsTo.nextRow();
                }
                qCarsTo.finalize();
                //
                iCars = iCars + iPickups - iDeliveries;
                sText.Format("%s - %d setouts / %d pickups (%d cars in train) ",qRoute.getStringField("name"),iDeliveries,iPickups,iCars);
                m_ctlTrainDetail.SetItemText(hParent,sText);
                //
                //qLocation.nextRow();
            }
            //qLocation.finalize();
            //
            m_ctlTrainDetail.SetCheck(hParent,TRUE);
        }

        //
        qRoute.nextRow();
    }
    qRoute.finalize();
    //
    //  which cars in TSS DONT get dropped off this time?
    //
    sSQL.Format("SELECT * FROM _TSC");
    CppSQLite3Query qCarsEnd = pDB->execQuery(sSQL);
    while(!qCarsEnd.eof())
    {
        int *pTest;
        int N = pDeliveredCarsArray.GetSize();
        if( N > 0 )
        {
            pTest = std::find(&pDeliveredCarsArray[0] , &pDeliveredCarsArray[0]+N , qCarsEnd.getIntField("Cars_FK"));
            if (pTest != &pDeliveredCarsArray[0]+N)
            {
                qCarsEnd.nextRow();
                continue;
            }
        }
        //
        //  get car info
        //
        sSQL.Format("SELECT car_number,reporting_marks,description FROM Cars,CarTypes WHERE Cars.id=%d AND CarTypes.id=Cars.CarTypes_FK",qCarsEnd.getIntField("Cars_FK"));
        CppSQLite3Query q3 = pDB->execQuery(sSQL);
        if( !q3.eof() )
        {
            //
            //  get industry or siding at this location
            //
            //  if empty and offlayout, then we're going to a siding
            //
            CString sTo;
            if( qCarsEnd.getIntField("loadempty") == 1 && qCarsEnd.getIntField("offlayout") == 1 )
            {
                sSQL.Format("SELECT name FROM Sidings WHERE id=%d",qCarsEnd.getIntField("Siding_to_FK"));
                CppSQLite3Query q4 = pDB->execQuery(sSQL);
                CString sDest("");
                if( !q4.eof() )
                {
                    CString sWork = q4.getStringField(0);
                    sWork.MakeUpper();
                    if( sWork.Find("SIDING") >= 0 )
                        sTo.Format("%s",q4.getStringField(0));
                    else
                        sTo.Format("'%s' siding",q4.getStringField(0));
                }
                q4.finalize();
            }
            else
            {
                if( qCarsEnd.getIntField("Industry_to_FK") == qCarsEnd.getIntField("Industry_from_FK") )
                {
                    sSQL.Format("SELECT name FROM Locations WHERE id=%d",iLastLocation);
                    CppSQLite3Query q4 = pDB->execQuery(sSQL);
                    if( !q4.eof() )
                        sTo.Format("%s yardmaster",q4.getStringField("name"));
                    else
                        sTo = "yardmaster";
                }
                else
                {
                    sSQL.Format("SELECT Industries.name,Locations.name,Sidings.capacity FROM Industries,Sidings,Locations WHERE Industries.id=%d AND Industries.Sidings_FK=Sidings.id AND Sidings.Locations_FK=Locations.id",qCarsEnd.getIntField("Industry_to_FK"));
                    CppSQLite3Query q4 = pDB->execQuery(sSQL);
                    if( !q4.eof() )
                        sTo.Format("%s at %s",q4.getStringField(0),q4.getStringField(1));
                }
            }
            //  fix for empty sidings?
            if( sTo.IsEmpty() )
            {
                int iSidingFK = GetSiding(iLastLocation);
                //
                sSQL.Format("SELECT name FROM Sidings WHERE id=%d",iSidingFK);
                CppSQLite3Query q4 = pDB->execQuery(sSQL);
                //CString sDest("");
                if( !q4.eof() )
                {
                    CString sWork = q4.getStringField(0);
                    sWork.MakeUpper();
                    if( sWork.Find("SIDING") >= 0 )
                        sTo.Format("%s",q4.getStringField(0));
                    else
                        sTo.Format("'%s' siding",q4.getStringField(0));
                }
                q4.finalize();
            }
            //
            //  see if delivery will exceed siding capacity
            //
            
            //
            CString sWork;
            sWork.Format("%s %s %s (%s) deliver to %s",q3.getStringField("description"),q3.getStringField("reporting_marks"),q3.getStringField("car_number"),qCarsEnd.getIntField("loadempty")==1?"Empty":"Load",(LPCTSTR)sTo);
            HTREEITEM pItem = m_ctlTrainDetail.InsertItem(sWork,hParent,TVI_LAST);

            node_data* pNodeData = new node_data();
            pNodeData->iType = NODETYPE_CAR;
            CString sText;
            sText.Format("%s %s %s",q3.getStringField("description"),q3.getStringField("reporting_marks"),q3.getStringField("car_number"));
            pNodeData->sText = (LPCTSTR)sText;
            pNodeData->iKey = qCarsEnd.getIntField("Cars_FK");

            m_ctlTrainDetail.SetItemData(pItem,(DWORD)pNodeData);
            m_ctlTrainDetail.SetCheck(pItem,TRUE);
            m_ctlTrainDetail.Expand(hParent,TVE_EXPAND);
        }
        q3.finalize();
        
        
        qCarsEnd.nextRow();
    }
    qCarsEnd.finalize();
    //
    //  fill locomotive and caboose lists
    //
    q = pDB->execQuery("SELECT id,road_name,car_number,capacity,description FROM Locomotives WHERE Locomotives.id NOT IN (SELECT Locomotive_FK FROM Trains_Scheduled WHERE train_run=0);");
    //
    while (!q.eof())
    {
        CString sLoco;
        sLoco.Format("%s #%s (%s)",q.getStringField("road_name"),q.getStringField("car_number"),q.getStringField("description"));
        int nIndex = m_ctlLocomotiveList.AddString(sLoco);
        m_ctlLocomotiveList.SetItemData(nIndex,q.getIntField("id"));
        q.nextRow();
        m_ctlLocomotiveList.SetCurSel(0);
    }
    q.finalize();
    //
    //  Caboose list
    //
    q = pDB->execQuery("SELECT id,reporting_marks,car_number FROM Caboose WHERE Caboose.id NOT IN (SELECT Caboose_FK FROM Trains_Scheduled WHERE train_run=0);");
    //
    while (!q.eof())
    {
        CString sCaboose;
        sCaboose.Format("%s #%s",q.getStringField("reporting_marks"),q.getStringField("car_number"));
        int nIndex = m_ctlCabooseList.AddString(sCaboose);
        m_ctlCabooseList.SetItemData(nIndex,q.getIntField("id"));
        q.nextRow();
        m_ctlCabooseList.SetCurSel(0);
    }
    q.finalize();
    //
    if( m_iScheduledTrainFK > 0 )
    {
        sSQL.Format("SELECT notes FROM Trains_Scheduled WHERE id=%d",m_iScheduledTrainFK);
        q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            m_sTrainNotes = q.getStringField("notes");
        }
        q.finalize();
    }
    //
    UpdateData(FALSE);
    //
    try {
        ValidateTrain();
    } catch (...) {}
    //
    m_ctlTrainDetail.EnsureVisible(m_ctlTrainDetail.GetRootItem());
    //
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CTrainDetailDialog::GetSiding(int iLocationFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d AND yard=1;",iLocationFK);    //  yard siding
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iRet=0;
    if( q.eof() )
    {
        q.finalize();
        sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d;",iLocationFK);    //  any siding
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
            iRet = q2.getIntField("id");
        q2.finalize();
    }
    else
    {
        iRet = q.getIntField("id");
        q.finalize();
    }
    return iRet;
}

void CTrainDetailDialog::OnClickTraindetail(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    //NMTREEVIEW* pTreeView = (NMTREEVIEW*)pNMHDR;
    CPoint pt;
    GetCursorPos(&pt);
    m_ctlTrainDetail.ScreenToClient(&pt);
    //UINT uFlags;

    TVHITTESTINFO hti;
    hti.pt.x = pt.x;
    hti.pt.y = pt.y;
    m_ctlTrainDetail.HitTest(&hti);
    //
    //int iClickCarFK(0);
    //bool bCheck(false);
    
    //if(ht != NULL && ( uFlags & TVHT_ONITEMSTATEICON))
    if(hti.flags & TVHT_ONITEMSTATEICON)
    {
        HTREEITEM ht = hti.hItem;
        PostMessage(UM_CHECKSTATECHANGE, 0, (LPARAM)ht);
        m_ctlTrainDetail.PostMessage(UM_CHECKSTATECHANGE, 0, (LPARAM)ht);
    }
}

LRESULT CTrainDetailDialog::OnCheckStateChanged(WPARAM pParam, LPARAM lParam)
{
    HTREEITEM   ht = (HTREEITEM)lParam;
    
    int iLocoCapacity = GetSelectedLocoCapacity();
    if( iLocoCapacity < 0 )
        return 0;
    
    // There is an item under the cursor.
    m_ctlTrainDetail.SelectItem(ht);
    int iClickCarFK = ((node_data*)m_ctlTrainDetail.GetItemData(ht))->iKey;
    int bCheck = m_ctlTrainDetail.GetCheck(ht);
    //
    //  If we're a location, check/uncheck everything underneath
    //
    int iSetoutsAdjust=0;
    int iPickupsAdjust=0;
    if( ((node_data*)m_ctlTrainDetail.GetItemData(ht))->iType == NODETYPE_LOCATION )
    {
        if ( m_ctlTrainDetail.ItemHasChildren(ht))
        {
            HTREEITEM htiChild = m_ctlTrainDetail.GetChildItem(ht);
            if (htiChild)
            {
                //m_ctlTrainDetail.ToggleItemState(htiChild);
                m_ctlTrainDetail.SetCheck(htiChild,m_ctlTrainDetail.GetCheck(ht));
                HTREEITEM htiSibling = m_ctlTrainDetail.GetNextSiblingItem(htiChild);
                while (htiSibling )
                {
                    //m_ctlTrainDetail.ToggleItemState(htiSibling);
                    m_ctlTrainDetail.SetCheck(htiSibling,m_ctlTrainDetail.GetCheck(ht));
                    htiSibling = m_ctlTrainDetail.GetNextSiblingItem(htiSibling);
                }
            }
            
        }
    }
    else
    {
        //
        //  If we're a car, and its a pickup, see if we drop it off somewhere,
        //  and check/uncheck that one, too.
        //
        //  'deliver to' and 'pickup from' are keys in the text
        //
        if( m_ctlTrainDetail.GetItemText(ht).Find("pickup from") > 0 ||
            m_ctlTrainDetail.GetItemText(ht).Find("deliver to") > 0 )
        {
            if( bCheck )
                iPickupsAdjust++;
            else
                iPickupsAdjust--;
            //
            TVITEM item;
            item.mask = TVIF_STATE|TVIF_PARAM;
            item.hItem = TVI_ROOT;
            HTREEITEM lastItem;
            do {
                do {
                    lastItem = item.hItem;
                    if (lastItem != TVI_ROOT) {
                        node_data* pData = (node_data*)m_ctlTrainDetail.GetItemData(item.hItem);
                        if( pData->iType == NODETYPE_CAR )
                        {
                            int iCarFK = ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iKey;
                            if( item.hItem != ht && iCarFK == iClickCarFK )
                            {
                                m_ctlTrainDetail.SetCheck(item.hItem,!m_ctlTrainDetail.GetCheck(item.hItem));
                                //
                                //  TODO: Fixup THIS parent's car count too!
                                //
                                {
                                    int iPickupsAdjust=0;
                                    int iSetoutsAdjust=0;
                                    if( m_ctlTrainDetail.GetItemText(item.hItem).Find("pickup from") > 0 )
                                    {
                                        if( bCheck )
                                            iPickupsAdjust++;
                                        else
                                            iPickupsAdjust--;
                                    }
                                    else
                                    {
                                        if( bCheck )
                                            iSetoutsAdjust++;
                                        else
                                            iSetoutsAdjust--;
                                    }
                                    HTREEITEM hParent;
                                    if( ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iType == NODETYPE_LOCATION )
                                        hParent = ht;
                                    else
                                        hParent = m_ctlTrainDetail.GetParentItem(item.hItem);
                                    //
                                    CString sText = m_ctlTrainDetail.GetItemText(hParent);
                                    CString sLocation = sText.Left(sText.Find("-"));
                                    sLocation.TrimRight();
                                    int iOriginalSetouts = atoi(sText.Mid(sText.Find("-")+1));
                                    int iOriginalPickups = atoi(sText.Mid(sText.Find("/")+1));
                                    int iCarsInTrain = atoi(sText.Mid(sText.Find("(")+1));
                                    iCarsInTrain = iCarsInTrain - iSetoutsAdjust + iPickupsAdjust;
                                    CString sCapErr(")");
                                    if( iCarsInTrain > iLocoCapacity )
                                        sCapErr = " * Locomotive capacity exceeded *)";
                                    CString sNewText;
                                    sNewText.Format("%s - %d setouts / %d pickups (%d cars in train%s",sLocation,iOriginalSetouts+iSetoutsAdjust,iOriginalPickups+iPickupsAdjust,iCarsInTrain,sCapErr);
                                    m_ctlTrainDetail.SetItemText(hParent,sNewText);
                                }
                            }
                        }
                    }
                } while ( (item.hItem = TreeView_GetChild( m_ctlTrainDetail.m_hWnd, lastItem )) );
                while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlTrainDetail.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlTrainDetail.m_hWnd, lastItem )) ) {}
            } while ( item.hItem );
        }
    }
    //
    //  change parent car count text
    //
    HTREEITEM hParent;
    if( ((node_data*)m_ctlTrainDetail.GetItemData(ht))->iType == NODETYPE_LOCATION )
        hParent = ht;
    else
        hParent = m_ctlTrainDetail.GetParentItem(ht);
    //
    CString sText = m_ctlTrainDetail.GetItemText(hParent);
    CString sLocation = sText.Left(sText.Find("-"));
    sLocation.TrimRight();
    int iOriginalSetouts = atoi(sText.Mid(sText.Find("-")+1));
    int iOriginalPickups = atoi(sText.Mid(sText.Find("/")+1));
    int iCarsInTrain = atoi(sText.Mid(sText.Find("(")+1));
    iCarsInTrain = iCarsInTrain - iSetoutsAdjust + iPickupsAdjust;
    CString sCapErr(")");
    if( iCarsInTrain > iLocoCapacity )
        sCapErr = " * Locomotive capacity exceeded *)";
    CString sNewText;
    sNewText.Format("%s - %d setouts / %d pickups (%d cars in train%s",sLocation,iOriginalSetouts+iSetoutsAdjust,iOriginalPickups+iPickupsAdjust,iCarsInTrain,sCapErr);
    m_ctlTrainDetail.SetItemText(hParent,sNewText);
    
    //
    ValidateTrain();
    //
    return 0;
}

HTREEITEM CTrainDetailDialog::GetLastLocation()
{
    TVITEM item;
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    HTREEITEM lastItem;
    HTREEITEM lastLocation(0);
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlTrainDetail.m_hWnd, &item );
                if( ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iType == NODETYPE_LOCATION )
                {
                    lastLocation = item.hItem;
                }
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlTrainDetail.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlTrainDetail.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlTrainDetail.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
    return lastLocation;
}

int CTrainDetailDialog::GetSelectedLocoCapacity()
{
    int iSel = m_ctlLocomotiveList.GetCurSel();
    if( iSel < 0 )
        return -1;
    int iLocomotiveFK = m_ctlLocomotiveList.GetItemData(iSel);
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT capacity FROM Locomotives WHERE id=%d",iLocomotiveFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iLocoCapacity=-1;
    if( !q.eof())
        iLocoCapacity = q.getIntField("capacity");
    q.finalize();
    return iLocoCapacity;
}

int CTrainDetailDialog::GetMaxMoves(int iLocationFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT max_moves FROM Trains_Location WHERE Train_FK=%d AND Location_FK=%d",m_iTrainFK,iLocationFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iMaxMoves(0);
    if( !q.eof() )
        iMaxMoves = q.getIntField("max_moves");
    q.finalize();
    return iMaxMoves;
}


void CTrainDetailDialog::ValidateTrain()
{
    UpdateData(TRUE);
    //
    //  run through train tree, checking for:
    //      Capacity issues (locomotive and siding)
    //      Pickup/setout pairings(??)
    //      Fixup cars in/out numbers
    //
    int iLocoCapacity = GetSelectedLocoCapacity();
    if( iLocoCapacity < 0 )
        return;
    //
    BOOL bError = FALSE;
    //
    //  validate locomotive capacity (number of cars)
    //
    TVITEM item;
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    HTREEITEM lastItem(0);
    HTREEITEM parentItem(0);
    HTREEITEM lastParentItem(0);
    int iCarsInTrain(0);
    int iSetoutsForLocation(0);
    int iPickupsForLocation(0);
    //
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlTrainDetail.m_hWnd, &item );
                if( ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iType == NODETYPE_LOCATION )    //  if its a location
                {
                    parentItem = item.hItem;
                    //iArrivingWith = iCarsInTrain;
                    /*
                    CString sText = m_ctlTrainDetail.GetItemText(item.hItem);
                    CString sLocation = sText.Left(sText.Find("-"));
                    sLocation.TrimRight();
                    int iOriginalSetouts = atoi(sText.Mid(sText.Find("-")+1));
                    int iOriginalPickups = atoi(sText.Mid(sText.Find("/")+1));
                    //int iCarsInTrain = atoi(sText.Mid(sText.Find("(")+1));
                    CString sCapErr(")");
                    if( iCarsInTrain > iLocoCapacity )
                    {
                        sCapErr = " * Locomotive capacity exceeded *)";
                        bError = TRUE;
                    }
                    CString sNewText;
                    sNewText.Format("%s - %d setouts / %d pickups (%d cars in train%s",sLocation,iOriginalSetouts,iOriginalPickups,iCarsInTrain,sCapErr);
                    //MessageBox(sNewText);
                    m_ctlTrainDetail.SetItemText(item.hItem,sNewText);
                    */
                    if( lastParentItem )
                    {
                        //CString sWork;
                        //sWork.Format("Setouts: %d, Pickups: %d",iSetoutsForLocation,iPickupsForLocation);
                        //MessageBox(sWork);
                        iCarsInTrain = iCarsInTrain - iSetoutsForLocation + iPickupsForLocation;
                        //
                        int iLocationFK = ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iKey;
                        int iMaxMoves = GetMaxMoves(iLocationFK);
                        CString sMoveErr("");
                        if( iMaxMoves > 0 && iMaxMoves < iSetoutsForLocation + iPickupsForLocation )
                        {
                            bError = TRUE;
                            sMoveErr.Format("* Maximum moves (%d) exceeded *",iMaxMoves);
                        }
                        //
                        CString sCapErr("");
                        if( iCarsInTrain > iLocoCapacity )
                        {
                            sCapErr = "* Locomotive capacity exceeded *";
                            bError = TRUE;
                        }
                        CString sText = m_ctlTrainDetail.GetItemText(lastParentItem);
                        CString sLocation = sText.Left(sText.Find("-"));
                        sLocation.TrimRight();
                        //
                        CString sNewText;
                        sNewText.Format("%s - %d setouts / %d pickups (%d cars in train) %s%s",sLocation,iSetoutsForLocation,iPickupsForLocation,iCarsInTrain,sCapErr,sMoveErr);
                        m_ctlTrainDetail.SetItemText(lastParentItem,sNewText);
                        //
                        lastParentItem = item.hItem;
                    }
                    iSetoutsForLocation=0;  //  reset setouts and pickups
                    iPickupsForLocation=0;
                }
                else    //  Its a car.  Drop-off or Pick-up?
                {
                     //if( !m_ctlTrainDetail.GetCheck(item.hItem) )
                     //   MessageBox("Is !Checked");
                    //
                    lastParentItem = parentItem;
                    //
                    CString sText = m_ctlTrainDetail.GetItemText(item.hItem);
                    if( m_ctlTrainDetail.GetCheck(item.hItem) )
                    {
                        if( sText.Find(" pickup ") >= 0 )
                        {
                            iPickupsForLocation++;
                        }
                        else
                        {
                            iSetoutsForLocation++;
                        }
                    }
                    //if( iCarsInTrain < 0 )
                    //    iCarsInTrain = 0;
                    //
                    sText = m_ctlTrainDetail.GetItemText(parentItem);
                    CString sLocation = sText.Left(sText.Find("-"));
                    sLocation.TrimRight();
                    //int iOriginalSetouts = atoi(sText.Mid(sText.Find("-")+1));
                    //int iOriginalPickups = atoi(sText.Mid(sText.Find("/")+1));
                    //iCarsInTrain = iArrivingWith + iOriginalPickups - iOriginalSetouts;
                    //CString sCapErr(")");
                    //if( iCarsInTrain > iLocoCapacity )
                    //{
                    //    sCapErr = " * Locomotive capacity exceeded *)";
                    //    bError = TRUE;
                    //}
                    //CString sNewText;
                    //sNewText.Format("%s - %d setouts / %d pickups * (%d cars in train%s",sLocation,iOriginalSetouts,iOriginalPickups,iCarsInTrain,sCapErr);
                    //m_ctlTrainDetail.SetItemText(parentItem,sNewText);
                }
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlTrainDetail.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlTrainDetail.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlTrainDetail.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
    //
    //  set out all cars
    //
    CString sText = m_ctlTrainDetail.GetItemText(lastParentItem);
    CString sLocation = sText.Left(sText.Find("-"));
    sLocation.TrimRight();
    //
    CString sNewText;
    sNewText.Format("%s - %d setouts / 0 pickups",sLocation,iCarsInTrain);
    m_ctlTrainDetail.SetItemText(lastParentItem,sNewText);
    //
    /*
    //
    //  validate siding capacities (inches)
    //
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlTrainDetail.m_hWnd, &item );
                //
                //  check passing siding capacity
                //
                if( ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iType == NODETYPE_LOCATION )    //  if its a location
                {
                    //
                    //  get siding capacity for this location (its in inches!)
                    //
                    int iSidingCapacity=3;
                    //
                    CString sText = m_ctlTrainDetail.GetItemText(item.hItem);
                    //MessageBox(sText);
                    CString sLocation = sText.Left(sText.Find("-"));
                    sLocation.TrimRight();
                    int iOriginalSetouts = atoi(sText.Mid(sText.Find("-")+1));
                    int iOriginalPickups = atoi(sText.Mid(sText.Find("/")+1));
                    int iCarsInTrain = atoi(sText.Mid(sText.Find("(")+1));
                    CString sCapErr(")");
                    //
                    //  FIXME: need to keep a running total of pickups/dropoffs for this location
                    //
                    if( iCarsInTrain > iSidingCapacity )
                    {
                        sCapErr = " ** Siding capacity exceeded ** )";
                        bError = TRUE;
                    }
                    CString sNewText;
                    sNewText.Format("%s - %d setouts / %d pickups (%d cars in train%s",sLocation,iOriginalSetouts,iOriginalPickups,iCarsInTrain,sCapErr);
                    //MessageBox(sNewText);
                    m_ctlTrainDetail.SetItemText(item.hItem,sNewText);
                }

                //
                //  check industry siding capacity
                //
                if( m_ctlTrainDetail.GetItemData(item.hItem) != -1 )    //  if its an industry
                {
                    //
                    //  get siding capacity for this industry
                    //
                    int iSidingCapacity = GetIndustrySidingCapacity(10);
                    //
                    //  get cars already at this industry
                    //
                    int iCarsAtIndustry=3;
                    //
                    CString sText = m_ctlTrainDetail.GetItemText(item.hItem);
                    //
                    //  'pickup from' = not dropping off
                    //
                    if( sText.Find(" pickup from ") > 0 )
                        continue;
                    //
                    //  FIXME: need to keep a running total of pickups/dropoffs for this industry
                    //
                    CString sCapErr("");
                    if( iCarsAtIndustry + 1 > iSidingCapacity ) //  plus the one I want to drop
                    {
                        sCapErr = " - ( ** Siding capacity exceeded ** )";
                        bError = TRUE;
                    }
                    CString sNewText;
                    sNewText.Format("%s%s",sText,sCapErr);
                    m_ctlTrainDetail.SetItemText(item.hItem,sNewText);
                }
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlTrainDetail.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlTrainDetail.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlTrainDetail.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
    */
    //
    //  car pickup/dropoff pairs    TODO: what does this mean?
    //

    //
    if( m_bIgnoreRestrictions )
        m_ctlOK.EnableWindow(TRUE);
    else
        m_ctlOK.EnableWindow(!bError);
    UpdateData(FALSE);
}

void CTrainDetailDialog::OnCancel() 
{
    CleanupTree();
    //
	CDialog::OnCancel();
}

void CTrainDetailDialog::OnOK() 
{
    //
    //  Remove any un-checked items from _TSC
    //  Modify Location_to_FK based on each items new target
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    TVITEM item;
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    HTREEITEM lastItem;
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlTrainDetail.m_hWnd, &item );
                if( ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iType == NODETYPE_CAR )    //  if its a car
                {
                    if( !m_ctlTrainDetail.GetCheck(item.hItem) )
                    {
                        //
                        //  DELETE FROM _TSC WHERE Cars_FK = %d
                        //
                        sSQL.Format("DELETE FROM _TSC WHERE Cars_FK=%d;",((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iKey);
                        pDB->execDML(sSQL);
                    }
                    else    //  update location if it has changed
                    {
                        /*
                        int iCarFK = ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iKey;
                        HTREEITEM hParent = m_ctlTrainDetail.GetParentItem(item.hItem);
                        HTREEITEM lastLocation = GetLastLocation();
                        if( hParent == lastLocation )   //  only do this for cars moved to the last location
                        {
                            int iLocationFK = ((node_data*)m_ctlTrainDetail.GetItemData(lastLocation))->iKey;
                            int iOldLocationFK(0);
                            sSQL.Format("SELECT Location_to_FK FROM _TSC WHERE Cars_FK=%d;",iCarFK);
                            CppSQLite3Query q = pDB->execQuery(sSQL);
                            if( !q.eof() )
                            {
                                int iOldLocationFK = q.getIntField("Location_to_FK");
                            }
                            q.finalize();
                            if( iOldLocationFK != iLocationFK )
                            {
                                sSQL.Format("UPDATE _TSC SET Location_to_FK=%d, Industry_to_FK=0 WHERE Cars_FK=%d;",
                                    iLocationFK,iCarFK);
                                pDB->execDML(sSQL);
                            }
                        }                        
                        */
                        /*
                        HTREEITEM hParent = m_ctlTrainDetail.GetParentItem(item.hItem);
                        int iLocationFK = ((node_data*)m_ctlTrainDetail.GetItemData(hParent))->iKey;
                        CString sText(((node_data*)m_ctlTrainDetail.GetItemData(hParent))->sText.c_str());
                        sSQL.Format("UPDATE _TSC SET Location_to_FK=%d WHERE Cars_FK=%d;",
                            iLocationFK,
                            ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem))->iKey);
                        pDB->execDML(sSQL);
                        */
                    }
                }
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlTrainDetail.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlTrainDetail.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlTrainDetail.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
    //
    int iSel = m_ctlLocomotiveList.GetCurSel();
    if( iSel >= 0 )
        m_iLocomotive = m_ctlLocomotiveList.GetItemData(iSel);
    iSel = m_ctlCabooseList.GetCurSel();
    if( iSel >= 0 )
        m_iCaboose = m_ctlCabooseList.GetItemData(iSel);
    //
    CleanupTree();
    //
    CDialog::OnOK();
}

void CTrainDetailDialog::CleanupTree()
{
    TVITEM item;
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    HTREEITEM lastItem;
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlTrainDetail.m_hWnd, &item );
                if( ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem)) )
                {
                    delete ((node_data*)m_ctlTrainDetail.GetItemData(item.hItem));
                }
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlTrainDetail.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlTrainDetail.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlTrainDetail.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
}

int CTrainDetailDialog::GetLocomotive()
{
    return m_iLocomotive;
}

int CTrainDetailDialog::GetCaboose()
{
    return m_iCaboose;
}

int CTrainDetailDialog::GetIndustrySidingCapacity(int iIndustry)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT capacity FROM Sidings,Industries WHERE Industries.id=%d AND Industries.Sidings_FK=Sidings.id",iIndustry);
    TRACE(sSQL);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iCap=0;
    if( !q.eof() )
        iCap = q.getIntField("capacity");
    q.finalize();
    return iCap;
}

int CTrainDetailDialog::GetSidingCapacity(int iLocation)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    return 0;
}


void CTrainDetailDialog::OnIgnorerestrictions() 
{
    ValidateTrain();	
}

void CTrainDetailDialog::OnSelChangeLocomotiveList() 
{
    ValidateTrain();	
}
