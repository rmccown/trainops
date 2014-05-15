// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__FF9A1C27_7994_4317_A537_79FE6F32504B__INCLUDED_)
#define AFX_MAINFRM_H__FF9A1C27_7994_4317_A537_79FE6F32504B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MenuXP.h"
#include "StatusbarXP.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

    DECLARE_MENUXP()
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBarXP  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;

// Generated message map functions
public:
    afx_msg void OnFilePreferences();
    afx_msg void OnAppAbout();
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnConfigureCartypes();
    afx_msg void OnConfigureCars();
    afx_msg void OnConfigureLocations();
    afx_msg void OnConfigureSidings();
    afx_msg void OnConfigureIndustries();
    afx_msg void OnConfigureCommodities();
    afx_msg void OnConfigureLocomotives();
    afx_msg void OnTrains();
    afx_msg void OnReportsCarlocation();
    afx_msg void OnRuntrainsRuntrains();
    afx_msg void OnConfigureCaboose();
    afx_msg void OnReportsCarsnotfound();
    afx_msg void OnReportsCarlocationreportsCarlocationtrains();
    afx_msg void OnReportsCarLocationSorted();
    afx_msg void OnConfigurePhysicallayout();
    afx_msg void OnReportsCarlocationreportsCarlocationsortedall();
    afx_msg void OnHelpTopics();
    afx_msg void OnHelpRegister();
    afx_msg void OnHelpSendDB();
	afx_msg void OnReportsEquipmentlocation();
	afx_msg void OnUpdateConfigureCommodities(CCmdUI* pCmdUI);
	afx_msg void OnReportsEquipmentTracking();
	afx_msg void OnReportsIndustryusage();
	afx_msg void OnHelpReleaseNotes();
	afx_msg void OnReportsCarsNeeded();
	afx_msg void OnHelpSupportOpendatadirectory();
	afx_msg void OnReportsTrainBuildingForm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__FF9A1C27_7994_4317_A537_79FE6F32504B__INCLUDED_)
