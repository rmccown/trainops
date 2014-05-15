/*
	reports.cpp
*/

#include "stdafx.h"
#include "shlobj.h"
#include "TrainOps.h"

#include "RunTrainsDialog.h"

#include "Reports.h"
#include "julian.h"
#include <math.h>

#include "SimpleDate.h"

#include "PDFViewerDialog.h"

int g_iPageNumber;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    //CString sErr;
    //sErr.Format("PDF ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,(HPDF_UINT)detail_no);
    //MessageBox(0,sErr,"PDF error caught",MB_ICONSTOP|MB_OK);
    TRACE("PDF ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,(HPDF_UINT)detail_no);
    //throw std::exception ();
}

#define FONT_SIZE 12
#define FONT_SIZE_SMALL FONT_SIZE-3
#define FONT_SIZE_LARGE FONT_SIZE+3
#define FONT_SIZE_FORM19 FONT_SIZE-4

/*
    Car Location Report
*/

void DoCarLocationReport(int iTrainFK)
{
    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page;
    HPDF_Font detail_font;
//    HPDF_Image image;
    const char *detail_font_name;

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\carreport%d.pdf",fname,iRand);
    //unlink(fname);
    Sleep(2000);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"error: cannot create PdfDoc object","ERROR",MB_OK);
        return;
    }
    SetCursor(LoadCursor(NULL,IDC_WAIT));

    /* declaration for using Japanese encoding. */
    HPDF_UseJPEncodings (pdf);

    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* load ttf file */
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, ".\\TYPIW___.TTF", HPDF_TRUE);

    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    //
    Watermark(pdf,page);
    //
    detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
    //
    HPDF_Page_BeginText (page);
    HPDF_Page_SetSize(page,HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
    HPDF_Page_SetTextLeading(page,FONT_SIZE_SMALL-2);
    HPDF_Page_SetFontAndSize (page, detail_font, FONT_SIZE_SMALL-1);
    //
    HPDF_Page_EndText (page);
    //
    /*
    try {
        if( !g_sLogo.IsEmpty() )
        {
            image = HPDF_LoadJpegImageFromFile(pdf, g_sLogo);
            if( image )
            {
                int iw = HPDF_Image_GetWidth (image)/2;
                int ih = HPDF_Image_GetHeight (image)/2;
                int x = HPDF_Page_GetWidth(page) - 10 - iw ;
                int y = HPDF_Page_GetHeight(page) - 10 - ih;
                HPDF_STATUS hStat = HPDF_Page_DrawImage (page, image, x, y, iw, ih);
            }
            else
            {
                image = HPDF_LoadPngImageFromFile(pdf, g_sLogo);
                if( image )
                {
                    int iw = HPDF_Image_GetWidth (image)/2;
                    int ih = HPDF_Image_GetHeight (image)/2;
                    int x = HPDF_Page_GetWidth(page) - 10 - iw ;
                    int y = HPDF_Page_GetHeight(page) - 10 - ih;
                    HPDF_STATUS hStat = HPDF_Page_DrawImage (page, image, x, y, iw, ih);
                }
            }
        }
    }
    catch (...) {}
    */
    //
    HPDF_Page_BeginText (page);
    //

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
    char szPageTitle[255];
    sprintf(szPageTitle,"%s\nSystem-Wide Car Location Report - %s",g_sRRName,szDate);
    HPDF_Page_TextRect(page,0,770,600,740,szPageTitle,HPDF_TALIGN_CENTER,0);
    HPDF_Page_MoveTextPos(page, 187, -12);
    HPDF_Page_ShowText(page, "-------------------------------");
    HPDF_Page_MoveTextPos(page, -155, -25);
    //
    //  if( iTrainFK != 0 ) get list of cars
    //
    CArray <int,int> pCarArray;
    //
    if( iTrainFK > 0 )
    {
        CString sSQL;
        sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d",iTrainFK);
        q = pDB->execQuery(sSQL);
        while( !q.eof() )
        {
            pCarArray.Add(q.getIntField("Cars_FK"));
            q.nextRow();
        }
        q.finalize();
    }

    q = pDB->execQuery("SELECT Cars.id,car_number,reporting_marks,description,Location_FK,Industry_FK,Siding_FK FROM Cars,CarTypes WHERE Cars.out_of_service=0 AND CarTypes.id=Cars.CarTypes_FK ORDER BY Location_FK,Industry_FK;");
    int iCurrentLocation=0;
    int iCurrentIndustry=0;
    while (!q.eof())
    {
        int iCarFK = q.getIntField("id");
        //
        //  search for train FK
        //
        if( iTrainFK > 0 )
        {
            bool bFound=false;
            for(int i=0;i<=pCarArray.GetUpperBound();i++)
            {
                if( pCarArray.GetAt(i) == iCarFK )
                    bFound=true;
            }
            if( !bFound )
            {
                q.nextRow();
                continue;
            }
        }
        CString sCarNumber = q.getStringField("car_number");
        CString sReportingMarks = q.getStringField("reporting_marks");
        CString sDescription = q.getStringField("description");
        int iLocationFK = q.getIntField("Location_FK");
        int iIndustryFK = q.getIntField("Industry_FK");
        int iSidingFK = q.getIntField("Siding_FK");
        
        CString sSQL;
        if( iCurrentLocation != iLocationFK )
        {
            if( iLocationFK == -1 )
            {
                HPDF_Page_MoveToNextLine(page);
                HPDF_Page_ShowText (page, "Off-Layout");
                HPDF_Page_MoveToNextLine(page);
                char szDashLine[255];
                memset(&szDashLine,0,sizeof(szDashLine));
                for( int i=0;i<10;i++ )
                    szDashLine[i] = '-';
                HPDF_Page_ShowText (page, szDashLine);
                HPDF_Page_MoveToNextLine(page);
            }
            else
            {
                sSQL.Format("SELECT name FROM Locations WHERE id=%d",iLocationFK);
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if(!q2.eof())
                {
                    CString sLocation = q2.getStringField("name");
                    HPDF_Page_MoveToNextLine(page);
                    HPDF_Page_ShowText (page, sLocation);
                    HPDF_Page_MoveToNextLine(page);
                    char szDashLine[255];
                    memset(&szDashLine,0,sizeof(szDashLine));
                    for( int i=0;i<sLocation.GetLength();i++ )
                        szDashLine[i] = '-';
                    HPDF_Page_ShowText (page, szDashLine);
                    HPDF_Page_MoveToNextLine(page);
                }
                q2.finalize();
            }
        }
        //
        if( iCurrentLocation == 0 )
            CheckCaboose(page,iTrainFK);
        //
        iCurrentLocation = iLocationFK;
        if( iCurrentIndustry != iIndustryFK )
        {
            if( iIndustryFK != -1 )
            {
                sSQL.Format("SELECT name FROM Industries WHERE id=%d",iIndustryFK);
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if(!q2.eof())
                {
                    CString sIndustry = q2.getStringField("name");
                    HPDF_Page_MoveToNextLine(page);
                    HPDF_Page_ShowText (page, sIndustry);
                    HPDF_Page_MoveToNextLine(page);
                    char szDashLine[255];
                    memset(&szDashLine,0,sizeof(szDashLine));
                    for( int i=0;i<sIndustry.GetLength();i++ )
                        szDashLine[i] = '-';
                    HPDF_Page_ShowText (page, szDashLine);
                    HPDF_Page_MoveToNextLine(page);
                }
                else
                {
                    //
                    //  iIndustryFK=0?  On siding, not at industry
                    //
                    sSQL.Format("SELECT name FROM Sidings WHERE id=%d",iSidingFK);
                    CppSQLite3Query q2 = pDB->execQuery(sSQL);
                    if(!q2.eof())
                    {
                        CString sSiding;
                        sSiding.Format("%s siding",q2.getStringField("name"));
                        HPDF_Page_ShowText (page, sSiding);
                        HPDF_Page_MoveToNextLine(page);
                        char szDashLine[255];
                        memset(&szDashLine,0,sizeof(szDashLine));
                        for( int i=0;i<sSiding.GetLength();i++ )
                            szDashLine[i] = '-';
                        HPDF_Page_ShowText (page, szDashLine);
                        HPDF_Page_MoveToNextLine(page);
                    }
                    else
                    {
                        HPDF_Page_ShowText (page, "On Sidings");
                        HPDF_Page_MoveToNextLine(page);
                        HPDF_Page_ShowText (page, "==========");
                        HPDF_Page_MoveToNextLine(page);
                    }
                    q2.finalize();
                }
                q2.finalize();
            }
            else
            {
                sSQL.Format("SELECT name FROM Sidings WHERE id=%d",iSidingFK);
                CppSQLite3Query q2 = pDB->execQuery(sSQL);
                if(!q2.eof())
                {
                    HPDF_Page_ShowText (page, q2.getStringField("name"));
                    HPDF_Page_MoveToNextLine(page);
                }
                else
                {
                    //
                    //  check to see if we have a scheduled car siding here
                    //
                    HPDF_Page_ShowText (page, "On Sidings");
                    HPDF_Page_MoveToNextLine(page);
                    HPDF_Page_ShowText (page, "----------");
                    HPDF_Page_MoveToNextLine(page);
                }
                q2.finalize();
            }
        }
        iCurrentIndustry = iIndustryFK;

        CString sPickup("");
        if( iTrainFK == 0 )
        {
            sSQL.Format("SELECT 1 FROM Trains_Scheduled_Cars,Trains_Scheduled WHERE Trains_Scheduled.id=Trains_Scheduled_Cars.Trains_Scheduled_FK AND Cars_FK=%d AND Trains_Scheduled.train_run=0;",iCarFK);
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            if( !q3.eof() )
                sPickup = "(Scheduled for Pickup)";
            q3.finalize();
        }
        //
        CString sCarLine;
        sCarLine.Format("    %s %s %s %s",
            sDescription,
            sReportingMarks,
            sCarNumber,
            sPickup
            );
        HPDF_Page_ShowText(page, sCarLine);
        HPDF_Page_MoveToNextLine(page);

        q.nextRow();
    }
    q.finalize();
    //
    HPDF_Page_EndText (page);
    //
    HPDF_SaveToFile (pdf, fname);
    /* clean up */
    HPDF_Free (pdf);

    ShowPDF(fname);
    return;
}

/*
    Car Location Report
    Sorted by reporting marks
*/

