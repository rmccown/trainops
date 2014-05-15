// TrainOps.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TrainOps.h"

#include "_BldNum.h"

#include "MainFrm.h"
#include "TrainOpsDoc.h"
#include "TrainOpsView.h"

#include "IGDebugDialog.h"

#include "inputbox.h"
#include "FolderDlg.h"
#include "XMLite.h"

#include "NewVersionDialog.h"

#include "julian.h"

#include <iostream>
#include <shlobj.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib") 

//extern IID iid_IActiveLockEventSink;

//
//  Its a global so the TRACE macro can find it
//
CIGDebugDialog* m_pDebugDialog;

/*
#ifdef TRACE
#undef TRACE

#define TRACEMAXSTRING	1024

inline void TRACE(const char* format,...)
{
    char szBuffer[TRACEMAXSTRING];
    va_list args;
    va_start(args,format);
    int nBuf;
    nBuf = _vsnprintf(szBuffer,
        TRACEMAXSTRING,
        format,
        args);
    va_end(args);
    
    _RPT0(_CRT_WARN,szBuffer);
    m_pDebugDialog->AddDebug(szBuffer);
}

#endif
  */
//
//  crash reporter
//

#ifndef _CRASH_RPT_
#include "../crashrpt/crashrpt/src/crashrpt.h"
#pragma comment(lib, "../crashrpt/crashrpt/lib/crashrpt")
#endif // _CRASH_RPT_

BOOL WINAPI CrashCallback(LPVOID lpvState)
{
    char szAppDataDir[MAX_PATH];;
    char szDB[MAX_PATH];;
    char szLic[MAX_PATH];;
    ITEMIDLIST* pidl;
    //
    memset(&szAppDataDir,0,sizeof(szAppDataDir));
    HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA , &pidl );
    if (hRes==NOERROR)
    {
        SHGetPathFromIDList( pidl, szDB );
        SHGetPathFromIDList( pidl, szLic );
        strcat(szDB,"\\TrainOps");
        strcat(szDB,"\\operations.db3");
        AddFile(lpvState, szAppDataDir, "Database");
        strcat(szLic,"\\TrainOps");
        strcat(szLic,"\\TrainOps.lic");
        AddFile(lpvState, szLic, "License Information");
    }
    return TRUE;
}
//

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsApp

BEGIN_MESSAGE_MAP(CTrainOpsApp, CWinApp)
	//{{AFX_MSG_MAP(CTrainOpsApp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsApp construction

CTrainOpsApp::CTrainOpsApp()
{
#ifdef _DEBUG
    char szWork[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,szWork);
    g_sSchemaPath.Format("%s/dbschema.xml",szWork);
#else
    g_sSchemaPath = "./dbschema.xml";
#endif
}

