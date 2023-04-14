
// SmartBoxMasterKeyAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SmartBox MasterKey Application.h"
#include "SmartBoxMasterKeyAppDlg.h"
#include "afxdialogex.h"
#include "CTab1.h"
#include "CTab2.h"
#include "CTab3.h"
#include "CTab4.h"
#include "CMycomm.h"
#include "HttpCall.h"
#include "json.h"
#include "PacketCoder.h"
#include "Base64Coder.h"
#include "ColoredListCtrl.h"

#include "sqlite3/sqlite3.h"

#if defined(_WIN64)
#pragma comment(lib,"sqlite3/sqlite3_64.lib")
#elif defined(_WIN32)
#pragma comment(lib,"sqlite3/sqlite3_32.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSmartBoxMasterKeyAppDlg* MainDlg;
HWND hCommWnd;

CSmartBoxMasterKeyAppDlg::CSmartBoxMasterKeyAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMARTBOXAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartBoxMasterKeyAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CSmartBoxMasterKeyAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSmartBoxMasterKeyAppDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_TRAYICON_MENU1, &CSmartBoxMasterKeyAppDlg::OnTrayiconMenu1)
	ON_COMMAND(ID_TRAYICON_MENU2, &CSmartBoxMasterKeyAppDlg::OnTrayiconMenu2)
	ON_MESSAGE(WM_TRAY_NOTIFICATION, &CSmartBoxMasterKeyAppDlg::OnTrayNotification)
	ON_MESSAGE(WM_COMM_READ, &CSmartBoxMasterKeyAppDlg::OnCommRead)
	ON_MESSAGE(WM_TAB2_UPDATE_VAL, &CSmartBoxMasterKeyAppDlg::OnTab2UpdateVal)
	ON_MESSAGE(WM_STATBARTXT, &CSmartBoxMasterKeyAppDlg::OnStatbartxt)
END_MESSAGE_MAP()

BOOL CSmartBoxMasterKeyAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//탭에 영향을 주는 초기화는 여기에 추가하세요.
	m_ListNumSensor = 0;
	m_COMConnected = 0;
	SensorList.InitHashTable(10007);
	Goods.InitHashTable(101);
	RECT r;
	GetWindowRect(&r);
	SetWindowPos(this, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOSIZE);


	static UINT BASED_CODE indicators[] =
	{
		ID_SEPARATOR,
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL
	};
	MainDlg = this;
	hCommWnd = AfxGetMainWnd()->GetSafeHwnd();

	CRect rect;
	CString cStr;

	m_statusBar.Create(this);
	m_statusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	//ID44 %s(%s) ⓒ 2020 AJUSoft All rights reserved.
	//작업장이름(창고이름) ⓒ 2020 AJUSoft All rights reserved.
	CString ProductCaption;
	ProductCaption.Format(GetCaption(44),ClientName,work_zone_name);
	GetDlgItem(IDC_PRODUCT)->SetWindowText(ProductCaption);

	//setting the status bar....
	this->GetClientRect(&rect);
	m_statusBar.SetPaneInfo(0, ID_INDICATOR_STATTEXT, SBPS_NORMAL, rect.Width() - 32);
	m_statusBar.SetPaneInfo(1, ID_INDICATOR_STATICON, SBPS_STRETCH, 20);
	m_statusBar.MoveWindow(rect.left, rect.bottom - 20, rect.Width(), 20);

	My_Log(GetCaption(61),2);//ID61 Application 시작!!
	m_Tab.DeleteAllItems();
	m_Tab.InsertItem(0, GetCaption(62));//ID62 리스트
	m_Tab.InsertItem(1, GetCaption(63));//ID63 박스등록
	m_Tab.InsertItem(2, GetCaption(64));//ID64 교체
	m_Tab.InsertItem(3, GetCaption(65));//ID65 등록
	m_Tab.SetCurSel(0);

	GetAPIGOD();

	m_Tab.GetWindowRect(&rect);

	pTab1 = new CTab1;
	pTab1->p_parent = this;
	pTab1->Create(IDD_TAB1, &m_Tab);
	pTab1->SetWindowPos(NULL, 2, 26, rect.Width() - 8, rect.Height() - 34, SWP_NOZORDER | SWP_SHOWWINDOW);
	
	pTab2 = new CTab2;
	pTab2->p_parent = this;
	pTab2->Create(IDD_TAB2, &m_Tab);
	pTab2->SetWindowPos(NULL, 2, 26, rect.Width() - 8, rect.Height() - 34, SWP_NOZORDER);
	
	pTab3 = new CTab3;
	pTab3->p_parent = this;
	pTab3->Create(IDD_TAB3, &m_Tab);
	pTab3->SetWindowPos(NULL, 2, 26, rect.Width() - 8, rect.Height() - 34, SWP_NOZORDER);
	
	pTab4 = new CTab4;
	pTab4->p_parent = this;
	pTab4->Create(IDD_TAB4, &m_Tab);
	pTab4->SetWindowPos(NULL, 2, 26, rect.Width() - 8, rect.Height() - 34, SWP_NOZORDER);

	srand(time(NULL));

	APIInitalUpdate();

	COMAutoDetect();
	if(m_COMPort != _T(""))
		pTab4->m_Combo40.GetLBText(0, m_COMPort);

	pTab1->DataList_Update();

	SetCOMICON();

	ChangeCaption();

	timediff = GetGMTTimetoLocalTime();


	//pDlg0->SensorListChanged();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CSmartBoxMasterKeyAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSmartBoxMasterKeyAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

__time64_t CSmartBoxMasterKeyAppDlg::GetGMTTimetoLocalTime() 
{
	//GMT시간과 Local시간의 차이를 구함.
	//-> 로컬시간의 계산시 적용.
	//한국의 경우 +32400(+9시간)이다.
	//이 차이는 timediff에 저장된다.
	//localtime = utctime + timediff;
	struct tm LocalTm, GMTTm;
	__time64_t LocalTime, GMTTime, NowTime;

	NowTime = _time64(NULL);
	_localtime64_s(&LocalTm, &NowTime);
	_gmtime64_s(&GMTTm, &NowTime);
	LocalTime = _mktime64(&LocalTm);
	GMTTime = _mktime64(&GMTTm);

	__time64_t timeDifference = LocalTime - GMTTime;
	if (timeDifference < -3600 * 24/*1Day*/)
		timeDifference = 0;
	return timeDifference;
}

int CSmartBoxMasterKeyAppDlg::SetStatusBarText(CString Text)
{
	m_statusBar.SetPaneText(0, Text);
	My_Log(Text,2/*MSG*/);
	return RES_OK;
}

int presel = 0;
void CSmartBoxMasterKeyAppDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		pTab2->ShowWindow(SW_HIDE);
		pTab3->ShowWindow(SW_HIDE);
		pTab4->ShowWindow(SW_HIDE);
		pTab1->ShowWindow(SW_SHOW);
		break;
	case 1:
		pTab1->ShowWindow(SW_HIDE);
		pTab3->ShowWindow(SW_HIDE);
		pTab4->ShowWindow(SW_HIDE);
		pTab2->ShowWindow(SW_SHOW);
		break;
	case 2:
		pTab1->ShowWindow(SW_HIDE);
		pTab2->ShowWindow(SW_HIDE);
		pTab4->ShowWindow(SW_HIDE);
		pTab3->ShowWindow(SW_SHOW);
		break;
	case 3:
		pTab1->ShowWindow(SW_HIDE);
		pTab2->ShowWindow(SW_HIDE);
		pTab3->ShowWindow(SW_HIDE);
		pTab4->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	
	*pResult = 0;
}
BOOL CSmartBoxMasterKeyAppDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CSmartBoxMasterKeyAppDlg::DestroyWindow()
{
	delete pTab1;
	delete pTab2;
	delete pTab3;
	delete pTab4;
	sqlite3_close(pCaptionDB);
	
	isNDT00 = 1;
	isFinishedValue = 2;

	Sleep(100);
	
	return CDialogEx::DestroyWindow();
}

int IconRes[4] = { IDI_OFF,IDI_ON,IDI_WARN,IDI_SENSOR };
void CSmartBoxMasterKeyAppDlg::SetCOMICON()
{
	int icon = IconRes[m_COMConnected];
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(icon), IMAGE_ICON, 18, 18, 0);

	SETICON(hIcon);

	//Update the lable of Connection/Disconnection Button.
	pTab4->UpdateButton41(m_COMConnected);
}

void CSmartBoxMasterKeyAppDlg::COMConnect()
{
	CString str;
	if (m_COMConnected)
	{
		SetCOMICON();
		return;
	}
	if (SerialConnect(m_COMPort) == RES_OK)
	{
		if (Serial_Detect() != RES_OK) {
			m_COMConnected = 2;
			//CONSTVALUE
			str.Format(GetCaption(66), m_COMPort.GetString());//ID66 There's no Masterkey on %s
			SetStatusBarText(str);
		}
		else
		{
			m_COMConnected = 1;
			//CONSTVALUE
			str.Format(GetCaption(67), m_COMPort.GetString());//ID67 %s 마스터키 연결 성공!
			SetStatusBarText(str);
		}

		SetCOMICON();
	}
	//CONSTVALUE
	else 
	{
		CString str;
		str.Format(GetCaption(68),m_COMPort.GetString());//ID68 %s포트 연결 실패.
		AfxMessageBox(str);
		SetStatusBarText(str);
	}
}

int CSmartBoxMasterKeyAppDlg::COMDisconnect()
{
	int res = SerialDisconnect();
	if (res != RES_OK)
		AfxMessageBox(GetCaption(69));//ID69 COM 포트 닫기 실패.
	SetStatusBarText(GetCaption(70)); //ID70 COM port Close!!
	SetCOMICON();
	return res;
}

