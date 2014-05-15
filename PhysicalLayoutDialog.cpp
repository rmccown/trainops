// PhysicalLayoutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "PhysicalLayoutDialog.h"

#include "LocationDetailDialog.h"
#include "SidingDetailDialog.h"
#include "IndustryDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhysicalLayoutDialog dialog


CPhysicalLayoutDialog::CPhysicalLayoutDialog(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CPhysicalLayoutDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhysicalLayoutDialog)
	//}}AFX_DATA_INIT
}


void CPhysicalLayoutDialog::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhysicalLayoutDialog)
	DDX_Control(pDX, IDC_UPBUTTON, m_ctlUpButton);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_ctlDownButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_ctlDeleteButton);
	DDX_Control(pDX, IDC_EDITSIDINGBUTTON, m_ctlEditSidingButton);
	DDX_Control(pDX, IDC_EDITLOCATIONBUTTON, m_ctlEditLocationButton);
	DDX_Control(pDX, IDC_EDITINDUSTRYBUTTON, m_ctlEditIndustryButton);
	DDX_Control(pDX, IDC_NEWSIDINGBUTTON, m_ctlNewSidingButton);
	DDX_Control(pDX, IDC_NEWLOCATIONBUTTON, m_ctlNewLocationButton);
	DDX_Control(pDX, IDC_NEWINDUSTRYBUTTON, m_ctlNewIndustryButton);
	DDX_Control(pDX, IDC_LAYOUT, m_ctlLayout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhysicalLayoutDialog, CResizableDialog)
	//{{AFX_MSG_MAP(CPhysicalLayoutDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LAYOUT, OnRclickLayout)
	ON_NOTIFY(NM_DBLCLK, IDC_LAYOUT, OnDblclkLayout)
	ON_COMMAND(ID_INDUSTRYMENU_DELETEINDUSTRY, OnIndustrymenuDeleteindustry)
	ON_COMMAND(ID_INDUSTRYMENU_EDITINDUSTRY, OnIndustrymenuEditindustry)
	ON_COMMAND(ID_INDUSTRYMENU_NEWINDUSTRY, OnIndustrymenuNewindustry)
	ON_COMMAND(ID_LOCATIONMENU_EDITLOCATION, OnLocationmenuEditlocation)
	ON_COMMAND(ID_LOCATIONMENU_NEWLOCATION, OnLocationmenuNewlocation)
	ON_COMMAND(ID_LOCATIONNMENU_DELETELOCATION, OnLocationnmenuDeletelocation)
	ON_COMMAND(ID_SIDINGMENU_DELETESIDING, OnSidingmenuDeletesiding)
	ON_COMMAND(ID_SIDINGMENU_EDITSIDING, OnSidingmenuEditsiding)
	ON_COMMAND(ID_SIDINGMENU_NEWSIDING, OnSidingmenuNewsiding)
	ON_NOTIFY(NM_CLICK, IDC_LAYOUT, OnClickLayout)
	ON_BN_CLICKED(IDC_EDITINDUSTRYBUTTON, OnEditindustrybutton)
	ON_BN_CLICKED(IDC_EDITLOCATIONBUTTON, OnEditlocationbutton)
	ON_BN_CLICKED(IDC_EDITSIDINGBUTTON, OnEditsidingbutton)
	ON_BN_CLICKED(IDC_NEWINDUSTRYBUTTON, OnNewindustrybutton)
	ON_BN_CLICKED(IDC_NEWLOCATIONBUTTON, OnNewlocationbutton)
	ON_BN_CLICKED(IDC_NEWSIDINGBUTTON, OnNewsidingbutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_DOWNBUTTON, OnDownbutton)
	ON_BN_CLICKED(IDC_UPBUTTON, OnUpbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhysicalLayoutDialog message handlers

BOOL CPhysicalLayoutDialog::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = 14;
    lf.lfWeight = FW_NORMAL;
    lf.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
    strcpy(lf.lfFaceName, "Courier New");
    m_pFont.CreateFontIndirect(&lf);

    //Set Font
    m_ctlLayout.SetFont(&m_pFont);

    AddAnchor(IDOK, TOP_RIGHT);
    AddAnchor(IDCANCEL, TOP_RIGHT);
	
	AddAnchor(IDC_NEWSIDINGBUTTON, TOP_RIGHT);
	AddAnchor(IDC_NEWLOCATIONBUTTON, TOP_RIGHT);
	AddAnchor(IDC_NEWINDUSTRYBUTTON, TOP_RIGHT);

	AddAnchor(IDC_EDITSIDINGBUTTON, TOP_RIGHT);
	AddAnchor(IDC_EDITLOCATIONBUTTON, TOP_RIGHT);
	AddAnchor(IDC_EDITINDUSTRYBUTTON, TOP_RIGHT);

	AddAnchor(IDC_DELETEBUTTON, TOP_RIGHT);
	AddAnchor(IDC_DOWNBUTTON, TOP_RIGHT);
	AddAnchor(IDC_UPBUTTON, TOP_RIGHT);

	AddAnchor(IDC_LAYOUT, TOP_LEFT, BOTTOM_RIGHT);
    
    ReadDB();

    EnableSaveRestore(_T("PhysicalLayoutDialog"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPhysicalLayoutDialog::ReadDB()
{
    m_ctlLayout.DeleteAllItems();
    HTREEITEM pRoot = m_ctlLayout.InsertItem(g_sRRName);
    //
    //  Read locations
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT id,name FROM Locations ORDER BY sort_order;");
    //
    while (!q.eof())
    {
        HTREEITEM pLocation = m_ctlLayout.InsertItem(q.fieldValue(1),pRoot);
        m_ctlLayout.SetItemData(pLocation,atoi(q.fieldValue(0)));
        //
        //  get sidings for this location
        //
        CString sSQL;
        sSQL.Format("SELECT id,name,capacity,yard,interchange FROM Sidings WHERE Locations_FK=%d ORDER BY sort_order;",atoi(q.fieldValue(0)));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while (!q2.eof())
        {
            CString sSiding;
            CString sYard(""),sInt("");
            int iSidingRatio(0);
            if( q2.getIntField("yard") == 1 ) sYard = "[Yard";
            if( q2.getIntField("interchange") == 1 ) sInt = "Interchange] ";
            if( !sYard.IsEmpty() && !sInt.IsEmpty() )
                sYard = sYard + "/";
            if( sYard.IsEmpty() && !sInt.IsEmpty() ) sInt = "[" + sInt;
            if( sInt.IsEmpty() && !sYard.IsEmpty() ) sYard = sYard + "] ";
            //
            sSiding.Format("%s (len: %d\") %s%s",q2.getStringField("name"),q2.getIntField("capacity"),sYard,sInt);
            HTREEITEM pSiding = m_ctlLayout.InsertItem(sSiding,pLocation, TVI_LAST);
            m_ctlLayout.SetItemData(pSiding,q2.getIntField("id"));
            //
            //  get industries for this location
            //
            sSQL.Format("SELECT id,name FROM Industries WHERE Sidings_FK=%d ORDER BY sort_order;",q2.getIntField("id"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            while (!q3.eof())
            {
                CString sIndustry;
                int iIndustryRatio(0);
                sIndustry.Format("%s %s",q3.getStringField("name"),((CTrainOpsApp*)AfxGetApp())->GetIndustryRatios(q3.getIntField("id"),q2.getIntField("id"),&iIndustryRatio));
                HTREEITEM pIndustry = m_ctlLayout.InsertItem(sIndustry,pSiding, TVI_LAST);
                if( iIndustryRatio > 75 )
                    m_ctlLayout.SetItemColor(pIndustry, RGB(255, 0, 0));
                iSidingRatio += iIndustryRatio;
                //
                m_ctlLayout.SetItemData(pIndustry,-1*q3.getIntField("id"));
                //
                //  get car types for this industry
                //
                sSQL.Format("SELECT description,InOut,LoadEmptyDays,Quantity_low,Quantity_high,Quantity_percentage FROM Industries_Cars, CarTypes WHERE Industries_Cars.CarTypes_FK=CarTypes.id AND Industries_Cars.Industries_FK=%d ORDER BY description",-1*m_ctlLayout.GetItemData(pIndustry));
                CppSQLite3Query q4 = pDB->execQuery(sSQL);
                while (!q4.eof())
                {
                    //
                    CString sText;
                    sText.Format("%-17s - Time: %d | %s: %d-%d(%3d%%)",
                        q4.getStringField("description"),
                        q4.getIntField("LoadEmptyDays"),
                        q4.getIntField("InOut")==0?" In":"Out",
                        q4.getIntField("Quantity_low"),
                        q4.getIntField("Quantity_high"),
                        q4.getIntField("Quantity_percentage"));
                    HTREEITEM pCarType = m_ctlLayout.InsertItem(sText,pIndustry, TVI_LAST);
                    m_ctlLayout.SetItemData(pCarType,0);
                    q4.nextRow();
                }
                q4.finalize();
                //
                q3.nextRow();
            }
            //  re-show with siding %ages
            if( iSidingRatio > 0 )
            {
                sSiding.Format("%s (len: %d\") %s%s(Usage: %d%%)",q2.getStringField("name"),q2.getIntField("capacity"),sYard,sInt,iSidingRatio);
                m_ctlLayout.SetItemText(pSiding,sSiding);
                if( iSidingRatio > 75 )
                    m_ctlLayout.SetItemColor(pSiding, RGB(255, 0, 0));
            }
            //
            q3.finalize();
            //
            if( m_ctlLayout.ItemHasChildren(pSiding) && (!sInt.IsEmpty() || !sYard.IsEmpty()) )
            {
                m_ctlLayout.SetItemColor(pSiding,RGB(255,0,0));
            }
            //
            q2.nextRow();
        }
        q2.finalize();
        //
        q.nextRow();
    }
    //
    q.finalize();
    //
	m_ctlNewSidingButton.EnableWindow(FALSE);
	m_ctlNewLocationButton.EnableWindow(FALSE);
	m_ctlNewIndustryButton.EnableWindow(FALSE);
	m_ctlEditSidingButton.EnableWindow(FALSE);
	m_ctlEditLocationButton.EnableWindow(FALSE);
	m_ctlEditIndustryButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);

    m_ctlUpButton.EnableWindow(FALSE);
    m_ctlDownButton.EnableWindow(FALSE);

    TBTreeStateMgr::LoadTreeState(_T("PhysicalLayout"),&m_ctlLayout);
}



void CPhysicalLayoutDialog::OnRclickLayout(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    //
    //  figure out what item we're on
    //  if item parent is NULL, then its a location
    //  if item parent is NOT NULL, and its parent parent IS NULL, then its a siding
    //  otherwise, its an industry
    //
    UINT flags;
    CPoint ptScreen;
    if (!GetCursorPos(&ptScreen))
        return;
    CPoint ptClient(ptScreen);
    m_ctlLayout.ScreenToClient(&ptClient);
    HTREEITEM pItem = m_ctlLayout.HitTest(ptClient, &flags);
    if( !pItem )
        return;

    m_ctlLayout.SelectItem(pItem);
    
    
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    if( pParent == NULL )
    {
        CMenu menu;
        VERIFY(menu.LoadMenu(IDR_NEWLOCATIONMENU));
        CMenu *popup = menu.GetSubMenu(0);
        ASSERT(popup);
        VERIFY(popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
            ptScreen.x, ptScreen.y, this, NULL));
    }
    else
    {
        HTREEITEM pParent2 = m_ctlLayout.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
            CMenu menu;
            VERIFY(menu.LoadMenu(IDR_LOCATIONMENU));
            CMenu *popup = menu.GetSubMenu(0);
            ASSERT(popup);
            if( m_ctlLayout.GetChildItem(pItem) )
                popup->EnableMenuItem(ID_LOCATIONNMENU_DELETELOCATION, MF_DISABLED||MF_GRAYED );
            VERIFY(popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                ptScreen.x, ptScreen.y, this, NULL));
        }
        else
        {
            // if its a siding, show siding menu, otherwise, show industry menu
            int iData = m_ctlLayout.GetItemData(pItem);
            if( iData > 0 )
            {
                CMenu menu;
                VERIFY(menu.LoadMenu(IDR_SIDINGMENU));
                CMenu *popup = menu.GetSubMenu(0);
                ASSERT(popup);
                if( m_ctlLayout.GetChildItem(pItem) )
                    popup->EnableMenuItem(ID_SIDINGMENU_DELETESIDING, MF_DISABLED||MF_GRAYED );
                VERIFY(popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                    ptScreen.x, ptScreen.y, this, NULL));
            }
            else
            {
                CMenu menu;
                VERIFY(menu.LoadMenu(IDR_INDUSTRYMENU));
                CMenu *popup = menu.GetSubMenu(0);
                ASSERT(popup);
                if( m_ctlLayout.GetChildItem(pItem) )
                    popup->EnableMenuItem(ID_INDUSTRYMENU_DELETEINDUSTRY, MF_DISABLED||MF_GRAYED );
                VERIFY(popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                    ptScreen.x, ptScreen.y, this, NULL));
            }
        }
    }
    *pResult = 0;
}

