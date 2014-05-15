// EquipmentNotFoundDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "EquipmentNotFoundDialog.h"

#include "julian.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipmentNotFoundDialog dialog


CEquipmentNotFoundDialog::CEquipmentNotFoundDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEquipmentNotFoundDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipmentNotFoundDialog)
	//}}AFX_DATA_INIT
}


void CEquipmentNotFoundDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipmentNotFoundDialog)
	DDX_Control(pDX, IDC_NOTFOUNDTREE, m_ctlNotFoundTree);
	DDX_Control(pDX, IDC_EQUIPMENTTREE, m_ctlEquipmentTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquipmentNotFoundDialog, CDialog)
	//{{AFX_MSG_MAP(CEquipmentNotFoundDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_EQUIPMENTTREE, OnSelchangedEquipmenttree)
	ON_COMMAND(IDM_PRUNETOHERE, OnPrunetohere)
	ON_NOTIFY(NM_RCLICK, IDC_NOTFOUNDTREE, OnRclickNotfoundtree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipmentNotFoundDialog message handlers

BOOL CEquipmentNotFoundDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    ReadDB();

    CRect rcApp;
	GetClientRect(&rcApp);
    CRect rcDesktop;
	GetDesktopWindow()->GetClientRect(&rcDesktop);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEquipmentNotFoundDialog::ReadDB() 
{
    //  read the database into tree
    m_ctlEquipmentTree.DeleteAllItems();

    HTREEITEM hLocationRoot = m_ctlEquipmentTree.InsertItem("Cars");
    //
    //  Read all cars
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,description,type_id FROM CarTypes WHERE active=1 ORDER BY description;");
    //
    while (!q.eof())
    {
        CString sCarType;
        sCarType.Format("%s %s",q.getStringField("description"),q.getStringField("type_id"));
        HTREEITEM pCarType = m_ctlEquipmentTree.InsertItem(sCarType,hLocationRoot);
        m_ctlEquipmentTree.SetItemData(pCarType,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlEquipmentTree.Expand(hLocationRoot,TVE_EXPAND);
    //
    m_ctlEquipmentTree.EnsureVisible(hLocationRoot);
    //
    TBTreeStateMgr::LoadTreeState(_T("EquipmentNotFound"),&m_ctlEquipmentTree);
}

void CEquipmentNotFoundDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("EquipmentNotFound"),&m_ctlEquipmentTree);    

    CleanupTree();
	
	CDialog::OnOK();
}

void CEquipmentNotFoundDialog::CleanupTree()
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
            }
        } while ( (item.hItem = TreeView_GetChild( m_ctlEquipmentTree.m_hWnd, lastItem )) );
        while ( (! (item.hItem = TreeView_GetNextSibling( m_ctlEquipmentTree.m_hWnd, lastItem ))) && (lastItem = item.hItem = TreeView_GetParent( m_ctlEquipmentTree.m_hWnd, lastItem )) ) {}
    } while ( item.hItem );
}

void CEquipmentNotFoundDialog::OnSelchangedEquipmenttree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    //  get cars for this location/siding/etc
    m_ctlNotFoundTree.DeleteAllItems();
    //
    HTREEITEM hItem = TreeView_GetSelection(m_ctlEquipmentTree);
    
    if( m_ctlEquipmentTree.GetItemData(hItem) == -1 )
        return;
	
    GetCars(m_ctlEquipmentTree.GetItemData(hItem));
	
	*pResult = 0;
}