void DoCarLocationSortedReport(int iTrainFK)
{
    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page;
    HPDF_Font detail_font;
//    HPDF_Image image;
    const char *detail_font_name;

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\carreport%d.pdf",fname,iRand);
    //unlink(fname);
    Sleep(2000);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"error: cannot create PdfDoc object","ERROR",MB_OK);
        return;
    }
    SetCursor(LoadCursor(NULL,IDC_WAIT));

    /* declaration for using Japanese encoding. */
    HPDF_UseJPEncodings (pdf);

    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* load ttf file */
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, ".\\TYPIW___.TTF", HPDF_TRUE);

    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    //
    Watermark(pdf,page);
    //
    detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
    //
    HPDF_Page_BeginText (page);
    HPDF_Page_SetSize(page,HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
    HPDF_Page_SetTextLeading(page,FONT_SIZE-1);
    HPDF_Page_SetFontAndSize (page, detail_font, FONT_SIZE);
    //
    HPDF_Page_EndText (page);
    //
    /*
    try {
        if( !g_sLogo.IsEmpty() )
        {
            image = HPDF_LoadJpegImageFromFile(pdf, g_sLogo);
            if( image )
            {
                int iw = HPDF_Image_GetWidth (image)/2;
                int ih = HPDF_Image_GetHeight (image)/2;
                int x = HPDF_Page_GetWidth(page) - 10 - iw ;
                int y = HPDF_Page_GetHeight(page) - 10 - ih;
                HPDF_STATUS hStat = HPDF_Page_DrawImage (page, image, x, y, iw, ih);
            }
            else
            {
                image = HPDF_LoadPngImageFromFile(pdf, g_sLogo);
                if( image )
                {
                    int iw = HPDF_Image_GetWidth (image)/2;
                    int ih = HPDF_Image_GetHeight (image)/2;
                    int x = HPDF_Page_GetWidth(page) - 10 - iw ;
                    int y = HPDF_Page_GetHeight(page) - 10 - ih;
                    HPDF_STATUS hStat = HPDF_Page_DrawImage (page, image, x, y, iw, ih);
                }
            }
        }
    }
    catch (...) {}
    */
    //
    HPDF_Page_BeginText (page);
    //

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
    char szPageTitle[255];
    sprintf(szPageTitle,"%s\nSystem-Wide Car Location Report - %s",g_sRRName,szDate);
    HPDF_Page_TextRect(page,0,770,600,740,szPageTitle,HPDF_TALIGN_CENTER,0);
    HPDF_Page_MoveTextPos(page, 187, -12);
    HPDF_Page_ShowText(page, "-------------------------------");
    HPDF_Page_MoveTextPos(page, -155, -25);
    //
    //  if( iTrainFK != 0 ) get list of cars
    //
    CArray <int,int> pCarArray;
    //
    if( iTrainFK > 0 )
    {
        CString sSQL;
        sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d",iTrainFK);
        q = pDB->execQuery(sSQL);
        while( !q.eof() )
        {
            pCarArray.Add(q.getIntField("Cars_FK"));
            q.nextRow();
        }
        q.finalize();
    }

    q = pDB->execQuery("SELECT Cars.id,car_number,reporting_marks,description,Location_FK,Industry_FK FROM Cars,CarTypes WHERE CarTypes.id=Cars.CarTypes_FK ORDER BY reporting_marks,car_number;");
    int iCurrentLocation=0;
    int iCurrentIndustry=0;
    while (!q.eof())
    {
        int iCarFK = q.getIntField("id");
        //
        //  search for train FK
        //
        if( iTrainFK > 0 )
        {
            bool bFound=false;
            for(int i=0;i<=pCarArray.GetUpperBound();i++)
            {
                if( pCarArray.GetAt(i) == iCarFK )
                    bFound=true;
            }
            if( !bFound )
            {
                q.nextRow();
                continue;
            }
        }
        CString sCarNumber = q.getStringField("car_number");
        CString sReportingMarks = q.getStringField("reporting_marks");
        CString sDescription = q.getStringField("description");
        int iLocationFK = q.getIntField("Location_FK");
        int iIndustryFK = q.getIntField("Industry_FK");
        
        CString sSQL;
        CString sWhere, sIndustry;
        //
        if( iIndustryFK != -1 )
        {
            sSQL.Format("SELECT name FROM Industries WHERE id=%d",iIndustryFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if(!q2.eof())
            {
                sIndustry = q2.getStringField("name");
            }
            else
            {
                sIndustry = "On siding, not spotted at an industry";
            }
            q2.finalize();
        }
        else
        {
            sWhere = "Yard/Interchange Siding";
        }
        if( iLocationFK != -1 )
        {
            sSQL.Format("SELECT name FROM Locations WHERE id=%d",iLocationFK);
            CppSQLite3Query q2 = pDB->execQuery(sSQL);
            if(!q2.eof())
            {
                sWhere.Format("%s at %s",sIndustry,q2.getStringField("name"));
            }
            q2.finalize();
        }

        //
        CString sCarLine;
        sCarLine.Format("    %s %s %s %s",
            sDescription,
            sReportingMarks,
            sCarNumber,
            sWhere
            );
        HPDF_Page_ShowText(page, sCarLine);
        HPDF_Page_MoveToNextLine(page);

        q.nextRow();
    }
    q.finalize();
    //
    AllCaboose(page);
    //
    HPDF_Page_EndText (page);
    //
    HPDF_SaveToFile (pdf, fname);
    /* clean up */
    HPDF_Free (pdf);

    ShowPDF(fname);
    return;
}

void CheckCaboose(HPDF_Page page,int iTrainScheduledFK)
{

    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT * FROM Trains_Scheduled WHERE id=%d;",iTrainScheduledFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    if( !q.eof() )
    {
        sSQL.Format("SELECT reporting_marks,car_number FROM Caboose WHERE id=%d;",q.getIntField("Caboose_FK"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            CString sLine;
            sLine.Format("    %s %s Caboose",q2.getStringField("reporting_marks"),q2.getStringField("car_number"));
            HPDF_Page_ShowText(page, sLine);
            HPDF_Page_MoveToNextLine(page);
        }
        q2.finalize();
    }
    q.finalize();
}

void AllCaboose(HPDF_Page page)
{
    HPDF_Page_MoveToNextLine(page);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT id,reporting_marks,car_number FROM Caboose;");
    CppSQLite3Query q = pDB->execQuery(sSQL);
    while( !q.eof() )
    {
        sSQL.Format("SELECT Trains_FK FROM Trains_Scheduled WHERE train_run=0 AND Caboose_FK=%d;",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() ) //  is it on a train?
        {
            sSQL.Format("SELECT name,Location_FK FROM Trains_Location,Locations WHERE Locations.id=Trains_Location.Location_FK AND Trains_Location.id=%d;",q2.getIntField("Trains_FK"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            if( !q3.eof() ) //  where does the train start
            {
                CString sLine;
                sLine.Format("    %s %s Caboose at %s",q.getStringField("reporting_marks"),q.getStringField("car_number"),q3.getStringField("name"));
                HPDF_Page_ShowText(page, sLine);
                HPDF_Page_MoveToNextLine(page);
            }
        }
        q2.finalize();
        q.nextRow();
    }
    q.finalize();
}



int GetSiding(int iLocationFK)
{
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;

    sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%d AND yard=1;",iLocationFK);    //  yard siding
    CppSQLite3Query q = pDB->execQuery(sSQL);
    int iRet=0;
    if( q.eof() )
    {
        sSQL.Format("SELECT id FROM Sidings WHERE Locations_FK=%;",iLocationFK);    //  any siding
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
            iRet = q2.getIntField("id");
        q2.finalize();
    }
    else
    {
        iRet = q.getIntField("id");
    }
    q.finalize();
    return iRet;
}

void PrintSwitchlist(int iTrains_Scheduled_FK)
{
    //  FIXME: temp disabled
    if( g_bRealisticReports )
        PrintRealisticSwitchlist(iTrains_Scheduled_FK);
    else
        PrintSimpleSwitchlist(iTrains_Scheduled_FK);
}

void PrintRealisticSwitchlist(int iTrains_Scheduled_FK)
{
    std::vector<CCar> pCarTable;
    //
    //  Get the route for this scheduled train,
    //  and display car setouts/pickups per industry per location
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    char szDate[81];
    if( !q.eof() )
    {
        int y,m,d;
        JulianDate::JdToYmd( q.getIntField("rrdate")+q.getIntField("date"), &y,&m,&d);
        sprintf(szDate,"%02d/%02d/%d",m,d,y);
        CSimpleDate pDate(szDate);
        strcpy(szDate,pDate.GetFullDateStringLong());
    }
    q.finalize();
    //

    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page[100];
    int iPage = 0;
    HPDF_Font detail_font;
    //HPDF_Image image;
    const char *detail_font_name;
    char pszLine[255];
    CString sSeparator("---------------------------------------------------------------------------------------------------");

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\switchlist%d.pdf",fname,iRand);
    Sleep(2000);
    //unlink(fname);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"Unable to create switchlist!  Check for previously opened ones!","TrainOps Error",MB_ICONSTOP);
        return;
    }

    /* declaration for using Japanese encoding. */
    HPDF_UseJPEncodings (pdf);

    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* load ttf file */
    ::GetCurrentDirectory(254,pszLine);
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, ".\\TYPIW___.TTF", HPDF_TRUE);

    /* add a new page object. */
    page[iPage] = HPDF_AddPage (pdf);
    //
    Watermark(pdf,page[iPage]);
    //
    //detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
    detail_font = HPDF_GetFont (pdf, "Courier", "StandardEncoding");

    HPDF_Page_BeginText (page[iPage]);
    HPDF_Page_SetSize(page[iPage],HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
    //HPDF_Page_SetWidth(page[iPage],580);
    HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE);
    //
    // write title
    //
    char szTitle[255];
    sprintf(szTitle,"%s\nSwitchlist",g_sRRName);
    HPDF_Page_SetTextLeading(page[iPage],11);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, 12);
    HPDF_Page_TextRect(page[iPage],25,780,580,740,szTitle,HPDF_TALIGN_CENTER,0);
    HPDF_Page_SetTextLeading(page[iPage],9);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE-2.5);
    //
    CString sSQL;
    sSQL.Format("SELECT name,trainid,Trains_FK,Locomotive_FK,Caboose_FK FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.id=%d AND Trains_Scheduled.Trains_FK=Trains.id;",iTrains_Scheduled_FK);
    q = pDB->execQuery(sSQL);
    CString sTrainID = q.getStringField("trainid");
    int iTrainFK = q.getIntField(2);
    //BOOL bReverse = q.getIntField(3);
    int iLocoFK = q.getIntField(3);
    int iCabooseFK = q.getIntField(4);
    q.finalize();
    //  Loco info
    sSQL.Format("SELECT car_number,road_name,description FROM Locomotives WHERE id=%d;",iLocoFK);
    q = pDB->execQuery(sSQL);
    CString sLoco("");
    if( !q.eof() )
        sLoco.Format("%d - %s %s",q.getIntField(0),q.getStringField("road_name"),q.getStringField("description"));
    q.finalize();
    //  Caboose info
    sSQL.Format("SELECT car_number,reporting_marks FROM Caboose WHERE id=%d;",iCabooseFK);
    q = pDB->execQuery(sSQL);
    CString sCaboose("________________________________");
    if( !q.eof() )
        sCaboose.Format("%d - %s",q.getIntField(0),q.fieldValue(1));
    q.finalize();
    //
    sSQL.Format("SELECT Locations.name FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order;",iTrainFK);
    q = pDB->execQuery(sSQL);
    CString sFirstLoc(""), sLastLoc("");
    while( !q.eof() )
    {
        if( sFirstLoc.IsEmpty() )
            sFirstLoc = q.getStringField("name");
        sLastLoc = q.getStringField("name");
        q.nextRow();
    }
    q.finalize();
    //
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_MoveToNextLine(page[iPage]);
    CString sFirstLine;
    CString sTrain;
    sTrain.Format("%s, %s",sTrainID,szDate);
    sFirstLine.Format("Train #   : %-57s         at: %s",sTrain,sFirstLoc);
    HPDF_Page_ShowText(page[iPage], sFirstLine);
    HPDF_Page_MoveToNextLine(page[iPage]);
    //
    CString sSecondLine;
    CTime theTime;
    CString s;
    theTime = CTime::GetCurrentTime();
    s += theTime.Format( "%I:%M %p" );
    sSecondLine.Format("Call Time : %-56s Origin     : %s",s,sFirstLoc);
    HPDF_Page_ShowText(page[iPage], sSecondLine);
    HPDF_Page_MoveToNextLine(page[iPage]);
    //
    CString sThirdLine;
    sThirdLine.Format("Clearance#: %02d-%03d %49s Destination: %s",rand() % 100,rand() % 1000,"",sLastLoc);
    HPDF_Page_ShowText(page[iPage], sThirdLine);
    HPDF_Page_MoveToNextLine(page[iPage]);
    //
    CString sFourthLine;
    sFourthLine.Format("Engine    : %s    Cabin: %s",sLoco,sCaboose);
    HPDF_Page_ShowText(page[iPage], sFourthLine);
    HPDF_Page_MoveToNextLine(page[iPage]);
    //
    HPDF_Page_ShowText(page[iPage], sSeparator);
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_ShowText(page[iPage], "## Car Number  E LG KND  Consignee                SPOT          NxtLoc        Tons Contents   Move");
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_ShowText(page[iPage], sSeparator);
    HPDF_Page_MoveToNextLine(page[iPage]);

    sSQL.Format("SELECT Locations.name,Locations.id,Locations.notes FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order;",iTrainFK);
    q = pDB->execQuery(sSQL);
    //
    int iLocationCount=0;
    int iMove=1;
    int iTonnage=0;
    int iTotalTonnage=0;
    int iTotalFeet=0;
    int iPickups=0;
    int iSetouts=0;
    int iLoads=0;
    int iEmpties=0;
    //
    while (!q.eof())
    {
        int iLocationID = q.getIntField("id");
        CString sLocation(q.getStringField("name"));
        CString sNotes(q.getStringField("notes"));
        CString sLocationUpper(sLocation);
        sLocationUpper.MakeUpper();
        if( iLocationCount++ == 0 )
        {
            sprintf(pszLine,"CONSIST ON LEAVING %s",sLocationUpper);
            HPDF_Page_ShowText(page[iPage], pszLine);
        }
        else
        {
            //sprintf(pszLine,"ARRIVE %s WITH %d CARS",sLocation,pCarTable.size());
            //HPDF_Page_ShowText(page[iPage], pszLine);
        }
        //HPDF_Page_ShowText(page[iPage], pszLine);
        if( !sNotes.IsEmpty() )
        {
            sNotes = "*** " + sNotes + " ***";
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], (LPCTSTR)sNotes);
        }
        CString sSQL;
        HPDF_Page_MoveToNextLine(page[iPage]);
        //
        //  should this be sidings first? maybe
        //
        //  select all sidings for this location, then any industries in it.  
        //  show industries and cars, then show non-industry cars waiting pickup
        //
        sSQL.Format("SELECT Industries.id,Industries.name,Industries.notes FROM Trains_Industry,Industries WHERE Train_FK=%d AND Trains_Industry.Location_FK=%d AND Trains_Industry.Industry_FK=Industries.id",iTrainFK,iLocationID);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            CString sIndustry(q2.getStringField("name"));
            CString sNotes(q2.getStringField("notes"));
            //
            //  cars to drop off (If Industry_to_FK = Industry_from_FK, then only pick up)
            //
            sSQL.Format("SELECT Cars.reporting_marks || ' ' || Cars.car_number AS repmarks ,CarTypes.type_id,Cars.id, Trains_Scheduled_Cars.loadempty,car_length,tonnage,Location_to_FK, Location_from_FK FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Industry_to_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK AND Industry_to_FK != Industry_from_FK;",iTrains_Scheduled_FK,q2.getIntField(0));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            int iLine=0;
            while(!q3.eof())
            {
                //  print industry notes if we have them
                if( iLine++ == 0 )
                {
                    if( !sNotes.IsEmpty() )
                    {
                        sNotes = "*** " + sNotes + " ***";
                        HPDF_Page_ShowText(page[iPage], (LPCTSTR)sNotes);
                        HPDF_Page_MoveToNextLine(page[iPage]);
                    }
                }
                //
                int iFeet = q3.getIntField("car_length")==-1?g_iCarLength:q3.getIntField("car_length");
                BOOL bMove = q3.getIntField("Location_to_FK") == q3.getIntField("Location_from_FK");
                iTotalFeet -= iFeet;
                sprintf(pszLine,"%2d %-11s %s %2d %-4s %-24s %-13s %-13s  %d  %9s  %3s",
                    iMove++,CString(q3.getStringField("repmarks")).Left(11),q3.getIntField("loadempty")==1?"E":"L",
                    iFeet,q3.getStringField("type_id"),sIndustry.Left(24),sLocation.Left(13),"",
                    q3.getIntField("tonnage")<0?35:q3.getIntField("tonnage"),q3.getIntField("loadempty")==1?"":"Gen Mrch",bMove?"MOV":" IN");
                //
                iTonnage -= q3.getIntField("tonnage")<0?35:q3.getIntField("tonnage");
                iSetouts++;
                if( q3.getIntField("loadempty") == 0 )
                    iLoads++;
                else
                    iEmpties++;
                //
                for( int i=0;i<pCarTable.size();i++)
                {
                    CCar pCar;
                    pCar = pCarTable.at(i);
                    if( pCar.m_iCar_FK == q3.getIntField("id"))
                    {
                        pCarTable.erase(pCarTable.begin()+i);
                        break;
                    }
                }
                HPDF_Page_ShowText(page[iPage], pszLine);
                HPDF_Page_MoveToNextLine(page[iPage]);
                q3.nextRow();
            }
            q3.finalize();
            sSQL.Format("SELECT Cars.reporting_marks || ' ' || Cars.car_number AS repmarks,CarTypes.type_id,Cars.id, Trains_Scheduled_Cars.loadempty,car_length,Industry_to_FK,Location_to_FK,tonnage,Location_from_FK FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Industry_from_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK;",iTrains_Scheduled_FK,q2.getIntField(0));
            q3 = pDB->execQuery(sSQL);
            while(!q3.eof())
            {
                int iIndustryToFK = q3.getIntField("Industry_to_FK");
                int iLocationToFK = q3.getIntField("Location_to_FK");
                CString sTo("");
                if( iIndustryToFK > 0 )
                    sSQL.Format("SELECT name FROM Industries WHERE id=%d",iIndustryToFK);
                else if( iLocationToFK > 0 )
                    sSQL.Format("SELECT name FROM Locations WHERE id=%d",iLocationToFK);
                else
                    sSQL.Format("SELECT Locations.name FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order DESC;",iTrainFK);

                CppSQLite3Query q4 = pDB->execQuery(sSQL);
                if( !q4.eof() )
                {
                    sTo = q4.getStringField("name");
                }
                int iFeet = q3.getIntField("car_length")==-1?g_iCarLength:q3.getIntField("car_length");
                BOOL bMove = q3.getIntField("Location_to_FK") == q3.getIntField("Location_from_FK");
                iTotalFeet += iFeet;
                sprintf(pszLine,"%2d %-11s %s %2d %-4s %-24s %-13s %-13s  %d  %9s  %3s",
                    iMove++,CString(q3.getStringField("repmarks")).Left(11),q3.getIntField("loadempty")==1?"E":"L",
                    iFeet,q3.getStringField("type_id"),sIndustry.Left(24),sLocation.Left(13),sTo.Left(13),
                    q3.getIntField("tonnage")<0?35:q3.getIntField("tonnage"),q3.getIntField("loadempty")==1?"":"Gen Mrch",bMove?"MOV":"OUT");
                iTonnage += q3.getIntField("tonnage")<0?35:q3.getIntField("tonnage");
                iTotalTonnage += q3.getIntField("tonnage")<0?35:q3.getIntField("tonnage");
                iPickups++;
                if( q3.getIntField("loadempty") == 0 )
                    iLoads++;
                else
                    iEmpties++;
                //
                CCar pCar;
                pCar.m_iCar_FK = q3.getIntField("id");
                pCarTable.push_back(pCar);
                //TRACE("%s\n",pszLine);
                HPDF_Page_ShowText(page[iPage], pszLine);
                HPDF_Page_MoveToNextLine(page[iPage]);
                q3.nextRow();
            }
            q3.finalize();
            //
            q2.nextRow();
        }
        q2.finalize(); // end industries
        //
        //  non-industry cars for this location?
        //
        sSQL.Format("SELECT Cars.reporting_marks || ' ' || Cars.car_number AS repmarks,CarTypes.type_id,Cars.id,car_length,Industry_to_FK,Location_to_FK,tonnage,Trains_Scheduled_Cars.loadempty,Location_from_FK FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Location_from_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND Industry_from_FK in(-1,0) AND CarTypes.id = Cars.CarTypes_FK;",iTrains_Scheduled_FK,iLocationID);
