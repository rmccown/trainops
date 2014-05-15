#if !defined(AFX_TRAINSDIALOG_H__0E0F1AE5_41A8_4F2F_BB04_AAE3AF4C5F42__INCLUDED_)
#define AFX_TRAINSDIALOG_H__0E0F1AE5_41A8_4F2F_BB04_AAE3AF4C5F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrainsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainsDialog dialog

class CTrainsDialog : public CDialog
{
// Construction
public:
	CTrainsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrainsDialog)
	enum { IDD = IDD_TRAINSDIALOG };
	CButton	m_ctlNewTrain;
	CButton	m_ctlEditTrain;
	CButton	m_ctlDeleteTrain;
	CTreeCtrl	m_ctlTrains;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();

	// Generated message map functions
	//{{AFX_MSG(CTrainsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditTrain();
	afx_msg void OnDeleteTrain();
	afx_msg void OnNewTrain();
	afx_msg void OnClickTrains(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTrains(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINSDIALOG_H__0E0F1AE5_41A8_4F2F_BB04_AAE3AF4C5F42__INCLUDED_)
