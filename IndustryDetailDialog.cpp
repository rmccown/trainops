// IndustryDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustryDetailDialog.h"
#include "IndustryCarDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog dialog


CIndustryDetailDialog::CIndustryDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustryDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryDetailDialog)
	m_sIndustryName = _T("New Industry");
	m_sSidingName = _T("");
	m_sLocalInstructions = _T("");
	m_sRatioText = _T("");
	m_sRatioError = _T("");
	m_sUsageText = _T("");
	//}}AFX_DATA_INIT
    m_iSiding = -1;
}


void CIndustryDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryDetailDialog)
	DDX_Control(pDX, IDC_RATIOERROR, m_ctlRatioError);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Control(pDX, IDC_INDUSTRYNAME, m_ctlIndustryName);
	DDX_Control(pDX, IDC_SIDINGCOMBO, m_ctlSidingCombo);
	DDX_Text(pDX, IDC_INDUSTRYNAME, m_sIndustryName);
	DDX_CBString(pDX, IDC_SIDINGCOMBO, m_sSidingName);
	DDX_Text(pDX, IDC_LOCALINSTRUCTIONS, m_sLocalInstructions);
	DDX_Text(pDX, IDC_RATIO_TEXT, m_sRatioText);
	DDX_Text(pDX, IDC_RATIOERROR, m_sRatioError);
	DDX_Text(pDX, IDC_USAGE_TEXT, m_sUsageText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CIndustryDetailDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog message handlers

BOOL CIndustryDetailDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlTab.InitDialogs();
    m_ctlTab.m_Dialog0->SetIndustry(m_iIndustryID);
    m_ctlTab.m_Dialog0->Load();
    m_ctlTab.m_Dialog1->SetIndustry(m_iIndustryID);
    m_ctlTab.m_Dialog1->Load();
    //
    m_ctlTab.InsertItem(0,"Cars");
#if defined (_DEBUG)
    m_ctlTab.InsertItem(1,"Commodities");
#endif
    m_ctlTab.ActivateTabDialogs();
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  load sidings
    //
    CppSQLite3Query q = pDB->execQuery("select id,name from Sidings;");
    //
    while (!q.eof())
    {
        int nItem = m_ctlSidingCombo.AddString(q.getStringField("name"));
        m_ctlSidingCombo.SetItemData(nItem,q.getIntField("id"));
        q.nextRow();
    }
    q.finalize();
    //
    if( m_iIndustryID >= 0 )
    {
        CString sSQL;
        sSQL.Format("SELECT name,notes FROM Industries WHERE id=%d;",m_iIndustryID);
        q = pDB->execQuery(sSQL);
        if( !q.eof() )
        {
            m_sIndustryName = q.getStringField("name");
            m_sLocalInstructions= q.getStringField("notes");
        }
        q.finalize();
    }
    //
    for( int i=0;i<m_ctlSidingCombo.GetCount();i++ )
    {
        if( m_ctlSidingCombo.GetItemData(i) == m_iSiding )
        {
            m_ctlSidingCombo.SetCurSel(i);
        }
    }
    //
    UpdateData(FALSE);
    //
    RecalcRatios();
    //
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIndustryDetailDialog::RecalcRatios()
{
    UpdateData(TRUE);
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    //  cars in
    //
    float fPctIn(0);
    float fPctOut(0);
    std::vector<float> pCarsNeeded;   //  1000 car types?
    pCarsNeeded.reserve(1000);
    //
    for( int i=0;i<m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemCount();i++ )
    {
        if( m_ctlTab.m_Dialog0->m_ctlCarListIn.GetCheck(i) )
        {
            int iLoadEmptyDays = atoi(m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_TIME));
            float fQuanHigh = atof(m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_HIGH));
            float fQuanLow = atof(m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_LOW));
            float fPercent = atof(m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_PCT));
            float fPct = fQuanLow + ((fQuanHigh-fQuanLow) *(fPercent/100))*iLoadEmptyDays;
            fPctIn += fPct;
            //
            int iCarTypeFK = m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemData(i);
            CString sSQL;
            sSQL.Format("SELECT count(*) AS numcars FROM Cars WHERE CarTypes_FK=%d;",iCarTypeFK);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            float fCarCount(0.0);
            if( !q.eof() )
                fCarCount = q.getFloatField("numcars");
            q.finalize();
            //
            /*
            CString sDesc;
            sSQL.Format("SELECT description FROM CarTypes WHERE id=%d;",iCarTypeFK);
            q = pDB->execQuery(sSQL);
            if( !q.eof() )
                sDesc = q.getStringField("description");
            q.finalize();
            */
            //
            float fUsagePct = (((2 * fPct * iLoadEmptyDays) + (fPct * g_iOffLayoutTime))/fCarCount);
            pCarsNeeded[iCarTypeFK] = fUsagePct;
            //CString sWork;
            //sWork.Format(" %s: %3.2f",sDesc,fUsagePct);
            //sCarsNeededIn += sWork;
            //
        }
    }
    //
    //  cars out
    //
    for( i=0;i<m_ctlTab.m_Dialog0->m_ctlCarListOut.GetItemCount();i++ )
    {
        if( m_ctlTab.m_Dialog0->m_ctlCarListOut.GetCheck(i) )
        {
            int iLoadEmptyDays = atoi(m_ctlTab.m_Dialog0->m_ctlCarListOut.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_TIME));
            float fQuanHigh = atof(m_ctlTab.m_Dialog0->m_ctlCarListOut.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_HIGH));
            float fQuanLow = atof(m_ctlTab.m_Dialog0->m_ctlCarListOut.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_LOW));
            float fPercent = atof(m_ctlTab.m_Dialog0->m_ctlCarListOut.GetItemText(i,CIndustryDetailDialog_Cars::IDD_C_PCT));
            float fPct = fQuanLow + ((fQuanHigh-fQuanLow) *(fPercent/100))*iLoadEmptyDays;
            fPctOut += fPct;
            //
            int iCarTypeFK = m_ctlTab.m_Dialog0->m_ctlCarListIn.GetItemData(i);
            CString sSQL;
            sSQL.Format("SELECT count(*) AS numcars FROM Cars WHERE CarTypes_FK=%d;",iCarTypeFK);
            CppSQLite3Query q = pDB->execQuery(sSQL);
            float fCarCount(0.0);
            if( !q.eof() )
                fCarCount = q.getFloatField("numcars");
            q.finalize();
            //
            /*
            CString sDesc;
            sSQL.Format("SELECT description FROM CarTypes WHERE id=%d;",iCarTypeFK);
            q = pDB->execQuery(sSQL);
            if( !q.eof() )
                sDesc = q.getStringField("description");
            q.finalize();
            */
            //
            float fUsagePct = (((2 * fPct * iLoadEmptyDays) + (fPct * g_iOffLayoutTime))/fCarCount);
            pCarsNeeded[iCarTypeFK] += fUsagePct;

        }
    }
    //
    //  get average car length
    //
    CppSQLite3Query q = pDB->execQuery("SELECT AVG(car_length) AS avlen FROM Cars where car_length > 0");
    int iAverageCarLength = g_iCarLength;
    if( !q.eof() )
        iAverageCarLength = q.getIntField("avlen");
    q.finalize();
    CString sSQL;
    sSQL.Format("SELECT capacity FROM Sidings WHERE id=%d;",m_iSiding);
    q = pDB->execQuery(sSQL);
    int iSidingLength = 100;
    if( !q.eof() )
        iSidingLength = q.getIntField("capacity");
    q.finalize();
    //
    //  calculate siding use ratio
    //
    int iRatio = (((float)iAverageCarLength * (fPctIn+fPctOut))/ (float)iSidingLength)*100.0;
    //
    /*
    int iRatio;
    ((CTrainOpsApp*)AfxGetApp())->GetIndustryRatios(m_iIndustryID,m_iSiding,&iRatio);
*/
    //m_sRatioText.Format("Average Inbound: %1.1f, Average Outbound: %1.1f, Siding usage: %d%%",fPctIn,fPctOut,iRatio);
    m_sRatioText.Format("Projected Cars Per Day: Inbound: %1.1f  Outbound: %1.1f  Siding Usage: %d%%",fPctIn,fPctOut,iRatio);
    if( iRatio > 75 )
    {
        m_sRatioError = "Siding usage is high. Modify your in/out ratios to avoid overcrowding the siding.";
        m_ctlRatioError.SetTextColor(RED);
    }
    else
        m_sRatioError = "";
    //
    CString sCarsNeeded("");
    for( i=0;i<1000;i++ )
    {
        if( pCarsNeeded[i] > 0 )
        {
            CString sDesc("");;
            sSQL.Format("SELECT description FROM CarTypes WHERE id=%d;",i);
            q = pDB->execQuery(sSQL);
            if( !q.eof() )
                sDesc.Format(" %s: %3.2f",q.getStringField("description"),pCarsNeeded[i]);
            q.finalize();
            sCarsNeeded += sDesc;
        }
    }
    m_sUsageText.Format("Total Cars needed: %s",sCarsNeeded);
    //
    UpdateData(FALSE);
}