void CPhysicalLayoutDialog::OnDblclkLayout(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    //
    //  figure out what item we're on
    //  if item parent is NULL, then its a location
    //  if item parent is NOT NULL, and its parent parent IS NULL, then its a siding
    //  otherwise, its an industry
    //
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;

	*pResult = TRUE;
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    if( pParent != NULL )
    {
        CString sRootText = m_ctlLayout.GetItemText(pParent);
        if( sRootText == g_sRRName )
        {
            OnLocationmenuEditlocation();
            return;
        }
        else
        {
            int iData = m_ctlLayout.GetItemData(pItem);
            if( iData > 0 )
            {
                OnSidingmenuEditsiding();
                return;
            }
            else
                if( iData < 0 )
                {
                    OnIndustrymenuEditindustry();
                    return;
                }
        }
        return;
    }
}

void CPhysicalLayoutDialog::OnIndustrymenuDeleteindustry() 
{
	// TODO: Add your command handler code here
//    MessageBox("Delete Industry");
    OnDeleteButton();
}

void CPhysicalLayoutDialog::OnIndustrymenuEditindustry() 
{
	// TODO: Add your command handler code here
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    int iID = m_ctlLayout.GetItemData(pItem);
    if( iID < 0 )
        iID = iID * -1;
    int iSidingID = m_ctlLayout.GetItemData(pParent);
    if( iSidingID < 0 )
        iSidingID = iSidingID * -1;
    CIndustryDetailDialog pDlg;
    pDlg.SetIndustryID(iID);
    pDlg.SetIndustrySiding(iSidingID);
    if( pDlg.DoModal() == IDOK )
        ReadDB();
}