CTrainOpsApp::~CTrainOpsApp()
{
	m_pDB.close();
    if( m_pDebugDialog )
        delete m_pDebugDialog;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CTrainOpsApp object

CTrainOpsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsApp initialization

BOOL CTrainOpsApp::InitInstance()
{
    //
    //  singleton mutex for installer
    //
    CreateMutex(NULL, FALSE, "TrainOpsMutexName");
    //
    try {
        InitCommonControls();
        //
        CMenuXP::InitializeHook();
        //
       
        srand(time(NULL));
        
        TBTreeStateMgr::Initialize();
        //
        char szAppPath[255];
        ITEMIDLIST* pidl;
        HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA , &pidl );
        if (hRes==NOERROR)
        {
            SHGetPathFromIDList( pidl, szAppPath );
            strcat(szAppPath,"\\TrainOps\\TrainOps.ctss");
            TBTreeStateMgr::SetStorageFile(szAppPath);
        }
        else
            TBTreeStateMgr::SetStorageFile("TrainOps.ctss");

        
        m_lpvState = Install(CrashCallback, _T("rmccown@zbd.com"), _T("TrainOps Crash Report"));
        
        // Standard initialization
        // If you are not using these features and wish to reduce the size
        //  of your final executable, you should remove from the following
        //  the specific initialization routines you do not need.
        AfxEnableControlContainer();

#ifdef _AFXDLL
        Enable3dControls();			// Call this when using MFC in a shared DLL
#else
        Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
        if (RA_CheckForRestartProcessStart())
            RA_WaitForPreviousProcessFinish();
        //
        SetRegistryKey(_T("LargeScaleCentral"));
        //
        m_pDebugDialog = new CIGDebugDialog;
        m_pDebugDialog->Create(IDD_DEBUGDIALOG);
#ifndef _DEBUG
        m_pDebugDialog->ShowWindow(((CTrainOpsApp*)AfxGetApp())->GetProfileInt("Settings","DEBUG",0)==1?SW_SHOW:SW_HIDE);
#else
        //m_pDebugDialog->ShowWindow(SW_SHOW);
#endif
        // Change the registry key under which our settings are stored.
        
        LoadStdProfileSettings();  // Load standard INI file options (including MRU)
        
        // Register the application's document templates.  Document templates
        //  serve as the connection between documents, frame windows and views.
        
        
        CSingleDocTemplate* pDocTemplate;
        pDocTemplate = new CSingleDocTemplate(
            IDR_MAINFRAME,
            RUNTIME_CLASS(CTrainOpsDoc),
            RUNTIME_CLASS(CMainFrame),       // main SDI frame window
            RUNTIME_CLASS(CTrainOpsView));
        AddDocTemplate(pDocTemplate);
        
        // Parse command line for standard shell commands, DDE, file open
        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);
        
        // Dispatch commands specified on the command line
        if (!ProcessShellCommand(cmdInfo))
            return FALSE;

        VersionCheck();
        
        //	TRACE("SQLite Version: %s\n",m_pDB.SQLiteVersion());
        OpenDatabase();
        //
        /*
        ITEMIDLIST* pidl;
        char szDesktop[MAX_PATH];;
        memset(&szDesktop,0,sizeof(szDesktop));
        HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP , &pidl );
        if (hRes==NOERROR)
        {
            SHGetPathFromIDList( pidl, szDesktop );
            g_sDesktop = szDesktop;
        }
        */
        //
        //  see if we have a db schema in the current directory?
        //
        if( FileExists("./dbschema.xml") )
            g_sSchemaPath = "./dbschema.xml";
        //
        m_pMainWnd->ShowWindow(SW_SHOW);
        m_pMainWnd->UpdateWindow();
        //
        m_pSplash = new CSplashScreenEx();
        m_pSplash->Create(m_pMainWnd,NULL,0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
        m_pSplash->SetBitmap(IDB_SPLASH,255,0,255);
        m_pSplash->SetTextRect(CRect(50,52,291,150));
        m_pSplash->SetTextColor(RGB(0,0,0));
        m_pSplash->SetTextFormat(DT_SINGLELINE | DT_LEFT | DT_VCENTER);
        m_pSplash->Show();
        //
        //  run commands
        //
        if( !StartupProcessing() )
            return FALSE;
        //
        //  load defaults
        //
        CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
        CppSQLite3Query q = pDB->execQuery("SELECT * FROM _Config WHERE id=1;");
        //
        m_pSplash->SetText("Ready");
        m_pSplash->Hide();
        //
        if (q.eof() || (q.getIntField("id") == 1 && q.getIntField("rrdate") == -1))
        {
            q.finalize();
            ((CMainFrame*)AfxGetMainWnd())->OnFilePreferences();
        }
        else
        {
            g_sRRName = q.getStringField("rrname");
            g_sReportingMarks = q.getStringField("reporting_marks");
            g_iLoadEmptyDays = q.getIntField("LoadEmptyDays");
            g_iCarLength = q.getIntField("car_length");
            g_iMethod = q.getIntField("car_method");
            g_iOffLayoutTime = q.getIntField("offlayouttime");
            g_sLogo = q.getStringField("logo");
            g_bRealisticReports = q.getIntField("realistic_reports")==1?TRUE:FALSE;
            q.finalize();
        }
        //
        SetTitle();
        //
        //  delete the dbschema.xml file
        //
#ifndef _DEBUG
        CString sSchemaFile(g_sSchemaPath);
        //sSchemaFile += "\\dbschema.xml";
        if( FileExists((LPCTSTR)sSchemaFile) )
        {
            //
            //  delete schema to Recycle Bin
            //
            try {
                SHFILEOPSTRUCT shFileOp;
                char szFile[MAX_PATH];
                memset(&shFileOp,0,sizeof(shFileOp));
                memset(&szFile,0,sizeof(szFile));
                strcpy(szFile,(LPCTSTR)sSchemaFile);
                shFileOp.fFlags = FOF_SILENT|FOF_NOCONFIRMATION|FOF_ALLOWUNDO;
                shFileOp.wFunc = FO_DELETE;
                shFileOp.pFrom = szFile;
                SHFileOperation(&shFileOp);
            }
            catch(...)  //  Catch for Vista?  SHFileOperation broken in Vista
            {
                //unlink((LPCTSTR)sSchemaFile);
            }
            //
            MessageBox(0,"A database update has been applied","TrainOps Update Engine",MB_ICONINFORMATION);
        }
#endif
    }
    catch (CException* e)
    {
        e->ReportError();
        m_pSplash->SetText("Ready");
        m_pSplash->Hide();
    }

    return TRUE;
}

void CTrainOpsApp::SetTitle()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    char szDate[24];
    if( !q.eof() )
    {
        int y,m,d;
        JulianDate::JdToYmd( q.getIntField(0)+q.getIntField(1), &y,&m,&d);
        sprintf(szDate,"%d/%d/%d",m,d,y);
    }
    q.finalize();
    //
    CString sTitle;
    sTitle.Format("TrainOps - %s (%s) ",g_sRRName,szDate);
    m_pMainWnd->SetWindowText(sTitle);
}