void CIndustryDetailDialog::SetIndustryID(int iIndustryID)
{
    m_iIndustryID = iIndustryID;
}

void CIndustryDetailDialog::SetIndustryDefault()
{
    m_iIndustryID = -1;
}

void CIndustryDetailDialog::OnOK() 
{
    UpdateData();
    //
    //  get selected siding
    //
    int nItem = m_ctlSidingCombo.GetCurSel();
    int iSidingID = -1;
    if( nItem >= 0 )
        iSidingID = m_ctlSidingCombo.GetItemData(nItem);
    else
    {
        MessageBox("No siding selected");
        return;
    }
    //
    //  SQL sanitize?
    //
    //m_sIndustryName.Replace("'","");
    //m_sLocalInstructions.Replace("'","");
    //
    CppSQLite3DB* pDB = &((CTrainOpsApp*)AfxGetApp())->m_pDB;
    //
    if( m_iIndustryID != -1 )
    {
        CString sSQL;
        sSQL.Format("DELETE FROM Industries_Cars WHERE Industries_FK=%d;",m_iIndustryID);
        CppSQLite3Query q = pDB->execQuery(sSQL);
        q.finalize();
        sSQL.Format("DELETE FROM Industries_Commodities WHERE Industries_FK=%d;",m_iIndustryID);
        q = pDB->execQuery(sSQL);
        q.finalize();
        sSQL.Format("UPDATE Industries SET name=\"%s\", notes=\"%s\" WHERE id=%d",m_sIndustryName,m_sLocalInstructions,m_iIndustryID);
        q = pDB->execQuery(sSQL);
        q.finalize();
    }
    else
    {
        CString sSQL;
        sSQL.Format("INSERT INTO Industries (Sidings_FK,name,notes) VALUES (%d,\"%s\",\"%s\");",iSidingID,m_sIndustryName,m_sLocalInstructions);
        CppSQLite3Query q = pDB->execQuery(sSQL);
        q.finalize();
        sSQL = "SELECT last_insert_rowid()";
        q = pDB->execQuery(sSQL);   
        m_iIndustryID = atoi(q.fieldValue(0));
        q.finalize();
    }
    //
    //  Save car types?
    //
    if( g_iMethod == METHOD_CARS )
    {
        m_ctlTab.m_Dialog0->Save(m_iIndustryID);
    }
	//
    //  Save commodities?
    //
    if( g_iMethod == METHOD_COMMODITIES )
    {
        m_ctlTab.m_Dialog1->Save();
    }
    //
    //  Update siding
    //
    CString sSQL;
    sSQL.Format("UPDATE Industries SET Sidings_FK=%d WHERE id=%d",iSidingID,m_iIndustryID);
    pDB->execQuery(sSQL);
    //
	CDialog::OnOK();
}


BOOL CIndustryDetailDialog::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(pMsg->message==WM_KEYDOWN)
    {
        if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
            pMsg->wParam=NULL ;
    }
    
    return CDialog::PreTranslateMessage(pMsg);
}    
