// EquipmentLocationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "EquipmentLocationDialog.h"

#include "julian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipmentLocationDialog dialog


CEquipmentLocationDialog::CEquipmentLocationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEquipmentLocationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipmentLocationDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEquipmentLocationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipmentLocationDialog)
	DDX_Control(pDX, IDC_CARSLIST, m_ctlCarsList);
	DDX_Control(pDX, IDC_EQUIPMENTTREE, m_ctlEquipmentTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquipmentLocationDialog, CDialog)
	//{{AFX_MSG_MAP(CEquipmentLocationDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_EQUIPMENTTREE, OnSelchangedEquipmenttree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipmentLocationDialog message handlers

BOOL CEquipmentLocationDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//    DWORD dwStyle =m_ctlCarsList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
//    dwStyle |= LVS_EX_GRIDLINES;
//    dwStyle |= LVS_EX_FULLROWSELECT;
//    m_ctlCarsList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

    m_ctlCarsList.InsertColumn(0,"Car Number");
    m_ctlCarsList.InsertColumn(1,"Reporting Marks");
    m_ctlCarsList.InsertColumn(2,"Car Type");
    m_ctlCarsList.InsertColumn(3,"E/L");
    m_ctlCarsList.InsertColumn(4,"Last Movement");
    m_ctlCarsList.InsertColumn(5,"Available on");
    //
    m_ctlCarsList.SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarsList.SetColumnWidth(1,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarsList.SetColumnWidth(2,85);
    m_ctlCarsList.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarsList.SetColumnWidth(4,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarsList.SetColumnWidth(5,LVSCW_AUTOSIZE_USEHEADER);
    //m_ctlCarsList.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
    //
    ReadDB();
    
	m_ctlCarsList.RestoreState("State","EquipmentLoocationDialog_m_ctlCarsList");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

enum {
    ETD_LOCATION=0,
    ETD_SIDING,
    ETD_INDUSTRY,
    ETD_TRAIN,
    ETD_INTERCHANGE,
    ETD_FOREIGNCARS,
    ETD_OUTOFSERVICE
};
typedef struct {
    int iType;
    int iID;

} _EquipmentTreeData;

void CEquipmentLocationDialog::ReadDB() 
{
    //  read the database into tree
    m_ctlEquipmentTree.DeleteAllItems();

    HTREEITEM hLocationRoot = m_ctlEquipmentTree.InsertItem("Locations");
    //
    //  Read locations
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Locations ORDER BY sort_order;");
    //
    while (!q.eof())
    {
        HTREEITEM pLocation = m_ctlEquipmentTree.InsertItem(q.getStringField("name"),hLocationRoot);
        _EquipmentTreeData* pData = new _EquipmentTreeData;
        pData->iType = ETD_LOCATION;
        pData->iID = q.getIntField("id");
        m_ctlEquipmentTree.SetItemData(pLocation,(DWORD)pData);
        //
        //  get sidings for this location
        //
        CString sSQL;
        sSQL.Format("SELECT id,name FROM Sidings WHERE Locations_FK=%d;",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while( !q2.eof() )
        {
            HTREEITEM pSiding = m_ctlEquipmentTree.InsertItem(q2.getStringField("name"),pLocation);
            _EquipmentTreeData* pData = new _EquipmentTreeData;
            pData->iType = ETD_SIDING;
            pData->iID = q2.getIntField("id");
            m_ctlEquipmentTree.SetItemData(pSiding,(DWORD)pData);
            //
            //  get industries for this siding
            //
            sSQL.Format("SELECT id,name FROM Industries WHERE Sidings_FK=%d;",q2.getIntField("id"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            while( !q3.eof() )
            {
                HTREEITEM pIndustry = m_ctlEquipmentTree.InsertItem(q3.getStringField("name"),pSiding);
                _EquipmentTreeData* pData = new _EquipmentTreeData;
                pData->iType = ETD_INDUSTRY;
                pData->iID = q3.getIntField("id");
                m_ctlEquipmentTree.SetItemData(pIndustry,(DWORD)pData);
                //
                q3.nextRow();
            }
            q3.finalize();
            q2.nextRow();
        }
        q2.finalize();
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlEquipmentTree.Expand(hLocationRoot,TVE_EXPAND);
    //
    HTREEITEM hTrains = m_ctlEquipmentTree.InsertItem("Scheduled Trains");
    //
    q = pDB->execQuery("SELECT Trains_Scheduled.id,name,trainid FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.Trains_FK=Trains.id AND train_run=0;");
    //
    while (!q.eof() )
    {
        CString sText;
        sText.Format("%s %s",q.getStringField("name"),q.getStringField("trainid"));
        HTREEITEM pTrain = m_ctlEquipmentTree.InsertItem(sText,hTrains);
        _EquipmentTreeData* pData = new _EquipmentTreeData;
        pData->iType = ETD_TRAIN;
        pData->iID = q.getIntField("id");
        m_ctlEquipmentTree.SetItemData(pTrain,(DWORD)pData);
        //
        q.nextRow();
    }
    q.finalize();
    m_ctlEquipmentTree.Expand(hTrains,TVE_EXPAND);
    //
    /*
    HTREEITEM hItem = m_ctlEquipmentTree.InsertItem("Interchanges");
    m_ctlEquipmentTree.SetItemColor(hItem,RGB(192,0,0));
    _EquipmentTreeData* pData = new _EquipmentTreeData;
    pData->iType = ETD_INTERCHANGE;
    m_ctlEquipmentTree.SetItemData(hItem,(DWORD)pData);
    */
    //
    HTREEITEM hItem = m_ctlEquipmentTree.InsertItem("Foreign Cars");
    m_ctlEquipmentTree.SetItemColor(hItem,RGB(0,0,255));
    _EquipmentTreeData*pData = new _EquipmentTreeData;
    pData->iType = ETD_FOREIGNCARS;
    m_ctlEquipmentTree.SetItemData(hItem,(DWORD)pData);
    //
    hItem = m_ctlEquipmentTree.InsertItem("Out-of-serivce");
    m_ctlEquipmentTree.SetItemColor(hItem,RGB(255,0,255));
    pData = new _EquipmentTreeData;
    pData->iType = ETD_OUTOFSERVICE;
    m_ctlEquipmentTree.SetItemData(hItem,(DWORD)pData);
    //
    m_ctlEquipmentTree.EnsureVisible(hLocationRoot);
    TBTreeStateMgr::LoadTreeState(_T("EquipmentLocation"),&m_ctlEquipmentTree);
}


void CEquipmentLocationDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("EquipmentLocation"),&m_ctlEquipmentTree);    

    CleanupTree();
	
	m_ctlCarsList.SaveState("State","EquipmentLoocationDialog_m_ctlCarsList");

	CDialog::OnOK();
}

void CEquipmentLocationDialog::CleanupTree()
{
    TVITEM item;
    item.mask = TVIF_STATE|TVIF_PARAM;
    item.hItem = TVI_ROOT;
    HTREEITEM lastItem;
    do {
        do {
            lastItem = item.hItem;
            if (lastItem != TVI_ROOT) {
                TreeView_GetItem( m_ctlEquipmentTree.m_hWnd, &item );
                if( ((_EquipmentTreeData*)m_ctlEquipmentTree.GetItemData(item.hItem)) )
                {
                    delete ((_EquipmentTreeData*)m_ctlEquipmentTree.GetItemData(item.hItem));
                }
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlEquipmentTree.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlEquipmentTree.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlEquipmentTree.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
}

void CEquipmentLocationDialog::OnSelchangedEquipmenttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    //  get cars for this location/siding/etc
    m_ctlCarsList.DeleteAllItems();
    //
    HTREEITEM hItem = TreeView_GetSelection(m_ctlEquipmentTree);
    
    _EquipmentTreeData* pData = ((_EquipmentTreeData*)m_ctlEquipmentTree.GetItemData(hItem));

    if( !pData )
        return;
	
    GetCars(pData->iType,pData->iID);
    //
	*pResult = 0;
}

void CEquipmentLocationDialog::GetCars(int iType, int iKey)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    int iRRDate(0);
    int iLoadEmpty(1);
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate  FROM _Config WHERE id=1;");
    if( !q.eof() )
        iRRDate = q.getIntField("rrdate");
    q.finalize();
    //
    switch (iType)
    {
    case ETD_LOCATION:
        {
            CString sSQL;
            sSQL.Format("SELECT car_number,reporting_marks,loadempty,CarTypes_FK,delivery_date FROM Cars WHERE Location_FK=%d ORDER BY reporting_marks,car_number;",iKey);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            while( !q.eof() )
            {
                int nIndex = m_ctlCarsList.InsertItem(m_ctlCarsList.GetItemCount(),q.getStringField("car_number"));
                m_ctlCarsList.SetItemText(nIndex,1,q.getStringField("reporting_marks"));
                m_ctlCarsList.SetItemText(nIndex,2,GetCarType(q.getIntField("CarTypes_FK")));
                m_ctlCarsList.SetItemText(nIndex,3,q.getIntField("loadempty")==1?"E":"L");
                //
                int y,m,d;
                char szDate[24];
                JulianDate::JdToYmd(q.getIntField("delivery_date")+iRRDate, &y,&m,&d);
                sprintf(szDate,"%d/%d/%d",m,d,y);
                m_ctlCarsList.SetItemText(nIndex,4,szDate);
                //
                JulianDate::JdToYmd(q.getIntField("delivery_date")+iRRDate+g_iLoadEmptyDays, &y,&m,&d);
                sprintf(szDate,"%d/%d/%d",m,d,y);
                m_ctlCarsList.SetItemText(nIndex,5,szDate);
                //
                q.nextRow();
            }
            q.finalize();
            break;
        }
    case ETD_SIDING:
        {
            CString sSQL;
            sSQL.Format("SELECT car_number,reporting_marks,loadempty,CarTypes_FK,delivery_date FROM Cars WHERE Siding_FK=%d ORDER BY reporting_marks,car_number;",iKey);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            while( !q.eof() )
            {
                int nIndex = m_ctlCarsList.InsertItem(m_ctlCarsList.GetItemCount(),q.getStringField("car_number"));
                m_ctlCarsList.SetItemText(nIndex,1,q.getStringField("reporting_marks"));
                m_ctlCarsList.SetItemText(nIndex,2,GetCarType(q.getIntField("CarTypes_FK")));
                m_ctlCarsList.SetItemText(nIndex,3,q.getIntField("loadempty")==1?"E":"L");
                int y,m,d;
                char szDate[24];
                JulianDate::JdToYmd(q.getIntField("delivery_date")+iRRDate, &y,&m,&d);
                sprintf(szDate,"%d/%d/%d",m,d,y);
                m_ctlCarsList.SetItemText(nIndex,4,szDate);
                //
                q.nextRow();
            }
            q.finalize();
            break;
        }
    case ETD_INDUSTRY:
        {
            CString sSQL;
            sSQL.Format("SELECT car_number,reporting_marks,loadempty,CarTypes_FK,delivery_date FROM Cars WHERE Industry_FK=%d ORDER BY reporting_marks,car_number;",iKey);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            while( !q.eof() )
            {
                int nIndex = m_ctlCarsList.InsertItem(m_ctlCarsList.GetItemCount(),q.getStringField("car_number"));
                m_ctlCarsList.SetItemText(nIndex,1,q.getStringField("reporting_marks"));
                m_ctlCarsList.SetItemText(nIndex,2,GetCarType(q.getIntField("CarTypes_FK")));
                m_ctlCarsList.SetItemText(nIndex,3,q.getIntField("loadempty")==1?"E":"L");
                int y,m,d;
                char szDate[24];
                JulianDate::JdToYmd(q.getIntField("delivery_date")+iRRDate, &y,&m,&d);
                sprintf(szDate,"%d/%d/%d",m,d,y);
                m_ctlCarsList.SetItemText(nIndex,4,szDate);
                //
                q.nextRow();
            }
            q.finalize();
            break;
        }
    case ETD_TRAIN:
        {
            CString sSQL;
            sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d;",iKey);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            while( !q.eof() )
            {
                sSQL.Format("SELECT car_number,reporting_marks,loadempty,CarTypes_FK,delivery_date FROM Cars WHERE id=%d ORDER BY reporting_marks,car_number;",q.getIntField("Cars_FK"));
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if( !q2.eof() )
                {
                    int nIndex = m_ctlCarsList.InsertItem(m_ctlCarsList.GetItemCount(),q2.getStringField("car_number"));
                    m_ctlCarsList.SetItemText(nIndex,1,q2.getStringField("reporting_marks"));
                    m_ctlCarsList.SetItemText(nIndex,2,GetCarType(q2.getIntField("CarTypes_FK")));
                    m_ctlCarsList.SetItemText(nIndex,3,q2.getIntField("loadempty")==1?"E":"L");
                    int y,m,d;
                    char szDate[24];
                    JulianDate::JdToYmd(q2.getIntField("delivery_date")+iRRDate, &y,&m,&d);
                    sprintf(szDate,"%d/%d/%d",m,d,y);
                    m_ctlCarsList.SetItemText(nIndex,4,szDate);
                }
                q2.finalize();
                //
                q.nextRow();
            }
            q.finalize();
            break;
        }
    case ETD_INTERCHANGE:
        {
            MessageBox("Cars at interchanges");
            break;
        }
    case ETD_FOREIGNCARS:
        {
            CString sSQL;
            sSQL.Format("SELECT car_number,reporting_marks,loadempty,CarTypes_FK,delivery_date FROM Cars WHERE foreign_road=1 ORDER BY reporting_marks,car_number;");
            CppSQLite3Query q = pDB->execQuery(sSQL);
            while( !q.eof() )
            {
                int nIndex = m_ctlCarsList.InsertItem(m_ctlCarsList.GetItemCount(),q.getStringField("car_number"));
                m_ctlCarsList.SetItemText(nIndex,1,q.getStringField("reporting_marks"));
                m_ctlCarsList.SetItemText(nIndex,2,GetCarType(q.getIntField("CarTypes_FK")));
                m_ctlCarsList.SetItemText(nIndex,3,q.getIntField("loadempty")==1?"E":"L");
                //
                int y,m,d;
                char szDate[24];
                JulianDate::JdToYmd(q.getIntField("delivery_date")+iRRDate, &y,&m,&d);
                sprintf(szDate,"%d/%d/%d",m,d,y);
                m_ctlCarsList.SetItemText(nIndex,4,szDate);
                q.nextRow();
            }
            q.finalize();
            break;
        }
    case ETD_OUTOFSERVICE:
        {
            CString sSQL;
            sSQL.Format("SELECT car_number,reporting_marks,loadempty,CarTypes_FK,delivery_date FROM Cars WHERE out_of_service=1 AND Cars.id NOT IN (SELECT Cars_FK FROM Trains_Scheduled_Cars,Trains_Scheduled WHERE Trains_Scheduled.id=Trains_Scheduled_Cars.Trains_Scheduled_FK AND train_run=0) ORDER BY reporting_marks,car_number;");
            CppSQLite3Query q = pDB->execQuery(sSQL);
            while( !q.eof() )
            {
                int nIndex = m_ctlCarsList.InsertItem(m_ctlCarsList.GetItemCount(),q.getStringField("car_number"));
                m_ctlCarsList.SetItemText(nIndex,1,q.getStringField("reporting_marks"));
                m_ctlCarsList.SetItemText(nIndex,2,GetCarType(q.getIntField("CarTypes_FK")));
                m_ctlCarsList.SetItemText(nIndex,3,q.getIntField("loadempty")==1?"E":"L");
                int y,m,d;
                char szDate[24];
                JulianDate::JdToYmd(q.getIntField("delivery_date")+iRRDate, &y,&m,&d);
                sprintf(szDate,"%d/%d/%d",m,d,y);
                m_ctlCarsList.SetItemText(nIndex,4,szDate);
                //
                q.nextRow();
            }
            q.finalize();
            break;
        }
    }
}

CString CEquipmentLocationDialog::GetCarType(int iCarTypeFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT description FROM CarTypes WHERE id=%d;",iCarTypeFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    CString sType("");
    if( !q.eof() )
        sType = q.getStringField("description");
    q.finalize();
    return sType;
}
