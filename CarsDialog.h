#if !defined(AFX_CARSDIALOG_H__C5BBBF98_E38A_4241_B276_DDB12F199646__INCLUDED_)
#define AFX_CARSDIALOG_H__C5BBBF98_E38A_4241_B276_DDB12F199646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarsDialog.h : header file
//

#include <Afxwin.h>
#include <Afxtempl.h>

#include "TOListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCarsDialog dialog

class CCarsDialog : public CDialog
{
// Construction
public:
	CCarsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCarsDialog)
	enum { IDD = IDD_CARS_DIALOG };
	CButton	m_ctlEditCarsButton;
	CTOListCtrl	m_ctlCarsList;
	CComboBox	m_ctlCarTypeCombo;
	CString	m_sReportingMarks;
	int		m_iCarQuantity;
	//}}AFX_DATA
    
    CImageList m_pImageList;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    void ReadDB();


// Implementation
protected:
    HTREEITEM GetNextTreeItem(const CTreeCtrl& treeCtrl, HTREEITEM hItem);

    float GetCarUsage(int iCarTypeFK);

	// Generated message map functions
	//{{AFX_MSG(CCarsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddcars();
	virtual void OnOK();
	afx_msg void OnEditCarsButton();
	afx_msg void OnDblclkCarslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCarslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARSDIALOG_H__C5BBBF98_E38A_4241_B276_DDB12F199646__INCLUDED_)