void CPhysicalLayoutDialog::OnIndustrymenuNewindustry() 
{
	// TODO: Add your command handler code here
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    int iSidingID = m_ctlLayout.GetItemData(pItem);
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    CIndustryDetailDialog pDlg;	
    pDlg.SetIndustryDefault();
    pDlg.SetIndustrySiding(iSidingID);
    if( pDlg.DoModal() == IDOK )
        ReadDB();
}

void CPhysicalLayoutDialog::OnLocationmenuEditlocation() 
{
	// TODO: Add your command handler code here
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    CLocationDetailDialog pDlg;
    int iID = m_ctlLayout.GetItemData(pItem);
    pDlg.SetLocationID(iID);
    pDlg.DoModal();
    //
    ReadDB();
}

void CPhysicalLayoutDialog::OnLocationmenuNewlocation() 
{
	// TODO: Add your command handler code here
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    CLocationDetailDialog pDlg;
    pDlg.SetLocationID(-1);
    if( pDlg.DoModal() == IDOK )
        ReadDB();
}

void CPhysicalLayoutDialog::OnLocationnmenuDeletelocation() 
{
	// TODO: Add your command handler code here
//    MessageBox("Delete Location");
    OnDeleteButton();
}

void CPhysicalLayoutDialog::OnSidingmenuDeletesiding() 
{
	// TODO: Add your command handler code here
//    MessageBox("Delete Siding");
    OnDeleteButton();
}