void CTrainOpsApp::OpenDatabase()
{
    ITEMIDLIST* pidl;
    char szAppDir[MAX_PATH];;
    memset(&szAppDir,0,sizeof(szAppDir));
    HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA , &pidl );
    if (hRes==NOERROR)
    {
        SHGetPathFromIDList( pidl, szAppDir );
        strcat(szAppDir,"\\TrainOps");
        CreateDirectory(szAppDir,NULL); //  make the directory if it doesnt already exist
        //
        //  if we're debugging, pick what DB we want
        //
#if defined(_DEBUG)
        CFileDialog pDlg(TRUE,NULL,"operations.db3",OFN_READONLY,"TrainOps Database Files (*.db3)||");
        pDlg.m_ofn.lpstrTitle = "Open Database";
        pDlg.m_ofn.lpstrInitialDir = szAppDir;
        int iRet = pDlg.DoModal();
        if( iRet == IDCANCEL )
            exit(-1);
        strcpy(szAppDir,pDlg.GetPathName());
#else
        strcat(szAppDir,"\\operations.db3");
#endif
        try {
            m_pDB.open(szAppDir);
        }
        catch (...)
        {
            CString sErr("No database located at ");
            sErr += szAppDir;
            MessageBox(0,sErr,"ERROR",MB_OK);
            exit(-1);
        }
    }
    else
    {
        MessageBox(0,"Unable to locate database directory","ERROR",MB_OK);
        exit(-1);
    }
}

void CTrainOpsApp::CloseDatabase()
{
    m_pDB.close();
}

