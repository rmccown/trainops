// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TrainOps.h"

#include "MainFrm.h"

#include "AboutDialog.h"

#include "PreferencesDialog.h"

#include "LocationDialog.h"
#include "SidingsDialog.h"
#include "IndustriesDialog.h"
#include "PhysicalLayoutDialog.h"

#include "CabooseDialog.h"
#include "CarTypesDialog.h"
#include "CommoditiesDialog.h"
#include "CarsDialog.h"
#include "CarLocationTrainsDialog.h"
#include "EquipmentNotFoundDialog.h"
#include "EquipmentLocationDialog.h"
#include "EquipmentTrackingDialog.h"
#include "IndustryUsageDialog.h"
#include "IndustryCarOrdersDialog.h"

#include "LocomotiveDialog.h"
#include "RunTrainsDialog.h"
#include "TrainsDialog.h"

#include "Reports.h"

#include "SendFileTo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

IMPLEMENT_MENUXP(CMainFrame, CFrameWnd);

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_MENUXP_MESSAGES()
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_CONFIGURE_CARTYPES, OnConfigureCartypes)
	ON_COMMAND(ID_CONFIGURE_CARS, OnConfigureCars)
	ON_COMMAND(ID_CONFIGURE_LOCATIONS, OnConfigureLocations)
	ON_COMMAND(ID_CONFIGURE_SIDINGS, OnConfigureSidings)
	ON_COMMAND(ID_CONFIGURE_INDUSTRIES, OnConfigureIndustries)
	ON_COMMAND(ID_CONFIGURE_COMMODITIES, OnConfigureCommodities)
	ON_COMMAND(ID_CONFIGURE_LOCOMOTIVES, OnConfigureLocomotives)
	ON_COMMAND(IDM_TRAINS, OnTrains)
	ON_COMMAND(ID_REPORTS_CARLOCATION, OnReportsCarlocation)
	ON_COMMAND(ID_RUNTRAINS_RUNTRAINS, OnRuntrainsRuntrains)
	ON_COMMAND(ID_CONFIGURE_CABOOSE, OnConfigureCaboose)
	ON_COMMAND(ID_REPORTS_CARSNOTFOUND, OnReportsCarsnotfound)
	ON_COMMAND(ID_REPORTS_CARLOCATIONREPORTS_CARLOCATIONTRAINS, OnReportsCarlocationreportsCarlocationtrains)
	ON_COMMAND(ID_CONFIGURE_PHYSICALLAYOUT, OnConfigurePhysicallayout)
	ON_COMMAND(ID_REPORTS_CARLOCATIONREPORTS_CARLOCATIONSORTEDALL, OnReportsCarlocationreportsCarlocationsortedall)
	ON_COMMAND(IDM_HELP_TOPICS, OnHelpTopics)
	ON_COMMAND(IDM_HELP_REGISTER, OnHelpRegister)
	ON_COMMAND(ID_REPORTS_EQUIPMENTLOCATION, OnReportsEquipmentlocation)
	ON_UPDATE_COMMAND_UI(ID_CONFIGURE_COMMODITIES, OnUpdateConfigureCommodities)
	ON_COMMAND(ID_REPORTS_INDUSTRYUSAGE, OnReportsIndustryusage)
	ON_COMMAND(ID_HELP_RELEASENOTES, OnHelpReleaseNotes)
	ON_COMMAND(ID_REPORTS_CARSNEEDED, OnReportsCarsNeeded)
	ON_COMMAND(IDM_HELP_SUPPORT_OPENDATADIRECTORY, OnHelpSupportOpendatadirectory)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PREFERENCES, OnFilePreferences)
	ON_COMMAND(ID_REPORTS_TRAINBUILDINGFORM, OnReportsTrainBuildingForm)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(IDM_HELP_SENDDB, OnHelpSendDB)
//	ON_COMMAND(ID_REPORTS_CARSNOTFOUND, OnReportsCarsnotfound)
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_REPORTS_EQUIPMENTTRACKING, OnReportsEquipmentTracking)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    CMenuXP::SetXPLookNFeel (this);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    //
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

    CRect pWndRect,pDesktopRect;
    CPoint pPoint;
    GetWindowRect(pWndRect);
    int nWidth = pWndRect.Width();
    int nHeight = pWndRect.Height();
    GetDesktopWindow()->GetWindowRect(pDesktopRect);
    pPoint.x = pDesktopRect.Width() / 2;
    pPoint.y = pDesktopRect.Height() / 2;
    GetDesktopWindow()->ClientToScreen(&pPoint);
    pPoint.x -= nWidth / 2;
    pPoint.y -= nHeight / 2;
    //
    MoveWindow(pPoint.x,pPoint.y,pWndRect.Width(),pWndRect.Height(),false);
    //
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnAppAbout()
{
    CAboutDialog aboutDlg;
	if( aboutDlg.DoModal() == IDC_RESTART )
    {
        if (!RA_ActivateRestartProcess())
        {
            // Handle restart error here
            return;
        }
        exit(0);
    }
}