void CPhysicalLayoutDialog::OnSidingmenuEditsiding() 
{
	// TODO: Add your command handler code here
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    CSidingDetailDialog pDlg;	
    int iID = m_ctlLayout.GetItemData(pItem);
    pDlg.SetSidingID(iID);
    pDlg.DoModal();
    //
    ReadDB();
}

void CPhysicalLayoutDialog::OnSidingmenuNewsiding() 
{
	// TODO: Add your command handler code here
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    CSidingDetailDialog pDlg;	
    pDlg.SetSidingDefault();
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    CString sName = m_ctlLayout.GetItemText(pItem);
    pDlg.SetSidingLocation(sName);
    if( pDlg.DoModal() == IDOK )
        ReadDB();
}

void CPhysicalLayoutDialog::OnClickLayout(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//
    //  Figure out what buttons to enable/disable
    //
	m_ctlNewSidingButton.EnableWindow(FALSE);
	m_ctlNewLocationButton.EnableWindow(FALSE);
	m_ctlNewIndustryButton.EnableWindow(FALSE);
	m_ctlEditSidingButton.EnableWindow(FALSE);
	m_ctlEditLocationButton.EnableWindow(FALSE);
	m_ctlEditIndustryButton.EnableWindow(FALSE);
    m_ctlDeleteButton.EnableWindow(FALSE);

    m_ctlUpButton.EnableWindow(FALSE);
    m_ctlDownButton.EnableWindow(FALSE);

    UINT flags;
    CPoint ptScreen;
    if (!GetCursorPos(&ptScreen))
        return;
    CPoint ptClient(ptScreen);
    m_ctlLayout.ScreenToClient(&ptClient);
    HTREEITEM pItem = m_ctlLayout.HitTest(ptClient, &flags);
    if( !pItem )
        return;

    m_ctlLayout.SelectItem(pItem);
    
    BOOL bCar=FALSE;
    
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    if( pParent == NULL )
    {
        m_ctlNewLocationButton.EnableWindow(TRUE);
    }
    else
    {
        HTREEITEM pParent2 = m_ctlLayout.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
    	    m_ctlNewSidingButton.EnableWindow(TRUE);
        	m_ctlEditLocationButton.EnableWindow(TRUE);
            if( !m_ctlLayout.ItemHasChildren(pItem) )
                m_ctlDeleteButton.EnableWindow(TRUE);
        }
        else
        {
            // if its a siding, show siding menu, otherwise, show industry menu
            int iData = m_ctlLayout.GetItemData(pItem);
            if( iData > 0 )
            {
        	    m_ctlNewIndustryButton.EnableWindow(TRUE);
            	m_ctlEditSidingButton.EnableWindow(TRUE);
                if( !m_ctlLayout.ItemHasChildren(pItem) )
                    m_ctlDeleteButton.EnableWindow(TRUE);
            }
            else
            {
                if( iData != 0 )
                {
            	    m_ctlEditIndustryButton.EnableWindow(TRUE);
                    if( !m_ctlLayout.ItemHasChildren(pItem) )
                        m_ctlDeleteButton.EnableWindow(TRUE);
                }
                else
                    bCar = TRUE;
            }
        }
    }
    //
    //  move up/down buttons.  If the child is at the TOP (no previous),
    //  only enable the down button.  If the child is at the BOTTOM (no next),
    //  only enable the up button.  Otherwise, enable them both.
    //
    if( !bCar )
    {
        if( m_ctlLayout.GetNextSiblingItem(pItem) != NULL )
            m_ctlDownButton.EnableWindow(TRUE);
        if( m_ctlLayout.GetPrevSiblingItem(pItem) != NULL )
            m_ctlUpButton.EnableWindow(TRUE);
    }
    //
	*pResult = 0;
}

