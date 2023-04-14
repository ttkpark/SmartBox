
// SmartBoxDaemonDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SmartBoxDaemon.h"
#include "SmartBoxDaemonDlg.h"
#include "afxdialogex.h"

#include "CTab0.h"
#include "CTab1.h"
#include "CDataTableDlg.h"
#include "HttpCall.h"
#include "Base64Coder.h"
#include "json.h"
#include "PacketCoder.h"
#include "CBoxManageDlg.h"
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSmartBoxDaemonDlg* MainDlg;
HWND hCommWnd;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
//	virtual void PostNcDestroy();
public:
//	afx_msg void OnTrayicon32771();
//	afx_msg void OnTrayicon32772();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


void DataList::initContents(int _NumofContents) {
	focus = 0;
	contents = _NumofContents;
	data = (void**)malloc(_NumofContents * 4);
	Size = (int*)malloc(_NumofContents * 4);
	memset(data, 0, _NumofContents * 4);
	memset(Size, 0, _NumofContents * 4);
}
void DataList::initData(int ContentID, void* _data, int dSize) {
	if (ContentID > contents)return;
	data[ContentID] = malloc(dSize);
	memcpy(data[ContentID], _data, dSize);
	Size[ContentID] = dSize;
}
void DataList::DeleteData(int ContentID) {
	if (data[ContentID] == 0)return;
	free(data[ContentID]);
}
void* DataList::WhereisData(int ContentID, int& _len) {
	if (_len != 0)_len = Size[ContentID];
	return data[ContentID];
}
int DataList::AlldataSize() {
	int sum = 0;
	for (int i = 0; i < contents; i++)
		sum += Size[i];
	return sum;
}
void DataList::DeleteContents() {
	for (int i = 0; i < contents; i++) {
		DeleteData(i);
	}
	if (data != 0)free(data);
	if (Size != 0)free(Size);
}
int DataList::getfocusID() {
	return focus;
}
int DataList::setfocusID(int _focus) {
	return focus = _focus;
}
int DataList::getendID() {
	for (int i = 0; i < contents; i++) {
		if (Size[i] == 0) {
			if (i == 0)return 0;
			else return i - 1;
		}
	}
}
DataList::~DataList()
{
	DeleteContents();
}

wchar_t COMPortTestStrW[20];
char COMPortTestStr[20];
wchar_t* COMPort::GetPortNameW() {
	wsprintfW(COMPortTestStrW, L"COM%d", number);
	return COMPortTestStrW;
}
char* COMPort::GetPortName() {
	sprintf_s(COMPortTestStr, "COM%d", number);
	return COMPortTestStr;
}


// CSmartBoxDaemonDlg dialog


CSmartBoxDaemonDlg::CSmartBoxDaemonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMARTBOXDAEMON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isNDT00 = 0;
}

void CSmartBoxDaemonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, m_EditText);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

#define WM_TRAY_NOTIFICATION WM_APP + 1

BEGIN_MESSAGE_MAP(CSmartBoxDaemonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_MESSAGE(WM_TRAY_NOTIFICATION, &CSmartBoxDaemonDlg::OnTrayNotifycation)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSmartBoxDaemonDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_TRAYICON_32771, &CSmartBoxDaemonDlg::OnTrayicon32771)
	ON_COMMAND(ID_TRAYICON_32772, &CSmartBoxDaemonDlg::OnTrayicon32772)
	ON_COMMAND(WM_COMM_READ,(AFX_PMSG)&CSmartBoxDaemonDlg::OnCommunication)
	ON_MESSAGE(WM_COMM_READ, &CSmartBoxDaemonDlg::OnCommRead)
	ON_MESSAGE(WM_TAB1_UPDATE_VAL, &CSmartBoxDaemonDlg::OnTab1UpdateVal)
	ON_MESSAGE(WM_STATBARTXT, &CSmartBoxDaemonDlg::OnStatbartxt)
END_MESSAGE_MAP()


//CString 타입에 해당하는 해시 함수가 없으므로 다음과 같이 정의

