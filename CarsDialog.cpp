// CarsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CarsDialog.h"

#include "CarsListDialog.h"

#include <vector>
//#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarsDialog dialog
#define FIXED_ROW_COUNT         1
#define FIXED_COLUMN_COUNT      1
#define TREE_COLUMN             1//2
#define CTL_COLUMN_BEGIN        (TREE_COLUMN + 1)

CCarsDialog::CCarsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCarsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCarsDialog)
	m_sReportingMarks = _T("");
	m_iCarQuantity = 1;
	//}}AFX_DATA_INIT
}


void CCarsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarsDialog)
	DDX_Control(pDX, IDC_EDITCARSBUTTON, m_ctlEditCarsButton);
	DDX_Control(pDX, IDC_CARSLIST, m_ctlCarsList);
	DDX_Control(pDX, IDC_CARTYPE_COMBO, m_ctlCarTypeCombo);
	DDX_Text(pDX, IDC_CARTYPE_REPORTINGMARKS, m_sReportingMarks);
	DDX_Text(pDX, IDC_ADDCARS_QUAN, m_iCarQuantity);
	DDV_MinMaxInt(pDX, m_iCarQuantity, 1, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCarsDialog, CDialog)
	//{{AFX_MSG_MAP(CCarsDialog)
	ON_BN_CLICKED(IDC_ADDCARS, OnAddcars)
	ON_BN_CLICKED(IDC_EDITCARSBUTTON, OnEditCarsButton)
	ON_NOTIFY(NM_DBLCLK, IDC_CARSLIST, OnDblclkCarslist)
	ON_NOTIFY(NM_CLICK, IDC_CARSLIST, OnClickCarslist)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarsDialog message handlers

BOOL CCarsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    //m_ctlCarsList.SetCol
    m_ctlCarsList.InsertColumn(0,"Car Code");
    m_ctlCarsList.InsertColumn(1,"Car Type");
    m_ctlCarsList.InsertColumn(2,"Quantity",LVCFMT_RIGHT);
    m_ctlCarsList.InsertColumn(3,"Usage",LVCFMT_RIGHT);
    m_ctlCarsList.SetColumnWidth(0,65);
    m_ctlCarsList.SetColumnWidth(1,120);
    m_ctlCarsList.SetColumnWidth(2,55);
    m_ctlCarsList.SetColumnWidth(3,55);
    m_ctlCarsList.SetExtendedStyle(m_ctlCarsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

    ReadDB();

	m_ctlCarsList.RestoreState("State","CarsDialog_m_ctlCarsList");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCarsDialog::ReadDB()
{
    m_ctlCarsList.DeleteAllItems();
    m_ctlCarTypeCombo.ResetContent();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CppSQLite3Query q = pDB->execQuery("select id,description,type_id from CarTypes WHERE active=1 order by description;");
    //
    while (!q.eof() )
    {
        TRACE("%s %s\n",q.fieldValue(0),q.fieldValue(1));
        //
        int nIndex = m_ctlCarsList.InsertItem(1,q.getStringField("type_id"));
        m_ctlCarsList.SetItemData(nIndex,q.getIntField("id"));
        m_ctlCarsList.SetItemText(nIndex,1,q.getStringField("description"));
        CString sSQL;
        sSQL.Format("SELECT count(*) AS carcount FROM Cars WHERE CarTypes_FK=%d;",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            m_ctlCarsList.SetItemText(nIndex,2,q2.getStringField("carcount"));
        }
        q2.finalize();
        CString sUsage;
        float fUsage = GetCarUsage(q.getIntField("id"));
        sUsage.Format("%3.0f%%",fUsage);
        m_ctlCarsList.SetItemText(nIndex,3,sUsage);
        m_ctlCarTypeCombo.AddString(q.getStringField("description"));
        //
        q.nextRow();
    }
    q.finalize();
    //
    m_ctlEditCarsButton.EnableWindow(FALSE);
}

void CCarsDialog::OnAddcars() 
{
    // TODO: Add your control notification handler code here
	UpdateData();
    int nIndex = m_ctlCarTypeCombo.GetCurSel();
    CString sCarType;
    m_ctlCarTypeCombo.GetLBText(nIndex, sCarType);
    if( sCarType.IsEmpty() )
        return;
    if( m_iCarQuantity < 1 )
        return;
    if( m_sReportingMarks.IsEmpty())
        return;
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("select id from CarTypes where description='%s';",sCarType);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    TRACE("CarType_FK: %s\n",q.fieldValue(0));
    if( atoi(q.fieldValue(0)) == 0 )
        return;
    //
    int iCarNumber = 1000;
    for( int i=0;i<m_iCarQuantity;i++)
    {
        CString sSQL;
        sSQL.Format("INSERT INTO Cars (CarTypes_FK,car_number,reporting_marks,car_length) values (%s,'%d','%s',%d);",q.fieldValue(0),iCarNumber++,m_sReportingMarks,g_iCarLength);
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
    }
    ReadDB();
    m_iCarQuantity = 1;
    m_sReportingMarks = "";
    //
    UpdateData(FALSE);
}


void CCarsDialog::OnOK() 
{
    m_ctlCarsList.SaveState("State","CarsDialog_m_ctlCarsList");

	CDialog::OnOK();
}

void CCarsDialog::OnEditCarsButton() 
{
	// TODO: Add your control notification handler code here
    int iCarTypeFK = m_ctlCarsList.GetItemData(m_ctlCarsList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarTypeFK < 0 )
        return;

    CCarsListDialog pDlg;
	pDlg.SetCarType(iCarTypeFK);
    pDlg.DoModal();
    //  changes?
    ReadDB();
}

void CCarsDialog::OnDblclkCarslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int iCarTypeFK = m_ctlCarsList.GetItemData(m_ctlCarsList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarTypeFK < 0 )
        return;

    CCarsListDialog pDlg;
	pDlg.SetCarType(iCarTypeFK);
    pDlg.DoModal();
    //
	*pResult = 0;
}