void CPhysicalLayoutDialog::OnEditindustrybutton() 
{
	// TODO: Add your control notification handler code here
	OnIndustrymenuEditindustry();
}

void CPhysicalLayoutDialog::OnEditlocationbutton() 
{
	// TODO: Add your control notification handler code here
    OnLocationmenuEditlocation();
}

void CPhysicalLayoutDialog::OnEditsidingbutton() 
{
	// TODO: Add your control notification handler code here
    OnSidingmenuEditsiding();	
}

void CPhysicalLayoutDialog::OnNewindustrybutton() 
{
	// TODO: Add your control notification handler code here
	OnIndustrymenuNewindustry();
}

void CPhysicalLayoutDialog::OnNewlocationbutton() 
{
	// TODO: Add your control notification handler code here
	OnLocationmenuNewlocation();
}

void CPhysicalLayoutDialog::OnNewsidingbutton() 
{
	// TODO: Add your control notification handler code here
	OnSidingmenuNewsiding();
}

void CPhysicalLayoutDialog::OnDeleteButton() 
{
	// TODO: Add your control notification handler code here
	//
    //  to delete something, everything below it must be deleted beforehand
    //
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    CString sMsg;
    sMsg.Format("Are you sure you want to delete '%s'?",m_ctlLayout.GetItemText(pItem));
    if( MessageBox(sMsg,"LSC TrainOps",MB_YESNO) == IDNO )
        return;
    //
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
    //
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    if( pParent == NULL )
    {
        return;
    }
    else
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        HTREEITEM pParent2 = m_ctlLayout.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
            sSQL.Format("DELETE FROM Locations WHERE id=%d",m_ctlLayout.GetItemData(pItem));
            pDB->execDML(sSQL);
            //MessageBox(sSQL);
            MessageBox("Location Deleted","TrainOps");
        }
        else
        {
            // if its a siding, show siding menu, otherwise, show industry menu
            int iData = m_ctlLayout.GetItemData(pItem);
            if( iData > 0 )
            {
                sSQL.Format("DELETE FROM Sidings WHERE id=%d",m_ctlLayout.GetItemData(pItem));
                pDB->execDML(sSQL);
                //MessageBox(sSQL);
                MessageBox("Siding Deleted","TrainOps");
            }
            else
            {
                if( iData != 0 )
                {
                    sSQL.Format("DELETE FROM Industries WHERE id=%d",-1*m_ctlLayout.GetItemData(pItem));
                    //MessageBox(sSQL);
                    pDB->execDML(sSQL);
                    MessageBox("Industry Deleted","TrainOps");
                }
            }
        }
    }
    //
    ReadDB();
}