typedef struct
{
	uint8_t* RACString;
	uint8_t  RACStrlen;          // RAC정보  -> (str) N+1 바이트 텍스트 ( 길이(1바이트) + Text... )
	uint8_t  AdjustVal[25];      // 보정값   -> (dat) 25바이트 데이터
	uint32_t NowTime;            // 현재시간 -> (int) 8 바이트 1970년으로부터 초
	uint32_t SendTime;           // 전송주기 -> (int) 4 바이트(분) 
	uint32_t DangerTime;         // 위험기간 -> (int) 4 바이트(일) 
	uint32_t SavingTime;         // 보관기간 -> (int) 4 바이트(일)
	uint8_t  startDate[4];       // 시작시간 -> (dat) 4 바이트 ( YY YY MM DD )
} DatafromPC;
typedef struct
{
	DatafromPC Data;
	uint8_t  ProductBitmap[968]; // 상품명   -> (dat) 968바이트 한글 비트맵
	uint8_t  APPKEY[16];         // APPKEY   -> (dat) 16 바이트
}SendDat;
SendDat Sendata;
void CSmartBoxMasterKeyAppDlg::COMAutoDetect()
{
	AutoDetect();
}

int count = 0;
UINT ThreadForSensorAccess(LPVOID param);
int COMReadfocus = 0;
long CSmartBoxMasterKeyAppDlg::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);//받는 데이터 타입을 알기 위해
	CString str = _T("");
	BYTE aByte; //데이터를 저장할 변수 

	int iSize = (ComPort.m_QueueRead).GetSize(); //포트로 들어온 데이터 갯수
												 //SetCommState에 설정된 내용 때문에 거의 8개씩 들어옴
	uint8_t Serach = 0;
	if (isWorkingAccess)
		Serach = 0;
	for (int i = 0; i < iSize; i++)//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
	{

		(ComPort.m_QueueRead).GetByte(&aByte);//큐에서 데이터 한개를 읽어옴
		if (aByte == 2)COMReadfocus = 0;

		if (AutoDetectReadWaiting) {
			if (Serach == 0 && aByte == 2)Serach++;
			if (aByte != 2 && Serach == 1) {
				if (aByte == 6)Serach++; else Serach = 0;
			}
			if (aByte != 6 && Serach == 2) {
				if (aByte == 3)
					AutoDetectReadWaiting = false;
				else Serach = 0;
			}
		}
		else
		{
			if (!m_SensorAccessed)
			{
 				if (Accessfocus == 0) { ReadWaiting = 1; if (aByte == 2)Accessfocus++; else Accessfocus = 0; }
				else if (Accessfocus == 1) { if (aByte == 'N')Accessfocus++; else Accessfocus = 0; }
				else if (Accessfocus == 2) { if (aByte == 'D')Accessfocus++; else Accessfocus = 0; }
				else if (Accessfocus == 3) { if (aByte == 'T')Accessfocus++; else Accessfocus = 0; }
				else if (Accessfocus == 4) { if (aByte == 5) { Accessfocus = 5; ReadWaiting = 2; } else Accessfocus = 0; }
				else if (!ReadWaiting && Accessfocus == 5) {
					Accessfocus = 6;
					m_SensorAccessed = true;
					ReadWaiting = 2;
				}
			}
			if (!Serach) {
				if (ReadWaiting == 1 && aByte == 2)
				{
					ReadWaiting = 2; lastfoc = COMReadfocus;//i=커서, focus를 커서에 맞춘다.
				}
				if (ReadWaiting == 2) {
					ReadString[COMReadfocus - lastfoc] = aByte;
					if (aByte == 4 || aByte == 3) {
						ReadString[COMReadfocus - lastfoc + 1] = 0;//마지막 문자열 정리
						//ReadStringLength = iSize;
						ReadStringLength = COMReadfocus -lastfoc+1;
						ReadWaiting = false;
						isReceivedRTNNDT = 1;
						Serach = 3;

						ReadStr_Process();

						CString Log;
						Log.Format(_T("UART Received : %S"), ReadBuffer); My_Log(Log, 2);
					}
				}
			}
		}

		COMReadfocus++;
		if (aByte == 3 || aByte == 10)COMReadfocus = 0;
	}
	if (Serach == 3)
	{
		if (ReadString[0] == 0x02 && strstr((char*)ReadString, "RTN") && strstr((char*)ReadString, "NDT") && strstr((char*)ReadString, "00"))
		{
			isNDT00 = 1;
			isFinishedValue = 1;
		}
		
		//SetStatusBarText(CString(str));
	}
	if (Accessfocus == 5) {
		Accessfocus = 6;
		m_SensorAccessed = true;
		ReadWaiting = 2;
	}
	if (Accessfocus == 6 && !isWorkingAccess)
	{
		Accessfocus = 0;
		isWorkingAccess = 1;

		ReadStr_Process();
		SensorAccessThread = AfxBeginThread(ThreadForSensorAccess, this);
		SensorAccessThread->m_bAutoDelete = TRUE;
		Sleep(100);
	}

	return RES_OK;
}
void CSmartBoxMasterKeyAppDlg::ReadStr_Process()//ReadString로 읽은 데이터가 앞.뒤로 잘렸을 경우에 처리함.
{
	ReadBuffer = ReadString;
}

UINT ThreadForSensorAccess(LPVOID param)
{
	CSmartBoxMasterKeyAppDlg* p = (CSmartBoxMasterKeyAppDlg*)param;
	p->OnTrayiconMenu1();
	//char* ss = (char*)memchr(p->ReadBuffer, 0x1F, p->ReadStringLength);
	char* ss = (char*)p->ReadBuffer + 15;
	if (ss)p->SensorAccess(ss - 15);
	p->isWorkingAccess = 0;
	p->m_SensorAccessed = false;
	return RES_OK;
}

int CSmartBoxMasterKeyAppDlg::TrayButtonEvent()
{
	memset(&nid, 0, sizeof nid);
	nid.cbSize = sizeof nid;
	nid.uID = SMARTBOXAPPLICATION_TRAY;
	nid.hWnd = this->GetSafeHwnd();
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
	lstrcpy(nid.szTip, L"SmartBox Application App Control");
	nid.uCallbackMessage = WM_TRAY_NOTIFICATION;

	Shell_NotifyIcon(NIM_ADD, &nid);
	this->ShowWindow(SW_HIDE);
	return RES_OK;
}
//출처: http://bigmark.tistory.com/21 [마크의 맥시멈 라이프]

afx_msg LRESULT CSmartBoxMasterKeyAppDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (wParam != SMARTBOXAPPLICATION_TRAY)return RES_ERR;

	switch (lParam) {
	case WM_RBUTTONDOWN:
	{
		CPoint point;
		GetCursorPos(&point);
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	}
	break;
	case WM_LBUTTONDBLCLK:
		OnTrayiconMenu1();
		break;
	}
	//Shell_NotifyIcon(NIM_DELETE,&nid);
	return RES_OK;
}

void CSmartBoxMasterKeyAppDlg::OnTrayiconMenu1()
{
	if (!Shell_NotifyIcon(NIM_DELETE, &nid))
	{
		return;
	}
	this->ShowWindow(TRUE);
}


void CSmartBoxMasterKeyAppDlg::OnTrayiconMenu2()
{
	if (!Shell_NotifyIcon(NIM_DELETE, &nid))
	{
		AfxMessageBox(GetCaption(71));//ID71 트레이 아이콘 제거 실패
		return;
	}
	CDialogEx::OnOK();
}


CString str;

// 사용가능한 COM포트 조회
int CSmartBoxMasterKeyAppDlg::COMPortSeek()
{
	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	wchar_t szData[20], szName[100];
	char cszData[20], cszName[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	COMPort nowPort; 
	char* pointer; int i = 0, portnum = 0;;
	
	pTab4->m_Combo40.ResetContent();
	memset(szData, 0, sizeof szData);
	memset(szName, 0, sizeof szName);
	ActualSerialPorts.DeleteContents();
	ActualSerialPorts.initContents(64);
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, 0, 0, 0, 0)) {
		index++;
		RegQueryValueEx(hKey, szName, 0, &dwType, (LPBYTE)szData, &dwSize2);
		for (i = 0; i < ((sizeof szData) / sizeof(wchar_t)); i++)
			cszData[i] = szData[i];
		for (i = 0; i < ((sizeof szName) / sizeof(wchar_t)); i++)
			cszName[i] = szName[i];
		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
		pointer = strstr(cszData, "COM");
		if (pointer == NULL)continue;
		portnum = atoi(pointer + 3);
		if (portnum == 0 || portnum > 255)continue;
		pointer = strstr(cszName, "Device\\") + 7;
		if (pointer == 0)pointer = strstr(cszName, "DEVICE\\") + 7;
		if (pointer == 0)pointer = strstr(cszName, "\\") + 1;
		if (pointer == 0)pointer = cszName;

		memset(&nowPort, 0, sizeof nowPort);
		nowPort.number = portnum;
		memcpy(nowPort.Descreption, pointer, min(strlen(pointer), 30));
		ActualSerialPorts.initData(ActualSerialPorts.getfocusID(), (void*)&nowPort, sizeof nowPort);
		ActualSerialPorts.setfocusID(ActualSerialPorts.getfocusID() + 1);

		pTab4->m_Combo40.AddString(nowPort.GetPortNameW());

	};
	pTab4->m_Combo40.SetCurSel(0);
	RegCloseKey(hKey);

	return RES_OK;
}

