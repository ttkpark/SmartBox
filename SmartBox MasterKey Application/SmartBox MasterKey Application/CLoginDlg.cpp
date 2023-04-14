// CLoginDlg.cpp : implementation file
//

#include "pch.h"
#include "SmartBox MasterKey Application.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "json.h"
#include "SmartBoxMasterKeyAppDlg.h"

////https://m.blog.naver.com/websearch/70045995543
//#include <winsock.h>
//#include <Ipexport.h>
//#include <Icmpapi.h>
//
//#pragma comment(lib, "Iphlpapi")
//#pragma comment(lib, "wsock32")


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITID, m_EditID);
	DDX_Control(pDX, IDC_EDITPW, m_EditPW);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginDlg message handlers


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return true;
		if (pMsg->wParam == VK_RETURN)
		{
			OnOK();
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// CSmartBoxMasterKeyAppDlg message handlers
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
BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	p_parent->LangTable.InitHashTable(301);

	//ini file의 경로 설정
	CString inifile;
	p_parent->herePath = ReadExePath();
	p_parent->inifile = p_parent->herePath + _T("\\MasterKey.ini");
	inifile = p_parent->inifile;

	p_parent->SystemConst_Read();
	p_parent->SetLang(p_parent->nowLang);

	p_parent->My_Log(_T("Dialog Login Started!!"),0/*LOG*/);

	LPWSTR res = (LPWSTR)malloc(1024 * sizeof(WCHAR));
	
	GetPrivateProfileString(_T("DEFAULTS"), _T("idxClient"), _T(""), res, 1024, inifile);
	idxClient = res;
	
	GetPrivateProfileString(_T("DEFAULTS"), _T("work_zone_num"), _T(""), res, 1024, inifile);
	work_zone_num = res;

	free(res);

	p_parent->idxClient = idxClient;
	p_parent->work_zone_num = work_zone_num;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


int CLoginDlg::GETAPILGN()
{
	CString Ret;
	Ret = idxClient + _T("^") + ID + _T("^") + PW;

	if ((Ret = p_parent->QueryAPI(_T("lgn"), p_parent->Encoding100(p_parent->Base64Encoding(Ret, NULL), NULL))) == _T("Error"))
		return -1;

	//AfxMessageBox(Ret);
	CString RangeNumber;

	Json::Value rootArray, root, var;

	JsonfromCString(rootArray, Ret);

	if (!rootArray[0]["Result"].isNull() || rootArray[0]["token_key"].isNull())
	{
		CString s_result;
		JsonDatatoString(rootArray[0]["Result"], s_result);
		int res = _wtol(s_result);
		if (res == 2 || res == 5){	
			//CONSTVALUE
			AfxMessageBox(_T("Wrong ID or Password!!"));
			return -2;
		}

		return -1;
	}

	
	JsonDatatoString(rootArray[0]["UserStatus"], Token);
	if (Token != "1") // Client is absent (UserStatus!=1)
	{
		// CONSTVALUE
		AfxMessageBox(_T("this Client is Not Activiated!!"));
		return -2;
	}

	JsonDatatoString(rootArray[0]["token_key"], Token);
	p_parent->Token = Token;

	return 0;
}


void CLoginDlg::OnOK()
{
	if (GetFocus() == GetDlgItem(IDOK));
	{
		m_EditID.GetWindowText(ID);
		m_EditPW.GetWindowText(PW);

		if (Pingtoserver())return;

		int res = GETAPILGN();
		if (res != 0)
		{
			//if (res != -2)AfxMessageBox(_T("Error"));
			return;
		}
		p_parent->GetAPISYS();
		CDialogEx::OnOK();//go to the Main Dialog!!
	}
}


int CLoginDlg::Pingtoserver()
{


	return 0;
}


CString CLoginDlg::GetCaption(int id)
{
	return p_parent->GetCaption(id);
}
