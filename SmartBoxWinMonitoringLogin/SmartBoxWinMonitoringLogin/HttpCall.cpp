#include "pch.h"
#include "HttpCall.h"
#include "pch.h"
#include "HttpCall.h"

CString HTTPEncodeStr(CString ToCode)
{
	CString RetStr, AddStr;
	int i, max;
	unsigned short asc;
	unsigned char c;
	max = (unsigned int)ToCode.GetLength();
	for (i = 0; i < max; i++)
	{
		c = ToCode[i];
		asc = c;//(unsigned int)c;
		if (asc > 47 && asc < 58)
		{
			RetStr += c;//Interim[(int)i];
		}
		else if (asc > 64 && asc < 91)
		{
			RetStr += c;//Interim[(int)i];
		}
		else if (asc > 96 && asc < 123)
		{
			RetStr += c;//Interim[(int)i];
		}
		else if (asc == 32)
		{
			RetStr += "+";
		}
		else
		{
			AddStr.Format(L"%%%2x", asc);
			int iv = (int)AddStr.GetAt(1);
			if ((int)AddStr.GetAt(1) == 32)
			{
				AddStr.SetAt(1, '0');
			}
			RetStr += AddStr;
		}
	}
	return RetStr;
}

CString HTTPDeCodeStr(CString ToCode)
{
	CString RetStr, AddStr;
	int i, max;
	unsigned short asc;
	unsigned char c;
	max = (unsigned int)ToCode.GetLength();
	for (i = 0; i < max;)
	{
		c = ToCode[i];
		asc = c;//(unsigned int)c;
		if (asc == 37)
		{
			AddStr = ToCode.Mid(i + 1, 2);
			i += 3;
			wscanf_s((LPCTSTR)AddStr, "%2x", &asc);
			RetStr += (char)asc;
		}
		else if (asc == 43)
		{
			RetStr += ' ';
			i++;
		}
		else
		{
			RetStr += c;
			i++;
		}
	}
	return RetStr;
}

void DeCodeFile(CFile* DataFile, CHttpFile* HttpFile)
{
	CString AddStr;
	int i, max;
	unsigned short asc;
	unsigned char c;
	max = (unsigned int)HttpFile->GetLength();
	//	HttpFile->SeekToBegin();
	for (i = 0; i < max; i++)
	{
		HttpFile->Read(&c, 1);
		asc = c;
		if (asc == 37)
		{
			HttpFile->Read(AddStr.GetBuffer(2), 2);
			AddStr.ReleaseBuffer();
			wscanf_s((LPCTSTR)AddStr, "%2x", &asc);
			DataFile->Write(&asc, 1);
		}
		else if (asc == 43)
		{
			c = (char)32;
			DataFile->Write(&c, 1);
		}
		else
		{
			DataFile->Write(&c, 1);
		}
	}
	return;
}

CString GetTheUserName()
{
	int status;
	WCHAR lpName[255];
	unsigned long lpnLength = 255;
	// Get the log-on name of the person using product
	status = GetUserName(lpName, &lpnLength);
	return(lpName);
}

CString strCompleteString;
int HttpCall(CString ServerAddress,
	int ServerPort,
	CString Process,
	CString Query,
	CString DataBlock)
{
	CString strUser;
	strUser = GetTheUserName();
	long lread, max;
	// No Need to parse URL we create entities themselves.
	DWORD ServiceType = INTERNET_SERVICE_HTTP;
	CString UserName = _T("anonymous");
	CString UserPassWord = _T("");

	CString lf;
	lf.Format(_T("%c%c"), char(13), char(10)); // need unix line feed

	CString EnCodeData = HTTPEncodeStr(DataBlock);

	//
	// Setup Server
	DWORD AccessType = PRE_CONFIG_INTERNET_ACCESS;
	CString UserDef = _T("HttpCall");	// User Application

	CInternetSession* pHttpSession = new CInternetSession(UserDef, 1, AccessType);
	//pHttpSession->SetStatusWnd(pStatusWnd);

	CHttpConnection* pHttpConnection = NULL;
	pHttpConnection = pHttpSession->GetHttpConnection(ServerAddress, ServerPort, UserName, UserPassWord);
	CHttpFile* pHttpFile = NULL;
	DWORD HttpRequestFlags;
	//
	if (Query.GetLength() > 0) Process += '?' + HTTPEncodeStr(Query);


	// Open the request and send it;
	TRY	// if the request has an invalid port it fails. Need to look for option for error test
	{
		pHttpSession->SetOption(INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT, 5000);
		pHttpSession->SetOption(INTERNET_OPTION_CONTROL_SEND_TIMEOUT, 5000);
		pHttpSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 5000);
		pHttpSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 5000);

		HttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE;


		DWORD TotalLen;
		TotalLen = EnCodeData.GetLength();

		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			Process, NULL, 1, NULL, _T("1.0"), HttpRequestFlags);

		pHttpFile->SendRequest();

		max = pHttpFile->GetLength();
	}
		CATCH_ALL(e)
	{
		CString strError;
		e->GetErrorMessage(strError.GetBuffer(1024), 1024);
		e->Delete();
		AfxMessageBox(_T("서버 연결에 실패했습니다."));
		//AfxMessageBox(strError);
		strError.ReleaseBuffer();
	}
	END_CATCH_ALL
		DWORD dwRet;
	pHttpFile->QueryInfoStatusCode(dwRet);
	//
	// Read Data
	CString strRetBufLen;
	pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strRetBufLen);
	max = _wtol(strRetBufLen);

	if (max <= 0)
	{
		max = pHttpFile->GetLength();
	}

	CString szHeaders;
	pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, szHeaders);

	BOOL bUTF8mode = szHeaders.Find(_T("charset=UTF-8")) >= 0 ||
		szHeaders.Find(_T("charset=utf-8")) >= 0 ||
		szHeaders.Find(_T("CHARSET=UTF-8")) >= 0 ||
		szHeaders.Find(_T("CHARSET=utf-8")) >= 0;

	strCompleteString.Empty();

	if (dwRet == 200)
	{
		CString strResult;
		while (pHttpFile->ReadString(strResult))
		{
			if (bUTF8mode)
				strCompleteString += CW2T(CA2W((char*)strResult.GetBuffer(), CP_UTF8));
			else
				strCompleteString += CW2T(CA2W((char*)strResult.GetBuffer()));

			strResult.ReleaseBuffer();
		}
	}
	//[출처] MFC HTTP PROTOCOL POST / GET 전송 | 작성자 인로즈


	pHttpFile->Close();
	pHttpConnection->Close();
	pHttpSession->Close();
	delete pHttpFile;
	delete pHttpConnection;
	delete pHttpSession;
	return dwRet;
}