CString CSmartBoxMasterKeyAppDlg::AutoDetect()
{
	if(m_COMConnected)COMDisconnect();

	int Len = 0, connectID = 0; DWORD Len2; CString str, Port;
	COMPortSeek();
	if (ActualSerialPorts.getendID() == -1)return NULLSTRING;
	for (int i = 0; i < ActualSerialPorts.getendID() + 1; i++) {
		Port = ((COMPort*)ActualSerialPorts.WhereisData(i, Len))->GetPortNameW();
		str.Format(GetCaption(72), Port.GetString());//ID72 %s port Connecting..
		SetStatusBarText(str);

		if (SerialConnect(Port) != RES_OK)continue;

		if (Serial_Detect() == RES_OK)
		{
			str.Format(GetCaption(73), Port.GetString());//ID73 %s Masterkey Detected!!
			SetStatusBarText(str);

			m_COMPort = ComPort.m_sPortName;
			connectID = i;
			pTab4->m_Combo40.SetCurSel(connectID);
			m_COMConnected = 1;
			COMConnect();

			return ((COMPort*)ActualSerialPorts.WhereisData(i, Len))->GetPortNameW();
		}

		if (SerialDisconnect() != RES_OK)continue;
	}
	str.Format(GetCaption(74), Port.GetString());//ID74 %s Masterkey Detecting failed..
	SetStatusBarText(str);

	pTab4->m_Combo40.SetCurSel(connectID);

	return NULLSTRING;
}

int CSmartBoxMasterKeyAppDlg::Serial_Write(int _Length)
{
	if (!m_COMConnected)return Lengthis0;
	return (int)ComPort.write(m_InterfaceStr, _Length);
}

int CSmartBoxMasterKeyAppDlg::Serial_Read(DWORD Length)
{
	if (!m_COMConnected)return Lengthis0;
	return (int)ComPort.read(m_InterfaceStr, Length);
}

int CSmartBoxMasterKeyAppDlg::SerialConnect(CString port)
{
	if (m_COMConnected)return RES_ERR;
	if (ComPort.open(port) == 0) {
		m_COMConnected = true;
		return RES_OK;
	}
	return RES_ERR;
}

int CSmartBoxMasterKeyAppDlg::SerialDisconnect()
{
	if (!m_COMConnected)return RES_ERR;
	if (ComPort.close() == 0) {
		m_COMConnected = false;
		return RES_OK;
	}
	return RES_ERR;
}


int CSmartBoxMasterKeyAppDlg::Serial_Detect()
{
	//[echoCommand     Packet]  STX 'e' ETX
	m_InterfaceStr[0] = ' ';
	m_InterfaceStr[1] = 2; 
	m_InterfaceStr[2] = 'e'; 
	m_InterfaceStr[3] = 3;

	Serial_Write(4);
	AutoDetectReadWaiting = true;
	Sleep(400);
	OnCommunication(NULL, NULL);
	if (!AutoDetectReadWaiting) {//if it was successful
		return RES_OK;
	}
	return RES_ERR;
}


afx_msg LRESULT CSmartBoxMasterKeyAppDlg::OnCommRead(WPARAM wParam, LPARAM lParam)
{
	return OnCommunication(wParam, lParam);
}


int CSmartBoxMasterKeyAppDlg::SensorAccess(char* serialnum)
{
	//serialnum[15] = 0;
	if (!strstr(serialnum, "\02NDT\05")) return RES_ERR;

	CString serial;
	int ret = 0;
	bool isSensorInitSucceed = false;

	ReadWaiting = 1;
	isReceivedRTNNDT = 0;

	// Interrupt!! Sensor Input
	LPWSTR _buf = (LPWSTR)malloc((strlen(serialnum) + 1) * sizeof(*_buf));
	for (int i = 0; i < strlen(serialnum) + 1; i++)_buf[i] = (wchar_t)serialnum[i];
	wchar_t* point;
	if (point = StrChrW(_buf, 0x1f))*point = 0;
	if (!(point = StrChrW(_buf, 0x05)))point = _buf; else point++;
	
	
	//point = _T("sbkkb000000010");
	//CONSTVALUE
	str.Format(GetCaption(75), point);//ID75 Sensor Accessed!! : %s
	AfxMessageBox(str);

	int res = DataReady(serial = point);
	free(_buf);
	if (res == RES_ERR)
	{
		sprintf_s((char*)m_InterfaceStr, sizeof(m_InterfaceStr), (const char*)"\02NDT\005000100\03");
		res = ComPort.write(m_InterfaceStr, strlen((char*)m_InterfaceStr));

		return RES_ERR;
	}
	if (strstr((char*)ReadBuffer, "\02RTN\05NDT")) return RES_ERR;

	memset(ReadBuffer, 0, BUFF_SIZE);

	//Make the packet.
	PacketData();


	//Transmit the Sensor data

	sprintf_s((char*)m_InterfaceStr, sizeof(m_InterfaceStr), (const char*)"\02NDT\05%04X%s%c01\03", PacketBuflen, PacketBuf,0x1F);

	CString Log;
	Log.Format(_T("UART Send : %S"), (char*)m_InterfaceStr); My_Log(Log, 2);

	res = ComPort.write(m_InterfaceStr, strlen((char*)m_InterfaceStr));

	// 마스터키 수신
	// 1초동안 무응답이면 에러 처리함.
	// 데이터 올 때마다 체크. 이상하면 에러처리.
	clock_t start;
	while (1)
	{
		start = clock();//1초마다
		while (ReadWaiting && !isReceivedRTNNDT) { if (clock() - start > 2200)break; }
		if (isReceivedRTNNDT)
		{
			ReadWaiting = 0;
			break;
		}
		if (clock() - start > 2200){
			ReadWaiting = 2;break;//에러처리
		}

		isReceivedRTNNDT = 0;
		ReadWaiting = 1;
	}
	if (!ReadWaiting)
	{
		CString Log;
		Log.Format(_T("ReadBuffer(%d): %S"), ReadStringLength, ReadBuffer); My_Log(Log, 1);

		char* point, * seek = (char*)ReadBuffer; int len = ReadStringLength;

		point = strstr((char*)seek, "\02RTN\05NDT");

		if (point)
		{
			_buf = (LPWSTR)malloc((strlen(point) + 1) * sizeof(*_buf));
			for (int i = 0; i < strlen(point) + 1; i++)_buf[i] = (wchar_t)point[i];

			if (StrStrW(_buf, _T("01")))
			{
				//CallMessageBox(GetCaption(76));//ID76 성공적으로 초기화되었습니다.
				str.Format(GetCaption(77),serial);//ID77 센서 %s : 초기화 성공.
				::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
				isSensorInitSucceed = true;
			}
			else
			{
				free(_buf);
				CallMessageBox(GetCaption(78));//ID78 초기화 오류.
				str.Format(GetCaption(79), serial);//ID79 센서 %s : 초기화 실패.
				::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
				return RES_ERR;
			}
			free(_buf);


			ret = RES_OK;
		}
	}
	else {
		if (ReadWaiting == 1)
		{
			AfxMessageBox(GetCaption(80));//ID80 마스터키 통신 오작동.
			str.Format(GetCaption(81), serial);//ID81 센서 %s : 초기화 타임아웃!!(최대 대기 시간 초과)
			::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
		}
		else if (ReadWaiting == 2)
		{
			str.Format(GetCaption(94), serial);//ID94 마스터키 무응답!!
			AfxMessageBox(str);//ID80 마스터키 통신 오작동.
			::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
		}
	}
	;
	if (isSensorInitSucceed)
	{
		if (m_isSensorReplace)res = Process_Replace();
		else res = GetAPIBox();//센서 등록 API

		if (!(res == RES_OK))
			CallMessageBox(GetCaption(92));//ID92 API 통신 오류. 센서를 다시 초기화하세요.

		ChangeCaption();
		Sleep(100);
		pTab4->OnBnClickedApiretry();//Tab1 리스트 다시 그리기
	}

	return ret;
}
UINT ThreadForCallMessage(LPVOID param)
{
	CSmartBoxMasterKeyAppDlg* p = (CSmartBoxMasterKeyAppDlg*)param;
	p->MessageBox(p->MessagetoPrint.GetString());
	return RES_OK;
}
void CSmartBoxMasterKeyAppDlg::CallMessageBox(CString Msg)
{
	MessagetoPrint = Msg;
	CallMessageThread = AfxBeginThread(ThreadForCallMessage, this);
	CallMessageThread->m_bAutoDelete = TRUE;
}

CString CSmartBoxMasterKeyAppDlg::HTTPGet(CString URL, CString* RetVal)
{
	// input : http://192.168.0.100:20130/api/

	// L"192.168.0.100"
	// 20130 (':'없으면 80)
	// /api/
	//CString URL = L"http://192.168.0.100:20130/api/slr/070805040604050506070802090207010305030702020202100";

	int part_DColon = URL.Find(_T("://"));
	int part_IP = part_DColon + 3;
	int part_PColon = URL.Find(_T(':'), part_IP);
	int part_Page = URL.Find(_T('/'), part_IP);

	CString Part_ip = URL.Mid(part_IP, (part_PColon == -1 ? part_Page : part_PColon) - part_IP);
	CString Part_port = (part_PColon != -1) ? URL.Mid(part_PColon + 1, part_Page - part_PColon - 1) : _T("80");
	CString Part_page = URL.Mid(part_Page);

	int int_port = _wtoi(Part_port.GetString());

	//UpdateData(TRUE);

	int Retval = HttpCall((LPWSTR)Part_ip.GetString(), int_port, Part_page, NULL, NULL);
	if (Retval != 200)
	{
		CString ret = NULLSTRING;

		ret.Format(_T("Error!! Result : %d"), Retval);
		return ret;
	}

	extern CString strCompleteString;
	if (RetVal)*RetVal = strCompleteString;
	return strCompleteString;
}
PacketCoder Packetcoder;
CString CSmartBoxMasterKeyAppDlg::Encoding100(CString data, CString* ret)
{
	// TODO: Add your implementation code here.
	//CString ss = _T("070805040604050506070802090207010305030702020202100");

	int random = rand();
	int mty = (int)(random * (20.0 / (double)RAND_MAX))+100;

	CString rets = Packetcoder.Encode(data, mty);

	if (ret)*ret = rets;
	return rets;
}
CString CSmartBoxMasterKeyAppDlg::Encoding800(CString data, CString* ret)
{
	// TODO: Add your implementation code here.
	//CString ss = _T("070805040604050506070802090207010305030702020202100");

	int mty = (int)(rand() * (20 / (double)RAND_MAX))+890;

	CString rets = Packetcoder.Encode(data, mty);

	if (ret)*ret = rets;
	return rets;
}

