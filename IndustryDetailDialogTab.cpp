// IndustryDetailDialogTab.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustryDetailDialogTab.h"

#include "IndustryDetailDialogCartypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialogTab


CIndustryDetailDialogTab::~CIndustryDetailDialogTab()
{
}


BEGIN_MESSAGE_MAP(CIndustryDetailDialogTab, CTabCtrl)
	//{{AFX_MSG_MAP(CIndustryDetailDialogTab)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialogTab message handlers


    //Constructor for the class derived from CTabCtrl
    CIndustryDetailDialogTab::CIndustryDetailDialogTab()
    {
       m_DialogID[0] =IDD_INDUSTRYDETAILDIALOG_CARTYPES;
       m_DialogID[1] =IDD_INDUSTRYDETAILDIALOG_CARTYPES;


       m_Dialog[0] = new CIndustryDetailDialogCartypes();
       m_Dialog[1] = new CIndustryDetailDialogCartypes();

       m_nPageCount = 2;

    }

    //This function creates the Dialog boxes once
    void CIndustryDetailDialogTab::InitDialogs()
    {
    m_Dialog[0]->Create(m_DialogID[0],GetParent());
    m_Dialog[1]->Create(m_DialogID[1],GetParent());
    }

    //Selection change event for the class derived from CTabCtrl
    void CIndustryDetailDialogTab::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
    {
       // TODO: Add your control notification handler code here
       ActivateTabDialogs();
       *pResult = 0;
    }

    void CIndustryDetailDialogTab::ActivateTabDialogs()
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