template <> UINT AFXAPI HashKey<CString>(CString str)
{
	LPCTSTR key = (LPCTSTR)str;
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

// CSmartBoxDaemonDlg message handlers

BOOL CSmartBoxDaemonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MainDlg = this;
	hCommWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	//탭에 영향을 주는 초기화는 여기에 추가하새요.
	m_iSensorCnt = 0;
	m_ListNumSensor = 0;
	m_COMConnected = 0;
	idxClient = _T("1");
	SensorList.InitHashTable(10007);

	/*
	CMap<CString, CString, SensorDataList, SensorDataList&> List;
	List.InitHashTable(10007);

	SensorDataList sdt;
	CString ss;
	//List[_T("sbkkb000000001")] = sdt;
	//List[_T("sbkkb000000002")] = sdt;
	ss = _T("sbkkb000000001"); List[ss] = sdt;
	ss = _T("sbkkb000000002"); List[ss] = sdt;
	
	
	CString namea = _T("sbkkb000000001");
	if (List.Lookup(namea, sdt))
		AfxMessageBox(_T("ss1"));

	namea = _T("sbkkb000000002");
	if (List.Lookup(namea, sdt))
		AfxMessageBox(_T("ss2"));

	//namea = _T("sbkkb000000002");
	//if (List.Lookup(namea, sdt))
	//	AfxMessageBox(_T("ss2"));

	namea = _T("sbkkb000000003");
	if (List.Lookup(namea, sdt))
		AfxMessageBox(_T("ss3"));
	*/
	/*
	CString name = _T("sbkkb000000001");
	if (SensorList.Lookup(name, sdt))
		AfxMessageBox(_T("ss1"));

	name = _T("sbkkb000000002");
	if (SensorList.Lookup(name, sdt))
		AfxMessageBox(_T("ss2"));

	name = _T("sbkkb000000003");
	if (SensorList.Lookup(name, sdt))
		AfxMessageBox(_T("ss3"));
	*/

	//int len = SensorList.GetCount();

	SystemConst_Read();
	
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

	CRect rect;
	CString cStr;

	m_statusBar.Create(this);
	m_statusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	

	//setting the status bar....
	this->GetClientRect(&rect);
	m_statusBar.SetPaneInfo(0, ID_INDICATOR_STATTEXT, SBPS_NORMAL, rect.Width()-32);
	m_statusBar.SetPaneInfo(1, ID_INDICATOR_STATICON, SBPS_STRETCH, 20);
	m_statusBar.MoveWindow(rect.left, rect.bottom - 20, rect.Width(), 20);

	SetStatusBarText(_T("This is ID_INDICATOR_STATTEXT!!"));

	m_Tab.InsertItem(0, _T("설정"));
	m_Tab.InsertItem(1, _T("수동"));

	m_Tab.SetCurSel(0);

	m_Tab.GetWindowRect(&rect);


	pDataDlg = new CDataTableDlg();
	pDataDlg->p_parent = this;
	pDataDlg->Create(IDD_DATATABLE, CWnd::GetDesktopWindow());
	pDataDlg->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_ASYNCWINDOWPOS);


	pDlg0 = new CTab0;
	pDlg0->p_parent = this;
	pDlg0->Create(IDD_TAB0, &m_Tab);
	pDlg0->SetWindowPos(NULL, 1, 21, rect.Width() - 4, rect.Height() - 24, SWP_NOZORDER | SWP_SHOWWINDOW);
	

	pDlg1 = new CTab1;
	pDlg1->p_parent = this;
	pDlg1->Create(IDD_TAB1, &m_Tab);
	pDlg1->SetWindowPos(NULL, 1, 21, rect.Width() - 4, rect.Height() - 24, SWP_NOZORDER);

	COMAutoDetect();
	pDlg0->m_COMCombo.GetLBText(0, m_COMPort);
	pDlg0->p_parent->SetStatusBarText(m_COMPort);

	pDlg0->SensorListChanged();
	SetCOMICON();

	srand(time(NULL));
	

	pBoxDlg = new CBoxManageDlg();
	pBoxDlg->p_parent = this;
	pBoxDlg->Create(IDD_BOXMANAGE,CWnd::GetDesktopWindow());


	//CString retval = _T("");
	//HTTPGet(L"http://192.168.0.100:20130/api/slr/070805040604050506070802090207010305030702020202100", &retval);
	//AfxMessageBox(retval);

	// TODO: Add extra initialization here
	 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmartBoxDaemonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartBoxDaemonDlg::OnPaint()
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
HCURSOR CSmartBoxDaemonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//BOOL CSmartBoxDaemonDlg::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	cs.style &= ~(WS_THICKFRAME);
//	cs.cx = 100;
//	cs.cy = 100;
//	return CDialogEx::PreCreateWindow(cs);
//}


BOOL CSmartBoxDaemonDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreCreateWindow(cs);
}



int CSmartBoxDaemonDlg::SetStatusBarText(CString Text)
{
	m_statusBar.SetPaneText(0, Text);
	return 0;
}


void CSmartBoxDaemonDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		pDlg0->ShowWindow(SW_SHOW);
		pDlg1->ShowWindow(SW_HIDE);
		break;
	case 1:
		pDlg0->ShowWindow(SW_HIDE);
		pDlg1->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	
	*pResult = 0;
}


BOOL CSmartBoxDaemonDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return true;

	return CDialogEx::PreTranslateMessage(pMsg);
}

int IconRes[4] = { IDI_OFF,IDI_ON,IDI_WARN,IDI_SENSOR };
void CSmartBoxDaemonDlg::SetCOMICON()
{
	int icon = IconRes[m_COMConnected];
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(icon), IMAGE_ICON, 18, 18, 0);

	SETICON(hIcon);

	if (m_COMConnected)
		pDlg0->m_COMConnectBtn.SetWindowText(_T("연결 끊기"));
	else
		pDlg0->m_COMConnectBtn.SetWindowText(_T("연결"));
}

void CSmartBoxDaemonDlg::COMConnect()
{
	CString str;
	//SetCOMPortState(p_parent->m_COMConnected);
	if (m_COMConnected)
	{
		SetCOMICON();
		return;
	}
	if (SerialConnect(m_COMPort) == IDS_OK)
	{
		if (Serial_Detect() != IDS_OK) {
			m_COMConnected = 2;
			str.Format(_T("There's no Masterkey on %s"), m_COMPort.GetString());
			SetStatusBarText(str);
		}
		else
		{
			m_COMConnected = 1;
			str.Format(_T("%s 마스터키 연결 성공!"), m_COMPort.GetString());
			SetStatusBarText(str);
		}

		SetCOMICON();
	}
	else AfxMessageBox(_T("COM 포트 연결 실패."));
}

int CSmartBoxDaemonDlg::COMDisconnect()
{
	if (!m_COMConnected)
	{
		SetCOMICON();
		return 0;
	}
	if (SerialDisconnect() != IDS_OK)
		AfxMessageBox(_T("COM 포트 닫기 실패."));
	SetStatusBarText(_T("COM port Close!!\r\n"));
	SetCOMICON();
	return 0;
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
	uint8_t  startDate[4];      // 시작시간 -> (dat) 4 바이트 ( YY YY MM DD )
} DatafromPC;
typedef struct
{
	DatafromPC Data;
	uint8_t  ProductBitmap[968]; // 상품명   -> (dat) 968바이트 한글 비트맵
	uint8_t  APPKEY[16];         // APPKEY   -> (dat) 16 바이트
}SendDat;
SendDat Sendata;
void CSmartBoxDaemonDlg::COMAutoDetect()
{
	AutoDetect();
}

