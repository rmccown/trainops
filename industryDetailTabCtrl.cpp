// industryDetailTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "industryDetailTabCtrl.h"

#include "IndustryDetailDialog_Cars.h"
#include "IndustryDetailDialog_Commodities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CindustryDetailTabCtrl


//Constructor for the class derived from CTabCtrl
CindustryDetailTabCtrl::CindustryDetailTabCtrl()
{
    m_DialogID[0] = IDD_INDUSTRYDETAILDIALOG_CARS;
    m_DialogID[1] = IDD_INDUSTRYDETAILDIALOG_COMMODITIES;
    
    
    m_Dialog0 = new CIndustryDetailDialog_Cars();
    m_Dialog1 = new CIndustryDetailDialog_Commodities();
    m_Dialog[0] = m_Dialog0;
    m_Dialog[1] = m_Dialog1;
    
    m_nPageCount = 2;
    
}

CindustryDetailTabCtrl::~CindustryDetailTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CindustryDetailTabCtrl, CTabCtrl)
//{{AFX_MSG_MAP(CindustryDetailTabCtrl)
ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CindustryDetailTabCtrl message handlers


//This function creates the Dialog boxes once
void CindustryDetailTabCtrl::InitDialogs()
{
    m_Dialog[0]->Create(m_DialogID[0],GetParent());
    m_Dialog[1]->Create(m_DialogID[1],GetParent());
}

//Selection change event for the class derived from CTabCtrl
void CindustryDetailTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    ActivateTabDialogs();
    *pResult = 0;
}

void CindustryDetailTabCtrl::ActivateTabDialogs()
{
    int nSel = GetCurSel();
    if(m_Dialog[nSel]->m_hWnd)
        m_Dialog[nSel]->ShowWindow(SW_HIDE);
    
    CRect l_rectClient;
    CRect l_rectWnd;
    
    GetClientRect(l_rectClient);
    AdjustRect(FALSE,l_rectClient);
    GetWindowRect(l_rectWnd);
    GetParent()->ScreenToClient(l_rectWnd);
    l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
    for(int nCount=0; nCount < m_nPageCount; nCount++){
        m_Dialog[nCount]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
    }
    m_Dialog[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
    
    m_Dialog[nSel]->ShowWindow(SW_SHOW);
    
}


