// CTab0.cpp : implementation file
//

#include "pch.h"
#include "SmartBoxDaemon.h"
#include "CTab0.h"
#include "afxdialogex.h"
#include "CDataTableDlg.h"
#include "CBoxManageDlg.h"

#include "SmartBoxDaemonDlg.h"

// CTab0 dialog

IMPLEMENT_DYNAMIC(CTab0, CDialogEx)

CTab0::CTab0(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB0, pParent)
{

}

CTab0::~CTab0()
{
}

void CTab0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMSTATE, m_COMStateIcon);
	DDX_Control(pDX, IDC_SETABLEBTN, m_SensorTableBtn);
	DDX_Control(pDX, IDC_SENSORNUM, m_NumofListSensor);
	DDX_Control(pDX, IDC_APIURL, m_ApiURLEdit);
	DDX_Control(pDX, IDC_COMCOMBO, m_COMCombo);
	DDX_Control(pDX, IDC_COMCONNECT, m_COMConnectBtn);
	DDX_Control(pDX, IDC_AUTODATA, m_AutoQueryChk);
}


BEGIN_MESSAGE_MAP(CTab0, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDC_COMCONNECT, &CTab0::OnBnClickedComconnect)
ON_BN_CLICKED(IDC_COMAUTO, &CTab0::OnBnClickedComauto)
ON_BN_CLICKED(IDC_APIQUERY, &CTab0::OnBnClickedApiquery)
ON_EN_CHANGE(IDC_APIURL, &CTab0::OnEnChangeApiurl)
ON_CBN_EDITCHANGE(IDC_COMCOMBO, &CTab0::OnCbnEditchangeComcombo)
ON_CBN_SELCHANGE(IDC_COMCOMBO, &CTab0::OnCbnSelchangeComcombo)
ON_BN_CLICKED(IDC_TRAY, &CTab0::OnBnClickedTray)
ON_BN_CLICKED(IDC_SETABLEBTN, &CTab0::OnBnClickedSetablebtn)
ON_BN_CLICKED(IDC_AUTODATA, &CTab0::OnBnClickedAutodata)
ON_BN_CLICKED(IDC_BUTTON1, &CTab0::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTab0 message handlers

//CString str;
void CTab0::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC dc(this);
	//CString str;
	//str.Format(_T("(%d,%d)"),point.x, point.y);
	//p_parent->SetStatusBarText(str);
	//dc.TextOut(0,0,str);
	//Invalidate(0);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CTab0::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	
	//dc.TextOut(0, 20, str);

	CDialogEx::OnPaint();
}


void CTab0::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


//BOOL CTab0::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//	CRect rect;
//	GetClientRect(&rect);
//	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
//	Invalidate(0);
//
//	return CDialogEx::OnEraseBkgnd(pDC);
//}


BOOL CTab0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255));

	m_COMCombo.ResetContent();
	m_COMCombo.SetCurSel(0);

	//초기화값 저장 필요!
	isAutoQuery = p_parent->Systemconst.d_isAutoQuery;
	m_AutoQueryChk.SetCheck(isAutoQuery);
	p_parent->m_ApiURL = p_parent->Systemconst.m_ApiURL;//_T("http://192.168.0.100:20130/api/");

	m_ApiURLEdit.SetWindowText(p_parent->m_ApiURL);
	m_COMConnectBtn.SetWindowText(_T("연결"));

	if (isAutoQuery)OnBnClickedApiquery();
	isWorkingDataListDlg = 0;
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab0::OnBnClickedComconnect()
{
	// TODO: Add your control notification handler code here
	if (!p_parent->m_COMConnected)
	{
		p_parent->COMConnect();
	}
	else p_parent->COMDisconnect();
}


void CTab0::OnBnClickedComauto()
{
	// TODO: Add your control notification handler code here
	p_parent->COMAutoDetect();
}


void CTab0::OnBnClickedApiquery()
{
	p_parent->GetAPISLR(p_parent->idxClient);
}


void CTab0::OnEnChangeApiurl()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	m_ApiURLEdit.GetWindowText(p_parent->m_ApiURL);

	p_parent->Systemconst.m_ApiURL = p_parent->m_ApiURL;
	p_parent->SystemConst_Store();
}

/*
void CTab0::SetCOMPortState(int state)
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE((state ? IDI_ON : IDI_OFF)), IMAGE_ICON, 20, 20, 0);
	m_COMStateIcon.SetIcon(hIcon);
}
*/


void CTab0::OnCbnEditchangeComcombo()
{
	m_COMCombo.GetWindowText(p_parent->m_COMPort);
}


void CTab0::OnCbnSelchangeComcombo()
{
	int i = m_COMCombo.GetCurSel();
	m_COMCombo.GetLBText(i, p_parent->m_COMPort);
}


void CTab0::OnBnClickedTray()
{
	p_parent->BTTray();
}

void CTab0::OnBnClickedSetablebtn()
{
	//if (!isWorkingDataListDlg)
	{
		p_parent->pDataDlg->ShowWindow(SW_MAXIMIZE);
		//DataListDlgThread = AfxBeginThread(ThreadForModal, this);
	}
}



void CTab0::SensorListChanged()
{
	CString str;
	if (p_parent->m_ListNumSensor == 0)
		m_SensorTableBtn.EnableWindow(FALSE);
	else 
		m_SensorTableBtn.EnableWindow(TRUE);

	str.Format(_T("센서 %d개"), p_parent->m_ListNumSensor);
	m_NumofListSensor.SetWindowText(str);
}


void CTab0::OnBnClickedAutodata()
{
	isAutoQuery = m_AutoQueryChk.GetCheck();

	p_parent->Systemconst.d_isAutoQuery = isAutoQuery;
	p_parent->SystemConst_Store();
}


BOOL CTab0::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}

//when it've gotten the Box ManagementDialog DoModal Button Message
void CTab0::OnBnClickedButton1()
{
	p_parent->pBoxDlg->ShowWindow(SW_SHOW);
}