BOOL CTrainOpsApp::StartupProcessing()
{
    //
    //  check database integrity
    //
    m_pSplash->SetText("Checking Updates");
    RunCommands(&m_pDB);
    m_pSplash->SetText("Checking Database");
    CheckTables(&m_pDB);
    //
    m_pSplash->SetText("Verifying Data Integrity");
    VerifyLocationTablesData(&m_pDB);
    VerifySidingTablesData(&m_pDB);
    VerifyIndustryTablesData(&m_pDB);
    //VerifyCarTablesData(&m_pDB);
    //
    m_pSplash->SetText("Verifying Config Data");
    VerifyConfigData(&m_pDB);
    //
    return TRUE;
}
/*
BOOL CTrainOpsApp::CheckLicenseCRC(int debug)
{
    if(debug) return TRUE;
    VectorNameAndCrc vNameCrc;
    DWORD activelock3Crc			= 603433813;
    DWORD alcryptoCrc         = 2581601961;
    
    char windowsSystem[255];
    {
        GetSystemDirectory(windowsSystem,255);
        struct CNameAndCrc cnac;
        cnac.crc = activelock3Crc;
        cnac.msg = "Possible wrong version of activelock3.dll";
        cnac.name =  windowsSystem;
        cnac.name += CString("\\activelock3.5.dll");
        vNameCrc.push_back(cnac);
    }
    
    {
        struct CNameAndCrc cnac;
        cnac.crc = alcryptoCrc;
        cnac.msg = "Possible wrong version of alcrypto3.dll";
        cnac.name =  windowsSystem;
        cnac.name += CString("\\alcrypto3.dll");
        vNameCrc.push_back(cnac);
    }
    return util.CheckAllCrcs(debug, vNameCrc);
}
*/
CString CTrainOpsApp::GetProgramPath() 
{ 
    // the exe's directory
    CString strPath("");
    {
        char cPath[999];  cPath[0] = '\0';
        GetModuleFileName(NULL, cPath, 999);
        strPath = cPath;
    }
    int index = strPath.ReverseFind('\\'); // find last index of '\'
    if (index > 0) {
        strPath = strPath.Left(index);
    }
    return strPath;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Database routines/checking/creating/etc
 *
 *  SQLITE_MASTER table contains all table names, etc.
 *
 *  select sql from SQLITE_MASTER WHERE type="table" AND name="Cars"; returns SQL to create the table
 */
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//
//  RunCommands.  Run entries in the <Commands> element
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::RunCommands(CppSQLite3DB* db)
{
    try
    {
        CString sFile(g_sSchemaPath);
        //sFile += "\\dbschema.xml";
        CFile myFile(sFile, CFile::modeRead);
        char pXML[32766];
        DWORD dwRead = myFile.Read(&pXML, sizeof(pXML));
        myFile.Close();
    	XNode xml;
		xml.Load( pXML );
        //
        XNodes childs;
        childs = xml.GetChilds("Commands");
    	XNode xml2;
        try
        {
		    xml2.Load( childs[0]->GetXML() );
        }
        catch(...)
        {
            return;
        }
	    LPXNode child;
        //
	    for( int i = 0 ; i < xml2.GetChildCount(); i++)
        {
	    	child = xml2.GetChild(i);
        	//CString sField = child->GetAttrValue(_T("name"));
        	CString sCmd = child->GetAttrValue(_T("type"));
            try { db->execDML(sCmd); } catch(...){}
        }
    }
    catch(CFileException* pe)
    {
        pe->Delete();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  CheckTables - Verify that every table entry is in the database,
//                If it isnt, call CreateTable
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::CheckTables(CppSQLite3DB* db)
{
    try
    {
        CString sFile(g_sSchemaPath);
        //sFile += "\\dbschema.xml";
        CFile myFile(sFile, CFile::modeRead);
        char pXML[32766];
        DWORD dwRead = myFile.Read(&pXML, sizeof(pXML));
        myFile.Close();
        //
    	XNode xml;
		xml.Load( pXML );
        LPXNode child;
    	for( int i = 0 ; i < xml.GetChildCount(); i++)
	    {
		    child = xml.GetChild(i);
            //
            //  child is now the entire table tag.
            //
            CString sTest = child->name;
            m_pSplash->SetText(sTest);
            if( sTest == "Commands" )
                continue;
           	CString sTableName = child->GetAttrValue(_T("name"));
        	if( !m_pDB.tableExists(sTableName) )
            {
                m_pSplash->SetText("Creating Table "+sTableName);
                CreateTable(sTableName,&m_pDB);
                //MessageBox(0,sTableName,"CreateTable",MB_OK);
                if( sTableName == "CarTypes" )
                    FillCarTypesData(&m_pDB);
            }
            m_pSplash->SetText("Updating Table "+sTableName);
            UpdateTable(sTableName,db);
	    }
    }
    catch(CFileException* pe)
    {
        //char szMsg[256];
        //pe->GetErrorMessage(szMsg, sizeof(szMsg));
        //MessageBox(NULL, szMsg, "TrainOps ERROR", MB_ICONHAND);
        pe->Delete();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  CreateTable - Given sTableName, create the table in the database
//                using all entries in the XML file for that table
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::CreateTable(CString sTableName,CppSQLite3DB* db)
{
    try
    {
        CString sFile(g_sSchemaPath);
        //sFile += "\\dbschema.xml";
        CFile myFile(sFile, CFile::modeRead);
        char pXML[32766];
        DWORD dwRead = myFile.Read(&pXML, sizeof(pXML));
        myFile.Close();
    	XNode xml;
		xml.Load( pXML );
        XNodes childs;
        childs = xml.GetChilds("table");
        //
        for( int i = 0 ; i < childs.size(); i++)
        {
            XNode xml2;
            xml2.Load( childs[i]->GetXML() );
            if( sTableName == xml2.GetAttrValue(_T("name")) )
            {
                //
                XNode xml2;
                xml2.Load( childs[i]->GetXML() );
                LPXNode child;
                CString sSQL;
                sSQL.Format("CREATE TABLE %s (",sTableName);
                //
                for( int ii = 0 ; ii < xml2.GetChildCount(); ii++)
                {
                    CString sAdd;
                    child = xml2.GetChild(ii);
                    CString sField = child->GetAttrValue(_T("name"));
                    CString sType = child->GetAttrValue(_T("type"));
                    sAdd.Format("%s%s %s",ii==0?"":",",sField,sType);
                    sSQL += sAdd;
                }
                sSQL += ");";
                db->execDML(sSQL);
                //MessageBox(0,sSQL,"DEBUG",MB_OK);
            }
        }
    }
    catch(CFileException* pe)
    {
        //char szMsg[256];
        //pe->GetErrorMessage(szMsg, sizeof(szMsg));
        //MessageBox(NULL, szMsg, "TrainOps ERROR", MB_ICONHAND);
        pe->Delete();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  UpdateTable - Verify each table schema against the XML file (if present).
//                Add any fields if they are needed.
//
//                Currently we do NOT delete any fields NOT present in the XML file.
//                How can we add this?
//                Re-create table?
//                  ALTER TABLE Cars RENAME TO old_Cars, then:
//                  CREATE cars AS SELECT <fields> FROM old_Cars
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::UpdateTable(CString sTableName,CppSQLite3DB* db)
{
    try
    {
        CString sFile(g_sSchemaPath);
        //sFile += "\\dbschema.xml";
        CFile myFile(sFile, CFile::modeRead);
        char pXML[32766];
        DWORD dwRead = myFile.Read(&pXML, sizeof(pXML));
        myFile.Close();
    	XNode xml;
		xml.Load( pXML );
        XNodes childs,childs2;
        childs = xml.GetChilds("table");
        // find sTableName in childs
        for( int i = 0 ; i < childs.size(); i++)
        {
        	XNode xml2;
	    	xml2.Load( childs[i]->GetXML() );
            if( sTableName == xml2.GetAttrValue(_T("name")) )
            {
                //
                XNode xml3;
                xml3.Load( childs[i]->GetXML() );
                LPXNode child;
                //
                //  what about where there are no records?  Everything seems to fail
                //
                CString sSQL;
                sSQL.Format("SELECT sql FROM SQLITE_MASTER WHERE type=\"table\" AND name=\"%s\";",sTableName);
                CppSQLite3Query q = db->execQuery(sSQL);
                CString sBuildSQL = q.fieldValue(0);
                q.finalize();
                //
                //  adding any fields?
                //
                CString sNewFields;
                for( int ii = 0 ; ii < xml3.GetChildCount(); ii++)
                {
                    child = xml3.GetChild(ii);
                    CString sField = child->GetAttrValue(_T("name"));
                    CString sType = child->GetAttrValue(_T("type"));
                    sNewFields += sField;
                    sNewFields += " ";
                    if( sBuildSQL.Find(sField) == -1 ) 
                    {
                        CString sSQL;
                        sSQL.Format("ALTER TABLE %s ADD COLUMN %s %s;",sTableName,sField,sType);
                        //AfxMessageBox(sSQL);
                        db->execDML(sSQL);
                    }
                }
                //
                //  deleting any fields? 
                //
                /*
                sNewFields.TrimRight(" ");
                sNewFields.Replace(" ",", ");
                sSQL.Format("CREATE %s AS SELECT %s FROM old_%s",sTableName,sNewFields,sTableName);
                AfxMessageBox(sSQL);
                */
            }
        }
    }
    catch(CFileException* pe)
    {
        //char szMsg[256];
        //pe->GetErrorMessage(szMsg, sizeof(szMsg));
        //MessageBox(NULL, szMsg, "TrainOps ERROR", MB_ICONHAND);
        pe->Delete();
    }
}

void CTrainOpsApp::FillCarTypesData(CppSQLite3DB* db)
{
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"2BHP\",\"2 Bay Hopper\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"3BHP\",\"3 Bay Hopper\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"BX\",\"Boxcar\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"CH\",\"Covered Hopper\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"CT\",\"Cattle Car\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"FC\",\"Flat Car\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"GD\",\"Gondola Car\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"RF\",\"Refrigerator Car\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"SM\",\"Stock Car\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"TK\",\"Tank car\",1);");

    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"PSCH\",\"Passenger Coach\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"PSBG\",\"Baggage Car\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"PSCB\",\"Combine\",1);");
    db->execDML("INSERT INTO CarTypes (type_id,description,active) VALUES (\"PSOB\",\"Observation Car\",1);");
}

////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::VerifyLocationTablesData(CppSQLite3DB* pDB)
{
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  VerifySidingTablesData
//      Find sidings with a bad location FK, and delete them
//      Make sure there is at least ONE 'yard' siding
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::VerifySidingTablesData(CppSQLite3DB* pDB)
{
    BOOL bHasYard(FALSE);
    pDB->execDML("CREATE TEMP TABLE _sidings AS SELECT * FROM Sidings");
    CppSQLite3Query q = pDB->execQuery("SELECT * FROM _sidings;");
    while (!q.eof())
    {
        int iSidingFK = q.getIntField("id");
        int iLocationFK = q.getIntField("Locations_FK");
        if( q.getIntField("yard")==1 )
            bHasYard = TRUE;
        //
        q.nextRow();
        //
        CString sSQL;
        sSQL.Format("SELECT 1 FROM Locations WHERE id=%d",iLocationFK);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if(q2.eof())
        {
            q2.finalize();
            sSQL.Format("DELETE FROM Sidings WHERE id=%d",iSidingFK);
            pDB->execDML(sSQL);
        }
        else
            q2.finalize();
    }
    q.finalize();
    //
    if( !bHasYard )
    {
        m_pSplash->ShowWindow(SW_HIDE);
        MessageBox(0,"You do not have any sidings marked as a yard.\n\nCars will pile up on your sidings.\n\nYou really should correct this.","TrainOps Note",MB_OK);
        m_pSplash->ShowWindow(SW_SHOW);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  VerifyIndustryTablesData = Verify each entry in the industry table against being on an
//                  invalid siding.  Delete if it is.  Also delete any records out of the
//                  Industries_Cars and Industries_Commodities tables
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::VerifyIndustryTablesData(CppSQLite3DB* pDB)
{
    pDB->execDML("CREATE TEMP TABLE _industries AS SELECT * FROM Industries");
    CppSQLite3Query q = pDB->execQuery("SELECT id,Sidings_FK FROM _industries;");
    while (!q.eof())
    {
        int iIndustryFK = q.getIntField("id");
        int iSidingFK = q.getIntField("Sidings_FK");
        q.nextRow();
        //
        CString sSQL;
        sSQL.Format("SELECT 1 FROM Sidings WHERE id=%d",iSidingFK);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if(q2.eof())
        {
            q2.finalize();
            sSQL.Format("DELETE FROM Industries WHERE id=%d",iIndustryFK);
            pDB->execDML(sSQL);
            // delete records from Indusries_Cars and Industries_Commodities as well
            sSQL.Format("DELETE FROM Industries_Cars WHERE Industries_FK=%d",iIndustryFK);
            pDB->execDML(sSQL);
            sSQL.Format("DELETE FROM Industries_Commodities WHERE Industries_FK=%d",iIndustryFK);
            pDB->execDML(sSQL);
        }
        else
            q2.finalize();
    }
    q.finalize();
    //
    //  verify Industries_Cars tables
    //
    pDB->execDML("CREATE TEMP TABLE _IC AS SELECT * FROM Industries_Cars");
    q = pDB->execQuery("SELECT Industries_FK FROM _IC;");
    while (!q.eof())
    {
        int iIndustryFK = q.getIntField("Industries_FK");
        CString sSQL;
        sSQL.Format("SELECT 1 FROM Industries WHERE id=%d",iIndustryFK);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if(q2.eof())
        {
            q2.finalize();
            sSQL.Format("DELETE FROM Industries_Cars WHERE Industries_FK=%d",iIndustryFK);
            pDB->execDML(sSQL);
        }
        else
            q2.finalize();
        //
        q.nextRow();
    }
    q.finalize();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  VerifyCarTablesData - Verify each entry in the cars table has valid 
//                locations, industries, etc. 
//
////////////////////////////////////////////////////////////////////////////////////////////

void CTrainOpsApp::VerifyCarTablesData(CppSQLite3DB* pDB)
{
    try { pDB->execDML("DROP TABLE _cars;"); } catch (...) {}
    pDB->execDML("CREATE TEMP TABLE _cars AS SELECT * FROM Cars");
    //
    CppSQLite3Query q = pDB->execQuery("SELECT * FROM _cars;");
    int iCurrentLocation=0;
    int iCurrentIndustry=0;
    while (!q.eof())
    {
        int iCarFK = q.getIntField("id");
        int iLocationFK = q.getIntField("Location_FK");
        int iIndustryFK = q.getIntField("Industry_FK");
        int iSidingFK = q.getIntField("Siding_FK");
        bool bOffLayout = q.getIntField("offlayout")==1;
        CString sSQL;
        //
        //  any car that is ONLY on a location/siding, move to a yard track somewhere
        //
        if(( iLocationFK != -1 && iSidingFK != -1 && iIndustryFK == -1 ) || ( iLocationFK != -1 && iSidingFK == -1 ))
        {
            if( iSidingFK != -1 )
            {
                sSQL.Format("SELECT yard FROM Sidings WHERE id=%d",iSidingFK);
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if(!q2.eof())
                {
                    if( q2.getIntField("yard") != 1 )
                    {
                        sSQL.Format("SELECT * FROM Sidings WHERE yard=1 ORDER BY RANDOM()");
                        CppSQLite3Query q3 = pDB->execQuery(sSQL);
                        if(!q3.eof())
                        {
                            iSidingFK = q3.getIntField("id");
                            iLocationFK = q3.getIntField("Locations_FK");
                            q3.finalize();
                            sSQL.Format("UPDATE Cars SET Location_FK=%d, Siding_FK=%d WHERE id=%d",iLocationFK,iSidingFK,iCarFK);
                            pDB->execDML(sSQL);
                        }
                        else
                            q3.finalize();
                    }
                }
                q2.finalize();
            }
            else
            {
                sSQL.Format("SELECT * FROM Sidings WHERE yard=1 ORDER BY RANDOM()");
                CppSQLite3Query q3 = pDB->execQuery(sSQL);
                if(!q3.eof())
                {
                    iSidingFK = q3.getIntField("id");
                    iLocationFK = q3.getIntField("Locations_FK");
                    q3.finalize();
                    sSQL.Format("UPDATE Cars SET Location_FK=%d, Siding_FK=%d WHERE id=%d",iLocationFK,iSidingFK,iCarFK);
                    pDB->execDML(sSQL);
                }
                else
                    q3.finalize();
            }
        }
        // verify actual locations
        if( iLocationFK != -1 )
        {
            sSQL.Format("SELECT 1 FROM Locations WHERE id=%d",iLocationFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if(q2.eof())
            {
                q2.finalize();
                sSQL.Format("UPDATE Cars SET Location_FK=-1 WHERE id=%d",iCarFK);
                pDB->execDML(sSQL);
            }
            else
                q2.finalize();
        }
        else
        {
            sSQL.Format("SELECT id FROM Locations ORDER BY RANDOM();");
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            int iLocationFK=-1;
            if( !q2.eof() )
            {
                iLocationFK = q2.getIntField("id");
            }
            q2.finalize();
            sSQL.Format("UPDATE Cars SET Location_FK=%d WHERE id=%d",iLocationFK,iCarFK);
            pDB->execDML(sSQL);
        }
        //
        if( iSidingFK != -1 )
        {
            sSQL.Format("SELECT 1 FROM Sidings WHERE id=%d",iSidingFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if(q2.eof())
            {
                q2.finalize();
                sSQL.Format("UPDATE Cars SET Siding_FK=-1 WHERE id=%d",iCarFK);
                pDB->execDML(sSQL);
            }
            else
                q2.finalize();
        }
        if( iIndustryFK != -1 )
        {
            sSQL.Format("SELECT 1 FROM Industries WHERE id=%d",iIndustryFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if(q2.eof())
            {
                q2.finalize();
                sSQL.Format("UPDATE Cars SET Industry_FK=-1 WHERE id=%d",iCarFK);
                pDB->execDML(sSQL);
            }
            else
                q2.finalize();
        }
        //  fixup bad car records
        if( iLocationFK == -1 && iIndustryFK == -1 && iSidingFK == -1 && !bOffLayout )
        {
            //  mark car as available
            sSQL.Format("UPDATE cars SET delivery_date=0 WHERE id=%d",iCarFK);
            pDB->execDML(sSQL);
        }
        //
        //  make sure industry matches siding matches location
        //

        //
        q.nextRow();
    }
    //
    //  make sure every car is on-layout somewhere
    //
    CString sSQL;
    sSQL.Format("SELECT id FROM Locations ORDER BY RANDOM();");
    CppSQLite3Query q2 = pDB->execQuery(sSQL);
    int iLocationFK=0;
    if( !q2.eof() )
    {
        iLocationFK = q2.getIntField("id");
    }
    sSQL.Format("UPDATE cars SET offlayout=0, Location_FK=%d WHERE offlayout=1",iLocationFK);
    pDB->execDML(sSQL);
    //
    q.finalize();
}


//

void CTrainOpsApp::VerifyConfigData(CppSQLite3DB* pDB)
{
    CppSQLite3Query q = pDB->execQuery("SELECT date FROM _Config WHERE id=1");
    bool bBadDate(false);
    if( q.eof() )
    {
        q.finalize();
        SetupConfig(pDB);
    }
    else
    {
        if( q.getIntField("date") < 2 )
            bBadDate = true;

    }
    q.finalize();
    //
    //  if we have a new db, advance the date so we can build trains
    //
    if( bBadDate )
        pDB->execDML("UPDATE _Config SET date=2");
}

void CTrainOpsApp::SetupConfig(CppSQLite3DB* db)
{
    db->execDML("INSERT INTO _Config (id,date,rrdate) VALUES (1,1,-1);");
}

int CTrainOpsApp::GetDate()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT date,rrdate FROM _Config WHERE id=1");
    int iDate = q.getIntField("date");
  
    return iDate;
}

CString CTrainOpsApp::GetDateString()
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    char szDate[24];
    if( !q.eof() )
    {
        int y,m,d;
        JulianDate::JdToYmd( q.getIntField(0)+q.getIntField(1), &y,&m,&d);
        sprintf(szDate,"%d/%d/%d",m,d,y);
    }
    q.finalize();
    return CString(szDate);
}

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsApp message handlers

int CTrainOpsApp::ExitInstance() 
{
    TBTreeStateMgr::Uninitialize();

    CMenuXP::UninitializeHook();
    //
    int nRes = CWinApp::ExitInstance();
    RA_DoRestartProcessFinish();
    return nRes;
} // End of CWinApp::ExitInstance();

void CTrainOpsApp::VersionCheck() 
{
    //
    //  check http://www.largescalecentral.com/TrainOps/_BldNum.h
    //  for current version number
    //
    HRESULT hr = URLDownloadToFile ( NULL, "http://www.largescalecentral.com/TrainOps/_BldNum.h", "./_version.txt", 0, NULL );
    if( hr == S_OK )
    {
        FILE* fh = fopen("./_version.txt","r");
        if( fh )
        {
            char szWork[255];
            int iMin,iMax,iBuild;
            while( fgets(szWork,254,fh) )
            {
                CString sWork(szWork);
                if( sWork.Find("FILE_VERSION0" ) > 0)
                    iMax = atoi(sWork.Mid(sWork.Find('(')+1));
                if( sWork.Find("FILE_VERSION1" ) > 0)
                    iMin = atoi(sWork.Mid(sWork.Find('(')+1));
                if( sWork.Find("BUILD_NUMBER" ) > 0)
                    iBuild = atoi(sWork.Mid(sWork.Find('(')+1));
            }
            fclose(fh);
            unlink("./_version.txt");
            //
            if( iMax > FILE_VERSION0 ||
                iMin > FILE_VERSION1 ||
                iBuild > BUILD_NUMBER 
                )
            {
                CNewVersionDialog pDlg;	
                if( pDlg.DoModal() == IDYES )
                {
                    hr = URLDownloadToFile ( NULL, "http://www.largescalecentral.com/TrainOps/TrainOps_setup.exe", "./TrainOps_setup.exe", 0, NULL );
                    if( hr == S_OK )
                    {
                        WinExec("TrainOps_setup.exe",SW_SHOW);
                        exit(0);
                    }
                    else
                        MessageBox(0,"Unable to download setup!\nVisit http://www.largescalecental.com/TrainOps to download the latest update.","DEBUG",MB_ICONSTOP|MB_OK);
                }
            }
        }
    }
}

//
//  help mapping
//
static const DWORD aMenuHelpIDs[] =
{
    IDD_RUNTRAINSDIALOG, 15001,
    IDD_TERMINATETRAINDIALOG, 15001,
    IDD_TRAINDETAILDIALOG, 15001,
    IDD_CABOOSEDIALOG, 15002,
    IDD_CABOOSEDETAILDIALOG, 15002,
    IDD_PHYSICAL_LAYOUT_DIALOG, 15013,
    IDD_PREFDIALOG,  15014 ,
    IDD_SIDINGDETAILDIALOG, 15016 ,

        0,    0
};
/*
#define Bad_Order_Cards	15000
#define Build_Run_and_Terminate_Trains	15001
#define Cabooses	15002
#define Car_Location_Reports	15003
#define Car_Types	15004
#define Cars	15005
#define Concepts	15006
#define Equipment_Location	15007
#define Form_19s	15008
#define Industries	15009
#define Installing	15010
#define Locations	15011
#define Locomotives	15012
#define Physical_Layout	15013
#define Preferences_and_Defaults	15014
#define Reference	15015
#define Sidings	15016
#define Switchlists	15017
#define Trains	15018
#define Tutorial	15019
#define Updating	15020
#define Welcome_to_TrainOps	15021
*/

// Subtract 1 from the total number of ID pairs to account
// for the NULL pair at the end of the array.
DWORD numHelpIDs = (sizeof(aMenuHelpIDs)/sizeof(DWORD))/2 - 1;

void CTrainOpsApp::WinHelp(DWORD dwData, UINT nCmd) 
{
    DWORD i;
    switch (nCmd)
    {
    case HELP_CONTEXT:
        char szAppPath[255]; 
        ITEMIDLIST* pidl;
        HRESULT hRes = SHGetSpecialFolderLocation( NULL, 0x0026 , &pidl );
        if (hRes==NOERROR)
        {
            SHGetPathFromIDList( pidl, szAppPath );
            strcat(szAppPath,"\\LargeScaleCentral\\TrainOps\\TrainOps.chm");
        }        
        // If it is a help context command, search for the
        // control ID in the array.
        for (i= 0; i < numHelpIDs*2; i+=2)
        {
            if (aMenuHelpIDs[i] == LOWORD (dwData) )
            {
                i++;  // pass the help context id to HTMLHelp
                CString sHelp;
                //sHelp.Format("hh.exe \"%s::/Reference.htm\" -mapid %d",szAppPath,aMenuHelpIDs[i]);
                sHelp.Format("hh.exe -mapid %d \"%s\"",aMenuHelpIDs[i],szAppPath);
                WinExec(sHelp,SW_SHOW);
                return;
            }
        }
        
        // If the control ID cannot be found,
        // display the default topic.
        if (i == numHelpIDs*2)
        {
             CString sHelp;
             sHelp.Format("hh.exe \"%s\"",szAppPath);
             WinExec(sHelp,SW_SHOW);
        }
        break;
    }
	
}

CString CTrainOpsApp::GetIndustryRatios(int iIndustryFK,int iSidingFK,int* iRatio)
{
    CString sRatioText("");
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    //
    //  cars in
    //
    sSQL.Format("SELECT * from Industries_Cars WHERE Industries_FK=%d AND InOut=0",iIndustryFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    float fPctIn(0);
    float fPctOut(0);
    while( !q.eof() )
    {
        int iLoadEmptyDays = q.getIntField("LoadEmptyDays");
        float fQuanHigh = q.getFloatField("Quantity_high");
        float fQuanLow = q.getFloatField("Quantity_low");
        float fPercent = q.getFloatField("Quantity_percentage");
        float fPct = fQuanLow + (fQuanHigh *(fPercent/100))*iLoadEmptyDays;
        fPctIn += fPct;
        //
        q.nextRow();
    }
    q.finalize();
    //
    //  cars out
    //
    sSQL.Format("SELECT * from Industries_Cars WHERE Industries_FK=%d AND InOut=1",iIndustryFK);
    q = pDB->execQuery(sSQL);
    while( !q.eof() )
    {
        int iLoadEmptyDays = q.getIntField("LoadEmptyDays");
        float fQuanHigh = q.getFloatField("Quantity_high");
        float fQuanLow = q.getFloatField("Quantity_low");
        float fPercent = q.getFloatField("Quantity_percentage");
        float fPct = fQuanLow + (fQuanHigh *(fPercent/100))*iLoadEmptyDays;
        fPctOut += fPct;
        //
        q.nextRow();
    }
    q.finalize();
    //
    //  get average car length
    //
    q = pDB->execQuery("SELECT AVG(car_length) AS avlen FROM Cars where car_length > 0");
    int iAverageCarLength = g_iCarLength;
    if( !q.eof() )
        iAverageCarLength = q.getIntField("avlen");
    q.finalize();
    //
    sSQL.Format("SELECT capacity FROM Sidings WHERE id=%d;",iSidingFK);
    q = pDB->execQuery(sSQL);
    int iSidingLength = 100;
    if( !q.eof() )
        iSidingLength = q.getIntField("capacity");
    q.finalize();
    //
    //  calculate siding use ratio
    //
    *iRatio = (((float)iAverageCarLength * (fPctIn+fPctOut))/ (float)iSidingLength)*100.0;
    //
    sRatioText.Format("(In: %1.1f | Out: %1.1f | Usage: %d%%)",fPctIn,fPctOut,*iRatio);
    //
    return sRatioText;
}