#define BUFFER_SIZE 4095

/////////////////////////////////////////////////////////////////////////////
// CHttpSession

CHttpSession::CHttpSession(LPCTSTR szAgentName = NULL,
	DWORD dwContext = 1,
	DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG)//,
	//LPCTSTR pstrProxyName = NULL, 
	//LPCTSTR pstrProxyBypass = NULL, 
	//DWORD dwFlags = 0 )
	:CInternetSession(szAgentName,
		dwContext,
		dwAccessType,
		NULL,//pstrProxyName, 
		NULL,//pstrProxyBypass, 
		0)//dwFlags) 
{
	CommonConstruct();
}

CHttpSession::~CHttpSession()
{
}

void CHttpSession::CommonConstruct()
{
	m_pStatusWnd = NULL;
	try {
		EnableStatusCallback(TRUE);
	}
	catch (...)
	{
	}
}
// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHttpSession, CInternetSession)
	//{{AFX_MSG_MAP(CHttpSession)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CHttpSession member functions

void CHttpSession::OnStatusCallback(DWORD dwContext,
	DWORD dwInternetStatus,
	LPVOID lpvStatusInformation,
	DWORD dwStatusInformationLength)
{
	// Status callbacks need thread-state protection. 
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CString str;

	switch (dwInternetStatus)
	{
	case INTERNET_STATUS_RESOLVING_NAME:
		str.Format(L"Resolving name for %s", lpvStatusInformation);
		break;

	case INTERNET_STATUS_NAME_RESOLVED:
		str.Format(L"Resolved name for %s", lpvStatusInformation);
		break;

	case INTERNET_STATUS_HANDLE_CREATED:
		//str.Format(L"Handle %8.8X created", hInternet);
		break;

	case INTERNET_STATUS_CONNECTING_TO_SERVER:
	{
		str.Format(L"Connecting to socket address ");
	}
	break;

	case INTERNET_STATUS_REQUEST_SENT:
		str.Format(L"Request sent");
		break;

	case INTERNET_STATUS_SENDING_REQUEST:
		str.Format(L"Sending request...");
		break;

	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		str.Format(L"Connected to socket address");
		break;

	case INTERNET_STATUS_RECEIVING_RESPONSE:
		return;
		str.Format(L"Receiving response...");
		break;

	case INTERNET_STATUS_RESPONSE_RECEIVED:
		str.Format(L"Response received");
		break;

	case INTERNET_STATUS_CLOSING_CONNECTION:
		str.Format(L"Closing the connection to the server");
		break;

	case INTERNET_STATUS_CONNECTION_CLOSED:
		str.Format(L"Connection to the server closed");
		break;

	case INTERNET_STATUS_HANDLE_CLOSING:
		return;
		str.Format(L"Handle closed");
		break;

	case INTERNET_STATUS_REQUEST_COMPLETE:
		// See the CInternetSession constructor for details on INTERNET_FLAG_ASYNC.
		// The lpvStatusInformation parameter points at an INTERNET_ASYNC_RESULT 
		// structure, and dwStatusInformationLength contains the final completion 
		// status of the asynchronous function. If this is ERROR_INTERNET_EXTENDED_ERROR, 
		// the application can retrieve the server error information by using the 
		// Win32 function InternetGetLastResponseInfo. See the ActiveX SDK for more 
		// information about this function. 
		if (dwStatusInformationLength == sizeof(INTERNET_ASYNC_RESULT))
		{
			INTERNET_ASYNC_RESULT* pResult = (INTERNET_ASYNC_RESULT*)lpvStatusInformation;
			str.Format(L"Request complete, dwResult = %8.8X, dwError = %8.8X",
				pResult->dwResult, pResult->dwError);
		}
		else
			str.Format(L"Request complete");
		break;

	case INTERNET_STATUS_CTL_RESPONSE_RECEIVED:
	case INTERNET_STATUS_REDIRECT:
	default:
		str.Format(L"Unknown status: %d", dwInternetStatus);
		break;
	}

	SetStatus(str);

	TRACE("CHttpSession::OnStatusCallback: %s\n", str);
}

void CHttpSession::SetStatus(LPCTSTR fmt, ...)
{
	va_list args;
	WCHAR buffer[512];

	va_start(args, fmt);
	_vstprintf(buffer, (size_t)512, fmt, args);
	va_end(args);

	TRACE1("CHttpSession::SetStatus: %s\n", buffer);

	AfxMessageBox(buffer);
}