//        sSQL.Format("SELECT Cars.reporting_marks || ' ' || Cars.car_number AS repmarks,CarTypes.type_id,Cars.id, Trains_Scheduled_Cars.loadempty,car_length,Industry_to_FK,Location_to_FK,tonnage FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Location_to_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK;",iTrains_Scheduled_FK,iLocationID);
        q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            int iIndustryToFK = q2.getIntField("Industry_to_FK");
            int iLocationToFK = q2.getIntField("Location_to_FK");
            CString sTo("");
            if( iIndustryToFK > 0 )
                sSQL.Format("SELECT name FROM Industries WHERE id=%d",iIndustryToFK);
            else if( iLocationToFK > 0 )
                sSQL.Format("SELECT name FROM Locations WHERE id=%d",iLocationToFK);
            else
                sSQL.Format("SELECT Locations.name FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order DESC;",iTrainFK);
            
            CppSQLite3Query q4 = pDB->execQuery(sSQL);
            if( !q4.eof() )
            {
                sTo = q4.getStringField("name");
            }
            //
            int iFeet = q2.getIntField("car_length")==-1?g_iCarLength:q2.getIntField("car_length");
            BOOL bMove = q2.getIntField("Location_to_FK") == q2.getIntField("Location_from_FK");
            iTotalFeet += iFeet;
            sprintf(pszLine,"%2d %-11s %s %2d %-4s %-24s %-13s %-13s  %d  %9s  %3s",
                iMove++,CString(q2.getStringField("repmarks")).Left(11),q2.getIntField("loadempty")==1?"E":"L",
                iFeet,q2.getStringField("type_id"),"Agent",sLocation.Left(13),sTo.Left(13),
                q2.getIntField("tonnage")<0?35:q2.getIntField("tonnage"),q2.getIntField("loadempty")==1?"":"Gen Mrch",bMove?"MOV":"OUT");
            iTonnage += q2.getIntField("tonnage")<0?35:q2.getIntField("tonnage");
            iTotalTonnage += q2.getIntField("tonnage")<0?35:q2.getIntField("tonnage");
            iPickups++;
            if( q2.getIntField("loadempty") == 0 )
                iLoads++;
            else
                iEmpties++;
            //
            CCar pCar;
            pCar.m_iCar_FK = q2.getIntField("id");
            pCarTable.push_back(pCar);
            /*
            for( int i=0;i<pCarTable.size();i++)
            {
                CCar pCar;
                pCar = pCarTable.at(i);
                if( pCar.m_iCar_FK == q2.getIntField("id"))
                {
                    pCarTable.erase(pCarTable.begin()+i);
                    break;
                }
            }
            */
            //