CString CSmartBoxMasterKeyAppDlg::Base64Encoding(CString data, CString* ret)
{
	CString res = Base64Coder::Encoding(data);
	if (ret)*ret = res;
	return res;
}

int JsonfromCString(Json::Value& val, CString& str)
{
	std::stringstream Data;

	Json::CharReaderBuilder reader;

	std::string out;
	// TODO : 한글 처리
	CW2A p(str);
	out = p;
	Data << out;
	std::string errs;

	bool isok = Json::parseFromStream(reader, Data, &val, &errs) == true;
	return isok ? RES_OK : RES_ERR;
}
int JsonDatatoString(Json::Value& val, CString& str)
{
	Json::StreamWriterBuilder writer;
	std::unique_ptr<Json::StreamWriter> write;
	std::stringstream Out;

	writer.validate(&val);
	write = std::unique_ptr<Json::StreamWriter>(writer.newStreamWriter());
	write->write(val, &Out);

	bool isString = val.isString();
	bool isNull   = val.isNull();

	CA2W p(Out.str().c_str());

	if (isNull)
	{
		str = _T("");
		return RES_OK;
	}

	str = p;

	if (isString)
	{
		/*
		char* strs = (char*)malloc(str.GetLength() - 1);
		for (int i = 0; i < str.GetLength() - 1; i++)
			strs[i] = str.GetBuffer()[i + 1];
		strs[str.GetLength() - 2] = 0;
		*/

		str = str.Mid(1, str.GetLength() - 2);//MultibytecodeDecode((CString)(str));
		//free(strs);
	}

	//std::string st = val.toStyledString();
	//str = st.c_str();
	//str = val.asCString();

	return RES_OK;
}

// 시스템 초깃값(Systemconst) 저장 파일(inifile)에 쓰기
int CSmartBoxMasterKeyAppDlg::SystemConst_Store()
{
	//WritePrivateProfileString(_T("DEFAULTS"), _T("APIURL"), Systemconst.m_ApiURL, file);

	CString s_nowLang;s_nowLang.Format(_T("%d"), nowLang);
	WritePrivateProfileString(_T("DEFAULTS"), _T("NOWLANGUAGE"), s_nowLang, inifile);

	return RES_OK;
}

// 시스템 초깃값(Systemconst) 저장 파일(inifile) 읽고 적용하기
int CSmartBoxMasterKeyAppDlg::SystemConst_Read()
{
	LPWSTR res = (LPWSTR)malloc(1024 * sizeof(WCHAR));

	GetPrivateProfileString(_T("DEFAULTS"), _T("APIURL"), _T(""), res, 1024, inifile);
	m_ApiURL = res;

	GetPrivateProfileString(_T("DEFAULTS"), _T("inifile"), _T(""), res, 1024, inifile);
	if (lstrlenW(res)==0)Logroot = herePath;
	else Logroot = res;

	GetPrivateProfileString(_T("DEFAULTS"), _T("LangPath"), _T(""), res, 1024, inifile);
	if (lstrlenW(res) == 0)Langpath = herePath;
	else Langpath = res;

	GetPrivateProfileString(_T("DEFAULTS"), _T("NOWLANGUAGE"), _T(""), res, 1024, inifile);
	if (_wtoi(res) == 0 && _tcslen(res) != 1)//if text
		nowLang = 1;
	else nowLang = _wtoi(res);
	/*

	GetPrivateProfileString(_T("DEFAULTS"), _T("LANGNUM"), _T(""), res, 1024, inifile);
	int nLang = _wtoi(res);

	CString s_langN;
	LangTable.RemoveAll();
	for (int i = 0; i < nLang; i++)
	{
		s_langN.Format(_T("LANG%d"), i);
		GetPrivateProfileString(_T("DEFAULTS"), s_langN, _T("DEFAULT"), res, 1024, inifile);
		s_langN = res;
		LangTable.SetAt(i, s_langN);
	}
	*/
	
	free(res);

	return RES_OK;
}

// 시스템 초깃값(Systemconst) 초기화
void CSmartBoxMasterKeyAppDlg::SystemConst_Reset()
{
}

// 센서 접근 후 사용자가 버튼 누를 때까지의 작업을 제어한다.
int CSmartBoxMasterKeyAppDlg::DataReady(CString str)
{
	m_AccessedSerial = str;//m_AccessedSerial : 접근한 센서의 equip_sn(시리얼넘버)
	SensorDataList sdt;
	int len = 0;

	if (m_Tab.GetCurSel() == 2)//Tab3이라면 : 대체할 새로운 센서 입력
	{
		pTab3->m_Button3A.EnableWindow(TRUE);
		m_isSensorReplace = true;
		pTab3->m_Edit3A.SetWindowTextW(str);
		m_COMConnected = 3;
		SetCOMICON();
	}
	else
	{
		m_isSensorReplace = false;

		// 1. 탭 1로 커서 이동
		m_Tab.SetCurSel(1);
		LRESULT i = 0;
		OnTcnSelchangeTab1(NULL, (LRESULT*)&i);
		//CONSTVALUE
		CString Msgstr;
		Msgstr.Format(GetCaption(82),str);//ID82 센서 %s 접근 : 셋팅 후 '전송 버튼' 클릭하세요.
		::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&Msgstr), NULL);

		GetAPISDI(str, sdt);
		SensorDataList2SensorData(sdt, AccessData);

		if (AccessData.work_zone_num == _T(""))AccessData.work_zone_num = work_zone_num;
		if (AccessData.work_zone_name == _T(""))AccessData.work_zone_name = work_zone_name;

		pTab2->SensorAccessed();

		// 3. ICon 변경(인터럽트, 센서 접근)
		m_COMConnected = 3;
		SetCOMICON();
		pTab2->m_Button2A.EnableWindow(TRUE);
		::PostMessage(hCommWnd, WM_TAB2_UPDATE_VAL, NULL, NULL);
		//pTab2->SensorAccessed();

	}

	// 잔송 버튼이 눌리면
	isNDT00 = 0;
	isFinishedValue = 0;
	while (!isFinishedValue);

	if (isFinishedValue == 2)return RES_ERR;//잘 못 받으면
	if (isNDT00)//NDT00 받으면
	{
		CString str; str.Format(GetCaption(81), m_AccessedSerial);//ID81 마스터키 대기시간 60초를 초과했습니다.
		CallMessageBox(str);
		pTab2->AccessdatOut();
		pTab2->m_Button2A.EnableWindow(FALSE);
		m_COMConnected = 1;
		SetCOMICON();
		return RES_ERR;
	}
	// 4. List데이터 가져오기

	// 5. return.(전송)

	if (m_isSensorReplace)//교체 모드라면
	{
		pTab3->m_Button3A.EnableWindow(FALSE);
		pTab3->m_Edit3A.GetWindowText(m_toSerialNo);// 교체에 사용할 toserial을 입력받는다.
		TransmitData = replaceData;
	}
	else //아니라면
	{
		pTab2->AccessdatOut();
		pTab2->m_Button2A.EnableWindow(FALSE);
		TransmitData = AccessData;
	}

	m_COMConnected = 1;
	SetCOMICON();

	return RES_OK;
}

int CSmartBoxMasterKeyAppDlg::SETICON(HICON hIcon)
{
	m_statusBar.GetStatusBarCtrl().SetIcon(1, hIcon);
	return RES_OK;
}

void CSmartBoxMasterKeyAppDlg::SensorDataList2SensorData(SensorDataList& in, SensorInitData& out)
{
	out.m_StrAPPKEY = in.appkey;  // APPKEY
	out.m_StrStartDate = in.StartDate;  // 시작시간
	out.m_StrWarningmin = in.warningmin;  // 경고기간
	out.m_StrDangermin = in.dangermin;  // 위험기간
	out.m_StrSavetotmin = in.savetotmin;  // 보관기간
	out.m_StrProduct = in.idxGoods;  // 상품명
	out.m_StrRACLL = in.RACLL;  // RAC정보(LL)
	out.m_StrRACMM = in.RACMM;  // RAC정보(MM)
	out.m_StrRACSS = in.RACSS;  // RAC정보(SS)
	out.m_StrSendTime = in.SendTimeSec;  // 전송주기
	out.m_StrRevision = in.Revision;  // 보정값(TP^-5,HU^10)
	out.m_isReplace.Format(_T("%d"),m_isSensorReplace);// 교체 플래그
	out.m_BoxNum = in.BoxNum;	//박스 번호
	out.work_zone_num = in.work_zone_num; // 창고 번호
	out.m_idxWE = in.idxWE; // 창고 번호
	out.range01 = in.range01; // 센서 1 범위

}