void CPhysicalLayoutDialog::OnOK() 
{
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
	
	CResizableDialog::OnOK();
}

void CPhysicalLayoutDialog::OnCancel() 
{
    TBTreeStateMgr::SaveTreeState(_T("PhysicalLayout"),&m_ctlLayout);    
	
	CResizableDialog::OnCancel();
}

static LPARAM CopyData(const CTreeCtrl& tree, HTREEITEM hItem, LPARAM lParam)
{
  if (lParam == 0)
    return 0;
  CString* ps = (CString*)lParam;
  CString* psNew = new CString(*ps);
  return (LPARAM)psNew;
}

void CPhysicalLayoutDialog::OnDownbutton() 
{
	// TODO: Add your control notification handler code here
    //
    //  get next sibling, and swap places
    //
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    HTREEITEM pNextItem = m_ctlLayout.GetNextSiblingItem(pItem);
    if( !pNextItem )
        return;
    //
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    HTREEITEM pNewItem = MoveTreeItem(m_ctlLayout,pItem,pParent,FALSE,CopyData,pNextItem);
    ExpandTreeItem(m_ctlLayout,pNewItem,TVE_EXPAND);
    m_ctlLayout.SelectItem(pNewItem);
    //
    //  re-save sort_orders for this sub-tree
    //
    if( pParent == NULL )
    {
        return;
    }
    else
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        HTREEITEM pParent2 = m_ctlLayout.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
            //
            HTREEITEM hChild = m_ctlLayout.GetChildItem(pParent);
            int iOrder=0;
            while(hChild)
            {
                sSQL.Format("UPDATE Locations SET sort_order=%d WHERE id=%d",iOrder++,m_ctlLayout.GetItemData(hChild));
                pDB->execDML(sSQL);
                hChild = m_ctlLayout.GetNextSiblingItem(hChild);
            }

        }
        else
        {
            int iData = m_ctlLayout.GetItemData(pNewItem);
            if( iData > 0 )
            {
                HTREEITEM hChild = m_ctlLayout.GetChildItem(pParent);
                int iOrder=0;
                while(hChild)
                {
                    sSQL.Format("UPDATE Sidings SET sort_order=%d WHERE id=%d",iOrder++,m_ctlLayout.GetItemData(hChild));
                    pDB->execDML(sSQL);
                    hChild = m_ctlLayout.GetNextSiblingItem(hChild);
                }
            }
            else
            {
                if( iData != 0 )
                {
                    HTREEITEM hChild = m_ctlLayout.GetChildItem(pParent);
                    int iOrder=0;
                    while(hChild)
                    {
                        sSQL.Format("UPDATE Industries SET sort_order=%d WHERE id=%d",iOrder++,m_ctlLayout.GetItemData(hChild));
                        pDB->execDML(sSQL);
                        hChild = m_ctlLayout.GetNextSiblingItem(hChild);
                    }
                }
            }
        }
    }
}