//            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], pszLine);
            HPDF_Page_MoveToNextLine(page[iPage]);
            q2.nextRow();
        }
        q2.finalize();
        //
        //  off-layout bound cars dropped off at any sidings here?
        //

        //
        q.nextRow();
        //
        //  leave with X cars, except at last location
        //
        if( !q.eof() )
        {
            CString sLeaveLine;
            sLeaveLine.Format("DEPART %s WITH %d CARS, %d TONS %d INCHES",sLocationUpper,pCarTable.size(),iTonnage,iTotalFeet);
            ////HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], sLeaveLine);
            //HPDF_Page_MoveToNextLine(page[iPage]);
        }
        else
        {
            CString sArriveLine;
            sArriveLine.Format("ARRIVE %s",sLocationUpper);
            HPDF_Page_ShowText(page[iPage], sArriveLine);
            for( int i=0;i<pCarTable.size();i++)
            {
                CCar pCar;
                pCar = pCarTable.at(i);
                //sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id FROM Cars,CarTypes WHERE Cars.id=%d AND CarTypes.id = Cars.CarTypes_FK;",pCar.m_iCar_FK);
                sSQL.Format("SELECT Cars.reporting_marks || ' ' || Cars.car_number AS repmarks,CarTypes.type_id,Cars.id, Trains_Scheduled_Cars.loadempty,Industry_to_FK,Location_to_FK,car_length,tonnage FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Cars.id=%d AND CarTypes.id = Cars.CarTypes_FK;",pCar.m_iCar_FK);
                q2 = pDB->execQuery(sSQL);
                if( !q2.eof() )
                {
                    CString sCarLine;
                    int iFeet = q2.getIntField("car_length")==-1?g_iCarLength:q2.getIntField("car_length");
                    iTotalFeet += iFeet;
                    sCarLine.Format("%2d %-11s %s %2d %-4s %-24s %-13s %-13s  %d  %9s   IN", //  OUT
                        iMove++,CString(q2.getStringField("repmarks")).Left(11),q2.getIntField("loadempty")==1?"E":"L",iFeet,q2.getStringField("type_id"),"",sLocation.Left(13),q2.getIntField("loadempty")==1?"":"Gen Mrch",q2.getIntField("tonnage")<0?35:q2.getIntField("tonnage"),"");
                    HPDF_Page_MoveToNextLine(page[iPage]);
                    HPDF_Page_ShowText(page[iPage], sCarLine);
                }
                q2.finalize();
            }
        }
    }
    q.finalize();
    //
    //  Footer
    //
    HPDF_Page_MoveTextPos(page[iPage],25,10);
    HPDF_Page_SetTextLeading(page[iPage],12);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, 14);
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_TextOut(page[iPage],25,120,"               *** Couple Cars at 4 mph or less ***");
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_SetTextLeading(page[iPage],9);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE-2.5);
    //
    HPDF_Page_ShowText(page[iPage], sSeparator);
    HPDF_Page_MoveToNextLine(page[iPage]);
    CString sText;
    sText.Format("CARS  Loads     : %2d               Pickups   : %2d              Total Weight: %4d tons",iLoads,iPickups,iTotalTonnage);
    HPDF_Page_ShowText(page[iPage], sText);
    HPDF_Page_MoveToNextLine(page[iPage]);
    sText.Format("      Empties   : %2d               Setouts   : %2d              Total Length: %4d inches",iEmpties,iSetouts,iTotalFeet);
    HPDF_Page_ShowText(page[iPage], sText);
    HPDF_Page_MoveToNextLine(page[iPage]);
    sText.Format("      Total Cars: %2d               Movements : %2d",iLoads+iEmpties,iSetouts+iPickups);
    HPDF_Page_ShowText(page[iPage], sText);
    HPDF_Page_MoveToNextLine(page[iPage]);
    //
    HPDF_Page_ShowText(page[iPage], sSeparator);
    HPDF_Page_MoveToNextLine(page[iPage]);
    sText.Format("EXCPEPTIONS  Average weight/car: %3.1f tons.  Tons/operative brake: %3.1f",(float)(iTotalTonnage/(iSetouts+iPickups)),(float)(iTotalTonnage/((iSetouts+iPickups)+2)));
    HPDF_Page_ShowText(page[iPage], sText);
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_ShowText(page[iPage], "             This train is limited to: 45 mph");
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_ShowText(page[iPage], "             *** Movement on other than main tracks must not exceed 15 mph ***");
    HPDF_Page_MoveToNextLine(page[iPage]);
    //
    HPDF_Page_ShowText(page[iPage], sSeparator);
    HPDF_Page_MoveToNextLine(page[iPage]);
    //    
    HPDF_Page_EndText (page[iPage]);
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    ShowPDF(fname);
    return;
}

void PrintSimpleSwitchlist(int iTrains_Scheduled_FK)
{
    std::vector<CCar> pCarTable;
    //
    //  Get the route for this scheduled train,
    //  and display car setouts/pickups per industry per location
    //
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

    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page[100];
    int iPage = 0;
    HPDF_Font detail_font;
    HPDF_Image image(0);
    const char *detail_font_name;
    char pszLine[255];

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\switchlist%d.pdf",fname,iRand);
    Sleep(2000);
    //unlink(fname);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"Unable to create switchlist!  Check for previously opened ones!","TrainOps Error",MB_ICONSTOP);
        return;
    }

    /* declaration for using Japanese encoding. */
    HPDF_UseJPEncodings (pdf);
    
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
    
    /* load ttf file */
    ::GetCurrentDirectory(254,pszLine);
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, ".\\TYPIW___.TTF", HPDF_TRUE);
    
    /* add a new page object. */
    page[iPage] = HPDF_AddPage (pdf);
    //
    Watermark(pdf,page[iPage]);
    //
    detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
    
    HPDF_Page_BeginText (page[iPage]);
    HPDF_Page_SetSize(page[iPage],HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
    HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE);
    //
    HPDF_Page_EndText (page[iPage]);
    //
    HPDF_Page_BeginText (page[iPage]);
    //
    // write text
    CString sSQL;
    sSQL.Format("SELECT name,trainid,Trains_FK,Locomotive_FK,Caboose_FK FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.id=%d AND Trains_Scheduled.Trains_FK=Trains.id;",iTrains_Scheduled_FK);
    q = pDB->execQuery(sSQL);
    char szTitle[255];
    if( !q.eof() )
        sprintf(szTitle,"Switchlist - Train %s (%s) - %s\n%s",q.getStringField("trainid"),q.getStringField("name"),szDate,g_sRRName);
    int iTrainFK = q.getIntField("Trains_FK");
    int iLocoFK = q.getIntField("Locomotive_FK");
    int iCabooseFK = q.getIntField("Caboose_FK");
    q.finalize();
    //  Loco info
    sSQL.Format("SELECT car_number,road_name,description FROM Locomotives WHERE id=%d;",iLocoFK);
    q = pDB->execQuery(sSQL);
    CString sLoco("");
    if( !q.eof() )
        sLoco.Format("Locomotive:  %d - %s %s",q.getIntField("car_number"),q.getStringField("road_name"),q.getStringField("description"));
    q.finalize();
    //  Caboose info
    sSQL.Format("SELECT car_number,reporting_marks FROM Caboose WHERE id=%d;",iCabooseFK);
    q = pDB->execQuery(sSQL);
    CString sCaboose("Caboose:     ________________________________");
    if( !q.eof() )
        sCaboose.Format("Caboose:     %d - %s",q.getIntField("car_number"),q.getStringField("reporting_marks"));
    q.finalize();
    HPDF_Page_TextRect(page[iPage],10,770,580,740,szTitle,HPDF_TALIGN_CENTER,0);