afx_msg LRESULT CSmartBoxMasterKeyAppDlg::OnTab2UpdateVal(WPARAM wParam, LPARAM lParam)
{
	pTab2->SensorAccessed();
	return RES_OK;
}


LPCTSTR format_What[13] = { _T("bt"),_T("tp"),_T("ox"),_T("du"),_T("co"),_T("hu"),_T("ni"),_T("pp"),_T("hd"),_T("uv"),_T("ps"),_T("cm"),_T("mt") };
int isEqualText(CString a, CString b)
{
	if (a == b)return true;
	else if (a.MakeUpper() == b.MakeUpper())return true;
	else return false;
}
int format_a(CString str)
{
	for (int i = 0; i < sizeof(format_What) / sizeof(LPWSTR); i++)
		if (isEqualText(format_What[i], str))return i;

	return 0xFF;
}
int SendPacketinData(void* dat, int len, uint8_t* buf, int& nowfocus)
{
	buf += nowfocus;
	//buf[0] = len;
	memcpy(buf, dat, len);
	return nowfocus += len;
}
int SendPacketinCString(CString str, uint8_t* buf, int& nowfocus)
{
	int len = str.GetLength();
	
	wchar_t* wstr = (wchar_t*)str.GetString();

	CW2A d(str, CP_UTF8);
	char* strUtf8 = d.m_psz;
	int nLen = strlen(d);
	//출처: https://yeobi27.tistory.com/entry/MFC-유니코드-멀티바이트-UTF8-문자열-인코딩-변환-모음 [KKIMSSI]

	buf += nowfocus;
	buf[0] = nLen;
	memcpy(buf + 1, strUtf8, nLen);


	return nowfocus += nLen+1;
}

char* Projects[1] = { "sb" };
CString countries[1] = { _T("kk") };
char sensorTypes[3] = { 'b','h','o' };
void Apn2EUI(char* APnum, uint8_t* APPKEY)
{
	union {
		struct {
			char Project[2];
			char country[2];
			char idxCli[4];
			char idxAP[4];
			char _null;
		}sd;
		char apn[13];
	}Temp;
	int iAP, iCli;
	char buf[5] = { 0, };

	memcpy(Temp.apn, APnum, 13);

	memcpy(buf, Temp.sd.idxCli, sizeof(Temp.sd.idxCli));
	iCli = atoi(buf);
	memcpy(buf, Temp.sd.idxAP, sizeof(Temp.sd.idxAP));
	iAP = atoi(buf);

	// println("snm        : %s",Temp.apn);
	// println("project    : %c%c",Temp.sd.Project[0],Temp.sd.Project[1]);
	// println("country    : %c%c",Temp.sd.country[0],Temp.sd.country[1]);
	// println("idxCli     : %d",iCli);
	// println("idxAP      : %d",iAP);
	// println("_null      : %d",Temp.sd._null);

	char buf1[16] = { 0xA7, 0x7B, 0x8B, 0xAB, 0x23, 0x6c, 0xcd, 0x00 ,  0x00, 0x00, 0x00, 0x00 ,  0x00, 0x00, 0x00, 0x00 };
	memset(buf, 0, sizeof(buf));
	memcpy(buf, Temp.sd.country, 2);

	buf1[7] = 0xFF;
	int i = 0;
	for (i = 0; i < sizeof(countries) / sizeof(char*); i++)
		if (isEqualText(countries[i], CString(buf)))
			buf1[7] = i;

	for (i = 0; i < 2; i++)
	{
		buf1[i + 12] = (iCli >> ((1 - i) * 8)) & 0xFF;
	}
	for (i = 0; i < 2; i++)
	{
		buf1[i + 14] = (iAP >> ((1 - i) * 8)) & 0xFF;
	}
	//memcpy(buf1+12,&iCli,2);
	//memcpy(buf1+14,&iAP ,2);
	//for(int i=7;i>4;i--){buf1[i] = (number)&0xFF;number>>=8;}

	// buf1[8] = (number)&0xFF;number>>=8;
	// buf1[7] = (number)&0xFF;number>>=8;...
	memcpy(APPKEY, buf1, 16);
	//println("APPKEY   : %s",printBin(APPKEY,16));
}
//TransmitData
//TxPacket( = ReadString's region), TxPacketLen
//->PacketBuf, PacketBuflen
int CSmartBoxMasterKeyAppDlg::PacketData()
{
	// 데이터 보낼 변수를 준비하는 함수
	// 정보(TransmitData)를 수합하여 RacketBuf에 코드로 저장한다.

	CString TransmitDataLog;
	//Log Start!!
	TransmitDataLog.Format(GetCaption(93));//ID93 Data Transmission Start!! 마스터키 전송 시작!
	My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("BoxNum         : %s"), TransmitData.m_BoxNum); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("idxWE          : %s"), TransmitData.m_idxWE); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("isReplace      : %s"), TransmitData.m_isReplace); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrAPPKEY      : %s"), TransmitData.m_StrAPPKEY); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrDangermin   : %s"), TransmitData.m_StrDangermin); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrProduct     : %s"), TransmitData.m_StrProduct); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrRACLL       : %s"), TransmitData.m_StrRACLL); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrRACMM       : %s"), TransmitData.m_StrRACMM); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrRACSS       : %s"), TransmitData.m_StrRACSS); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrRevision    : %s"), TransmitData.m_StrRevision); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrSavetotmin  : %s"), TransmitData.m_StrSavetotmin); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrSendTime    : %s"), TransmitData.m_StrSendTime); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrStartDate   : %s"), TransmitData.m_StrStartDate); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("StrWarningmin  : %s"), TransmitData.m_StrWarningmin); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("range01 Type   : %s"), TransmitData.range01.sensortype); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("range01 d01    : [%f,%f]"), TransmitData.range01.d01fr, TransmitData.range01.d01to); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("range01 d02    : [%f,%f]"), TransmitData.range01.d02fr, TransmitData.range01.d02to); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("range01 w01    : [%f,%f]"), TransmitData.range01.w01fr, TransmitData.range01.w01to); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("range01 w02    : [%f,%f]"), TransmitData.range01.w02fr, TransmitData.range01.w02to); My_Log(TransmitDataLog, 2);
	//TransmitDataLog.Format(_T("range01 Type   : %s"), TransmitData.range01.d01fr); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("work_zone_name : %s"), TransmitData.work_zone_name); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("work_zone_num  : %s"), TransmitData.work_zone_num); My_Log(TransmitDataLog, 2);
	TransmitDataLog.Format(_T("")); My_Log(TransmitDataLog, 2);


	TxPacketLen = 0;

	struct tm times; __time64_t timesend, timenow64;
	/*
	sscanf_s(CW2A(TransmitData.m_StrStartDate), "%d-%d-%d %d:%d:%d",
		&times.tm_year, &times.tm_mon, &times.tm_mday,
		&times.tm_hour, &times.tm_min, &times.tm_sec);
	times.tm_isdst = 0;
	times.tm_year -= 1900;
	times.tm_mon -= 1;
	*/

	//현재 시간 구하기
	//timesend = _time64(NULL);
	timenow64 = _time64(NULL);
	_localtime64_s(&times, &timenow64);
	TransmitData.m_StrStartDate.Format(_T("%04d-%02d-%02d")// %02d:%02d:%02d")
		, times.tm_year + 1900, times.tm_mon + 1, times.tm_mday);//, times.tm_hour, times.tm_min, times.tm_sec);
	AccessData.m_StrStartDate = TransmitData.m_StrStartDate;
	timenow64 += timediff;

	if (m_isSensorReplace)
	{
		//교체(Tab3) 시 초기화 시간 = DB의 시간
		sscanf_s(CW2A(TransmitData.m_StrStartDate), "%d-%d-%d %d:%d:%d",
			&times.tm_year, &times.tm_mon, &times.tm_mday,
			&times.tm_hour, &times.tm_min, &times.tm_sec);
		times.tm_isdst = 0;
		times.tm_year -= 1900;
		times.tm_mon -= 1;

		timesend = _mktime64(&times);
	}
	else
	{
		//초기화 모드(Tab2)
		timesend = timenow64;
	}
	
	My_Log(_T("Sending Time : ") + TransmitData.m_StrStartDate, 2);

	TxPacket = ReadString;
	memset(TxPacket, 0, 256);
	//패킷 완성!!
	uint8_t APPKey[16] = { 0, };
	Apn2EUI(CW2A(TransmitData.m_StrAPPKEY), (uint8_t*)APPKey);

	SendPacketinData(APPKey, 16, TxPacket, TxPacketLen);

	long long stime = timesend, timenow = timenow64;

	SendPacketinData(&stime, sizeof(stime), TxPacket, TxPacketLen);
	SendPacketinData(&timenow, sizeof(timenow), TxPacket, TxPacketLen);

	int warningmin, dangermin, savetotmin;
	warningmin = _wtoi(TransmitData.m_StrWarningmin);
	dangermin = _wtoi(TransmitData.m_StrDangermin);
	savetotmin = _wtoi(TransmitData.m_StrSavetotmin);

	SendPacketinData(&warningmin, sizeof(int), TxPacket, TxPacketLen);
	SendPacketinData(&dangermin, sizeof(int), TxPacket, TxPacketLen);
	SendPacketinData(&savetotmin, sizeof(int), TxPacket, TxPacketLen);

	//idxGoods(번호)를 글자로 변환
	CString GoodsName;
	if (Goods.Lookup(_wtoi(TransmitData.m_StrProduct), GoodsName))
		;
	else GoodsName = TransmitData.m_StrProduct;
	//변환 후 데이터 패킷에 추가한다.
	SendPacketinCString(GoodsName, TxPacket, TxPacketLen);

	SendPacketinCString(TransmitData.m_StrRACLL, TxPacket, TxPacketLen);
	SendPacketinCString(TransmitData.m_StrRACMM, TxPacket, TxPacketLen);
	SendPacketinCString(TransmitData.m_StrRACSS, TxPacket, TxPacketLen);

	int SendTime;// 전송주기(초)
	SendTime = _wtoi(TransmitData.m_StrSendTime);

	SendPacketinData(&SendTime, sizeof(int), TxPacket, TxPacketLen);
	uint32_t isreplace = _wtoi(TransmitData.m_isReplace);
	SendPacketinData(&isreplace, sizeof(int), TxPacket, TxPacketLen);


	//보정값 저장함수
	if (TransmitData.m_StrRevision == "")
	{
		for (int i = 0; i < 2; i++)
			TxPacket[TxPacketLen + i] = 0xFF;
	}
	else {//if there's any Revision Value
		WCHAR* point = TransmitData.m_StrRevision.GetBuffer();
		int focus = 0, count = 0;
		while ((focus = TransmitData.m_StrRevision.Find(_T('^'), focus)) != -1) { count++; focus++; }

		focus = 0;
		for (int i = 0; i < 2; i++)
		{
			if (focus != -1 && (focus = TransmitData.m_StrRevision.Find(_T('^'), focus)) != -1)
			{
				CString field = TransmitData.m_StrRevision.Mid(focus - 2, TransmitData.m_StrRevision.GetLength() - (focus - 2));

				int type = format_a(field.Mid(0, 2));
				float val = _wtof(field.Mid(3));
				TxPacket[TxPacketLen + i] = type;
				memcpy(TxPacket + (TxPacketLen + 2 + i * 4), &val, sizeof(float));

				focus++;
			}
			else
			{
				TxPacket[TxPacketLen + i] = 0xFF;
			}

		}
	}
	TxPacketLen += 10;
