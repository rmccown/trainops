#if !defined(AFX_TERMINATENEWLOCATIONDIALOG_H__7F31EF85_7652_4B94_9015_C23C801C8BD0__INCLUDED_)
#define AFX_TERMINATENEWLOCATIONDIALOG_H__7F31EF85_7652_4B94_9015_C23C801C8BD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerminateNewLocationDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTerminateNewLocationDialog dialog

class CTerminateNewLocationDialog : public CDialog
{
// Construction
public:
	CTerminateNewLocationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTerminateNewLocationDialog)
	enum { IDD = IDD_TERMINATENEWLOCATIONDIALOG };
	CListBox	m_ctlSidingList;
	CListBox	m_ctlIndustryList;
	CListBox	m_ctlLocationList;
	CString	m_sIndustry;
	CString	m_sLocation;
	CString	m_sSiding;
	//}}AFX_DATA
    int m_iIndustry;
    int m_iLocation;
    int m_iSiding;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerminateNewLocationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTerminateNewLocationDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLocationlist();
	virtual void OnOK();
	afx_msg void OnSelchangeIndustrylist();
	afx_msg void OnSelchangeSidinglist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMINATENEWLOCATIONDIALOG_H__7F31EF85_7652_4B94_9015_C23C801C8BD0__INCLUDED_)