int count = 0;
UINT ThreadForSensorAccess(LPVOID param);
long CSmartBoxDaemonDlg::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);//받는 데이터 타입을 알기 위해
	CString str = _T("");
	CString result;
	BYTE aByte; //데이터를 저장할 변수 

	int iSize = (ComPort.m_QueueRead).GetSize(); //포트로 들어온 데이터 갯수
												 //SetCommState에 설정된 내용 때문에 거의 8개씩 들어옴
	uint8_t Serach = 0;
	if (isWorkingAccess)
		Serach = 0;
	for (int i = 0; i < iSize; i++)//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
	{

		(ComPort.m_QueueRead).GetByte(&aByte);//큐에서 데이터 한개를 읽어옴

		

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
					ReadWaiting = 2; lastfoc = i;
				}
				if (ReadWaiting == 2) {
					ReadString[i-lastfoc] = aByte;
					if (aByte == 4 || aByte == 3) {
						ReadString[i - lastfoc + 1] = 0;
						ReadStringLength = iSize;
						ReadWaiting = false;
						isReceivedRTNNDT = 1;
						Serach = 3;
					}
				}
			}
		}

		if (!Hex)
		{
			str.Format(_T("%c"), aByte);
			count++;
		}
		else
		{
			str.Format(_T(" %2X "), aByte);
			count += 4;
		}
		if (count >= 90)
		{
			count = 0;
			result += "\r\n";
		}

		result += str;

	}
	//if (result.GetLength() > 1)SetStatusBarText(result);
	if (Serach == 3)
	{
		if (ReadString[0] == 0x02 && strstr((char*)ReadString, "RTN") && strstr((char*)ReadString, "NDT") && strstr((char*)ReadString, "00"))
		{
			isNDT00 = 1;
			isFinishedValue = 1;
		}
		SetStatusBarText(CString(str));
	}
	if ( Accessfocus == 5) {
		Accessfocus = 6;
		m_SensorAccessed = true;
		ReadWaiting = 2;
	}
	if (Accessfocus == 6 && !isWorkingAccess)
	{
		Accessfocus = 0;
		isWorkingAccess = 1;
		/*
		char* ss = (char*)memchr((void*)ReadString, 0x1F, ReadStringLength);
		if (ss)SensorAccess(ss - 15);

		isWorkingAccess = 0;
		m_SensorAccessed = false;
		*/
		SensorAccessThread = AfxBeginThread(ThreadForSensorAccess, this);
		Sleep(100);
	}

	return 0;
}

UINT ThreadForSensorAccess(LPVOID param)
{
	CSmartBoxDaemonDlg* p = (CSmartBoxDaemonDlg*)param;
	p->OnTrayicon32771();
	//char* ss = (char*)memchr(p->ReadString, 0x1F, p->ReadStringLength);
	char* ss = (char*)p->ReadString + 15;
	if (ss)p->SensorAccess(ss - 15);
	p->isWorkingAccess = 0;
	p->m_SensorAccessed = false;
	return 0;
}

int CSmartBoxDaemonDlg::BTTray()
{
	memset(&nid, 0, sizeof nid);
	nid.cbSize = sizeof nid;
	nid.uID = 84922;
	nid.hWnd = this->GetSafeHwnd();
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
	lstrcpy(nid.szTip, L"WzmsDaemon Control");
	nid.uCallbackMessage = WM_TRAY_NOTIFICATION;

	Shell_NotifyIcon(NIM_ADD, &nid);
	this->ShowWindow(SW_HIDE);
	return 0;
}

//출처: http://bigmark.tistory.com/21 [마크의 맥시멈 라이프]

afx_msg LRESULT CSmartBoxDaemonDlg::OnTrayNotifycation(WPARAM wParam, LPARAM lParam)
{
	if (wParam != 84922)return 0;

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
		if (!Shell_NotifyIcon(NIM_DELETE, &nid)) {
			AfxMessageBox(L"트레이 아이콘 제거 실패");
			return -1;
		}
		this->ShowWindow(SW_SHOW);
		break;
	}
	//Shell_NotifyIcon(NIM_DELETE,&nid);
	return 1;
}

void CSmartBoxDaemonDlg::OnTrayicon32771()
{
	if (!Shell_NotifyIcon(NIM_DELETE, &nid))
	{
		//AfxMessageBox(_T("트레이 아이콘 제거 실패"));
		return;
	}
	this->ShowWindow(TRUE);
}

void CSmartBoxDaemonDlg::OnTrayicon32772()
{
	if (!Shell_NotifyIcon(NIM_DELETE, &nid))
	{
		AfxMessageBox(_T("트레이 아이콘 제거 실패"));
		return;
	}
	CDialogEx::OnOK();
}

// 사용가능한 COM포트 조회
int CSmartBoxDaemonDlg::COMPortSeek()
{
	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	wchar_t szData[20], szName[100];
	char cszData[20], cszName[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	COMPort nowPort;
	char* pointer; int i = 0, portnum = 0;;
	pDlg0->m_COMCombo.ResetContent();
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

		pDlg0->m_COMCombo.AddString(nowPort.GetPortNameW());

	};
	pDlg0->m_COMCombo.SetCurSel(0);
	RegCloseKey(hKey);

	return 0;
}

CString CSmartBoxDaemonDlg::AutoDetect()
{
	if (m_COMConnected)return 0;
	int Len = 0; DWORD Len2;CString str,Port;
	COMPortSeek();
	for (int i = 0; i < ActualSerialPorts.getendID() + 1; i++) {
		Port = ((COMPort*)ActualSerialPorts.WhereisData(i, Len))->GetPortNameW();

		str.Format(_T("%s port Connecting.."), Port.GetString());
		SetStatusBarText(str);

		if (SerialConnect(Port) != IDS_OK)continue;

		if (Serial_Detect() == IDS_OK)
		{
			str.Format(_T("%s Masterkey Detected!!"), Port.GetString());
			SetStatusBarText(str);

			m_COMPort = ComPort.m_sPortName;
			pDlg0->m_COMCombo.SetCurSel(i);
			m_COMConnected = 1;
			COMConnect();

			return ((COMPort*)ActualSerialPorts.WhereisData(i, Len))->GetPortNameW();
		}

		if (SerialDisconnect() != IDS_OK)continue;
	}
	str.Format(_T("%s Masterkey Detecting failed.."), Port.GetString());
	SetStatusBarText(str);

	return 0;
}

int CSmartBoxDaemonDlg::Serial_Write(int _Length)
{
	if (!m_COMConnected)return 0;
	// TODO: 여기에 구현 코드 추가.
	return (int)ComPort.write(m_InterfaceStr, _Length);
}

int CSmartBoxDaemonDlg::Serial_Read(DWORD Length)
{
	if (!m_COMConnected)return 0;
	// TODO: 여기에 구현 코드 추가.
	return (int)ComPort.read(m_InterfaceStr, Length);
}

