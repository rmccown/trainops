#if !defined(AFX_TRAINDETAILDIALOG_H__DF0DBBBE_4023_4444_AB8E_F3B5E41C8584__INCLUDED_)
#define AFX_TRAINDETAILDIALOG_H__DF0DBBBE_4023_4444_AB8E_F3B5E41C8584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrainDetailDialog.h : header file
//

#include <Afxwin.h>
#include <Afxtempl.h>

#include "RunTrainsDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTrainDetailDialog dialog

class CTrainDetailDialog : public CDialog
{
// Construction
public:
	CTrainDetailDialog(CWnd* pParent = NULL);   // standard constructor

    void SetTrainInfo(int iTrainFK, int iScheduledTrainFK=0) { m_iTrainFK = iTrainFK; m_iScheduledTrainFK = iScheduledTrainFK;} ;
// Dialog Data
	//{{AFX_DATA(CTrainDetailDialog)
	enum { IDD = IDD_TRAINDETAILDIALOG };
	CAMSAlphanumericEdit	m_ctlTrainNotes;
	CButton	m_ctlOK;
	CButton	m_ctlIgnoreRestrictions;
	CComboBox	m_ctlLocomotiveList;
	CComboBox	m_ctlCabooseList;
	CTreeCtrl	m_ctlTrainDetail;
	BOOL	m_bIgnoreRestrictions;
	CString	m_sTrainNotes;
	//}}AFX_DATA

    int GetLocomotive();
    int GetCaboose();
    int GetIndustrySidingCapacity(int iIndustry);
    int GetSidingCapacity(int iLocation);
    int GetMaxMoves(int iLocationFK);

    CString GetTrainNotes() { return m_sTrainNotes; } ;
    HTREEITEM GetLastLocation();
    int GetSiding(int iLocationFK);

    void CleanupTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    int m_iTrainFK;
    int m_iScheduledTrainFK;
    int m_iLocomotive;
    int m_iCaboose;

    void ValidateTrain();
    int GetSelectedLocoCapacity();

	// Generated message map functions
	//{{AFX_MSG(CTrainDetailDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTraindetail(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnIgnorerestrictions();
	afx_msg void OnSelChangeLocomotiveList();
	virtual void OnCancel();
	//}}AFX_MSG

    afx_msg LRESULT OnCheckStateChanged(WPARAM pParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINDETAILDIALOG_H__DF0DBBBE_4023_4444_AB8E_F3B5E41C8584__INCLUDED_)