void CPhysicalLayoutDialog::OnUpbutton() 
{
	// TODO: Add your control notification handler code here
    //
    //  get previous sibling, and swap places
    //
    HTREEITEM pItem = m_ctlLayout.GetSelectedItem();
    if( !pItem )
        return;
    HTREEITEM pPrevItem1 = m_ctlLayout.GetPrevSiblingItem(pItem);
    if( !pPrevItem1 )
        return;
    HTREEITEM pPrevItem = m_ctlLayout.GetPrevSiblingItem(pPrevItem1);
    if( !pPrevItem )
        pPrevItem = TVI_FIRST;
    //
    //  TVI_FIRST
    //
    HTREEITEM pParent = m_ctlLayout.GetParentItem(pItem);
    HTREEITEM pNewItem = MoveTreeItem(m_ctlLayout,pItem,pParent,FALSE,CopyData,pPrevItem);
    ExpandTreeItem(m_ctlLayout,pNewItem,TVE_EXPAND);
    m_ctlLayout.SelectItem(pNewItem);
    //
    //  re-save sort_orders for this sub-tree
    //
    if( pParent == NULL )
    {
        return;
    }
    else
    {
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CString sSQL;
        HTREEITEM pParent2 = m_ctlLayout.GetParentItem(pParent);
        if( pParent2 == NULL )
        {
            //
            HTREEITEM hChild = m_ctlLayout.GetChildItem(pParent);
            int iOrder=0;
            while(hChild)
            {
                sSQL.Format("UPDATE Locations SET sort_order=%d WHERE id=%d",iOrder++,m_ctlLayout.GetItemData(hChild));
                pDB->execDML(sSQL);
                hChild = m_ctlLayout.GetNextSiblingItem(hChild);
            }

        }
        else
        {
            int iData = m_ctlLayout.GetItemData(pNewItem);
            if( iData > 0 )
            {
                HTREEITEM hChild = m_ctlLayout.GetChildItem(pParent);
                int iOrder=0;
                while(hChild)
                {
                    sSQL.Format("UPDATE Sidings SET sort_order=%d WHERE id=%d",iOrder++,m_ctlLayout.GetItemData(hChild));
                    pDB->execDML(sSQL);
                    hChild = m_ctlLayout.GetNextSiblingItem(hChild);
                }
            }
            else
            {
                if( iData != 0 )
                {
                    HTREEITEM hChild = m_ctlLayout.GetChildItem(pParent);
                    int iOrder=0;
                    while(hChild)
                    {
                        sSQL.Format("UPDATE Industries SET sort_order=%d WHERE id=%d",iOrder++,m_ctlLayout.GetItemData(hChild));
                        pDB->execDML(sSQL);
                        hChild = m_ctlLayout.GetNextSiblingItem(hChild);
                    }
                }
            }
        }
    }
}

void CPhysicalLayoutDialog::ExpandTreeItem(CTreeCtrl &tree, HTREEITEM hItem, UINT nCode)
{
    HTREEITEM hChild;
    
    if(tree.ItemHasChildren(hItem))
    {
        tree.Expand(hItem, nCode);
        hChild = tree.GetChildItem(hItem);
        
        while(hChild)
        {
            ExpandTreeItem(tree, hChild, nCode);
            hChild = tree.GetNextItem(hChild, TVGN_NEXT);
        }
    }
}
////////////////////////////////////////////////////////////////////////////
// Function:	MoveTreeItem
// Version:	4
// Created:	24-Apr-2003
// Author:	Paul S. Vickery
// E-mail:	paul@vickeryhome.freeserve.co.uk
////////////////////////////////////////////////////////////////////////////
// Description:
//    Function to move or copy a tree item and its children to another location
//
// Parameters:
//    CTreeCtrl& tree - the tree control which owns the items
//    HTREEITEM hItem - the item to copy/move
//    HTREEITEM hItemTo - the new parent item for the item moved/copied
//    BOOL bCopyOnly - whether to move (copy+delete) or copy (Default: FALSE)
//    PFNMTICOPYDATA pfnCopyData - optional pointer to a function to copy the 
//                         item's data when making a duplicate item
//    HTREEITEM hItemPos - positioning for new (top) item. This can be one of 
//                         TVI_FIRST, TVI_LAST (Default), TVI_SORT, or TVI_ROOT.
//
// Return:
//    HTREEITEM - returns the new tree item created as a copy of hItem
//
////////////////////////////////////////////////////////////////////////////
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// If you use this code, or use it as a base for your own code, it would be 
// nice to hear from you simply so I know it's not been a waste of time!
//
// Copyright (c) 2003 Paul S. Vickery
//
////////////////////////////////////////////////////////////////////////////
// Version History:
//
// Version 4 - 24-Apr-2003
// =======================
// Added callback to copy item data when copying items, so data doesn't get 
// disposed of more than once
//
// Version 3 - 23-Apr-2003
// =======================
// Added code to prevent items being moved to their descendants (thanks to 
// Matt Korth for reporting this one)
//
// Version 2 - 06-Feb-2003
// =======================
// Fixed bug where moving a tree item with more than one child loses all but
// the first child (spotted by Jack Ploeg).
//
// Version 1 - 31-Jan-2003
// =======================
// Initial version
// 
////////////////////////////////////////////////////////////////////////////
HTREEITEM CPhysicalLayoutDialog::MoveTreeItem(CTreeCtrl& tree, HTREEITEM hItem, HTREEITEM hItemTo, 
		       BOOL bCopyOnly, PFNMTICOPYDATA pfnCopyData, 
		       HTREEITEM hItemPos)
{
    if (hItem == NULL || hItemTo == NULL)
        return NULL;
    if (hItem == hItemTo )//|| hItemTo == tree.GetParentItem(hItem))
        return hItem;
    // check we're not trying to move to a descendant
    HTREEITEM hItemParent = hItemTo;
    while (hItemParent != TVI_ROOT && 
        (hItemParent = tree.GetParentItem(hItemParent)) != NULL)
        if (hItemParent == hItem)
            return NULL;
        
        // copy items to new location, recursively, then delete old hierarchy
        // get text, and other info
        CString sText = tree.GetItemText(hItem);
        TVINSERTSTRUCT tvis;
        tvis.item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM | 
            TVIF_SELECTEDIMAGE | TVIF_STATE;
        tvis.item.hItem = hItem;
        // we don't want to copy selection/expanded state etc
        tvis.item.stateMask = (UINT)-1 & ~(TVIS_DROPHILITED | TVIS_EXPANDED | 
            TVIS_EXPANDEDONCE | TVIS_EXPANDPARTIAL | TVIS_SELECTED);
        tree.GetItem(&tvis.item);
        tvis.hParent = hItemTo;
        tvis.hInsertAfter = hItemPos;
        // if we're only copying, then ask for new data
        if (bCopyOnly && pfnCopyData != NULL)
            tvis.item.lParam = pfnCopyData(tree, hItem, tvis.item.lParam);
        HTREEITEM hItemNew = tree.InsertItem(&tvis);
        tree.SetItemText(hItemNew, sText);
        
        // now move children to under new item
        HTREEITEM hItemChild = tree.GetChildItem(hItem);
        while (hItemChild != NULL)
        {
            HTREEITEM hItemNextChild = tree.GetNextSiblingItem(hItemChild);
            MoveTreeItem(tree, hItemChild, hItemNew, bCopyOnly, pfnCopyData);
            hItemChild = hItemNextChild;
        }
        
        if (! bCopyOnly)
        {
            // clear item data, so nothing tries to delete stuff based on lParam
            tree.SetItemData(hItem, 0);
            // no (more) children, so we can safely delete top item
            tree.DeleteItem(hItem);
        }
        
        return hItemNew;
}

