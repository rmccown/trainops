#if !defined(AFX_EditTrainDialog_H__BAFC7D6B_137B_4CCD_B1FD_590EFEF601FA__INCLUDED_)
#define AFX_EditTrainDialog_H__BAFC7D6B_137B_4CCD_B1FD_590EFEF601FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditTrainDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CEditTrainDialog dialog

class CEditTrainDialog : public CDialog
{
// Construction
public:
	CEditTrainDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditTrainDialog)
	enum { IDD = IDD_EDITTRAIN };
	CStatic	m_ctlMaxMovesText;
	CEdit	m_ctlMaxMoves;
	CButton	m_ctlSwitchInterchanges;
	CButton	m_ctlLocalSwitcher;
	CButton	m_ctlAddButton;
	CButton	m_ctlRemoveButton;
	CTOListCtrl	m_ctlIndustryList;
	CButton	m_ctlDown;
	CButton	m_ctlUp;
	CListBox	m_ctlRouteLocationList;
	CListBox	m_ctlLocationList;
	BOOL	m_bLocalSwitcher;
	CString	m_sTrainCode;
	CString	m_sTrainName;
	BOOL	m_bSwitchInterchanges;
	int		m_iMaxMoves;
	//}}AFX_DATA

    void SetTrainID(int iTrainID) { m_iTrainID = iTrainID; } ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditTrainDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iTrainID;

	// Generated message map functions
	//{{AFX_MSG(CEditTrainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbutton();
	afx_msg void OnRemovebutton();
	afx_msg void OnSelchangeRoutelocationlist();
	afx_msg void OnUpbutton();
	afx_msg void OnDownbutton();
	afx_msg void OnSelchangeAvailablelocationlist();
	afx_msg void OnDblclkAvailablelocationlist();
	afx_msg void OnItemchangedIndustrylist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnLocalswitcher();
	afx_msg void OnChangeRoutecode();
	afx_msg void OnChangeRoutename();
	afx_msg void OnClose();
	afx_msg void OnInterchangeCheck();
	virtual void OnCancel();
	afx_msg void OnChangeMaxMoves();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EditTrainDialog_H__BAFC7D6B_137B_4CCD_B1FD_590EFEF601FA__INCLUDED_)
