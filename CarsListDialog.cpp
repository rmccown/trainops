// CarsListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "CarsListDialog.h"

#include "EditCarDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarsListDialog dialog


CCarsListDialog::CCarsListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCarsListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCarsListDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_iCarTypeFK=-1;
}


void CCarsListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarsListDialog)
	DDX_Control(pDX, IDC_EDITCARBUTTON, m_ctlEditCarButton);
	DDX_Control(pDX, IDC_DELETECARBUTTON, m_ctlDeleteCarButton);
	DDX_Control(pDX, IDC_CARSLIST, m_ctlCarList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCarsListDialog, CDialog)
	//{{AFX_MSG_MAP(CCarsListDialog)
	ON_BN_CLICKED(IDC_DELETECARBUTTON, OnDeletecarbutton)
	ON_BN_CLICKED(IDC_EDITCARBUTTON, OnEditcarbutton)
	ON_NOTIFY(NM_CLICK, IDC_CARSLIST, OnClickCarslist)
	ON_NOTIFY(NM_DBLCLK, IDC_CARSLIST, OnDblclkCarslist)
	ON_BN_CLICKED(IDC_ADDCARBUTTON, OnAddCarButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarsListDialog message handlers
enum {
    LIST_CARNO=0,
    LIST_REPMARKS,
    LIST_LOCATION,
    LIST_DESTINATION,
    LIST_RETURNTO,
    LIST_LENGTH,
    LIST_EMPTY,
    LIST_FOREIGN,
    LIST_COMMODITY
};

BOOL CCarsListDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlCarList.InsertColumn(LIST_CARNO,"Car Number");
    m_ctlCarList.InsertColumn(LIST_REPMARKS,"Reporting Marks");
    m_ctlCarList.InsertColumn(LIST_LOCATION,"Location");
    m_ctlCarList.InsertColumn(LIST_DESTINATION,"Destination");
    m_ctlCarList.InsertColumn(LIST_RETURNTO,"Return to");
    m_ctlCarList.InsertColumn(LIST_LENGTH,"Lgth");
    m_ctlCarList.InsertColumn(LIST_EMPTY,"E/L");
    m_ctlCarList.InsertColumn(LIST_FOREIGN,"F");
    if( g_iMethod == METHOD_COMMODITIES )
        m_ctlCarList.InsertColumn(LIST_COMMODITY,"Current Load");
    m_ctlCarList.SetColumnWidth(LIST_CARNO,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarList.SetColumnWidth(LIST_REPMARKS,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarList.SetColumnWidth(LIST_LOCATION,119);
    m_ctlCarList.SetColumnWidth(LIST_DESTINATION,119);
    m_ctlCarList.SetColumnWidth(LIST_RETURNTO,119);
    m_ctlCarList.SetColumnWidth(LIST_LENGTH,38);
    m_ctlCarList.SetColumnWidth(LIST_EMPTY,LVSCW_AUTOSIZE_USEHEADER);
    m_ctlCarList.SetColumnWidth(LIST_FOREIGN,20);
    if( g_iMethod == METHOD_COMMODITIES )
        m_ctlCarList.SetColumnWidth(LIST_COMMODITY,60);
    //
    m_ctlCarList.SetExtendedStyle(m_ctlCarList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

    ReadDB();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    return (int)(lParam1 - lParam2);
}

void CCarsListDialog::ReadDB()
{
    m_ctlCarList.DeleteAllItems();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("SELECT id,reporting_marks,car_number,loadempty,car_length,Location_FK,Commodities_FK,foreign_road,returnto FROM Cars WHERE CarTypes_FK=%d ORDER BY CAST(car_number AS INT);",m_iCarTypeFK);
    CppSQLite3Query q = pDB->execQuery(sSQL);
    //
    while (!q.eof() )
    {
        CString sCarNumber;
        sCarNumber.Format("%ld",atol(q.getStringField("car_number")));
        TRACE("%s\n",(LPCTSTR)sCarNumber);
        int nIndex = m_ctlCarList.InsertItem(m_ctlCarList.GetItemCount(),sCarNumber);
        m_ctlCarList.SetItemData(nIndex,q.getIntField("id"));
        m_ctlCarList.SetItemText(nIndex,LIST_REPMARKS,q.getStringField("reporting_marks"));
        if( q.getIntField("car_length") < 1 )
        {
            CString sLen;
            sLen.Format("%d *",g_iCarLength);
            m_ctlCarList.SetItemText(nIndex,LIST_LENGTH,sLen);
        }
        else
            m_ctlCarList.SetItemText(nIndex,LIST_LENGTH,q.getStringField("car_length"));
        m_ctlCarList.SetItemText(nIndex,LIST_EMPTY,q.getIntField("loadempty")==1?"L":"E");
        if( q.getIntField("foreign_road") == 1 )
            m_ctlCarList.SetItemText(nIndex,LIST_FOREIGN,"Y");
        //
        //  see if this car is in a train
        //
        sSQL.Format("SELECT Location_from_FK, Location_to_FK FROM Trains_Scheduled_Cars, Trains_Scheduled WHERE Trains_Scheduled_FK=Trains_Scheduled.id AND Cars_FK=%d AND train_run=0;",q.getIntField("id"));
        CppSQLite3Query q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
        {
            sSQL.Format("SELECT Locations.name FROM Locations WHERE id=%d",q2.getIntField("Location_from_FK"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            if( !q3.eof() )
                m_ctlCarList.SetItemText(nIndex,LIST_LOCATION,q3.getStringField("name"));
            q3.finalize();
            //
            sSQL.Format("SELECT Locations.name FROM Locations WHERE id=%d",q2.getIntField("Location_to_FK"));
            q3 = pDB->execQuery(sSQL);
            if( !q3.eof() )
                m_ctlCarList.SetItemText(nIndex,LIST_DESTINATION,q3.getStringField("name"));
            q3.finalize();
        }
        else
        {
            sSQL.Format("SELECT name FROM Locations WHERE id=%d",q.getIntField("Location_FK"));
            CppSQLite3Query q3 = pDB->execQuery(sSQL);
            if( !q3.eof() )
                m_ctlCarList.SetItemText(nIndex,LIST_LOCATION,q3.getStringField("name"));
            else
                m_ctlCarList.SetItemText(nIndex,LIST_LOCATION,"");
            q3.finalize();
        }
        q2.finalize();
        //
        //  return to
        //
        sSQL.Format("SELECT name FROM Industries WHERE id=%d",q.getIntField("returnto"));
        q2 = pDB->execQuery(sSQL);
        if( !q2.eof() )
            m_ctlCarList.SetItemText(nIndex,LIST_RETURNTO,q2.getStringField("name"));
        else
            m_ctlCarList.SetItemText(nIndex,LIST_RETURNTO,"");
        q2.finalize();
        //
        q.nextRow();
    }
    q.finalize();
    //
    sSQL.Format("SELECT description FROM CarTypes WHERE id=%d",m_iCarTypeFK);
    q = pDB->execQuery(sSQL);
    if( !q.eof() )
    {
        CString sTitle;
        sTitle.Format("Cars List - %s",q.getStringField("description"));
        SetWindowText(sTitle);
    }
    q.finalize();
    //
    //m_ctlCarList.SortItems(MyCompareProc,(LPARAM)&m_ctlCarList);
    //
    m_ctlEditCarButton.EnableWindow(FALSE);
    m_ctlDeleteCarButton.EnableWindow(FALSE);
}

void CCarsListDialog::OnClickCarslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int iCarFK = m_ctlCarList.GetItemData(m_ctlCarList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarFK < 0 )
        return;
	
    m_ctlEditCarButton.EnableWindow(TRUE);
    m_ctlDeleteCarButton.EnableWindow(TRUE);
	*pResult = 0;
}

void CCarsListDialog::OnDblclkCarslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int iCarFK = m_ctlCarList.GetItemData(m_ctlCarList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarFK < 0 )
        return;
    
    OnEditcarbutton();

	*pResult = 0;
}

void CCarsListDialog::OnDeletecarbutton()
{
    if( MessageBox("WARNING:\nDeleting a car can cause unintended results.\nThere is no way to un-do this action!\n\nAre you sure you want to delete this car?","TrainOps Delete Car",MB_YESNO|MB_ICONSTOP) != IDYES )
        return;

    int iCarFK = m_ctlCarList.GetItemData(m_ctlCarList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarFK < 0 )
        return;
    //    
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("DELETE FROM Cars WHERE id=%d;",iCarFK);
    pDB->execDML(sSQL);
    sSQL.Format("DELETE FROM Trains_Scheduled_Cars WHERE Cars_FK=%d;",iCarFK);
    pDB->execDML(sSQL);
    //
    //MessageBox(std::string("Car has been deleted.").c_str(),"TrainOps");
    //
    ReadDB();
}

void CCarsListDialog::OnEditcarbutton()
{
    int iCarFK = m_ctlCarList.GetItemData(m_ctlCarList.GetNextItem(-1, LVNI_SELECTED));
    if( iCarFK < 0 )
        return;
    //
    CEditCarDialog pDlg;
    pDlg.SetCarID(iCarFK);
    pDlg.DoModal();
    //
    ReadDB();
}

void CCarsListDialog::OnAddCarButton() 
{
    //  add a car, then show edit box
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    CString sSQL;
    sSQL.Format("INSERT INTO Cars (CarTypes_FK,car_number,reporting_marks) values (%d,'%d','%s');",m_iCarTypeFK,999999,g_sReportingMarks);
    pDB->execDML(sSQL);
    //
    sSQL = "SELECT last_insert_rowid()";
    CppSQLite3Query q = pDB->execQuery(sSQL);   
    int iCarFK = atoi(q.fieldValue(0));
    CEditCarDialog pDlg;
    pDlg.SetCarID(iCarFK);
    if( pDlg.DoModal() == IDCANCEL )
    {
        sSQL.Format("DELETE FROM Cars WHERE id=%d;",iCarFK);
        pDB->execDML(sSQL);
    }
    //
    ReadDB();
}
