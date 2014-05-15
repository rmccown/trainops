//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_PDFVIEWERDIALOG_H__1461A523_2253_475C_B8EA_ACED832E17FF__INCLUDED_)
#define AFX_PDFVIEWERDIALOG_H__1461A523_2253_475C_B8EA_ACED832E17FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PDFViewerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPDFViewerDialog dialog

class CPDFViewerDialog : public CDialog
{
// Construction
public:
	CPDFViewerDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPDFViewerDialog)
	enum { IDD = IDD_PDFVIEW_DIALOG };
	//}}AFX_DATA

    void SetFile(CString sFileName) { m_sFileName = sFileName; };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPDFViewerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString m_sFileName;
	// Generated message map functions
	//{{AFX_MSG(CPDFViewerDialog)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDFVIEWERDIALOG_H__1461A523_2253_475C_B8EA_ACED832E17FF__INCLUDED_)
