// ScheduledTrainsReportsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "ScheduledTrainsReportsDialog.h"

#include "Reports.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScheduledTrainsReportsDialog dialog

#define UM2_CHECKSTATECHANGE (WM_USER + 101)


CScheduledTrainsReportsDialog::CScheduledTrainsReportsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CScheduledTrainsReportsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScheduledTrainsReportsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScheduledTrainsReportsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScheduledTrainsReportsDialog)
	DDX_Control(pDX, IDC_PRINTBUTTON, m_ctlPrintButton);
	DDX_Control(pDX, IDC_TRAINSTREE, m_ctlTrainsTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScheduledTrainsReportsDialog, CDialog)
	//{{AFX_MSG_MAP(CScheduledTrainsReportsDialog)
	ON_BN_CLICKED(IDC_PRINTBUTTON, OnPrint)
	ON_NOTIFY(NM_CLICK, IDC_TRAINSTREE, OnClickTrainsTree)
	//}}AFX_MSG_MAP
    ON_MESSAGE(UM2_CHECKSTATECHANGE, OnCheckStateChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScheduledTrainsReportsDialog message handlers


BOOL CScheduledTrainsReportsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ctlTrainsTree.ModifyStyle( TVS_CHECKBOXES, 0 );
    m_ctlTrainsTree.ModifyStyle( 0, TVS_CHECKBOXES );
	// TODO: Add extra initialization here
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT Trains_Scheduled.id,name,trainid FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.Trains_FK=Trains.id AND train_run=0;");
    //
    m_ctlPrintButton.EnableWindow(FALSE);
    while (!q.eof() )
    {
        CString sTrain;
        sTrain.Format("%s #%s",q.getStringField("name"),q.getStringField("trainid"));
        HTREEITEM hItem = m_ctlTrainsTree.InsertItem(sTrain);
        m_ctlTrainsTree.SetItemData(hItem,q.getIntField("id"));
        m_ctlTrainsTree.SetCheck(hItem,TRUE);
        //
        HTREEITEM hItemSL = m_ctlTrainsTree.InsertItem("Switch List",hItem);
        m_ctlTrainsTree.SetItemData(hItemSL,-1);
        m_ctlTrainsTree.SetCheck(hItemSL,TRUE);
        HTREEITEM hItem19 = m_ctlTrainsTree.InsertItem("Form 19s",hItem);
        m_ctlTrainsTree.SetItemData(hItem19,-2);
        m_ctlTrainsTree.SetCheck(hItem19,TRUE);
        //  bad orders?
        CString sSQL;
        sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars, Cars WHERE Trains_Scheduled_Cars.Trains_Scheduled_FK=%d AND Trains_Scheduled_Cars.Cars_FK=Cars.id AND out_of_service=1",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        BOOL bBadOrder(FALSE);
        if( !q2.eof() )
        {
            HTREEITEM hItemBadOrder = m_ctlTrainsTree.InsertItem("Bad Orders",hItem);
            m_ctlTrainsTree.SetItemData(hItemBadOrder,-3);
            m_ctlTrainsTree.SetCheck(hItemBadOrder,TRUE);
        }
        q2.finalize();
        //
        m_ctlTrainsTree.Expand(hItem,TVE_EXPAND);
        q.nextRow();
        //
        m_ctlPrintButton.EnableWindow(TRUE);
    }
    q.finalize();

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScheduledTrainsReportsDialog::OnPrint() 
{
    HTREEITEM htiSibling = m_ctlTrainsTree.GetRootItem();
    while (htiSibling )
    {
        if( m_ctlTrainsTree.GetCheck(htiSibling) )
        {
            int iScheduledTrainFK = m_ctlTrainsTree.GetItemData(htiSibling);
            //
            HTREEITEM subItem = m_ctlTrainsTree.GetChildItem(htiSibling);
            do
            {
                CString sText = m_ctlTrainsTree.GetItemText(subItem);
                if( m_ctlTrainsTree.GetCheck(subItem) )
                {
                    switch (m_ctlTrainsTree.GetItemData(subItem))
                    {
                    case -1:
                        {
                            PrintSwitchlist(iScheduledTrainFK);
                            break;
                        }
                    case -2:
                        {
                            PrintForm19s(iScheduledTrainFK);
                            break;
                        }
                    case -3:
                        {
	                        PrintBadOrderCards(iScheduledTrainFK);
                            break;
                        }
                    }
                }
                subItem = m_ctlTrainsTree.GetNextSiblingItem(subItem);
            }
            while (subItem != NULL);

        }
        htiSibling = m_ctlTrainsTree.GetNextSiblingItem(htiSibling);
    }
	
}

void CScheduledTrainsReportsDialog::OnClickTrainsTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    //NMTREEVIEW* pTreeView = (NMTREEVIEW*)pNMHDR;
    CPoint pt;
    GetCursorPos(&pt);
    m_ctlTrainsTree.ScreenToClient(&pt);
    //UINT uFlags;

    TVHITTESTINFO hti;
    hti.pt.x = pt.x;
    hti.pt.y = pt.y;
    m_ctlTrainsTree.HitTest(&hti);
    //
    if(hti.flags & TVHT_ONITEMSTATEICON)
    {
        HTREEITEM ht = hti.hItem;
        PostMessage(UM2_CHECKSTATECHANGE, 0, (LPARAM)ht);
        m_ctlTrainsTree.PostMessage(UM2_CHECKSTATECHANGE, 0, (LPARAM)ht);
    }
}

LRESULT CScheduledTrainsReportsDialog::OnCheckStateChanged(WPARAM pParam, LPARAM lParam)
{
    HTREEITEM   ht = (HTREEITEM)lParam;
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    // There is an item under the cursor.
    m_ctlTrainsTree.SelectItem(ht);
    int bCheck = m_ctlTrainsTree.GetCheck(ht);
    //
    //  If we're a train, check/uncheck everything underneath
    //
    int iSetoutsAdjust=0;
    int iPickupsAdjust=0;
    if( m_ctlTrainsTree.GetItemData(ht) >= 0 )
    {
        if ( m_ctlTrainsTree.ItemHasChildren(ht))
        {
            HTREEITEM htiChild = m_ctlTrainsTree.GetChildItem(ht);
            if (htiChild)
            {
                //m_ctlTrainDetail.ToggleItemState(htiChild);
                m_ctlTrainsTree.SetCheck(htiChild,m_ctlTrainsTree.GetCheck(ht));
                HTREEITEM htiSibling = m_ctlTrainsTree.GetNextSiblingItem(htiChild);
                while (htiSibling )
                {
                    //m_ctlTrainDetail.ToggleItemState(htiSibling);
                    m_ctlTrainsTree.SetCheck(htiSibling,m_ctlTrainsTree.GetCheck(ht));
                    htiSibling = m_ctlTrainsTree.GetNextSiblingItem(htiSibling);
                }
            }
            
        }
    }
    SetCursor(LoadCursor(NULL,IDC_ARROW));
    return 0;
}

