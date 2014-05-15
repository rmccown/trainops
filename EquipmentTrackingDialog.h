#if !defined(AFX_EQUIPMENTTRACKINGDIALOG_H__B2F4E516_3A3A_44A9_865B_3BCD01DFF1B5__INCLUDED_)
#define AFX_EQUIPMENTTRACKINGDIALOG_H__B2F4E516_3A3A_44A9_865B_3BCD01DFF1B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EquipmentTrackingDialog.h : header file
//

#include "TOListCtrl.h"
#include "TreeCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CEquipmentTrackingDialog dialog

class CEquipmentTrackingDialog : public CDialog
{
// Construction
public:
	CEquipmentTrackingDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEquipmentTrackingDialog)
	enum { IDD = IDD_EQUIPMENTTRACKINGDIALOG };
	CTreeCtrlEx	m_ctlEquipmentTree;
	CTOListCtrl	m_ctlCarData;
	//}}AFX_DATA
    void ReadDB();
    void CleanupTree();
    void GetCars(int iCarFK);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEquipmentTrackingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEquipmentTrackingDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangedEquipmenttree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUIPMENTTRACKINGDIALOG_H__B2F4E516_3A3A_44A9_865B_3BCD01DFF1B5__INCLUDED_)
