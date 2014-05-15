#if !defined(AFX_NEWVERSIONDIALOG_H__0A2E02D6_982F_4E3D_B623_99D53085E4F0__INCLUDED_)
#define AFX_NEWVERSIONDIALOG_H__0A2E02D6_982F_4E3D_B623_99D53085E4F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewVersionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewVersionDialog dialog

class CNewVersionDialog : public CDialog
{
// Construction
public:
	CNewVersionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewVersionDialog)
	enum { IDD = IDD_NEWVERSIONDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewVersionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewVersionDialog)
	afx_msg void OnViewreadmebutton();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWVERSIONDIALOG_H__0A2E02D6_982F_4E3D_B623_99D53085E4F0__INCLUDED_)