//    HPDF_Page_MoveTextPos(page, -60, -12);
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_ShowText(page[iPage], sLoco);
    HPDF_Page_MoveToNextLine(page[iPage]);
    HPDF_Page_ShowText(page[iPage], sCaboose);

    sSQL.Format("SELECT Locations.name,Locations.id,Locations.notes FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order;",iTrainFK);
    q = pDB->execQuery(sSQL);
    //
    int iLocationCount = 0;
    int iTons = 0;
    //
    while (!q.eof())
    {
        int iLocationID = q.getIntField("id");
        CString sLocation(q.getStringField("name"));
        CString sNotes(q.getStringField("notes"));
        if( iLocationCount++ == 0 )
            sprintf(pszLine,"Assemble train at %s",sLocation);
        else
            sprintf(pszLine,"Proceed to %s, arrive with %d cars",sLocation,pCarTable.size());
        HPDF_Page_MoveToNextLine(page[iPage]);
        HPDF_Page_MoveToNextLine(page[iPage]);
        HPDF_Page_ShowText(page[iPage], pszLine);
        if( !sNotes.IsEmpty() )
        {
            sNotes = " ** " + sNotes + " **";
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], (LPCTSTR)sNotes);
        }
        CString sSQL;
        //
        //  get sidings where Location_FK=this location
        //
        /*
        sSQL.Format("SELECT id,name FROM Sidings WHERE Locations_FK=%d;",iLocationID);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while( !q2.eof() )
        {
            //
            //  find cars at industries on this siding
            //  (select from Trains_Scheduled_Cars where siding_fk= order by industry_fk)
            //
            //  cars NOT at industries on this siding will have an industry_FK of -1 (?)
            //
            sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars WHERE Trains_Scheduled_FK=%d AND Siding_from_FK=%d ORDER BY Industry_from_FK",
                iTrains_Scheduled_FK,q2.getIntField("id"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            while( !q3.eof() )
            {
                MessageBox(0,q3.getStringField("Cars_FK"),"DEBUG",MB_OK);
                q3.nextRow();
            }
            q3.finalize();

            q2.nextRow();
        }
        q2.finalize();
        */
        //
        //  local moves for this locatoin (Location_from_FK == Location_to_FK && Location_from_FK=%d)
        //  build a "moved cars" string, too, so we don't look for it in any of the queries below
        //
        CString sLocalMoves("");
        sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id,Industry_to_FK, Industry_from_FK, Siding_to_FK,Siding_from_FK FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Location_to_FK=%d AND Location_to_FK=Location_from_FK AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK;",iTrains_Scheduled_FK,iLocationID);
        CppSQLite3Query q2a = pDB->execQuery(sSQL);
        while(!q2a.eof() )
        {
            CString sWork;
            sWork.Format("%d,%s",q2a.getIntField("id"),sLocalMoves);
            sLocalMoves = sWork;
            //
            CString sLocalMove;
            //
            //  if industry from == -1, use siding from
            //
            CString sFrom("");
            if( q2a.getIntField("Industry_from_FK") == -1 )
                sSQL.Format("SELECT name FROM Sidings WHERE id=%d",q2a.getIntField("Siding_from_FK"));
            else
                sSQL.Format("SELECT name FROM Industries WHERE id=%d",q2a.getIntField("Industry_from_FK"));
            CppSQLite3Query q2b = pDB->execQuery(sSQL);
            if( !q2b.eof() )
                sFrom = q2b.getStringField("name");
            q2b.finalize();
            //
            //  if industry to == -1, use siding to
            //
            CString sTo("");
            if( q2a.getIntField("Industry_to_FK") == -1 )
                sSQL.Format("SELECT name FROM Sidings WHERE id=%d",q2a.getIntField("Siding_to_FK"));
            else
                sSQL.Format("SELECT name FROM Industries WHERE id=%d",q2a.getIntField("Industry_to_FK"));
            q2b = pDB->execQuery(sSQL);
            if( !q2b.eof() )
                sTo = q2b.getStringField("name");
            q2b.finalize();
            //
            sLocalMove.Format("    Move: %s %s %s / %s to %s",
                q2a.getStringField("description"),q2a.getStringField("car_number"),
                q2a.getStringField("reporting_marks"),sFrom,sTo);
            //
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], (LPCTSTR)sLocalMove);
            q2a.nextRow();
        }
        q2a.finalize();
        //
        int iLen = sLocalMoves.GetLength();
        if( iLen > 0 )
        {
            if( sLocalMoves.GetAt(iLen-1) == ',' )
            {
                sLocalMoves = sLocalMoves.Left(iLen-1);
            }
        }
        //
        //  select all industries in for this location, 
        //      show cars FOR industries
        //      show cars FROM industries
        //
        sSQL.Format("SELECT Industries.id,Industries.name,Industries.notes FROM Trains_Industry,Industries WHERE Train_FK=%d AND Trains_Industry.Location_FK=%d AND Trains_Industry.Industry_FK=Industries.id",iTrainFK,iLocationID);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            CString sIndustry;
            CString sNotes(q2.getStringField("notes"));
            sIndustry.Format("  Industry: %s",q2.getStringField("name"));
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], sIndustry);
            if( !sNotes.IsEmpty() )
            {
                sNotes = "   ** " + sNotes + " **";
                HPDF_Page_MoveToNextLine(page[iPage]);
                HPDF_Page_ShowText(page[iPage], (LPCTSTR)sNotes);
            }
            //
            //  cars to drop off
            //
            //sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Industry_to_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK AND Industry_to_FK != Industry_from_FK;",iTrains_Scheduled_FK,q2.getIntField(0));
            sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id,Cars.tonnage FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Industry_to_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK AND Cars.id NOT IN (%s);",iTrains_Scheduled_FK,q2.getIntField("id"),sLocalMoves);
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            while(!q3.eof())
            {
                sprintf(pszLine,"    Drop off: %s %s %s",q3.getStringField("description"),q3.getStringField("car_number"),q3.getStringField("reporting_marks"));

                for( int i=0;i<pCarTable.size();i++)
                {
                    CCar pCar;
                    pCar = pCarTable.at(i);
                    if( pCar.m_iCar_FK == q3.getIntField(3))
                    {
                        pCarTable.erase(pCarTable.begin()+i);
                        break;
                    }
                }
                iTons -= q3.getIntField("tonnage");
                HPDF_Page_MoveToNextLine(page[iPage]);
                HPDF_Page_ShowText(page[iPage], pszLine);
                q3.nextRow();
            }
            q3.finalize();
            //
            //  cars to pick up
            //
            sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id, Cars.tonnage FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Industry_from_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK AND Cars.id NOT IN (%s);",iTrains_Scheduled_FK,q2.getIntField(0),sLocalMoves);
            q3 = pDB->execQuery(sSQL);
            while(!q3.eof())
            {
                sprintf(pszLine,"    Pick up : %s %s %s",q3.getStringField("description"),q3.getStringField("car_number"),q3.getStringField("reporting_marks"));
                CCar pCar;
                pCar.m_iCar_FK = q3.getIntField("id");
                pCarTable.push_back(pCar);
                iTons += q3.getIntField("tonnage");
                //TRACE("%s\n",pszLine);
                HPDF_Page_MoveToNextLine(page[iPage]);
                HPDF_Page_ShowText(page[iPage], pszLine);
                q3.nextRow();
            }
            q3.finalize();
            //
            q2.nextRow();
        }
        q2.finalize(); // end industries pickups and drop-offs
        //
        //  non-industry cars from this location?
        //
        sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id,Cars.tonnage FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Location_from_FK=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND Industry_from_FK in(-1,0) AND CarTypes.id = Cars.CarTypes_FK AND Cars.id NOT IN (%s);",iTrains_Scheduled_FK,iLocationID,sLocalMoves);
        q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            sprintf(pszLine,"  Pick-up from %s Yardmaster: %s %s %s",sLocation,q2.getStringField("description"),q2.getStringField("car_number"),q2.getStringField("reporting_marks"));
            ASSERT(pszLine);
            //
            CCar pCar;
            pCar.m_iCar_FK = q2.getIntField("id");
            pCarTable.push_back(pCar);
            iTons += q2.getIntField("tonnage");
            //
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], pszLine);
            q2.nextRow();
        }
        q2.finalize();
        //
        //  off-layout bound cars dropped off at any sidings here?
        //
        sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id,Siding_to_FK,Cars.tonnage FROM Trains_Scheduled_Cars,Cars,CarTypes WHERE Trains_Scheduled_FK=%d AND Location_to_FK=%d AND Industry_to_FK=-1 AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND CarTypes.id = Cars.CarTypes_FK AND Cars.id NOT IN (%s);",iTrains_Scheduled_FK,iLocationID,sLocalMoves);
        q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            sSQL.Format("SELECT name,interchange,yard FROM Sidings WHERE id=%d;",q2.getIntField("Siding_to_FK"));
            CppSQLite3Query q2a = pDB->execQuery(sSQL);
            CString sSidingType("Siding");
            if( !q2a.eof() )
            {
                sSidingType.Format("%s Siding",q2a.getStringField("name"));
                //if( q2a.getIntField("interchange") == 1 )
                //    sSidingType = "Interchange";
                //if( q2a.getIntField("yard") == 1 )
                //    sSidingType = "Yard";
            }
            q2a.finalize();

            sprintf(pszLine,"  %s Drop-off: %s %s %s",sSidingType,q2.getStringField("description"),q2.getStringField("car_number"),q2.getStringField("reporting_marks"));
            //
            for( int i=0;i<pCarTable.size();i++)
            {
                CCar pCar;
                pCar = pCarTable.at(i);
                if( pCar.m_iCar_FK == q2.getIntField("id"))
                {
                    pCarTable.erase(pCarTable.begin()+i);
                    break;
                }
            }
            iTons += q2.getIntField("tonnage");
            //
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], pszLine);
            q2.nextRow();
        }
        q2.finalize();

        //
        q.nextRow();
        //
        //  leave with X cars, except at last location
        //
        if( !q.eof() )
        {
            CString sLeaveLine;
            sLeaveLine.Format("Leave %s with %d cars (%d tons)",sLocation,pCarTable.size(),iTons);
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], sLeaveLine);
        }
        else
        {
            for( int i=0;i<pCarTable.size();i++)
            {
                CCar pCar;
                pCar = pCarTable.at(i);
                sSQL.Format("SELECT Cars.car_number, Cars.reporting_marks,CarTypes.description,Cars.id FROM Cars,CarTypes WHERE Cars.id=%d AND CarTypes.id = Cars.CarTypes_FK;",pCar.m_iCar_FK);
                q2 = pDB->execQuery(sSQL);
                if( !q2.eof() )
                {
                    CString sCarLine;
                    sCarLine.Format("    Drop Off : %s %s %s",q2.getStringField("description"),q2.getStringField("car_number"),q2.getStringField("reporting_marks"));
                    HPDF_Page_MoveToNextLine(page[iPage]);
                    HPDF_Page_ShowText(page[iPage], sCarLine);
                }
                q2.finalize();
            }
        }
    }
    q.finalize();
    //
    //  
    //

    /*
    HPDF_Page_MoveToNextLine(page[iPage]);
    for( int i=0;i<100;i++)
    {
        HPDF_Point p = HPDF_Page_GetCurrentTextPos(page[iPage]);
        TRACE("x=%f, y=%f\n",p.x,p.y);
        if( p.y < 50 )
        {
            HPDF_Page_EndText (page[iPage]);
            iPage++;
            page[iPage] = HPDF_AddPage (pdf);
            HPDF_Page_SetSize(page[iPage],HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
            HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE);
            HPDF_Page_BeginText(page[iPage]);
            HPDF_Page_MoveTextPos(page[iPage], 0, HPDF_Page_GetHeight(page[iPage]));
        }

        HPDF_Page_MoveToNextLine(page[iPage]);
        HPDF_Page_ShowText(page[iPage], "***Terminate train***");
    }
*/
    HPDF_Page_EndText (page[iPage]);

    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    ShowPDF(fname);
    return;

}