/*
CString CPhysicalLayoutDialog::GetIndustryRatios(int iIndustryFK,int iSidingFK,int* iRatio)
{
    CString sRatioText("");
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    //  cars in
    //
    sSQL.Format("SELECT * from Industries_Cars WHERE Industries_FK=%d AND InOut=0",iIndustryFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    float fPctIn(0);
    float fPctOut(0);
    while( !q.eof() )
    {
        int iLoadEmptyDays = q.getIntField("LoadEmptyDays");
        float fQuanHigh = q.getFloatField("Quantity_high");
        float fQuanLow = q.getFloatField("Quantity_low");
        float fPercent = q.getFloatField("Quantity_percentage");
        float fPct = fQuanLow + (fQuanHigh *(fPercent/100))*iLoadEmptyDays;
        fPctIn += fPct;
        //
        q.nextRow();
    }
    q.finalize();
    //
    //  cars out
    //
    sSQL.Format("SELECT * from Industries_Cars WHERE Industries_FK=%d AND InOut=1",iIndustryFK);
    q = pDB->execQuery(sSQL);
    while( !q.eof() )
    {
        int iLoadEmptyDays = q.getIntField("LoadEmptyDays");
        float fQuanHigh = q.getFloatField("Quantity_high");
        float fQuanLow = q.getFloatField("Quantity_low");
        float fPercent = q.getFloatField("Quantity_percentage");
        float fPct = fQuanLow + (fQuanHigh *(fPercent/100))*iLoadEmptyDays;
        fPctOut += fPct;
        //
        q.nextRow();
    }
    q.finalize();
    //
    //  get average car length
    //
    q = pDB->execQuery("SELECT AVG(car_length) AS avlen FROM Cars where car_length > 0");
    int iAverageCarLength = g_iCarLength;
    if( !q.eof() )
        iAverageCarLength = q.getIntField("avlen");
    q.finalize();
    //
    sSQL.Format("SELECT capacity FROM Sidings WHERE id=%d;",iSidingFK);
    q = pDB->execQuery(sSQL);
    int iSidingLength = 100;
    if( !q.eof() )
        iSidingLength = q.getIntField("capacity");
    q.finalize();
    //
    //  calculate siding use ratio
    //
    *iRatio = (((float)iAverageCarLength * (fPctIn+fPctOut))/ (float)iSidingLength)*100.0;
    //
    sRatioText.Format("(In: %1.1f | Out: %1.1f | Usage: %d%%)",fPctIn,fPctOut,*iRatio);
    //
    UpdateData(FALSE);

    return sRatioText;
}
*/