int CSmartBoxDaemonDlg::SerialConnect(CString port)
{
	if (m_COMConnected)return -1;
	// TODO: 여기에 구현 코드 추가.
	if (ComPort.open(port) == 0) {
		m_COMConnected = true;
		return IDS_OK;
	}
	return -1;
}

int CSmartBoxDaemonDlg::SerialDisconnect()
{
	if (!m_COMConnected)return -1;
	// TODO: 여기에 구현 코드 추가.
	if (ComPort.close() == 0) {
		m_COMConnected = false;
		return IDS_OK;
	}
	return -1;
}


int CSmartBoxDaemonDlg::Serial_Detect()
{
	m_InterfaceStr[0] = ' '; m_InterfaceStr[1] = 2; m_InterfaceStr[2] = 'e'; m_InterfaceStr[3] = 3;//[echoCommand     Packet]  STX 'e' ETX

	Serial_Write(4);
	AutoDetectReadWaiting = true;
	Sleep(400);
	OnCommunication(NULL, NULL);
	if (!AutoDetectReadWaiting) {//if it was successful
		return IDS_OK;
	}
	return -1;
}


afx_msg LRESULT CSmartBoxDaemonDlg::OnCommRead(WPARAM wParam, LPARAM lParam)
{
	return OnCommunication(wParam, lParam);
}


CString str;


int CSmartBoxDaemonDlg::SensorAccess(char* serialnum)
{
	// TODO: Add your implementation code here.
	//serialnum[15] = 0;
	if (!strstr(serialnum, "\02NDT\05")) return -1;

	CString serial;
	int ret = 0;

	ReadWaiting = 1;
	isReceivedRTNNDT = 0;

	// Interrupt!! Sensor Input
	LPWSTR _buf = (LPWSTR)malloc((strlen(serialnum) + 1) * sizeof(*_buf));
	for (int i = 0; i < strlen(serialnum) + 1; i++)_buf[i] = (wchar_t)serialnum[i];
	wchar_t* point;
	if (point = StrChrW(_buf, 0x1f))*point = 0;
	if (!(point = StrChrW(_buf, 0x05)))point = _buf; else point++;
	str.Format(L"Sensor Accessed!! : %s", point);
	AfxMessageBox(str);

	int res = DataReady(serial = point);
	free(_buf);
	if (res == -1)return -1;
	if (strstr((char*)ReadString, "\02RTN\05NDT")) return -1;

	//Make the packet.
	PacketData();


	//Transmit the Sensor data

	sprintf_s((char*)m_InterfaceStr, sizeof(m_InterfaceStr), (const char*)"\02NDT\05%04X%s\03", PacketBuflen, PacketBuf);
	res = ComPort.write(m_InterfaceStr, strlen((char*)m_InterfaceStr));
	
	Sleep(100);

	if (!strstr((char*)ReadString, "\02RTN\05NDT"))
	{
		//ComPort.m_QueueRead.Clear();
		//Sleep(100);
		while (!isReceivedRTNNDT);
		m_SensorAccessed = true;
		//OnCommunication(0, 0);
	}
	if (!ReadWaiting)
	{

		char* point, * seek = (char*)ReadString; int len = ReadStringLength;
		
		point = strstr((char*)seek, "\02RTN\05NDT");


		// ((char*)ReadString, "\2ret\5NDT");
		if (point)
		{
			_buf = (LPWSTR)malloc((strlen(point) + 1) * sizeof(*_buf));
			for (int i = 0; i < strlen(point) + 1; i++)_buf[i] = (wchar_t)point[i];
			str.Format(L"Message : %s", _buf);
			if (StrStrW(_buf, _T("01")))
			{
				 AfxMessageBox(_T("성공적으로 초기화되었습니다."));
				 str = CString(_T("센서 ")) + serial + _T(" : 초기화 성공.");
				 ::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
			}
			else
			{
				AfxMessageBox(_T("초기화 오류."));
				str = CString(_T("센서 ")) + serial + _T(" : 초기화 실패 !!");
				::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
			}

			free(_buf);

			ret = 1;
		}
	}
	else {
		AfxMessageBox(_T("Receiving Timeout."));

		str = CString(_T("센서 ")) + serial + _T(" : 초기화 타임아웃!!(최대 대가 시간 초과)");
		::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&str), NULL);
	}
	;
	if(m_isSensorReplace)Process_Replace();

	return ret;
}


CString CSmartBoxDaemonDlg::HTTPGet(CString URL, CString* RetVal)
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
		CString ret = _T("");
		ret.Format(_T("Error!! Result : %d"), Retval);
		return ret;
	}

	extern CString strCompleteString;
	if (RetVal)*RetVal = strCompleteString;
	return strCompleteString;
}
PacketCoder Packetcoder;
CString CSmartBoxDaemonDlg::Encoding(CString data, CString* ret)
{
	// TODO: Add your implementation code here.
	//CString ss = _T("070805040604050506070802090207010305030702020202100");

	int mty = (int)(rand() * (20 / (double)RAND_MAX));
	if (mty < 20)mty += 100;

	CString rets = Packetcoder.Encode(data,mty);

	if (ret)*ret = rets;
	return rets;
}

CString CSmartBoxDaemonDlg::Base64Encoding(CString data, CString* ret)
{
	CString res = Base64Coder::Encoding(data);
	if (ret)*ret = res;
	return res;
}