//
//  get then increment train order number in registry
//
int GetTrainOrderNumber()
{
    int iTrainOrder = ((CTrainOpsApp*)AfxGetApp())->GetProfileInt("Settings","TrainOrderNumber",100);
    ((CTrainOpsApp*)AfxGetApp())->WriteProfileInt("Settings","TrainOrderNumber",++iTrainOrder);
    return iTrainOrder;
}

CString GetOperator(const CString & characters)
{
    CString result;
    int len = 3;
    LPTSTR p = result.GetBuffer(len);
    for(int i = 0; i < len; i++)
    {
        p[i] = characters[rand() % characters.GetLength()];
    }
    result.ReleaseBuffer(len);
    return result;
} 

void PrintForm19s(int iTrains_Scheduled_FK)
{
    //
    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page[100];
    int iPage = 0;
    HPDF_Font detail_font;
    const char *detail_font_name;

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\form19_%d.pdf",fname,iRand);
    //unlink(fname);
    Sleep(2000);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"error: cannot create PdfDoc object","ERROR",MB_OK);
        return;
    }
    SetCursor(LoadCursor(NULL,IDC_WAIT));

    /* declaration for using Japanese encoding. */
    //HPDF_UseJPEncodings (pdf);
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* load ttf file */
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, "c:\\projects\\trainops\\AMANO___.TTF", HPDF_TRUE);
    //
    //detail_font = HPDF_GetFont (pdf, "Courier", "CP1250");
    detail_font = HPDF_GetFont (pdf, "Courier", "StandardEncoding");
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    char szDate[24];
    int y,m,d;
    if( !q.eof() )
    {
        JulianDate::JdToYmd( q.getIntField(0)+q.getIntField(1), &y,&m,&d);
        sprintf(szDate,"%2d/%2d/%d",m,d,y);
    }
    q.finalize();
    CTime t( 2008, m, d, 23, 50, 0 );
    CString sDate = t.Format( "%B %d" );
    t = t.GetCurrentTime();
    CString sTime = t.Format("%#I:%M %p");
    //
    //  counts and offsets for the 4 Form 19s on each page
    //
    int iFormPage=0;
    int iLeft[4] = {25,320,25,320};
    int iTop[4] = {775,775,380,380};
    //
    //  get all notes here
    //  Locations/notes and Industries/notes
    //
    CString sSQL;
    sSQL.Format("SELECT name,trainid,Trains_FK,Locomotive_FK,Caboose_FK,notes FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.id=%d AND Trains_Scheduled.Trains_FK=Trains.id;",iTrains_Scheduled_FK);
    q = pDB->execQuery(sSQL);
    int iTrainFK=0;
    int iLocoFK=0;
    int iCabooseFK=0;
    CString sTrainName("");
    CString sTrainID("");
    CString sTrainNotes("");
    if( !q.eof() )
    {
        sTrainName = q.getStringField("name");
        sTrainID = q.getStringField("trainid");
        iTrainFK = q.getIntField("Trains_FK");
        iLocoFK = q.getIntField("Locomotive_FK");
        iCabooseFK = q.getIntField("Caboose_FK");
        sTrainNotes = q.getStringField("notes");
    }
    q.finalize();
    //
    sSQL.Format("SELECT Locations.name,Locations.id,Locations.notes FROM Trains_Location,Locations WHERE Train_FK=%d AND Trains_Location.Location_FK=Locations.id ORDER BY route_order;",iTrainFK);
    q = pDB->execQuery(sSQL);
    //
    int iLocationCount = 0;
    CArray <CString,CString> pNotes;
    if( !sTrainNotes.IsEmpty() )
        pNotes.Add(sTrainNotes+"[");
    //
    int iTrainOrder = GetTrainOrderNumber();
    //
    while (!q.eof())
    {
        int iLocationID = q.getIntField("id");
        CString sLocation(q.getStringField("name"));
        CString sNotes(q.getStringField("notes"));
        //
        //  Industries for this location
        //
        sSQL.Format("SELECT Industries.notes,Industries.name FROM Trains_Industry, Industries WHERE Train_FK=%d AND Trains_Industry.Location_FK=%d AND Trains_Industry.Industry_FK = Industries.id;",iTrainFK,iLocationID);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        CArray <CString,CString> pIndustryNotes;
        while( !q2.eof() )
        {
            CString sNote;
            if( strlen(q2.getStringField("notes")) != 0 )
            {
                sNote.Format("%s at %s",q2.getStringField("notes"),q2.getStringField("name"));
                pIndustryNotes.Add(sNote);
            }
            q2.nextRow();
        }
        q2.finalize();

        if( sNotes != "" || pIndustryNotes.GetSize() > 0 )
        {
            for( int i=0;i<pIndustryNotes.GetSize();i++ )
            {
                if( sNotes.IsEmpty() && i==0 )
                    sNotes = pIndustryNotes.GetAt(i);
                else
                    sNotes += ", " + pIndustryNotes.GetAt(i);
            }
            sNotes += "[" + sLocation;
            pNotes.Add(sNotes);
        }
        q.nextRow();
    }
    q.finalize();
    //
    page[iPage] = HPDF_AddPage (pdf);
    Watermark(pdf,page[iPage]);
    BOOL bAddPage(FALSE);
    //
    for( int i=0;i<pNotes.GetSize();i++ )
    {
        if( bAddPage )
        {
            page[iPage] = HPDF_AddPage (pdf);
            Watermark(pdf,page[iPage]);
            bAddPage=FALSE;
        }
        CString sLocation = pNotes.GetAt(i).Mid(pNotes.GetAt(i).ReverseFind('[')+1);
        CString sNote = pNotes.GetAt(i).Left(pNotes.GetAt(i).ReverseFind('['));
        //
        HPDF_Page_BeginText (page[iPage]);
        HPDF_Page_SetSize(page[iPage],HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
        HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19+4);
        //
        char szPageTitle[255];
        sprintf(szPageTitle,"%s",g_sRRName);
        HPDF_Page_TextRect(page[iPage],iLeft[iFormPage],iTop[iFormPage],iLeft[iFormPage]+270,iTop[iFormPage]-20,szPageTitle,HPDF_TALIGN_CENTER,0);
        HPDF_Page_MoveTextPos(page[iPage], 187, -12);
        
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19+2);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+15,iTop[iFormPage]-10,"Form");
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19+3);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+18,iTop[iFormPage]-20,"19");
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19+2);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+234,iTop[iFormPage]-10,"Form");
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19+3);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+237,iTop[iFormPage]-20,"19");
        //
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19);
        //
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage],iTop[iFormPage]-40,"Train Order No. ___________");
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+185,iTop[iFormPage]-40,"Dated _______________    ____");
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+252,iTop[iFormPage]-40,CString(szDate).Mid(6,2));
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage],iTop[iFormPage]-65,"To C. & E. ________________________________________________________________________________");//640
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage],iTop[iFormPage]-80,"_______________________________________________________________  At _______________________");//610
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage],iTop[iFormPage]-95,"___________________________________________________________________________________________");//580
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+40,iTop[iFormPage]-335,"Each Employee Addressed Must Have A Copy Of This Order");
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage], iTop[iFormPage]-340,"___________________________________________________________________________________________");
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage], iTop[iFormPage]-352,"     Made                         Time              M                       Opr.");
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage], iTop[iFormPage]-360,"___________________________________________________________________________________________");
        //
        //detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
        detail_font = HPDF_GetFont (pdf, detail_font_name, "StandardEncoding");
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19+10);
        HPDF_Page_SetRGBFill (page[iPage], 0.0, 0.0, 0.75);
        //
        //  'pen' text
        //

        //CString sPage;
        //sPage.Format("Page %d",(iFormPage + (iPage * 4))+1);
        //HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+170,iTop[iFormPage],sPage);

        CString sTrainOrder;
        sTrainOrder.Format("%d",iTrainOrder);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+55,iTop[iFormPage]-40,sTrainOrder);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+205,iTop[iFormPage]-40,sDate);
        HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+258,iTop[iFormPage]-40,CString(szDate).Mid(8,2));
        HPDF_Page_TextOut(page[iPage], iLeft[iFormPage]+35,iTop[iFormPage]-65,"Train #" + sTrainID + ", " + sTrainName);
        if( sLocation.IsEmpty() )
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+35,iTop[iFormPage]-95,"General Train Orders");
        else
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+205,iTop[iFormPage]-80,sLocation);
        HPDF_Page_SetTextLeading (page[iPage], FONT_SIZE_FORM19+12);
        HPDF_Page_TextRect (page[iPage], iLeft[iFormPage], iTop[iFormPage]-100, iLeft[iFormPage]+280, iTop[iFormPage]-330,
            sNote, HPDF_TALIGN_LEFT, NULL);
        
        HPDF_Page_TextOut(page[iPage], iLeft[iFormPage]+28, iTop[iFormPage]-355,"Complete");
        HPDF_Page_TextOut(page[iPage], iLeft[iFormPage]+120, iTop[iFormPage]-355,sTime);
        HPDF_Page_TextOut(page[iPage], iLeft[iFormPage]+240, iTop[iFormPage]-355,GetOperator("ABCDEFGHIJKLMNPRSTVWY"));
        //
        HPDF_Page_SetRGBFill (page[iPage], 0.0, 0.0, 0.0);
        HPDF_Page_EndText (page[iPage]);
        //
        
        //
        //  Form 19 boxes
        //
        HPDF_Page_SetLineWidth(page[iPage],3);
        HPDF_Page_MoveTo (page[iPage], iLeft[iFormPage]-1,  iTop[iFormPage]);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage]+50, iTop[iFormPage]);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage]+50, iTop[iFormPage]-25);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage],    iTop[iFormPage]-25);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage],    iTop[iFormPage]);
        //
        HPDF_Page_MoveTo (page[iPage], iLeft[iFormPage]+219, iTop[iFormPage]);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage]+270, iTop[iFormPage]);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage]+270, iTop[iFormPage]-25);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage]+220, iTop[iFormPage]-25);
        HPDF_Page_LineTo (page[iPage], iLeft[iFormPage]+220, iTop[iFormPage]);
        //
        HPDF_Page_Stroke (page[iPage]);
        //
        detail_font = HPDF_GetFont (pdf, "Courier", "CP1250");
        HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE_FORM19);
        //
        iFormPage++;
        if( iFormPage > 3 )
        {
            iFormPage=0;
            iPage++;
            bAddPage=TRUE;
        }
    }
    //
    //  did we do anything?
    //
    if( sTrainNotes.IsEmpty() && pNotes.GetSize() == 0 )
    {
        HPDF_Free (pdf);
        return;
    }
    //
    HPDF_SaveToFile (pdf, fname);
    /* clean up */
    HPDF_Free (pdf);

    ShowPDF(fname);
    return;
}