#pragma pack(push,1)
	struct Sensordd {
		uint8_t SensorType;
		float d01fr;//위험 범위 1
		float d01to;//
		float d02fr;//위험 범위 2
		float d02to;//
		float w01fr;//경고 범위 1
		float w01to;//
		float w02fr;//경고 범위 2
		float w02to;//
	}sdt;
#pragma pack(pop)
	memcpy(&sdt.d01fr, &TransmitData.range01.d01fr, sizeof(float) * 8);
	sdt.SensorType = format_a(TransmitData.range01.sensortype);
	if (sdt.SensorType == 0xFF)//선서타입이 ff면 NAN처리
	{
		for (int i = 0; i < 8; i++)
			*(&sdt.d01fr + i) = NAN;
	}

	SendPacketinData(&sdt, sizeof(Sensordd), TxPacket, TxPacketLen);
	
	for (int i = 0; i < TxPacketLen; i++)
	{
		sprintf_s(PacketBuf + i * 2, 3, "%02X", TxPacket[i]);
		PacketBuf[i * 2 + 3] = 0;
	}
	PacketBuflen = TxPacketLen;
	return RES_OK;
}

afx_msg LRESULT CSmartBoxMasterKeyAppDlg::OnStatbartxt(WPARAM wParam, LPARAM lParam)
{
	LPWSTR str = *((LPWSTR*)wParam);
	SetStatusBarText(str);
	return RES_OK;
}

int CSmartBoxMasterKeyAppDlg::Process_Replace()
{
	Json::Value rootArray, Var;
	CString DatatoSend;
	CString APINameCHG = _T("chg");
	bool isWellAPIed = false;

	DatatoSend = m_frSerialNo + "^" + m_toSerialNo;
	isWellAPIed = APISendEncoding100RetJson(APINameCHG, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameCHG + _T("_req"), rootArray);
		return RES_ERR;//Connection Error
	}

	CString res;

	if (!rootArray[0]["v_RESULT"].isNull())
	{
		int CHGResult = rootArray[0]["v_RESULT"].asInt();
		CString ss;
		ss.Format(_T("%d"), CHGResult);

		switch (CHGResult)
		{
		case  1:AfxMessageBox(GetCaption(84)); break;//ID84 교체 작업 성공!
		case  4:AfxMessageBox(GetCaption(85)); break;//ID85 교체대상 센서가 등록되어있지 않습니다.
		case  7:AfxMessageBox(GetCaption(86)); break;//ID86 대체품 센서가 등록되어있지 않습니다.
		case 17:AfxMessageBox(GetCaption(87)); break;//ID87 대체품 센서가 사용 중입니다.
		case 40:AfxMessageBox(GetCaption(88)); break;//ID88 교체대상 센서가 사용 중이 아닙니다.
		default:AfxMessageBox(GetCaption(89) + ss); break;//ID89 Result from Server : 
		}

		if (CHGResult == 1)APIInitalUpdate();
		return CHGResult ? RES_OK : RES_ERR;
	}
	return RES_ERR;// CHG Request Error

}


//~~~/api/"apin"/"Msg"  -> return.
CString CSmartBoxMasterKeyAppDlg::QueryAPI(CString apin, CString Msg)
{
	//CString Senddata = Encoding100(Base64Encoding(Msg, NULL), NULL);
	CString URL = m_ApiURL + apin + "/" + Msg;

	CString Ret = HTTPGet(URL, NULL);

	if (Ret.Find(_T("Error!! Result : ")) != -1)
	{
		AfxMessageBox(Ret);
		return _T("Error");
	}

	Ret = Ret.Mid(1, Ret.GetLength() - 2);

	Ret.Replace(_T("\\\""), _T("\""));

	return Ret;
}


int CSmartBoxMasterKeyAppDlg::isSensorWorking(SensorDataList& sdt)
{
	int isok = (sdt.StartDate != _T("")) && (sdt.RACLL != _T("")) && (sdt.RACLL != _T("") || (sdt.RACMM != _T("")) && (sdt.RACMM == _T("") || sdt.RACSS == _T("")));
	return isok;
}


int CSmartBoxMasterKeyAppDlg::APISendEncoding100RetJson(CString APIname, CString DatatoSend, Json::Value &jsonRet)
{
	CString Ret, Send, logstr;
	
	logstr.Format(_T("API Sending [%s] : %s"), APIname, DatatoSend);
	My_Log(logstr, 0);
	
	Send = Encoding100(Base64Encoding(DatatoSend, NULL), NULL);// 전송 문자열 준비(Data:텍스트,Send:암호화된 데이터)

	if ((Ret = QueryAPI(APIname, Send)) == _T("Error"))//에러처리
	{
		logstr.Format(_T("API Error (%s_ERR) : %s"), APIname, Ret);//로그 저장
		My_Log(logstr, 0);
		return RES_ERR;//Connection Error
	}
	JsonfromCString(jsonRet, Ret);

	return RES_OK;
}

int CSmartBoxMasterKeyAppDlg::APIInitalUpdate()
{
	return GetAPISLR(idxClient);
}

int CSmartBoxMasterKeyAppDlg::GetAPISLR(CString idxClient, CString admin, DataList* pSensorData)
{
	Json::Value rootArray, Var;
	CString DatatoSend;
	CString APINameSLR = _T("slr");
	bool isWellAPIed = false;

	DatatoSend = idxClient + "^" + work_zone_num + "^" + admin;
	isWellAPIed = APISendEncoding100RetJson(APINameSLR, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameSLR + _T("_req"), rootArray);
		return RES_ERR;//Connection Error
	}

	if (!rootArray[0]["Result"].isNull())
	{
		CString Result;
		JsonDatatoString(rootArray[0]["Result"], Result);
		AfxMessageBox(GetCaption(89) + Result);//ID89 Result from Server : 
		return RES_ERR;// slr Request Error
	}

	int len = rootArray.size();
	SensorDataList sdt;
	SensorList.RemoveAll();
	SensordatasINITED = 1;

	for (int i = 0; i < len; i++)
	{
		Var = rootArray[i];

		JsonDatatoString(Var["serialno"], sdt.serialno);

		if (SensorList.Lookup(sdt.serialno, sdt))
			SensorList.RemoveKey(sdt.serialno);

		JsonDatatoString(Var["StartDate"], sdt.StartDate);
		JsonDatatoString(Var["alertstatus"], sdt.alertstatus);
		JsonDatatoString(Var["savetotmin"], sdt.savetotmin);
		JsonDatatoString(Var["warningmin"], sdt.warningmin);
		JsonDatatoString(Var["dangermin"], sdt.dangermin);
		JsonDatatoString(Var["idxGoods"], sdt.idxGoods);
		JsonDatatoString(Var["RACLL"], sdt.RACLL);
		JsonDatatoString(Var["RACMM"], sdt.RACMM);
		JsonDatatoString(Var["RACSS"], sdt.RACSS);
		JsonDatatoString(Var["BoxCode"], sdt.BoxNum);
		JsonDatatoString(Var["work_zone_num"], sdt.work_zone_num);
		JsonDatatoString(Var["work_zone_name"], sdt.work_zone_name);
		int i_idxWE = Var["idxWE"].asInt();
		sdt.idxWE.Format(_T("%d"), i_idxWE);

		SensorList.SetAt(sdt.serialno, sdt);
	}
	m_ListNumSensor = SensorList.GetCount();
	pTab1->DataList_Update();

	return RES_OK;
}
#define ConnErrorPrefix _T("_conn")
void CSmartBoxMasterKeyAppDlg::APIShowResultError(CString ErrCode, Json::Value& jsonIn)
{
	if (!jsonIn[0]["Result"].isNull())
	{
		CString StrResult;
		JsonDatatoString(jsonIn[0]["Result"], StrResult);
		ShowAPIError(ErrCode, _T(": ") + StrResult);
	}
	else ShowAPIError(ErrCode + ConnErrorPrefix, NULLSTRING);

}
void CSmartBoxMasterKeyAppDlg::ShowAPIError(CString ErrorCode, CString Error)
{
	CString logstr;
	logstr.Format(_T("API Error (%s_ERR) %s"), ErrorCode, Error);//로그 저장
	::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&logstr), NULL);
	Sleep(10);
}