int JsonCStringIn(Json::Value& val, CString& str);
int JsonCStringOut(Json::Value& val, CString& str);
int to36Jin_out(char ch)
{
	if ('0' <= ch && ch <= '9')return ch - '0';
	else if ('A' <= ch && ch <= 'Z')return ch - 'A' + 10;
	else if ('a' <= ch && ch <= 'z')return ch - 'a' + 10;
	else ch = 37;
}
CString MultibytecodeDecode(CString in) {

	/*
	int num = 0;
	wchar_t* point = in.GetBuffer();
	while ((point = StrStrW(point, _T("\\u"))))
	{
		num++; point += 2;
	}
	if (num == 0)return in;
	
	LPWSTR buf = (LPWSTR)malloc(num * 2 * sizeof(WCHAR));
	int focus = 0;

	std::stringstream str;
	str << CW2A(_T("{한글Korean}"));

	point = in.GetBuffer();
	while ((point = StrStrW(point, _T("\\u"))))
	{
		//char arr[4];
		//for (int i = 0; i < 4; i++)
		//	arr[i] = point[2 + i];

		//buf[focus++] = to36Jin_out(arr[0]) << 4 | to36Jin_out(arr[1]);
		//buf[focus++] = to36Jin_out(arr[2]) << 4 | to36Jin_out(arr[3]);

		point += 2;
	}
	//CString star;
	//star.Format(_T("%s"), buf);

	delete point;
	delete buf;
	*/

	return in;
}
int CSmartBoxDaemonDlg::GetAPISLR(CString idxClient, CString admin, DataList* pSensorData)
{
	CString Ret;
	if ((Ret = QueryAPI(_T("slr"), idxClient + "^1^" + admin)) == _T("Error"))
		return -1;

	//AfxMessageBox(Ret);
	CString show;

	Json::Value rootarr, root, var;

	JsonCStringIn(rootarr, Ret);

	if (!rootarr[0]["Result"].isNull())
	{
		JsonCStringOut(rootarr[0]["Result"], show);
		AfxMessageBox(_T("Result from Server : ")+show.Mid(1,show.GetLength()-2));
		return -1;
	}

	int len = rootarr.size();
	SensorDataList sdt;

	/*
	if (!pSensorData)pSensorData = &SensorList;
	pSensorData->DeleteContents();
	pSensorData->initContents(len);

	if (SensordatasINITED)
	{
		for (int i = 0; i < m_ListNumSensor; i++)
		{
			delete SensorDatas[i].serialno;
			delete SensorDatas[i].appkey;
			delete SensorDatas[i].StartDate;
			delete SensorDatas[i].WarningDate;
			delete SensorDatas[i].DangerDate;
			delete SensorDatas[i].LastDate;
			delete SensorDatas[i].alertstatus;
			delete SensorDatas[i].savetotmin;
			delete SensorDatas[i].warningmin;
			delete SensorDatas[i].dangermin;
			delete SensorDatas[i].GoodsName;
			delete SensorDatas[i].RACLL;
			delete SensorDatas[i].RACMM;
			delete SensorDatas[i].RACSS;
			delete SensorDatas[i].SendTimeSec;
		}
		free(SensorDatas);
	}
	m_ListNumSensor = len;

	SensorpDataList* psdt;
	SensorDatas = (SensorpDataList*)malloc(len * sizeof(SensorpDataList));
	*/
	SensordatasINITED = 1;
	/*
	{
		"serialno":"sbkkb000000001",
		"appkey":"sbkk00010003",
		"StartDate":"2020-02-19 10:43:38",
		"WarningDate":"2020-02-23 10:43:38",
		"DangerDate":"2020-02-24 10:44:38",
		"LastDate":"2020-02-26 10:43:38",
		"alertstatus":"d",
		"savetotmin":"10080",
		"warningmin":"5760",
		"dangermin":"7201",
		"GoodsName":"골드키위",
		"RACLL":"A",
		"RACMM":"F",
		"RACSS":"1",
		"SendTimeSec":"43200",
	}
	*/

	for (int i = 0; i < len; i++)
	{
		var = rootarr[i];

		/*
		psdt = &(SensorDatas[i]);		

		memset(psdt, 0, sizeof(*psdt));
		psdt->serialno = new CString();
		psdt->appkey = new CString();
		psdt->StartDate = new CString();
		psdt->WarningDate = new CString();
		psdt->DangerDate = new CString();
		psdt->LastDate = new CString();
		psdt->alertstatus = new CString();
		psdt->savetotmin = new CString();
		psdt->warningmin = new CString();
		psdt->dangermin = new CString();
		psdt->GoodsName = new CString();
		psdt->RACLL = new CString();
		psdt->RACMM = new CString();
		psdt->RACSS = new CString();
		psdt->SendTimeSec = new CString();
		*/

		SensorDataList sdt;

		if (SensorList.Lookup(sdt.serialno, sdt))
			SensorList.RemoveKey(sdt.serialno);

		JsonCStringOut(var["serialno"	], sdt.serialno);
		JsonCStringOut(var["appkey"		], sdt.appkey);
		JsonCStringOut(var["StartDate"	], sdt.StartDate);
		JsonCStringOut(var["WarningDate"], sdt.WarningDate);
		JsonCStringOut(var["DangerDate"	], sdt.DangerDate);
		JsonCStringOut(var["LastDate"	], sdt.LastDate);
		JsonCStringOut(var["alertstatus"], sdt.alertstatus);
		JsonCStringOut(var["savetotmin"	], sdt.savetotmin);
		JsonCStringOut(var["warningmin"	], sdt.warningmin);
		JsonCStringOut(var["dangermin"	], sdt.dangermin);
		JsonCStringOut(var["GoodsName"	], sdt.GoodsName);
		JsonCStringOut(var["RACLL"		], sdt.RACLL);
		JsonCStringOut(var["RACMM"		], sdt.RACMM);
		JsonCStringOut(var["RACSS"		], sdt.RACSS);
		JsonCStringOut(var["SendTimeSec"], sdt.SendTimeSec);
		JsonCStringOut(var["Revision"   ], sdt.Revision);
		

		SensorList.SetAt(sdt.serialno,sdt);
		//pSensorData->initData(i,psdt,sizeof*psdt));
	}
	m_ListNumSensor = SensorList.GetCount();
	pDlg0->SensorListChanged();
	pDataDlg->DataList_Update();
 
	//serialno,appkey,StartDate,WarningDate,DangerDate,LastDate
	//alertstatus,savetotmin,warningmin,dangermin,GoodsName ,RACLL,RACMM,RACSS,SendTimeSec


	return 0;
}
int JsonCStringIn(Json::Value& val, CString& str)
{
	std::stringstream Data;

	Json::CharReaderBuilder reader;

	std::string out;

	Data << CW2A(str);
	std::string errs;

	bool ok = Json::parseFromStream(reader, Data, &val, &errs);
	return ok;
}
int JsonCStringOut(Json::Value& val, CString& str)
{
	Json::StreamWriterBuilder writer;
	std::unique_ptr<Json::StreamWriter> write;
	std::stringstream Out;

	writer.validate(&val);
	write = std::unique_ptr<Json::StreamWriter>(writer.newStreamWriter());
	write->write(val, &Out);

	int i = val.isString();

	str = CString(Out.str().c_str());
	
	if (i)
	{
		char* strs = (char*)malloc(str.GetLength() - 1);
		for (int i = 0; i < str.GetLength() - 1; i++)
			strs[i] = str.GetBuffer()[i + 1];
		strs[str.GetLength() - 2] = 0;

		str = MultibytecodeDecode((CString)CA2W(strs));
		free(strs);
	}
	
	//std::string st = val.toStyledString();
	//str = st.c_str();
	//str = val.asCString();

	return 0;
}