void CEquipmentNotFoundDialog::GetCars(int iCarTypeFK)
{
    HTREEITEM hTreeRoot = m_ctlNotFoundTree.InsertItem("Train Dates");
    //
    m_iCarTypeFK = iCarTypeFK;
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    CString sSQL;
    sSQL.Format("SELECT * FROM Cars_Not_Found WHERE CarTypes_FK=%d ORDER BY train_date DESC;",iCarTypeFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iDate(0);
    int iDateTotal(0);
    int iGraphRow(1);
    //
    HTREEITEM hNotFoundRoot(0);
    while( !q.eof() )
    {
        if( iDate != q.getIntField("train_date") )
        {
            if( hNotFoundRoot )
            {
                CString sText;
                sText.Format("%s (%d)",m_ctlNotFoundTree.GetItemText(hNotFoundRoot),iDateTotal);
                m_ctlNotFoundTree.SetItemText(hNotFoundRoot,sText);
                iDateTotal=0;
            }
            CppSQLite3Query q2b = pDB->execQuery("SELECT rrdate FROM _Config WHERE id=1;");
            CString sDate("--/--/----");
            if( !q2b.eof() )
            {
                int y,m,d;
                JulianDate::JdToYmd( q2b.getIntField("rrdate")+q.getIntField("train_date"), &y,&m,&d);
                sDate.Format("%02d/%02d/%4d",m,d,y);
            }
            q2b.finalize();
            hNotFoundRoot = m_ctlNotFoundTree.InsertItem(sDate,hTreeRoot);
            m_ctlNotFoundTree.SetItemData(hNotFoundRoot,q.getIntField("train_date"));
            iDate = q.getIntField("train_date");
        }
        CppSQLite3Query q2 = pDB->execQuery("SELECT rrdate FROM _Config WHERE id=1;");
        if( !q2.eof() )
        {
            int y,m,d;
            JulianDate::JdToYmd( q2.getIntField("rrdate")+q.getIntField("train_date"), &y,&m,&d);
            CString sDate;
            sDate.Format("%02d/%02d/%4d",m,d,y);
            //
            //  industry
            //
            CString sIndustry("");
            sSQL.Format("SELECT name FROM Industries WHERE id=%d;",q.getIntField("Industries_FK"));
            CppSQLite3Query q2b = pDB->execQuery(sSQL);
            if( !q2b.eof() )
            {
                sIndustry = q2b.getStringField("name");
            }
            q2b.finalize();
            if( !sIndustry.IsEmpty() )
            {
                HTREEITEM hItem = m_ctlNotFoundTree.InsertItem(sIndustry,hNotFoundRoot);
                m_ctlNotFoundTree.SetItemData(hItem,q.getIntField("train_date"));
                iDateTotal++;
            }
        }
        q2.finalize();
        //
        q.nextRow();
        if( q.eof() )
        {
            if( hNotFoundRoot )
            {
                CString sText;
                sText.Format("%s (%d)",m_ctlNotFoundTree.GetItemText(hNotFoundRoot),iDateTotal);
                m_ctlNotFoundTree.SetItemText(hNotFoundRoot,sText);
            }
        }
    }
    q.finalize();
    //
    m_ctlNotFoundTree.Expand(hTreeRoot,TVE_EXPAND);
}

void CEquipmentNotFoundDialog::OnPrunetohere() 
{
    if( MessageBox("Are you sure you want to remove not-found data before this date?","TrainOps Question",MB_YESNO) != IDYES )
        return;
    //
    HTREEITEM hItem = m_ctlNotFoundTree.GetSelectedItem();
    if( !hItem )
        return;
    //
    
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Cars_Not_Found WHERE train_date <= %d AND CarTypes_FK=%d;",m_ctlNotFoundTree.GetItemData(hItem),m_iCarTypeFK);
    pDB->execDML(sSQL);
    //
    //  re-load the panel
    //
    m_ctlNotFoundTree.DeleteAllItems();
    GetCars(m_iCarTypeFK);
}

void CEquipmentNotFoundDialog::OnRclickNotfoundtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CPoint ptScreen;
    if (!GetCursorPos(&ptScreen))
        return;
    CPoint ptClient(ptScreen);
    m_ctlNotFoundTree.ScreenToClient(&ptClient);

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_EQUIPMENTNOTFOUNDMENU));
    CMenu *popup = menu.GetSubMenu(0);
    ASSERT(popup);
    VERIFY(popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
        ptScreen.x, ptScreen.y, this, NULL));
    
	*pResult = 0;
}
