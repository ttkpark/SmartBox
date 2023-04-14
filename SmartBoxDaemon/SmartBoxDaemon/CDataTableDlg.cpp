// CDataTableDlg.cpp : implementation file
//

#include "pch.h"
#include "SmartBoxDaemon.h"
#include "CDataTableDlg.h"
#include "afxdialogex.h"
#include "SmartBoxDaemonDlg.h"


// CDataTableDlg dialog

IMPLEMENT_DYNAMIC(CDataTableDlg, CDialogEx)

CDataTableDlg::CDataTableDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATATABLE, pParent)
{

}

CDataTableDlg::~CDataTableDlg()
{
}

void CDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CDataTableDlg, CDialogEx)
	ON_WM_SIZING()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDataTableDlg message handlers

/*
	CString serialno;     // 상품명
	CString appkey;
	CString StartDate;
	CString WarningDate;
	CString DangerDate;
	CString LastDate;
	CString alertstatus;
	CString savetotmin;
	CString warningmin;
	CString dangermin;
	CString GoodsName;
	CString RACLL;
	CString RACMM;
	CString RACSS;
	CString SendTimeSec;
	CString Revision;
*/


BOOL CDataTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	p_parent->SensorList;
	m_List.InsertColumn( 0, _T("SerialNum"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 1, _T("APPKEY"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 2, _T("시작일자"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 3, _T("경고일자"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 4, _T("위험일자"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 5, _T("한계일자"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 6, _T("현재상태"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 7, _T("경고기간"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 8, _T("위험기간"), LVCFMT_LEFT, 100);
	m_List.InsertColumn( 9, _T("보관기간"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(10, _T("상품명"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(11, _T("RACLL"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(12, _T("RACMM"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(13, _T("RACSS"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(14, _T("전송주기"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(15, _T("보정값"), LVCFMT_LEFT, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDataTableDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::DestroyWindow();
}


int CDataTableDlg::DataList_Update()
{
	if (!this)return 0;
	CString strKey;
	SensorDataList sdt;
	POSITION pos = p_parent->SensorList.GetStartPosition();
	int i = 0;
	m_List.DeleteAllItems();
	while (pos != NULL)
	{
		p_parent->SensorList.GetNextAssoc(pos, strKey, sdt);
		SensorDataAddtoList(i++, sdt);
	}
	CString str; str.Format(_T("센서 %d개"), p_parent->m_ListNumSensor);
	GetDlgItem(IDC_LISTTXT)->SetWindowText(str);

	return 0;
}

int CDataTableDlg::SensorDataAddtoList(int Col,SensorDataList& sdt)
{

	int index = m_List.InsertItem(Col, sdt.serialno,0);

	m_List.SetItemText(Col,  1, sdt.appkey);
	m_List.SetItemText(Col,  2, sdt.StartDate);
	m_List.SetItemText(Col,  3, sdt.WarningDate);
	m_List.SetItemText(Col,  4, sdt.DangerDate);
	m_List.SetItemText(Col,  5, sdt.LastDate);
	m_List.SetItemText(Col,  6, sdt.alertstatus);
	m_List.SetItemText(Col,  7, sdt.savetotmin);
	m_List.SetItemText(Col,  8, sdt.warningmin);
	m_List.SetItemText(Col,  9, sdt.dangermin);
	m_List.SetItemText(Col, 10, sdt.GoodsName);
	m_List.SetItemText(Col, 11, sdt.RACLL);
	m_List.SetItemText(Col, 12, sdt.RACMM);
	m_List.SetItemText(Col, 13, sdt.RACSS);
	m_List.SetItemText(Col, 14, sdt.SendTimeSec);
	m_List.SetItemText(Col, 15, sdt.Revision);


	return 0;
}


void CDataTableDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	m_List.SetWindowPos(NULL, 7, 20, (pRect->right - pRect->left) - 30, (pRect->bottom - pRect->top) - 73, SWP_NOZORDER);

	// TODO: Add your message handler code here
}


void CDataTableDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_List)
		m_List.SetWindowPos(NULL, 7, 20, cx - 14, cy - 34, SWP_NOZORDER);
	// TODO: Add your message handler code here
}