void CCarsDialog::OnClickCarslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int iCarTypeFK = m_ctlCarsList.GetItemData(m_ctlCarsList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarTypeFK < 0 )
        return;

    m_ctlEditCarsButton.EnableWindow(TRUE);
	*pResult = 0;
}

float CCarsDialog::GetCarUsage(int iCarTypeFK)
{
    //
    //  new math worked out with Bruce:
    //  Usage = 2 * IN/OUT * Load_Empty_Time + IN/OUT * Off_Layout_Time
    /*
        So, if IN is 0-2 (25%), then IN is .5
        and if OUT is 1-3(50%), then OUT is 2.
 
        Total projected is 2.5.
 
        But this gets multiplied by load-empty * 2, and off-layout
    */
    //
    //  read the Industries_Cars table, and calculate car usage for this car type
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT Quantity_low,Quantity_high,Quantity_percentage, LoadEmptyDays FROM Industries_Cars WHERE CarTypes_FK=%d;",iCarTypeFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    if( q.eof() )
    {
        q.finalize();
        return 0.0;
    }
    if( iCarTypeFK == 10 )
        int iWhoa = 1;
    float fInOut(0.00000);
    float fLoadEmptyDays(0.00000);
    float fOffLayoutTime = g_iOffLayoutTime*1.0;
    int iRec=0;
    while( !q.eof() )
    {
        iRec++;
        float fLow = q.getFloatField("Quantity_low");
        float fHigh = q.getFloatField("Quantity_high");
        float fPercent = q.getFloatField("Quantity_percentage");
        float fDemand = fLow + ((fHigh-fLow) * (fPercent / 100.000000));
        fInOut += fDemand;
        fLoadEmptyDays += q.getFloatField("LoadEmptyDays");
        q.nextRow();
    }
    q.finalize();
    fLoadEmptyDays /= iRec;
    //
    q.finalize();
    //
    sSQL.Format("SELECT count(*) AS numcars FROM Cars WHERE CarTypes_FK=%d;",iCarTypeFK);
    q = pDB->execQuery(sSQL);
    float fCarCount(0.0);
    if( !q.eof() )
        fCarCount = q.getFloatField("numcars");
    q.finalize();
    //
    float fPct = (((2 * fInOut * fLoadEmptyDays) + (fInOut * fOffLayoutTime))/fCarCount)*100.00;

    //CString sText;
    //sText.Format("Percent: %5.2f, InOut: %8.5f, LoadEmptyDays: %3.2f, OffLayout: %3.2f, CarCount: %3.2f",
    //    fPct, fInOut,fLoadEmptyDays,fOffLayoutTime,fCarCount);
    //MessageBox(sText);
    //
    return fPct;
}

void CCarsDialog::OnClose() 
{
    m_ctlCarsList.SaveState("State","CarsDialog_m_ctlCarsList");

	CDialog::OnClose();
}