void PrintBadOrderCards(int iTrains_Scheduled_FK)
{
    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page[100];
    int iPage = 0;
    HPDF_Font detail_font;
    const char *detail_font_name;

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\badorder_%d.pdf",fname,iRand);
    //unlink(fname);
    Sleep(2000);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"error: cannot create PdfDoc object","ERROR",MB_OK);
        return;
    }
    SetCursor(LoadCursor(NULL,IDC_WAIT));
    /* declaration for using Japanese encoding. */
    HPDF_UseJPEncodings (pdf);
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
    /* load ttf file */
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, ".\\AMANO___.TTF", HPDF_TRUE);
    //
    detail_font = HPDF_GetFont (pdf, "Courier", "CP1250");
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Cars_FK FROM Trains_Scheduled_Cars, Cars WHERE Trains_Scheduled_Cars.Trains_Scheduled_FK=%d AND Trains_Scheduled_Cars.Cars_FK=Cars.id AND out_of_service=1",iTrains_Scheduled_FK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    CArray <int,int> pBadOrderCars;
    while( !q.eof() )
    {
        pBadOrderCars.Add(q.getIntField("Cars_FK"));
        q.nextRow();
    }
    q.finalize();
    sSQL.Format("SELECT name,trainid,Trains.id FROM Trains_Scheduled,Trains WHERE Trains_Scheduled.id=%d AND Trains_Scheduled.Trains_FK=Trains.id;",iTrains_Scheduled_FK);
    q = pDB->execQuery(sSQL);
    CString sTrainName = q.getStringField("name");
    CString sTrainID = q.getStringField("trainid");
    int iTrainFK = q.getFloatField("id");
    q.finalize();
    //
    //  get first location, and a random location on the route
    //
    sSQL.Format("SELECT name FROM Trains_Location,Locations WHERE Train_FK=%d AND Locations.id=Trains_Location.Location_FK ORDER BY route_order;",iTrainFK);
    q = pDB->execQuery(sSQL);
    CString sFirstLocation;
    CString sDropLocation;
    while (!q.eof() )
    {
        if( sFirstLocation.IsEmpty() )
            sFirstLocation = q.getStringField("name");
        int iRand = (rand() % 100);
        if( iRand > 50 )
            sDropLocation = q.getStringField("name");
        q.nextRow();
    }
    q.finalize();
    if( sDropLocation.IsEmpty() )
        sDropLocation = sFirstLocation;
    //
    q = pDB->execQuery("SELECT rrdate,date FROM _Config WHERE id=1;");
    char szDate[24];
    int y,m,d;
    if( !q.eof() )
    {
        JulianDate::JdToYmd( q.getIntField(0)+q.getIntField(1), &y,&m,&d);
        sprintf(szDate,"%2d/%2d/%d",m,d,y);
    }
    q.finalize();
    CTime t( 2008, m, d, 23, 50, 0 );
    CString sDate = t.Format( "%B %d" );
    //t = t.GetCurrentTime();
    //CString sTime = t.Format("%#I:%M %p");
    //
    //  counts and offsets for the 4 bad order cards on each page
    //
    int iFormPage=0;
    int iLeft[4] = {25,25,25};
    int iTop[4] = {775,550,325};
    //
    page[iPage] = HPDF_AddPage (pdf);
    Watermark(pdf,page[iPage]);
    BOOL bAddPage(FALSE);
    //
    for( int i=0;i<pBadOrderCars.GetSize();i++ )
    {
        if( bAddPage )
        {
            page[iPage] = HPDF_AddPage (pdf);
            Watermark(pdf,page[iPage]);
            bAddPage=FALSE;
        }
        sSQL.Format("SELECT reporting_marks, car_number, Trains_Scheduled_Cars.loadempty FROM Cars,Trains_Scheduled_Cars WHERE Cars.id=%d AND Cars.id=Trains_Scheduled_Cars.Cars_FK AND Trains_Scheduled_FK=%d",pBadOrderCars.GetAt(i),iTrains_Scheduled_FK);
        q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            CString sReportingMarks = q.getStringField("reporting_marks");
            CString sCarNumber = q.getStringField("car_number");
            CString sLoadEmpty = q.getIntField("loadempty")==1?"Load":"Empty";
            TRACE("Bad order car %s %s\n",sReportingMarks,sCarNumber );

            HPDF_Page_SetSize(page[iPage],HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
            //
            HPDF_Page_SetRGBFill (page[iPage], 0, 0, 0);
            //
            HPDF_Page_SetLineWidth (page[iPage], 3);
            HPDF_Page_Rectangle(page[iPage], iLeft[iFormPage], iTop[iFormPage], HPDF_Page_GetWidth(page[iPage]) - 60,-200);
            HPDF_Page_MoveTo(page[iPage],iLeft[iFormPage]+60,iTop[iFormPage]-10);
            HPDF_Page_LineTo(page[iPage],iLeft[iFormPage]+60,iTop[iFormPage]-190);

            HPDF_Page_Stroke (page[iPage]);
            HPDF_Page_SetRGBFill (page[iPage], 0.9, 0.5, 0.5);
            HPDF_Page_Rectangle(page[iPage], iLeft[iFormPage]+90, iTop[iFormPage]-130, 450, 70);
            HPDF_Page_Fill (page[iPage]);

            //HPDF_Page_Stroke (page[iPage]);
            HPDF_Page_BeginText (page[iPage]);
            HPDF_Page_SetRGBFill (page[iPage], 0, 0, 0);

            float angle1 = 90;
            float rad1 = angle1 / 180 * 3.141592;

            HPDF_Page_SetTextRenderingMode (page[iPage], HPDF_FILL);
            HPDF_Font font = HPDF_GetFont (pdf, "Times-Roman", NULL);
            HPDF_Page_SetFontAndSize(page[iPage], font, FONT_SIZE+1);
            //
            HPDF_Page_SetTextMatrix (page[iPage], cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
                iLeft[iFormPage]+15, iTop[iFormPage]-190);
            HPDF_Page_ShowText (page[iPage], "So State: When damage is due to derailment,");
            //
            HPDF_Page_SetTextMatrix (page[iPage], cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
                iLeft[iFormPage]+25, iTop[iFormPage]-190);
            HPDF_Page_ShowText (page[iPage], "cornering, sideswiping, telescoping or other");
            //
            HPDF_Page_SetTextMatrix (page[iPage], cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
                iLeft[iFormPage]+35, iTop[iFormPage]-190);
            HPDF_Page_ShowText (page[iPage], "handling line responsibility conditionis.");
            //
            HPDF_Page_EndText (page[iPage]);
            //
            HPDF_Page_BeginText (page[iPage]);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-18,"Form 297-Rev.");
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+415,iTop[iFormPage]-18,"Train __________________");
            HPDF_Page_SetFontAndSize(page[iPage], font, FONT_SIZE+10);
            HPDF_Page_TextRect(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-20,iLeft[iFormPage]+500,iTop[iFormPage]-20,g_sRRName,HPDF_TALIGN_CENTER,0);
            HPDF_Page_SetFontAndSize(page[iPage], font, FONT_SIZE+2);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-55,"TO ______________________________________ SHOP ___________________________");
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+503,iTop[iFormPage]-55,CString(szDate).Mid(6,2));
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+518,iTop[iFormPage]-55,"____");
            //
            HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE+10);
            HPDF_Page_SetTextRenderingMode (page[iPage], HPDF_STROKE);
            font = HPDF_GetFont (pdf, "Helvetica-Bold", NULL);
            HPDF_Page_SetFontAndSize(page[iPage], font, FONT_SIZE+50);
            //
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+130,iTop[iFormPage]-115,"BAD ORDER");
            HPDF_Page_SetTextRenderingMode (page[iPage], HPDF_FILL);
            HPDF_Page_SetFontAndSize(page[iPage], font, FONT_SIZE+2);
            HPDF_Page_EndText (page[iPage]);
            HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
            //
            font = HPDF_GetFont (pdf, "Times-Roman", NULL);
            HPDF_Page_SetFontAndSize(page[iPage], font, FONT_SIZE+2);
            HPDF_Page_BeginText (page[iPage]);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-145,"DEFECTS: ________________________________________________________________________");
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-160," _________________________________________________________________________________");
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-175,"Car Initials_________________________No. ______________________Loaded or Empty________");
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+90,iTop[iFormPage]-190,"Place Carded___________________________________________Inspector ____________________");
            
            //
            //  'pen' text
            //
            //detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
            HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE+9);
            HPDF_Page_SetRGBFill (page[iPage], 0.0, 0.0, 0.75);
            HPDF_Page_TextOut(page[iPage], iLeft[iFormPage]+450,iTop[iFormPage]-19,"#"+sTrainID);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+440,iTop[iFormPage]-55,sDate);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+520,iTop[iFormPage]-55,CString(szDate).Mid(8,2));
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+110,iTop[iFormPage]-55,sDropLocation);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+145,iTop[iFormPage]-175,sReportingMarks);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+305,iTop[iFormPage]-175,sCarNumber);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+500,iTop[iFormPage]-175,sLoadEmpty);
            HPDF_Page_TextOut(page[iPage],iLeft[iFormPage]+165,iTop[iFormPage]-190,sFirstLocation);
            HPDF_Page_TextOut(page[iPage], iLeft[iFormPage]+450, iTop[iFormPage]-190,GetOperator("ABCDEFGHIJKLMNPRSTVWY"));
            //
            HPDF_Page_EndText (page[iPage]);
            //
            iFormPage++;
            if( iFormPage > 2 )
            {
                iFormPage=0;
                iPage++;
                bAddPage=TRUE;
            }
        }
        q.finalize();
        //
    }
    //
    HPDF_SaveToFile (pdf, fname);
    /* clean up */
    HPDF_Free (pdf);
    //
    ShowPDF(fname);
}

