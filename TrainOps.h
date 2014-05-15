// TrainOps.h : main header file for the TRAINOPS application
//

#if !defined(AFX_TRAINOPS_H__6B58DCA8_BD9C_4CED_8F6D_ABF9A62050D8__INCLUDED_)
#define AFX_TRAINOPS_H__6B58DCA8_BD9C_4CED_8F6D_ABF9A62050D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SQLite3.h"
#include "CppSQLite3.h"
#include "SplashScreenEx.h"

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsApp:
// See TrainOps.cpp for the implementation of this class
//

class CTrainOpsApp : public CWinApp
{
public:
    CTrainOpsApp();
    ~CTrainOpsApp();
    CppSQLite3DB m_pDB;
    void SetTitle();

protected:
    LPVOID m_lpvState;
    
    CString GetProgramPath();
    
private:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTrainOpsApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL
    
    // Implementation
    
    void VersionCheck();
    
    void OpenDatabase();
    void CloseDatabase();
    BOOL StartupProcessing();
    void RunCommands(CppSQLite3DB* db);
    void CheckTables(CppSQLite3DB* db);
    //
    void VerifyLocationTablesData(CppSQLite3DB* pDB);
    void VerifySidingTablesData(CppSQLite3DB* db);
    void VerifyIndustryTablesData(CppSQLite3DB* db);
    void VerifyCarTablesData(CppSQLite3DB* db);
    //
    void VerifyConfigData(CppSQLite3DB* db);
    //
    void CreateTable(CString sTableName,CppSQLite3DB* db);
    void UpdateTable(CString sTableName,CppSQLite3DB* db);
    //
    void FillCarTypesData(CppSQLite3DB* db);
    void SetupConfig(CppSQLite3DB* db);
    //
    int GetDate();
    CString GetDateString();
    CString GetIndustryRatios(int iIndustryFK,int iSidingFK,int* iRatio);

    CSplashScreenEx *m_pSplash;
    
    //{{AFX_MSG(CTrainOpsApp)

	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINOPS_H__6B58DCA8_BD9C_4CED_8F6D_ABF9A62050D8__INCLUDED_)
