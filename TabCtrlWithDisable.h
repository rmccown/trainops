#if !defined(AFX_TABCTRLWITHDISABLE_H__6D781400_6BCD_4715_BF68_9209E2DE6C6E__INCLUDED_)
#define AFX_TABCTRLWITHDISABLE_H__6D781400_6BCD_4715_BF68_9209E2DE6C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCtrlWithDisable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlWithDisable window

////////////////////////////////////////////////////////////////
// 1998 Microsoft Systems Journal. 
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 5.0 on Windows 95
// See DisabTab.cpp
// 
class CTabCtrlWithDisable : public CTabCtrl {
   DECLARE_DYNAMIC(CTabCtrlWithDisable)
public:
   CTabCtrlWithDisable();
   virtual ~CTabCtrlWithDisable();

   // functions you must implement/call
   virtual  BOOL IsTabEnabled(int iTab) = 0;          // you must override
   BOOL     TranslatePropSheetMsg(MSG* pMsg);         // call from prop sheet
   BOOL     SubclassDlgItem(UINT nID, CWnd* pParent); // non-virtual override

   // helpers
   int      NextEnabledTab(int iTab, BOOL bWrap);  // get next enabled tab
   int      PrevEnabledTab(int iTab, BOOL bWrap);  // get prev enabled tab
   BOOL     SetActiveTab(UINT iNewTab);            // set tab (fail if disabled)

protected:
   DECLARE_MESSAGE_MAP()
   afx_msg  void OnSelChanging(NMHDR* pNmh, LRESULT* pRes);

   // MFC overrides
   virtual  BOOL PreTranslateMessage(MSG* pMsg);
   virtual  void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

   // override to draw text only; eg, colored text or different font
   virtual  void OnDrawText(CDC& dc, CRect rc, CString sText, BOOL bDisabled);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRLWITHDISABLE_H__6D781400_6BCD_4715_BF68_9209E2DE6C6E__INCLUDED_)
