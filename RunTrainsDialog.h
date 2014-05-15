#if !defined(AFX_RUNTRAINSDIALOG_H__F41BA003_7F12_4ABA_8B1B_F60335743D4F__INCLUDED_)
#define AFX_RUNTRAINSDIALOG_H__F41BA003_7F12_4ABA_8B1B_F60335743D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunTrainsDialog.h : header file
//

#include <Afxwin.h>
#include <Afxtempl.h>

#include "amsEdit.h"
#include "XListCtrl.h"
#include "CoolBtn.h"

#include "IGDebugDialog.h"
#include "TOListCtrl.h"
#include <vector>

//  Helper classes

//
//  Car is the class used to store each car for this train
//  it is stored in the m_pCarTable CArray
//
class CCar
{
public:
    CCar()
    {
        m_iCar_FK=0;
        m_iIndustryFromFK=0;
        m_iLocationFromFK=0;
        m_iSidingFromFK=0;
        m_iIndustryToFK=0;
        m_iLocationToFK=0;
        m_iSidingToFK=0;
        m_bEmpty=TRUE;
        m_iCommodityFK=0;
        m_bForeignRoad=FALSE;
    }
public:
    int m_iCar_FK;          //  which car
    int m_iIndustryFromFK;  //  Industry from
    int m_iLocationFromFK;  //  Location from (If Industry from = 0, then its on a siding)
    int m_iSidingFromFK;    //  Siding from
    //
    int m_iIndustryToFK;    //  Industry to (If Industry to = 0, then its on the train till the end)
    int m_iLocationToFK;    //  Location to
    int m_iSidingToFK;      //  Siding to
    //
    bool m_bEmpty;          //  Is the car empty for full?
    bool m_bForeignRoad;    //  Is the car from a foreign road
    int m_iCommodityFK;     //  Commodity in car (TODO: future)
};

class CFoundCar
{
public:
    CFoundCar()
    {
        m_iCar_FK=0;
        m_iCarLocation=0;
        m_iCarIndustry=0;
        m_iCarSiding=0;
        m_bForeignRoad=false;
    }
    int m_iCar_FK;
    int m_iCarLocation;
    int m_iCarIndustry;
    int m_iCarSiding;
    bool m_bForeignRoad;
};

/////////////////////////////////////////////////////////////////////////////
// CRunTrainsDialog dialog

class CRunTrainsDialog : public CDialog
{
// Construction
public:
	CRunTrainsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRunTrainsDialog)
	enum { IDD = IDD_RUNTRAINSDIALOG };
	CButton	m_ctlReportsButton;
	CButton	m_ctlDetailButton;
	CAMSDateEdit	m_ctlCurrentDate;
	CButton	m_ctlTerminateButton;
	CButton	m_ctlDeleteButton;
	CButton	m_ctlBuildTrain;
	CXListCtrl	m_ctlTrainList;
	CTOListCtrl	m_ctlRunningTrainsList;
	//}}AFX_DATA

    CCoolBtn  m_ctlFormsBtn;

    std::vector<CCar> m_pCarTable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunTrainsDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();
    //
    void BuildByCars();
    void BuildByCommodities();
    void ValidateTrain(int iTrainFK);
    void AdvanceTheDate();
    //
    CString GetCarType(int);
    BOOL NeedThisCar(int iIndustryFK,int iCarTypeFK);
    BOOL CarReadyForPickup(int iCarFK);
    BOOL NeedThisCommodity(int iIndustryFK,int iCommodityFK);
    int GetCarQuantity(int iHigh, int iLow, int iPct);
    CFoundCar* GetCarFromOffLayout(int iCarType_FK,int iTrainFK,int iLocationFK);
    CFoundCar* GetEmptyCar(int iCarType_FK, int iTrainFK, int iLocationFK);
    CFoundCar* GetLoadedCar(int iCarTypeFK,int iTrainFK,int iLocationFK);
    bool GetYardSiding(int iTrainFK,int iLocationFK, int* iSiding,int* iSidingLocation);
    bool GetYardSidingForEmpties(int iTrainFK,int iLocationFK,int* iSiding,int* iSidingLocation);
    int GetFirstYardSiding(int iTrainFK,int iLocationFK);
    bool AnyYardSidings();
    BOOL CarOnTrain(int iCarFK);
    bool CarHasReturnTo(int iCarFK,int* iSiding,int* iSidingLocation);
    bool LocationOnRoute(int iTrainFK,int iLocationFK,int iToLocation);
    int GetTrainFK(int iTrains_Scheduled_FK);
    BOOL GetInterchangeSiding(int iTrainFK, int iStartLocation, int iEndLocation_FK, int* iInterchangeSiding,int* iInterchangeSidingLocation);
    CString GetScheduledCars();
    bool SwitchingSiding(int iTrainFK,int iSidingFK);
    int GetFirstTrainLocation(int iTrainFK);
    int GetLastTrainLocation(int iTrainFK);

    void CheckButtons();

	// Generated message map functions
	//{{AFX_MSG(CRunTrainsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRuntrainbutton();
	afx_msg void OnItemchangedTrainlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedLocolist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedRunningtrainslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeletebutton();
	afx_msg void OnTerminatebutton();
	afx_msg void OnAdvance();
	afx_msg void OnItemchangingTrainlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCabooselist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTrainDetailButton();
	afx_msg void OnForms();
	virtual void OnOK();
	afx_msg void OnReports();
	afx_msg void OnRecalc();
	//}}AFX_MSG
    afx_msg void OnSwitchlist();
    afx_msg void OnForm19();
    afx_msg void OnBadOrder();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNTRAINSDIALOG_H__F41BA003_7F12_4ABA_8B1B_F60335743D4F__INCLUDED_)