CCriticalSection g_criticalExe;
CString ReadExePath() {
	g_criticalExe.Lock();

	static TCHAR pBuf[256] = { 0, };
	memset(pBuf, 0, sizeof(pBuf));
	GetModuleFileName(NULL, pBuf, sizeof(pBuf));//현재 실행 경로를 가려오는 함수
	CString strFilePath;
	strFilePath.Format(_T("%s"), pBuf);
	strFilePath = strFilePath.Left(strFilePath.ReverseFind(_T('\\')));
	g_criticalExe.Unlock();
	return strFilePath;

}
// 시스템 초깃값(Systemconst) 저장 파일(settings.json)에 쓰기
int CSmartBoxDaemonDlg::SystemConst_Store()
{
	
	CString file = ReadExePath() + _T("\\Data.ini");

	CString Autoquery; Autoquery.Format(_T("%d"), Systemconst.d_isAutoQuery);
	WritePrivateProfileString(_T("DEFAULTS"), _T("AutoQuery"), Autoquery, file);
	WritePrivateProfileString(_T("DEFAULTS"), _T("APIURL"), Systemconst.m_ApiURL, file);

	/*
	Json::Value val;
	CString jsondata;

	//jsondata.Format(_T("%d"), Systemconst.d_isAutoQuery);
	//JsonCStringIn(val["AutoQuery"], jsondata);
	val["AutoQuery"] = Systemconst.d_isAutoQuery;

	//JsonCStringIn(val["APIURL"], Systemconst.m_ApiURL);
	val["APIURL"] = (char*)CT2CA(Systemconst.m_ApiURL);

	JsonCStringOut(val, jsondata);

	int index = 0;
	while ((index = jsondata.Find(_T('\n'), index)) != -1)jsondata.Delete(index++, 1);
	index = 0;
	while ((index = jsondata.Find(_T('\t'), index)) != -1)jsondata.Delete(index++, 1);

	CString path = _T("settings.json");

	CFileStatus fstat;
	if (CFile::GetStatus(path,fstat))
		CFile::Remove(path);
	CFile f;
	if (f.Open(path, CFile::modeCreate))f.Close();

	if (!f.Open(path,CFile::modeWrite))
	{ 
		return 0;
	}

	f.Write(jsondata.GetBuffer(), jsondata.GetLength() * 2);

	f.Close();
	*/

	return 1;
}

// 시스템 초깃값(Systemconst) 저장 파일(settings.json) 읽고 적용하기
int CSmartBoxDaemonDlg::SystemConst_Read()
{
	/*
	CString logfile = _T("settings.json");
	CFile f;
	CString ss;
	Json::Value val; //= _T("");
	//if (!SystemConst_Open(&val))
	if(!f.Open(logfile, CFile::modeRead))
	{
		//AfxMessageBox(_T("시스템 초깃값 불러오기 실패."));
		SystemConst_Reset();
		return 0;
	}
	int len = f.GetLength();
	WCHAR* buf = (WCHAR*)malloc((len + 1)*sizeof(WCHAR)); buf[len] = 0;
	int sread = f.Read(buf, len*2);
	ss = buf;
	free(buf);
	f.Close();
	int focus = 0;
	while ((focus = ss.Find(_T('\\'), focus)) != -1)ss.Delete(focus++, 1);

	
	JsonCStringIn(val, ss);

	// JsonCStringOut(val, ss);
	// AfxMessageBox(ss);

	JsonCStringOut(val["AutoQuery"], ss);
	//ss = val["AutoQuery"].toStyledString().c_str();//ss.Mid(1,ss.GetLength()-2);
	Systemconst.d_isAutoQuery = _wtoi(ss);
	
	ss.Format(_T("%d"), Systemconst.d_isAutoQuery);
	//AfxMessageBox(ss);

	JsonCStringOut(val["APIURL"], ss);
	//Systemconst.m_ApiURL = val["APIURL"].toStyledString().c_str();//ss.Mid(1,ss.GetLength()-2);
	Systemconst.m_ApiURL = ss;
	//AfxMessageBox(Systemconst.m_ApiURL);
	*/
	CString file = ReadExePath() + _T("\\Data.ini");

	LPWSTR res = (LPWSTR)malloc(1024*sizeof(WCHAR));
	GetPrivateProfileString(_T("DEFAULTS"), _T("AutoQuery"), _T(""), res, 1024, file);
	Systemconst.d_isAutoQuery = _wtoi(res);

	GetPrivateProfileString(_T("DEFAULTS"), _T("APIURL"), _T(""), res, 1024, file);
	Systemconst.m_ApiURL = res;

	free(res);


	return 1;
}

// 시스템 초깃값(Systemconst) 저장 파일(settings.json) 열기
//int CSmartBoxDaemonDlg::SystemConst_Open(Json::Value* out)
//{
//
//	CString logfile = _T("settings.json");
//	CFile f;
//
//	if (f.Open(logfile, CFile::modeRead))
//	{
//		int len = f.GetLength();
//		char* buf = (char*)malloc(len + 1); buf[len] = 0;
//		f.Read(buf, len);
//		f.Close();
//
//
//		Json::Value val;
//		std::stringstream Data;
//		Json::CharReaderBuilder reader;
//		Data << buf;
//		free(buf);
//		std::string errs;
//
//		Json::parseFromStream(reader, Data, &val, &errs);
//		if (out)*out = val;
//		return 1;
//	}
//	else return 0;
//}


