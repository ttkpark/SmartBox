
// SmartBoxLoginDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SmartBoxWinMonitoringLogin.h"
#include "SmartBoxLoginDlg.h"
#include "afxdialogex.h"
#include "Encrypt.h"
#include "json.h"
#include "HttpCall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartBoxLoginDlg dialog



CSmartBoxLoginDlg::CSmartBoxLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMARTBOXWINMONITORINGLOGIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartBoxLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BITMAP, Bitmap);
}

BEGIN_MESSAGE_MAP(CSmartBoxLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSmartBoxLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSmartBoxLoginDlg message handlers

CCriticalSection g_criticalExe;
CString ReadExePath() {
	g_criticalExe.Lock();

	static TCHAR pBuf[256] = { 0, };
	memset(pBuf, 0, sizeof(pBuf));
	GetModuleFileName(NULL, pBuf, sizeof(pBuf));//현재 실행 경로를 가져오는 함수
	CString strFilePath;
	strFilePath.Format(_T("%s"), pBuf);
	strFilePath = strFilePath.Left(strFilePath.ReverseFind(_T('\\')));
	g_criticalExe.Unlock();

	return strFilePath;

}

BOOL CSmartBoxLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetIniIinfo();
	SetBitmap();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmartBoxLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartBoxLoginDlg::OnPaint()
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
HCURSOR CSmartBoxLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
Encrypt encrypt;

int CSmartBoxLoginDlg::GETAPILGN(CString idxClient,CString ID,CString PW)
{
	CString Ret;
	Ret = idxClient + _T("^") + ID + _T("^") + PW;

	if ((Ret = QueryAPI(_T("lgn"), encrypt.Encoding100(encrypt.Base64Encoding(Ret, NULL), NULL))) == _T("Error"))
		return -1;

	CString RangeNumber;

	Json::Value rootArray, root, var;

	JsonfromCString(rootArray, Ret);

	if (!rootArray[0]["Result"].isNull() || rootArray[0]["token_key"].isNull())
	{
		CString s_result;
		JsonDatatoString(rootArray[0]["Result"], s_result);
		int res = _wtol(s_result);
		if (res == 2 || res == 5) {
			AfxMessageBox(_T("Wrong ID or Password!!"));
			return -2;
		}

		return -1;
	}


	JsonDatatoString(rootArray[0]["UserStatus"], Token);
	if (Token != "1") // Client is absent (UserStatus!=1)
	{
		AfxMessageBox(_T("this Client is absent!!"));
		return -2;
	}

	JsonDatatoString(rootArray[0]["token_key"], Token);
	JsonDatatoString(rootArray[0]["idxUser"], idxUser);
	if (idxUser == _T(""))idxUser = _T("0");

	return 0;
}

CString CSmartBoxLoginDlg::HTTPGet(CString URL, CString* RetVal)
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
//~~~/api/"apin"/"Msg"  -> return.
CString CSmartBoxLoginDlg::QueryAPI(CString apin, CString Msg)
{
	//CString Senddata = Encoding100(Base64Encoding(Msg, NULL), NULL);
	CString URL = m_ApiURL + apin + "/" + Msg;

	CString Ret = HTTPGet(URL, NULL);

	if (Ret.Find(_T("Error!! Result : ")) != -1)
	{
		//AfxMessageBox(Ret);
		return _T("Error");
	}

	Ret = Ret.Mid(1, Ret.GetLength() - 2);

	Ret.Replace(_T("\\\""), _T("\""));

	return Ret;
}

int CSmartBoxLoginDlg::APISendEncoding100RetJson(CString APIname, CString DatatoSend, Json::Value& jsonRet)
{
	CString Ret, Send;

	Send = encrypt.Encoding100(encrypt.Base64Encoding(DatatoSend, NULL), NULL);// 전송 문자열 준비(Data:텍스트,Send:암호화된 데이터)

	if ((Ret = QueryAPI(APIname, Send)) == _T("Error"))//에러처리
	{
		return RES_ERR;//Connection Error
	}
	JsonfromCString(jsonRet, Ret);

	return RES_OK;
}

void CSmartBoxLoginDlg::APIShowResultError(CString ErrCode, Json::Value& jsonIn)
{
	if (!jsonIn[0]["Result"].isNull())
	{
		CString StrResult;
		JsonDatatoString(jsonIn[0]["Result"], StrResult);
		ShowAPIError(ErrCode, _T(": ") + StrResult);
	}
	else ShowAPIError(ErrCode + ConnErrorPrefix, NULLSTRING);

}

void CSmartBoxLoginDlg::ShowAPIError(CString ErrorCode, CString Error)
{
	Sleep(10);
}

int CSmartBoxLoginDlg::JsonfromCString(Json::Value& val, CString& str)
{
	std::stringstream Data;

	Json::CharReaderBuilder reader;

	std::string out;

	CW2A p(str);
	out = p;
	Data << out;
	std::string errs;

	bool isok = Json::parseFromStream(reader, Data, &val, &errs) == true;
	return isok ? RES_OK : RES_ERR;
}

int CSmartBoxLoginDlg::JsonDatatoString(Json::Value& val, CString& str)
{
	Json::StreamWriterBuilder writer;
	std::unique_ptr<Json::StreamWriter> write;
	std::stringstream Out;

	writer.validate(&val);
	write = std::unique_ptr<Json::StreamWriter>(writer.newStreamWriter());
	write->write(val, &Out);

	bool isString = val.isString();
	bool isNull = val.isNull();

	CA2W p(Out.str().c_str());

	if (isNull)
	{
		str = _T("");
		return RES_OK;
	}

	str = p;

	if (isString)
	{
		str = str.Mid(1, str.GetLength() - 2);
	}

	return RES_OK;
}

void CSmartBoxLoginDlg::OnBnClickedOk()
{
	if (GetFocus() == GetDlgItem(IDOK));
	{
		GetDlgItem(IDC_ID)->GetWindowText(ID);
		GetDlgItem(IDC_PW)->GetWindowText(PW);

		int res = GETAPILGN(idxClient, ID, PW);
		if (res != 0)
		{
			//if (res != -2)AfxMessageBox(_T("Error"));
			return;
		}
		RunMonitoringProgram();
	}
}

BOOL CSmartBoxLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return true;
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedOk();
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

int CSmartBoxLoginDlg::RunMonitoringProgram()
{
	CString ExeName = _T("SmartBoxWinMonitoring.exe");
	CString ExeParam;
	CString ExetoRun = herePath + "\\" + ExeName;

	ExeParam += _T(" ") + encrypt.Encoding100(encrypt.Encoding(idxClient), NULL);
	ExeParam += _T(" ") + encrypt.Encoding100(encrypt.Encoding(work_zone_num), NULL);
	ExeParam += _T(" ") + encrypt.Encoding100(encrypt.Encoding(idxUser), NULL);
	ExeParam += _T(" ") + encrypt.Encoding100(encrypt.Encoding(ID), NULL);
	ExeParam += _T(" ") + encrypt.Encoding800(encrypt.Encoding(Token), NULL);
	ExeParam += _T(" ") + encrypt.Encoding800(encrypt.Encoding(m_ApiURL), NULL);

	// 파일 실행시킬 부분
	SHELLEXECUTEINFO ExeInfo;
	ZeroMemory(&ExeInfo, sizeof(ExeInfo));
	ExeInfo.cbSize = sizeof(ExeInfo);
	ExeInfo.lpVerb = _T("open");
	ExeInfo.lpFile = ExetoRun;                    // 실행할 파일명
	ExeInfo.lpParameters = ExeParam;              // 넘길 인자값
	ExeInfo.fMask = SEE_MASK_FLAG_NO_UI;
	ExeInfo.nShow = SW_SHOW;

	int ret = ::ShellExecuteEx(&ExeInfo);
	

	if(!ret){
		AfxMessageBox(_T("프로그램 실행 오류!"));
	}
	
	CDialogEx::OnOK();
	return 0;
}


int CSmartBoxLoginDlg::GetIniIinfo()
{
	herePath = ReadExePath();
	inifile = herePath + _T("\\Login.ini");

	LPWSTR res = (LPWSTR)malloc(1024 * sizeof(WCHAR));

	GetPrivateProfileString(_T("DEFAULTS"), _T("idxClient"), _T(""), res, 1024, inifile);
	idxClient = res;

	GetPrivateProfileString(_T("DEFAULTS"), _T("work_zone_num"), _T(""), res, 1024, inifile);
	work_zone_num = res;

	GetPrivateProfileString(_T("DEFAULTS"), _T("APIURL"), _T(""), res, 1024, inifile);
	m_ApiURL = res;

	free(res);

	return 0;
}


int CSmartBoxLoginDlg::SetBitmap()
{
	HBITMAP bmp = NULL;
	CString BitmapPath = herePath + _T("\\Login.bmp");

	bmp = (HBITMAP)LoadImage(0, BitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	Bitmap.SetBitmap(bmp);
	if (!bmp)::CloseHandle(bmp);

	return 0;
}