void CMainFrame::OnFilePreferences() 
{
    CPreferencesDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureLocations() 
{
    CLocationDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureSidings() 
{
    CSidingsDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureIndustries() 
{
    CIndustriesDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureCartypes() 
{
    CCarTypesDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureCommodities() 
{
    CCommoditiesDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureCars() 
{
    CCarsDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureLocomotives() 
{
    CLocomotiveDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigureCaboose() 
{
    CCabooseDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnTrains() 
{
    CTrainsDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnRuntrainsRuntrains() 
{
    ((CTrainOpsApp*)AfxGetApp())->VerifyCarTablesData(&((CTrainOpsApp*)AfxGetApp())->m_pDB);
    //
    CRunTrainsDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnReportsCarlocation() 
{
    DoCarLocationReport();
}


void CMainFrame::OnReportsCarlocationreportsCarlocationsortedall() 
{
    DoCarLocationSortedReport();
}

void CMainFrame::OnReportsCarsnotfound() 
{
    CEquipmentNotFoundDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnReportsCarlocationreportsCarlocationtrains() 
{
    CCarLocationTrainsDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnConfigurePhysicallayout() 
{
    CPhysicalLayoutDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnHelpTopics() 
{
    AfxGetApp()->WinHelp(0,HELP_CONTEXT);
}

void CMainFrame::OnHelpRegister() 
{
    ShellExecute(NULL, "open", "http://www.largescalecentral.com/TrainOps", NULL, NULL, SW_SHOWNORMAL); 
}

void CMainFrame::OnHelpSendDB() 
{
    if( MessageBox("This will send a copy of your database, via your email client,\nto TrainOps support.  Are you sure you want to do this?","TrainOps",MB_YESNO) != IDYES )
    {
        return;
    }
    //
    ITEMIDLIST* pidl;
    char szAppDir[MAX_PATH];;
    memset(&szAppDir,0,sizeof(szAppDir));
    HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA , &pidl );
    if (hRes==NOERROR)
    {
        SHGetPathFromIDList( pidl, szAppDir );
        strcat(szAppDir,"\\TrainOps\\operations.db3");
        //
        char szDesktop[MAX_PATH];;
        memset(&szDesktop,0,sizeof(szDesktop));
        HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOPDIRECTORY , &pidl );
        if (hRes==NOERROR)
        {
            SHGetPathFromIDList( pidl, szDesktop );
            strcat(szDesktop,"\\operations.db3");
            //
            CopyFile(szAppDir,szDesktop,FALSE);
        }
        //
        try {
            CSendFileTo sendTo( m_hWnd );
            sendTo.SendMail(_T(szAppDir), 
                _T("TrainOps database attached"),
                _T("\nA TrainOps database is attached to this message."),
                _T("rmccown@zbd.com")
                );
        }
        catch (...)
        {
            CString sErr("No database located at ");
            sErr += szAppDir;
            MessageBox(sErr,"ERROR",MB_OK);
            exit(-1);
        }
    }
    else
    {
        MessageBox("Unable to locate database directory","ERROR",MB_OK);
        exit(-1);
    }
    MessageBox("Your email client should be open, with the db file as an attachment.\nPlease describe the issue you are having, and then send the email.\n\nIf your email client is not open, a copy of your db has been placed on your desktop.\nPlease attach it to an email to rmccown@zbd.com.");
}

void CMainFrame::OnReportsEquipmentlocation() 
{
    CEquipmentLocationDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnUpdateConfigureCommodities(CCmdUI* pCmdUI) 
{
    if( g_iMethod == METHOD_COMMODITIES	)
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
    //
	if( !CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
        return FALSE;

    CMenuXP::UpdateMenuBar (this);

    return TRUE;
}

void CMainFrame::OnReportsEquipmentTracking() 
{
    CEquipmentTrackingDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnReportsIndustryusage() 
{
    CIndustryUsageDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnHelpReleaseNotes() 
{
	// TODO: Add your command handler code here
    ITEMIDLIST* pidl;
    char szAppDir[MAX_PATH];;
    memset(&szAppDir,0,sizeof(szAppDir));
    HRESULT hRes = SHGetSpecialFolderLocation( NULL, 0x0026 , &pidl );
    if (hRes==NOERROR)
    {
        SHGetPathFromIDList( pidl, szAppDir );
        strcat(szAppDir,"\\LargeScaleCentral\\TrainOps\\releasenotes.txt");
        CString sStart;
        sStart.Format("notepad %s",szAppDir);
    	WinExec(sStart,SW_SHOW);
    }
}

void CMainFrame::OnReportsCarsNeeded() 
{
    CIndustryCarOrdersDialog pDlg;	
    pDlg.DoModal();
}

void CMainFrame::OnHelpSupportOpendatadirectory() 
{
    ITEMIDLIST* pidl;
    char szAppDir[MAX_PATH];;
    memset(&szAppDir,0,sizeof(szAppDir));
    HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA , &pidl );
    if (hRes==NOERROR)
    {
        SHGetPathFromIDList( pidl, szAppDir );
        strcat(szAppDir,"\\TrainOps");
        CString sStart;
        sStart.Format("explorer %s",szAppDir);
    	WinExec(sStart,SW_SHOW);
    }	
}

void CMainFrame::OnReportsTrainBuildingForm() 
{
    DoTrainBuildingForm();
}