void DoCarsNotFoundReport(int iTrainFK,int iDate)
{
    /*
    char szPath[MAX_PATH+1];
    GetTempPath(MAX_PATH,szPath);
    sprintf(szPath,"%s\\carnotfoundreport.pdf",szPath);
    
    h_pdf hPdf=0;
    unsigned int nSkipChars;
    // Create the pdf file
    PdfxCreate(szPath, eCentimeter, &hPdf);
    if( !hPdf )
    {
        MessageBox(0,"Unable to create car not found report!  Check for previously opened ones!","TrainOps Error",MB_ICONSTOP);
        return;
    }
    g_iPageNumber = 1;
    PdfxNewStdPage(hPdf,eUS_A,ePortrait);
    // set some pdf information
    PdfxSetTitle(hPdf, "TrainOps Car Not Found Report");
    PdfxSetAuthor(hPdf, "TrainOps");
    PdfxSetSubject(hPdf, "Car Not Found Report");
    PdfxSetKeywords(hPdf, "C++");
    // set current font and paragraph style
    PdfxSetStdFont(hPdf, eCourier);
    PdfxSetFontSize(hPdf, 9);
    PdfxText_SetSides(hPdf, 1, 0.5, -1, -0.5);//-2
    PdfxText_SetParagraphIndent(hPdf, 1);
    // write text

    _CarReportTitle(hPdf);

    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;

    CString sSub("");
    if( iTrainFK > 0 && iDate == 0 )
        sSub.Format(" AND Cars_Not_Found.Trains_FK = %d",iTrainFK);
    if( iDate > 0 && iTrainFK > 0 )
        sSub.Format(" AND Cars_Not_Found.Trains_FK = %d AND Cars_Not_Found.train_date = %d",iTrainFK,iDate);
    if( iDate > 0 && iTrainFK == 0 )
        sSub.Format(" AND Cars_Not_Found.train_date",iTrainFK,iDate);
    //
    CString sSQL;
    sSQL.Format("SELECT CarTypes.description,Trains.name,Industries_FK,Trains.Route_FK,Trains.reverse,train_date FROM Cars_Not_Found,Trains,CarTypes WHERE Trains.id=Cars_Not_Found.Trains_FK AND CarTypes.id=Cars_Not_Found.CarTypes_FK %s ORDER BY train_date DESC;",sSub);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    while (!q.eof())
    {
        PdfxText_SetParagraphIndent(hPdf, 1);
        char pszLine[255];
        CString sCarType = q.fieldValue(0);
        CString sTrainName = q.fieldValue(1);
        int iDate = q.getIntField(5);
        CppSQLite3Query q2 = pDB->execQuery("SELECT rrdate FROM _Config WHERE id=1;");
        CString sDate("??/??/????");
        if( !q2.eof() )
        {
            int y,m,d;
            int iConfigDate = q2.getIntField(0);
            JulianDate::JdToYmd( iConfigDate+iDate, &y,&m,&d);
            sDate.Format("%02d/%02d/%04d",m,d,y);
        }
        q2.finalize();
        //
        char szTitle[255];
        sprintf(szTitle,"%s Train: %s\n - %s not found",sDate,sTrainName,sCarType);
        bool bParagraph = true;
        for(;;)
        {
            PdfxText_SetParagraphIndent(hPdf, 1);
            PdfxText_WriteText(hPdf,szTitle,eJustify,bParagraph,&nSkipChars);
            if( nSkipChars >= strlen(szTitle))
                break;
            //bParagraph = (szTitle[-1] == '\n' || szTitle[-1] == '\r');
            PdfxNewStdPage(hPdf,eUS_A,ePortrait);
            PdfxText_SetParagraphIndent(hPdf, 1);
            _CarReportTitle(hPdf);
            PdfxText_SkipLine(hPdf);
        }
        CString sSQL;
        sSQL.Format("SELECT Industries.id, Industries.name FROM Trains_Industry,Industries WHERE Trains_Industry.Train_FK=%d AND Trains_Industry.Industry_FK=Industries.id",iTrainFK);
        q2 = pDB->execQuery(sSQL);
        while(!q2.eof())
        {
            PdfxText_SetParagraphIndent(hPdf, 2);
            int iIndustryFK = q2.getIntField(0);
            sprintf(pszLine,"Industry: %s",q2.fieldValue(1));
            bool bParagraph = true;
            for(;;)
            {
                PdfxText_WriteText(hPdf,pszLine,eJustify,bParagraph,&nSkipChars);
                if( nSkipChars >= strlen(pszLine))
                    break;
                bParagraph = (pszLine[-1] == '\n' || pszLine[-1] == '\r');
                PdfxNewStdPage(hPdf,eUS_A,ePortrait);
                //
                _CarReportTitle(hPdf);
                PdfxText_SetParagraphIndent(hPdf, 1);

                PdfxText_WriteText(hPdf,szTitle,eJustify,bParagraph,&nSkipChars);
                PdfxText_SetParagraphIndent(hPdf, 2);
            }
            //
            q2.nextRow();
        }
        q2.finalize();
        //
        q.nextRow();
        PdfxText_SkipLine(hPdf);
    }
    q.finalize();
    //
    //close pdf file
    PdfxClose(hPdf);
    //
    ShowPDF(fname);
    */
}

CString GetWorkDirectory()
{
    char szPath[MAX_PATH];
    memset(&szPath,0,sizeof(szPath));
    //
    //  \Documents and Settings\[user]\Local Settings\Temp
    //
    GetTempPath(MAX_PATH,szPath);
    if( szPath[strlen(szPath)-1] == '\\' )
        szPath[strlen(szPath)-1] = '\0';
    //
    //  TODO: Check writable
    //
    //  \Documents and Settings\[user]\My Documents:
    //
    ITEMIDLIST* pidl;
    HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_PERSONAL , &pidl );
    if (hRes==NOERROR)
    {
        SHGetPathFromIDList( pidl, szPath );
        strcat(szPath,"\\TrainOps");
        CreateDirectory(szPath,NULL);
    }
    //
    return CString(szPath);
}

void Watermark(HPDF_Doc pdf, HPDF_Page page)
{
    /*
#ifndef _DEBUG
    if( ((CTrainOpsApp*)AfxGetApp())->m_pActiveLockMfc->LicenseStatus() == "Registered" )
        return;
#endif
    //
    float angle1 = 54;
    float rad1 = angle1 / 180 * 3.141592;

    HPDF_Font font = HPDF_GetFont (pdf, "Helvetica", NULL);
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
    HPDF_Page_SetRGBFill (page, 0.75, 0.75, 0.75);
    HPDF_Page_SetFontAndSize(page, font, 55);
    //
    HPDF_Page_BeginText (page);
    HPDF_Page_SetTextMatrix (page, cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
                130, 120);
    HPDF_Page_ShowText (page, "TrainOps Evaluation Version");
    //
    HPDF_Page_SetFontAndSize(page, font, 25);
    HPDF_Page_SetTextMatrix (page, cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
                210, 190);
    HPDF_Page_ShowText (page, "Visit http://www.largescalecentral.com/TrainOps");
    //
    HPDF_Page_EndText (page);
    HPDF_Page_SetRGBFill (page, 0, 0, 0);
    */
}

void ShowPDF(char* szFileName)
{
    //MessageBox(0,szFileName,"DEBUG",MB_OK);
    //if( FileExists(szFileName) )
    //    MessageBox(0,"File exists","DEBUG",MB_OK);
    //else
    //    MessageBox(0,"Does not exist","DEBUG",MB_OK);
    ShellExecute(NULL,"open",szFileName,0,0,SW_SHOW);
    //CPDFViewerDialog pDlg;
    //pDlg.SetFile(szFileName);
    //pDlg.DoModal();
}

void DoTrainBuildingForm()
{
    HPDF_Doc  pdf;
    char fname[MAX_PATH];
    HPDF_Page page[100];
    int iPage = 0;
    HPDF_Font detail_font;
    const char *detail_font_name;

    strcpy(fname,(LPCTSTR(GetWorkDirectory())));
    //
    int iRand = (rand() % 100000);
    sprintf(fname,"%s\\trainbuilding%d.pdf",fname,iRand);
    //unlink(fname);
    Sleep(2000);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        MessageBox(0,"error: cannot create PdfDoc object","ERROR",MB_OK);
        return;
    }
    SetCursor(LoadCursor(NULL,IDC_WAIT));

    /* declaration for using Japanese encoding. */
    HPDF_UseJPEncodings (pdf);
    
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
    
    /* load ttf file */
    detail_font_name = HPDF_LoadTTFontFromFile (pdf, ".\\TYPIW___.TTF", HPDF_TRUE);
    detail_font = HPDF_GetFont (pdf, detail_font_name, "90msp-RKSJ-H");
    
    /* add a new page object. */
    page[iPage] = HPDF_AddPage (pdf);
    //
    Watermark(pdf,page[iPage]);
    //
    HPDF_Page_BeginText (page[iPage]);
    HPDF_Page_SetSize(page[iPage],HPDF_PAGE_SIZE_LETTER,HPDF_PAGE_PORTRAIT);
    HPDF_Page_SetTextLeading(page[iPage],FONT_SIZE-1);
    HPDF_Page_SetFontAndSize (page[iPage], detail_font, FONT_SIZE);
    //
    HPDF_Page_EndText (page[iPage]);
    //
    HPDF_Page_BeginText (page[iPage]);
    HPDF_Page_TextRect(page[iPage],10,770,580,740,"Train Building Work Form",HPDF_TALIGN_CENTER,0);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT * FROM Locations ORDER BY sort_order");
    CppSQLite3Query q = pDB->execQuery(sSQL);

    while (!q.eof())
    {
        int iLocationID = q.getIntField("id");
        CString sLocation(q.getStringField("name"));
        HPDF_Page_MoveToNextLine(page[iPage]);
        HPDF_Page_MoveToNextLine(page[iPage]);
        HPDF_Page_ShowText(page[iPage], sLocation);
        //
        //  Industries for this location
        //
        sSQL.Format("SELECT Industries.id, Industries.name FROM Industries,Sidings WHERE Sidings.Locations_FK=%d AND Industries.Sidings_FK=Sidings.id",iLocationID);
        //TRACE(sSQL);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        while( !q2.eof() )
        {
            CString sName = q2.getStringField("name");
            int iIndustryID = q2.getIntField("id");

            //
            HPDF_Page_MoveToNextLine(page[iPage]);
            HPDF_Page_ShowText(page[iPage], "   " + sName);
            sSQL.Format("SELECT trainid FROM Trains_Industry,Trains WHERE Industry_FK=%d AND Trains.id=Trains_Industry.Train_FK",iIndustryID);
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            if( !q3.eof() )
            {
                HPDF_Page_MoveToNextLine(page[iPage]);
            }
            while( !q3.eof() )
            {
                CString sTrainID(q3.getStringField("trainid"));
                HPDF_Page_ShowText(page[iPage], "    - " + sTrainID);
                q3.nextRow();
            }
            q3.finalize();
            //
            q2.nextRow();
        }
        q2.finalize();
        q.nextRow();
    }
    HPDF_Page_EndText (page);
    //
    HPDF_SaveToFile (pdf, fname);
    /* clean up */
    HPDF_Free (pdf);
    //
    ShowPDF(fname);
}