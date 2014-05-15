#if !defined(AFX_LOCATIONDETAILDIALOG_H__C2791387_3798_4396_A3D8_7B04F46E032F__INCLUDED_)
#define AFX_LOCATIONDETAILDIALOG_H__C2791387_3798_4396_A3D8_7B04F46E032F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocationDetailDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLocationDetailDialog dialog

class CLocationDetailDialog : public CDialog
{
// Construction
public:
	CLocationDetailDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLocationDetailDialog)
	enum { IDD = IDD_LOCATIONDETAILDIALOG };
	CButton	m_ctlOK;
	CString	m_sLocationName;
	CString	m_sLocalInstructions;
	//}}AFX_DATA
    int m_iLocationID;

    CString GetLocationName() { return m_sLocationName; } ;
    void SetLocationID(int iLocationID) { m_iLocationID = iLocationID; };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocationDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLocationDetailDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLocationname();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCATIONDETAILDIALOG_H__C2791387_3798_4396_A3D8_7B04F46E032F__INCLUDED_)
