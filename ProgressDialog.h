#if !defined(AFX_PROGRESSDIALOG_H__45A7D9FA_5992_4662_919A_93708E8F956B__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__45A7D9FA_5992_4662_919A_93708E8F956B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

class CProgressDialog : public CDialog
{
// Construction
public:
	CProgressDialog(CWnd* pParent = NULL);   // standard constructor

    BOOL Create(UINT nID, CWnd * pWnd)
                      { return CDialog::Create(nID,pWnd);}

    void OnOK();
    void OnCancel();

// Dialog Data
	//{{AFX_DATA(CProgressDialog)
	enum { IDD = IDD_PROGRESSDIALOG };
	CProgressCtrl	m_ctlProgress;
	CString	m_sStatusText;
	//}}AFX_DATA
    int m_iCount;
    BOOL m_bUp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDIALOG_H__45A7D9FA_5992_4662_919A_93708E8F956B__INCLUDED_)