// 시스템 초깃값(Systemconst) 초기화
void CSmartBoxDaemonDlg::SystemConst_Reset()
{
	Systemconst.d_isAutoQuery = 0;
	Systemconst.m_ApiURL = _T("http://server.ip:0/api/");
}

int CSmartBoxDaemonDlg::DataReady(CString str)
{
	// 1. 탭 1로 커서 이동
	m_Tab.SetCurSel(1);
	int i = 0;
	OnTcnSelchangeTab1(NULL,(LRESULT*)&i);
	CString Msgstr = CString(_T("센서 ")) + str + _T(" 접근 : 셋팅 후 '전송 버튼' 클릭하세요.");
	::PostMessage(hCommWnd, WM_STATBARTXT, (WPARAM)(&Msgstr), NULL);
	//SetStatusBarText(CString(_T("센서 ")) + str + _T(" : 전송 버튼 클릭하세요."));

	// 2. List에 센서에 해당하는 정보 저장&출력

	SensorDataList sdt;
	int len = 0;
	if (SensorList.Lookup(str, sdt))
	{
		SensorDataList2SensorData(sdt,MannualData);
	}
	else;
	
	/*
	SensorpDataList *psdt;
	int len = 0;
	for (i = 0; i < SensorList.getendID(); i++)
	{
		psdt = (SensorpDataList*)SensorList.WhereisData(i, len);

		if (*psdt->serialno == str)
		{
			SensorpDataList2SensorData(*psdt, MannualData);
			break;
		}
	}
	*/

	// 3. ICon 변경(인터럽트, 센서 접근)
	m_COMConnected = 3;
	SetCOMICON();
	pDlg1->m_SendButton.EnableWindow(TRUE);
	::PostMessage(hCommWnd, WM_TAB1_UPDATE_VAL, NULL, NULL);
	//pDlg1->UpdateVals();

	// 잔송 버튼이 눌리면
	isNDT00 = 0;
	isFinishedValue = 0;
	while (!isFinishedValue);

	if (isNDT00)
	{ 
		AfxMessageBox(_T("마스터키 입력 Timeout!!(30초)"));
		pDlg1->SendVals();
		pDlg1->m_SendButton.EnableWindow(FALSE);
		m_COMConnected = 1;
		SetCOMICON();
		return -1;
	}
	// 4. List데이터 가져오기

	pDlg1->SendVals();
	pDlg1->m_SendButton.EnableWindow(FALSE);
	// 5. return.(전송)
	m_COMConnected = 1;
	SetCOMICON();

	TransmitData = MannualData;
	return 0;
}

int CSmartBoxDaemonDlg::SETICON(HICON hIcon)
{
	m_statusBar.GetStatusBarCtrl().SetIcon(1, hIcon);
	pDlg0->m_COMStateIcon.SetIcon(hIcon);
	pDlg1->m_Tab1ICON.SetIcon(hIcon);
	return 0;
}

void CSmartBoxDaemonDlg::SensorDataList2SensorData(SensorDataList& in, SensorData& out)
{
	out.m_StrAPPKEY = in.appkey;  // APPKEY
	out.m_StrStartDate = in.StartDate;  // 시작시간
	out.m_StrWarningmin = in.warningmin;  // 경고기간
	out.m_StrDangermin = in.dangermin;  // 위험기간
	out.m_StrSavetotmin = in.savetotmin;  // 보관기간
	out.m_StrProduct = in.GoodsName;  // 상품명
	out.m_StrRACLL = in.RACLL;  // RAC정보(LL)
	out.m_StrRACMM = in.RACMM;  // RAC정보(MM)
	out.m_StrRACSS = in.RACSS;  // RAC정보(SS)
	out.m_StrSendTime = in.SendTimeSec;  // 전송주기
	out.m_StrRevision = in.Revision;  // 보정값(TP^-5,HU^10)
	out.m_isReplace.Format(_T("%d"), MainDlg->m_isSensorReplace);// 교체 플래그
}

BOOL CSmartBoxDaemonDlg::DestroyWindow()
{
	/*
	if (SensordatasINITED)
	{
		for (int i = 0; i < m_ListNumSensor; i++)
		{
			delete SensorDatas[i].serialno;
			delete SensorDatas[i].appkey;
			delete SensorDatas[i].StartDate;
			delete SensorDatas[i].WarningDate;
			delete SensorDatas[i].DangerDate;
			delete SensorDatas[i].LastDate;
			delete SensorDatas[i].alertstatus;
			delete SensorDatas[i].savetotmin;
			delete SensorDatas[i].warningmin;
			delete SensorDatas[i].dangermin;
			delete SensorDatas[i].GoodsName;
			delete SensorDatas[i].RACLL;
			delete SensorDatas[i].RACMM;
			delete SensorDatas[i].RACSS;
			delete SensorDatas[i].SendTimeSec;
		}
		free(SensorDatas);
	}
	*/
	return CDialogEx::DestroyWindow();
}

afx_msg LRESULT CSmartBoxDaemonDlg::OnTab1UpdateVal(WPARAM wParam, LPARAM lParam)
{
	pDlg1->UpdateVals();
	return 0;
}

LPCTSTR format_What[13] = { _T("bt"),_T("tp"),_T("ox"),_T("du"),_T("co"),_T("hu"),_T("ni"),_T("pp"),_T("hd"),_T("uv"),_T("ps"),_T("cm"),_T("mt") };
int isEqualText(CString a, CString b)
{
	if (a == b)return 1;
	else if (a.MakeUpper() == b.MakeUpper())return 1;
	else
		return 0;
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
	memcpy(buf,dat, len);
	return nowfocus += len;
}
int SendPacketinCString(CString str, uint8_t* buf, int& nowfocus)
{
	int len = str.GetLength();

	buf += nowfocus;
	buf[0] = len;

	for (int i = 0; i < len; i++)
		buf[1 + i] = str.GetBuffer()[i];

	return nowfocus += len + 1;
}

