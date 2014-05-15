/*
	reports.h
*/

#include "hpdf.h"

void DoCarLocationReport(int iTrainFK=0);
void DoCarLocationSortedReport(int iTrainFK=0);
void CheckCaboose(HPDF_Page page,int iTrainScheduledFK);
void AllCaboose(HPDF_Page page);

void PrintSwitchlist(int iTrains_Scheduled_FK);
void PrintRealisticSwitchlist(int iTrains_Scheduled_FK);
void PrintSimpleSwitchlist(int iTrains_Scheduled_FK);

void PrintForm19s(int iTrains_Scheduled_FK);
void PrintBadOrderCards(int iTrains_Scheduled_FK);

void DoCarsNotFoundReport(int iTrainFK=0,int iDate=0);

void DoTrainBuildingForm();

CString GetWorkDirectory();
void Watermark(HPDF_Doc pdf, HPDF_Page page);
void ShowPDF(char* szFileName);
