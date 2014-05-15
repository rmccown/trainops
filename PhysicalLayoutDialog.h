#if !defined(AFX_PHYSICALLAYOUTDIALOG_H__2FD328B0_17D0_4A25_92BA_EA507CD22550__INCLUDED_)
#define AFX_PHYSICALLAYOUTDIALOG_H__2FD328B0_17D0_4A25_92BA_EA507CD22550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhysicalLayoutDialog.h : header file
//

#include "TreeCtrlEx.h"

#include "ResizableDialog.h"

// typedef for callback function for copying item data
// Passes in the tree control, the item being copied, and the item's data.
// The function should make and return a new copy of the data if required.
typedef LPARAM(*PFNMTICOPYDATA)(const CTreeCtrl&, HTREEITEM, LPARAM);

/////////////////////////////////////////////////////////////////////////////
// CPhysicalLayoutDialog dialog

class CPhysicalLayoutDialog : public CResizableDialog
{
// Construction
public:
	CPhysicalLayoutDialog(CWnd* pParent = NULL);   // standard constructor

    HTREEITEM MoveTreeItem(CTreeCtrl& tree, HTREEITEM hItem, HTREEITEM hItemTo, 
		       BOOL bCopyOnly = FALSE, PFNMTICOPYDATA pfnCopyData = NULL, 
		       HTREEITEM hItemPos = TVI_LAST);

    void ExpandTreeItem(CTreeCtrl &tree, HTREEITEM hItem, UINT nCode);

// Dialog Data
	//{{AFX_DATA(CPhysicalLayoutDialog)
	enum { IDD = IDD_PHYSICAL_LAYOUT_DIALOG };
	CButton	m_ctlUpButton;
	CButton	m_ctlDownButton;
	CButton	m_ctlDeleteButton;
	CButton	m_ctlEditSidingButton;
	CButton	m_ctlEditLocationButton;
	CButton	m_ctlEditIndustryButton;
	CButton	m_ctlNewSidingButton;
	CButton	m_ctlNewLocationButton;
	CButton	m_ctlNewIndustryButton;
	CTreeCtrlEx	m_ctlLayout;
	//}}AFX_DATA
    void ReadDB();

    CFont m_pFont;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhysicalLayoutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    //CString GetIndustryRatios(int iIndustryFK, int iSidingFK,int* iRatio);

	// Generated message map functions
	//{{AFX_MSG(CPhysicalLayoutDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickLayout(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLayout(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIndustrymenuDeleteindustry();
	afx_msg void OnIndustrymenuEditindustry();
	afx_msg void OnIndustrymenuNewindustry();
	afx_msg void OnLocationmenuEditlocation();
	afx_msg void OnLocationmenuNewlocation();
	afx_msg void OnLocationnmenuDeletelocation();
	afx_msg void OnSidingmenuDeletesiding();
	afx_msg void OnSidingmenuEditsiding();
	afx_msg void OnSidingmenuNewsiding();
	afx_msg void OnClickLayout(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditindustrybutton();
	afx_msg void OnEditlocationbutton();
	afx_msg void OnEditsidingbutton();
	afx_msg void OnNewindustrybutton();
	afx_msg void OnNewlocationbutton();
	afx_msg void OnNewsidingbutton();
	afx_msg void OnDeleteButton();
	virtual void OnOK();
	afx_msg void OnDownbutton();
	afx_msg void OnUpbutton();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHYSICALLAYOUTDIALOG_H__2FD328B0_17D0_4A25_92BA_EA507CD22550__INCLUDED_)