char* Projects[1] = { "sb" };
CString countries[1] = { _T("kk") };
char sensorTypes[3] = { 'b','h','o' };
void Apn2EUI(char* APnum, uint8_t * APPKEY)
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
int CSmartBoxDaemonDlg::PacketData()
{
	TxPacketLen = 0;

	tm times; time_t now;
	sscanf_s(CW2A(TransmitData.m_StrStartDate), "%d-%d-%d %d:%d:%d",
		&times.tm_year, &times.tm_mon, &times.tm_mday,
		&times.tm_hour, &times.tm_min, &times.tm_sec);
	times.tm_isdst = 0;
	times.tm_year -= 1900;
	times.tm_mon -= 1;
	
	TxPacket = ReadString;
	memset(TxPacket, 0, 256);
	//패킷 완성!!
	uint8_t APPKey[16] = { 0, };
	Apn2EUI(CW2A(TransmitData.m_StrAPPKEY), (uint8_t*)APPKey);

	SendPacketinData(APPKey, 16, TxPacket, TxPacketLen);

	long long stime = _mktime64(&times);

	SendPacketinData(&stime, sizeof(stime), TxPacket, TxPacketLen);

	int warningmin, dangermin, savetotmin;
	warningmin = _wtoi(TransmitData.m_StrWarningmin);
	dangermin  = _wtoi(TransmitData.m_StrDangermin);
	savetotmin = _wtoi(TransmitData.m_StrSavetotmin);

	SendPacketinData(&warningmin, sizeof(int), TxPacket, TxPacketLen);
	SendPacketinData(&dangermin , sizeof(int), TxPacket, TxPacketLen);
	SendPacketinData(&savetotmin, sizeof(int), TxPacket, TxPacketLen);

	SendPacketinCString(TransmitData.m_StrProduct, TxPacket, TxPacketLen);
	SendPacketinCString(TransmitData.m_StrRACLL, TxPacket, TxPacketLen);
	SendPacketinCString(TransmitData.m_StrRACMM, TxPacket, TxPacketLen);
	SendPacketinCString(TransmitData.m_StrRACSS, TxPacket, TxPacketLen);

	int SendTime;
	SendTime = _wtoi(TransmitData.m_StrSendTime);

	SendPacketinData(&SendTime, sizeof(int), TxPacket, TxPacketLen);
	uint32_t isreplace = _wtoi(TransmitData.m_isReplace);
	SendPacketinData(&isreplace, sizeof(int), TxPacket, TxPacketLen);


	/*
	CString m_StrAPPKEY;     // APPKEY
	CString m_StrStartDate;  // 시작일자
	CString m_StrWarningmin; // 경고기간
	CString m_StrDangermin;  // 위험기간
	CString m_StrSavetotmin; // 보관기간
	CString m_StrProduct;    // 상품명
	CString m_StrRACLL;      // RAC정보(LL)
	CString m_StrRACMM;      // RAC정보(MM)
	CString m_StrRACSS;      // RAC정보(SS)
	CString m_StrSendTime;   // 전송주기
	CString m_StrRevision;		 // 보정값
	CString m_isReplace;	 // 교체 플래그
	*/

	if (TransmitData.m_StrRevision == "")
	{
		for (int i = 0; i < 5; i++)
			TxPacket[TxPacketLen + i] = 0xFF;
	}
	else {//if there's any Revision Value
		WCHAR* point = TransmitData.m_StrRevision.GetBuffer();
		int focus = 0, count = 0;
		while ((focus = TransmitData.m_StrRevision.Find(_T('^'), focus)) != -1) { count++; focus++; }

		focus = 0;
		for (int i=0;i<5;i++)
		{
			if (focus!=-1 && (focus = TransmitData.m_StrRevision.Find(_T('^'), focus)) != -1)
			{
				CString field = TransmitData.m_StrRevision.Mid(focus - 2, TransmitData.m_StrRevision.GetLength()-(focus-2));

				int type = format_a(field.Mid(0, 2));
				float val = _wtof(field.Mid(3));
				TxPacket[TxPacketLen + i] = type;
				memcpy(TxPacket + (TxPacketLen + 5 + i * 4), &val, sizeof(float));

				focus++;
			}
			else
			{
				TxPacket[TxPacketLen + i] = 0xFF;
			}

		}
	}
	TxPacketLen += 25;

	//TxPacketLen = 123;
	TxPacket;

	for (int i = 0; i < TxPacketLen; i++)
	{
		sprintf_s(PacketBuf+i*2,3, "%02X", TxPacket[i]);
		PacketBuf[i*2+3] = 0;
	}
	PacketBuflen = TxPacketLen;
	return 0;
}

afx_msg LRESULT CSmartBoxDaemonDlg::OnStatbartxt(WPARAM wParam, LPARAM lParam)
{
	LPWSTR str = *((LPWSTR*)wParam);
	SetStatusBarText(str);
	return 0;
}

int CSmartBoxDaemonDlg::Process_Replace()
{
	CString Ret;
	if ((Ret = QueryAPI(_T("CHG"), m_frSerialNo + "^" + m_toSerialNo)) == _T("Error"))
		return -1;
	Json::Value root;
	JsonCStringIn(root,Ret);

	CString res;

	/*
	if (!root[0]["Result"].isNull())
	{
		JsonCStringOut(root[0]["Result"], res);
		res = res.Mid(1, res.GetLength() - 2);
		AfxMessageBox(_T("Result from Server : ") + res);
		return _wtoi(res) == 1;
	}
	*/
	if (!root[0]["v_RESULT"].isNull())
	{
		int a = root[0]["v_RESULT"].asInt();
		//JsonCStringOut(root[0]["v_RESULT"], res);
		//res = res.Mid(1, res.GetLength() - 2);
		CString ss; ss.Format(_T("%d"), a);
		AfxMessageBox(_T("Result from Server : ") + ss);
		if(a==1)GetAPISLR(_T("1"));
		return a == 1;
	}
	return -1;
	
}


//~~~/api/"apin"/Encoding[Base64["Msg"]]  -> return.
CString CSmartBoxDaemonDlg::QueryAPI(CString apin, CString Msg)
{
	CString Senddata = Encoding(Base64Encoding(Msg, NULL), NULL);
	CString URL = m_ApiURL + apin + "/" + Senddata;

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