char* qURLencode(char* str);
int CSmartBoxMasterKeyAppDlg::GetAPIBox()
{
	//800대 암호화!
	// (890 ~ 909)/api/wei/in^idxClient^work_zone_num^equip_sn^boxcode ->idxWE
	// (890 ~ 909)/api/wei/u1^idxWE^idxGoods^savetotmin^warningtotmin^dangertotmin^memo
	// (890 ~ 909)/api/wei/u2^idxWE^RacNo^startdate

	CString logstr;//로그 문자열
	CString APINameWEI = _T("wei");
	CString APINameSBD = _T("sbd");
	CString idxWE;
	Json::Value rootArray, root, var;
	CString DatatoSend,StrError;
	bool isWellAPIed = false;

	DatatoSend.Format(_T("in^%s^%s^%s^%s^%s"), AccessData.m_idxWE,idxClient,work_zone_num, m_AccessedSerial,AccessData.m_BoxNum);
	isWellAPIed = APISendEncoding100RetJson(APINameWEI, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameWEI + _T("_in_res"), rootArray);
		return RES_ERR;//Connection Error
	}
	JsonDatatoString(rootArray[0]["v_RESULT"], idxWE);


	SensorDataList sdt;
	if (SensorList.Lookup(m_AccessedSerial, sdt))
		SensorList.RemoveKey(m_AccessedSerial);
	sdt.idxWE = idxWE;
	SensorList.SetAt(m_AccessedSerial, sdt);


	// (890 ~ 909)/api/wei/u1^idxWE^idxGoods^savetotmin^warningtotmin^dangertotmin^memo
	DatatoSend.Format(_T("u1^%s^%s^%s^%s^%s^"), idxWE, AccessData.m_StrProduct, AccessData.m_StrSavetotmin, AccessData.m_StrWarningmin, AccessData.m_StrDangermin);
	isWellAPIed = APISendEncoding100RetJson(APINameWEI, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameWEI + _T("_u1_res"), rootArray);
		return RES_ERR;//Connection Error
	}

	
	CString idxweAfter;
	JsonDatatoString(rootArray[0]["v_RESULT"], idxweAfter);
	if (_wtoi(idxweAfter) != _wtoi(idxWE))//실패하면
	{
		ShowAPIError(APINameWEI + _T("_u1_idxWE"), NULLSTRING);
		return RES_ERR;//wei/u1 idxWE Error
	}

	//HTTP 인코딩 처리  ->>  sdt.RAC (utf8 문자 보내기 위해서)
	sdt.RACLL = qURLencode(CW2A(AccessData.m_StrRACLL, CP_UTF8));
	sdt.RACMM = qURLencode(CW2A(AccessData.m_StrRACMM, CP_UTF8));
	sdt.RACSS = qURLencode(CW2A(AccessData.m_StrRACSS, CP_UTF8));

	// (890 ~ 909)/api/wei/u2^idxWE^RacNo^startdate
	DatatoSend.Format(_T("u2^%s^%s^%s^%s^%s"), idxWE, sdt.RACLL, sdt.RACMM, sdt.RACSS, AccessData.m_StrStartDate);
	isWellAPIed = APISendEncoding100RetJson(APINameWEI, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameWEI + _T("_u2_res"), rootArray);
		return RES_ERR;//Connection Error
	}
	JsonDatatoString(rootArray[0]["v_RESULT"], idxWE);


	sdt.StartDate = AccessData.m_StrStartDate;
	if (isSensorWorking(sdt))
	{
		CString res;

		DatatoSend.Format(_T("%s55/rac_ll%s^mm%s^ss%s"), m_AccessedSerial, sdt.RACLL, sdt.RACMM, sdt.RACSS);
		isWellAPIed = APISendEncoding100RetJson(APINameSBD, DatatoSend, rootArray);
		if (isWellAPIed || rootArray[0]["Result"].isNull())//에러처리
		{
			APIShowResultError(APINameSBD + _T("/rac_req"), rootArray);
			return RES_ERR;//Connection Error
		}

		JsonDatatoString(rootArray[0]["Result"], res);
		if (res != _T("01"))
		{
			ShowAPIError(APINameSBD + _T("/rac"), NULLSTRING);
			return RES_ERR;// SBD/str Error
		}


		struct tm times; time_t now;
		now = _time64(NULL);
		_localtime64_s(&times, &now);
		AccessData.m_StrStartDate.Format(_T("%04d%02d%02d%02d%02d%02d")
			, times.tm_year + 1900, times.tm_mon + 1, times.tm_mday, times.tm_hour, times.tm_min, times.tm_sec);


		DatatoSend.Format(_T("%s55/str_dt%s"), m_AccessedSerial, AccessData.m_StrStartDate);
		isWellAPIed = APISendEncoding100RetJson(APINameSBD, DatatoSend, rootArray);
		if (isWellAPIed || rootArray[0]["Result"].isNull())//에러처리
		{
			APIShowResultError(APINameSBD + _T("/str_req"), rootArray);
			return RES_ERR;//Connection Error
		}

		JsonDatatoString(rootArray[0]["Result"], res);
		if (res != _T("01"))
		{
			ShowAPIError(APINameSBD + _T("/str"), NULLSTRING);
			return RES_ERR;// SBD/str Error
		}

	}

	CallMessageBox(GetCaption(76));//ID76 성공적으로 초기화되었습니다.

	return RES_OK;
}


int CSmartBoxMasterKeyAppDlg::GetAPIGOD()
{
	Json::Value rootArray;
	CString DatatoSend;
	CString APINameIFO = _T("ifo");
	bool isWellAPIed = false;

	DatatoSend.Format(_T("god^%s^^"), idxClient);
	isWellAPIed = APISendEncoding100RetJson(APINameIFO, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameIFO + _T("/god_req"), rootArray);
		return RES_ERR;//Connection Error
	}

	CString code, name;
	int numofcode = rootArray.size();
	for (int i = 0; i < numofcode; i++)// 상품정보의 수만큼 저장한다.
	{
		JsonDatatoString(rootArray[i]["code"], code);
		if (Goods.Lookup(_wtoi(code), name))
		{
			//이미 있으면 지우고 쓴다.(덮어쓰기)
			Goods.RemoveKey(_wtoi(code));
		}

		JsonDatatoString(rootArray[i]["name"], name);
		Goods.SetAt(_wtoi(code),name);
	}


	return 0;
}


int CSmartBoxMasterKeyAppDlg::GetAPISYS()
{
	Json::Value rootArray;
	CString DatatoSend;
	CString APINameSYS = _T("sys");
	bool isWellAPIed = false;

	DatatoSend = idxClient + "^" + work_zone_num;
	isWellAPIed = APISendEncoding100RetJson(APINameSYS, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameSYS + _T("_req"), rootArray);
		return RES_ERR;//Connection Error
	}

	if (!rootArray[0]["Result"].isNull())
	{
		CString result;
		JsonDatatoString(rootArray[0]["Result"], result);
		AfxMessageBox(GetCaption(89) + result);//ID89 Result from Server : 
		return RES_ERR;
	}
	JsonDatatoString(rootArray[0]["ClientName"], ClientName);
	JsonDatatoString(rootArray[0]["work_zone_name"], work_zone_name);

	return RES_OK;
}


int CSmartBoxMasterKeyAppDlg::GetAPISDI(CString equip_sn, SensorDataList& sdt)
{
	if(equip_sn == _T(""))return -1;//NO frserial
	Json::Value rootArray;
	CString DatatoSend;
	CString APINameSDI = _T("sdi");
	bool isWellAPIed = false;

	DatatoSend = idxClient + "^" + work_zone_num + "^" + equip_sn;
	isWellAPIed = APISendEncoding100RetJson(APINameSDI, DatatoSend, rootArray);
	if (isWellAPIed || !rootArray[0]["Result"].isNull())//에러처리
	{
		APIShowResultError(APINameSDI + _T("_req"), rootArray);
		return RES_ERR;//Connection Error
	}

	if (!rootArray[0]["Result"].isNull())
	{
		CString Result;
		JsonDatatoString(rootArray[0]["Result"], Result);
		AfxMessageBox(GetCaption(89) + Result);//ID89 Result from Server : 
		return -1;// slr Request Error
	}

	Json::Value var = rootArray[0];

	JsonDatatoString(var["serialno"], sdt.serialno);

	//if (SensorList.Lookup(sdt.serialno, sdt))
	//	SensorList.RemoveKey(sdt.serialno);

	JsonDatatoString(var["appkey"], sdt.appkey);
	JsonDatatoString(var["StartDate"], sdt.StartDate);
	JsonDatatoString(var["WarningDate"], sdt.WarningDate);
	JsonDatatoString(var["DangerDate"], sdt.DangerDate);
	JsonDatatoString(var["LastDate"], sdt.LastDate);
	JsonDatatoString(var["alertstatus"], sdt.alertstatus);
	JsonDatatoString(var["savetotmin"], sdt.savetotmin);
	JsonDatatoString(var["warningmin"], sdt.warningmin);
	JsonDatatoString(var["dangermin"], sdt.dangermin);
	JsonDatatoString(var["idxGoods"], sdt.idxGoods);
	JsonDatatoString(var["RACLL"], sdt.RACLL);
	JsonDatatoString(var["RACMM"], sdt.RACMM);
	JsonDatatoString(var["RACSS"], sdt.RACSS);
	JsonDatatoString(var["SendTimeSec"], sdt.SendTimeSec);
	JsonDatatoString(var["Revision"], sdt.Revision);
	JsonDatatoString(var["BoxCode"], sdt.BoxNum);
	JsonDatatoString(var["work_zone_num"], sdt.work_zone_num);
	JsonDatatoString(var["work_zone_name"], sdt.work_zone_name);

	JsonDatatoString(var["sensortype"], sdt.range01.sensortype);
	
	CString RangeNumber;
	JsonDatatoString(var["w01fr"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.w01fr = NAN; else sdt.range01.w01fr = _wtof(RangeNumber);
	JsonDatatoString(var["w01to"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.w01to = NAN; else sdt.range01.w01to = _wtof(RangeNumber);
	JsonDatatoString(var["d01fr"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.d01fr = NAN; else sdt.range01.d01fr = _wtof(RangeNumber);
	JsonDatatoString(var["d01to"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.d01to = NAN; else sdt.range01.d01to = _wtof(RangeNumber);
	JsonDatatoString(var["w02fr"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.w02fr = NAN; else sdt.range01.w02fr = _wtof(RangeNumber);
	JsonDatatoString(var["w02to"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.w02to = NAN; else sdt.range01.w02to = _wtof(RangeNumber);
	JsonDatatoString(var["d02fr"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.d02fr = NAN; else sdt.range01.d02fr = _wtof(RangeNumber);
	JsonDatatoString(var["d02to"], RangeNumber); if (RangeNumber == _T(""))sdt.range01.d02to = NAN; else sdt.range01.d02to = _wtof(RangeNumber);

	int i_idxWE = var["idxWE"].asInt();
	sdt.idxWE.Format(_T("%d"), i_idxWE);

	return 0;
}


void CreateFolder(CString csPath)
{
	//input D:\\folder1\\file\\f
	//단계 : 
	//1.D:\\   ->생성X
	//2.D:\\folder1\\ 
	//3.D:\\folder1\\file\\
	//4.D:\\folder1\\file\\f

	int focus=0;
	while (1)
	{
		focus = csPath.Find(_T("\\"), ++focus);
		if (focus <= 1)break;
		CString tmpPath = csPath.Left(focus);

		if(csPath.GetAt(focus-1) != _T(':'))//루트 디렉토리가 아니라면
			CreateDirectory(tmpPath, NULL);
	}
	CreateDirectory(csPath, NULL);
}

char* qURLencode(char* str)
{
	char* encstr, buf[2 + 1];
	unsigned char c;
	int i, j;

	if (str == NULL) return NULL;
	if ((encstr = (char*)malloc((strlen(str) * 3) + 1)) == NULL)
		return NULL;

	for (i = j = 0; str[i]; i++)
	{
		c = (unsigned char)str[i];
		if ((c >= '0') && (c <= '9')) encstr[j++] = c;
		else if (c == ' ') encstr[j++] = '+';    // 웹에 맞게 추가
		else if ((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
		else if ((c >= 'a') && (c <= 'z')) encstr[j++] = c;
		else if ((c == '@') || (c == '.') || (c == '/') || (c == '\\')
			|| (c == '-') || (c == '_') || (c == ':'))
			encstr[j++] = c;
		else
		{
			sprintf_s(buf, "%02x", c);
			encstr[j++] = '%';
			encstr[j++] = buf[0];
			encstr[j++] = buf[1];
		}
	}
	encstr[j] = '\0';

	return encstr;
}


int iswritinglog = 0;
LPWSTR logKinds[3] = {_T("LOG"),_T("ERR"),_T("MSG")};
// 로그 출력 함수
// Type : { 0:"LOG", 1:"ERR", 2:"MSG" }
int CSmartBoxMasterKeyAppDlg::MyLog_Add(CString logData, int Type)
{
	iswritinglog = 1;
	CFile file;
	CString Logpath, Log;
	time_t nows;
	nows = _time64(NULL);
	tm now;
	_localtime64_s(&now, &nows);

	Logpath.Format(_T("%s\\%s\\%04d%02d")
		, Logroot
		, _T("MasterKey")
		, now.tm_year + 1900, now.tm_mon + 1);


	CString filename;
	filename.Format(_T("%04d-%02d-%02d.log"), now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);

	filename = Logpath + _T("\\") + filename;

	

	if (!file.Open(filename, CFile::modeWrite))// 열 파일이 없다면
	{
		CreateFolder(Logpath);

		if (!file.Open(filename, CFile::modeCreate|CFile::modeWrite))
		{
			AfxMessageBox(GetCaption(90));//ID90 파일 만들기 실패.
			return RES_ERR;
		}

		TCHAR szTmp[2];
		WORD wd = 0xfeff;//유니코드 식별자
		memcpy(szTmp, &wd, 2);

		file.Write(szTmp,2);
		file.Flush();

	}
	file.SeekToEnd();

	CString logtext;
	logtext.Format(_T("%04d-%02d-%02d %02d:%02d:%02d [ %3s ] :  %s \r\n")
			, now.tm_year + 1900, now.tm_mon + 1, now.tm_mday
			, now.tm_hour, now.tm_min, now.tm_sec
			, logKinds[((unsigned)Type > 2 ? 0 : Type)]
			, logData);
	TCHAR* szdata = new TCHAR[logtext.GetLength() + 1];
	int buflen = logtext.GetLength() + 1;
	_tcscpy_s(szdata, buflen, logtext);// 버퍼 준비
	int nLen = _tcslen(szdata);// 길이 구함

	file.Write(szdata, nLen * sizeof(WCHAR));
	file.Close();

	free(szdata);

	//AfxMessageBox(logData);
	iswritinglog = 0;

	return RES_OK;
}


//afx_msg LRESULT CSmartBoxMasterKeyAppDlg::OnLog(WPARAM wParam, LPARAM lParam)
//{
//	while (iswritinglog)Sleep(1);
//	return 0;
//}

// 로그 출력 함수
// Type : { 0:"LOG", 1:"ERR", 2:"MSG" }
int CSmartBoxMasterKeyAppDlg::My_Log(CString Log, int type)
{
	while (iswritinglog)Sleep(1);
	MyLog_Add(Log, type);
	return RES_OK;
}


int CSmartBoxMasterKeyAppDlg::SetLang(int nLang)
{
	CaptionDBpath = Langpath + _T("\\language.db");

	CW2A p(CaptionDBpath);
	char* path = p;
	int nRes = sqlite3_open(path, &pCaptionDB);
	if (nRes)
	{
		captionavail = 0;

		AfxMessageBox(GetCaption(91));//ID91 Error loading language.
		return RES_ERR;
	}
	captionavail = 1;
	char** result = NULL;
	char* pErr;
	int rows, columns;
	char* query = "SELECT * FROM language";
	nRes = sqlite3_get_table(pCaptionDB, query, &result, &rows, &columns, &pErr);
	if (nRes)
	{
		AfxMessageBox(CString(sqlite3_errmsg(pCaptionDB)));
		return RES_ERR;
	}
	CaptionList.RemoveAll();
	LangTable.RemoveAll();
	CString str,idxCaption,Caption,c,Language;
	int NowLang;

	for (int i = 1; i < columns; i++)
	{
		Language = CA2W(result[i], CP_UTF8);//LangTable에 언어 종류 입력
		LangTable.SetAt(i-1, Language);
	}
	if (!LangTable.Lookup(nLang, Language))//현재 언어 찾아보기
	{
		AfxMessageBox(GetCaption(91));//ID91 Error loading language.
		return RES_ERR;
	}

	#define ArrayYX(y,x,NumofRows) (y*NumofRows + x)

	for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
	{
		idxCaption = CA2W(result[ArrayYX(rowCtr, 0, columns)], CP_UTF8);
		Caption    = CA2W(result[ArrayYX(rowCtr, nLang + 1, columns)], CP_UTF8);

		CaptionList.SetAt(_wtoi(idxCaption), Caption);
	}

	sqlite3_free_table(result);

	return RES_OK;
}

// 언어에 따른 Caption을 얻는 함수.
int CSmartBoxMasterKeyAppDlg::GetCaption(int id, CString& out)
{
	if (!captionavail || !CaptionList.Lookup(id, out) || out.GetString() == 0)
	 // 언어 파일 로드가 안 되었거나   겸색 시에 없거나 값이 0일 경우
	{
		out.Format(_T("ID%d"), id);
	}
	return RES_OK;
}
CString CSmartBoxMasterKeyAppDlg::GetCaption(int id)
{
	CString s_caption;
	GetCaption(id, s_caption);
	return s_caption;
}


void CSmartBoxMasterKeyAppDlg::ChangeCaption()
{
	//갭션 언어 설정!!
	int a = m_Tab.GetCurSel();

	m_Tab.DeleteAllItems();
	m_Tab.InsertItem(0, GetCaption(62));//ID62 리스트
	m_Tab.InsertItem(1, GetCaption(63));//ID63 박스등록
	m_Tab.InsertItem(2, GetCaption(64));//ID64 교체
	m_Tab.InsertItem(3, GetCaption(65));//ID65 등록
	m_Tab.SetCurSel(a);

	pTab1->ChangeCaption();
	pTab2->ChangeCaption();
	pTab3->ChangeCaption();
	pTab4->ChangeCaption();
}

int CSmartBoxMasterKeyAppDlg::KeyProcess(CString str)
{
	pTab2->m_Edit21.SetWindowText(str);

	m_Tab.SetCurSel(1);//to TAB2(to go to the 등록 tab.)
	LRESULT k = 0;
	OnTcnSelchangeTab1(0, &k);
	return RES_OK;
}
