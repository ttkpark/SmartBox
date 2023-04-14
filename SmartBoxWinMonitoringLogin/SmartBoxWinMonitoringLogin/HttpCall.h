#pragma once
#include <afxinet.h>	// MFC Internet Connectivity Classes
#include <afxsock.h>	// MFC socket extensions

#if !defined(MAB_HTTP_CALL_INCLUDED_)
#define MAB_HTTP_CALL_INCLUDED_


CString HTTPEncodeStr(CString ToCode);
CString HTTPDeCodeStr(CString ToCode);
void DeCodeFile(CFile* DataFile, CHttpFile* HttpFile);
CString GetTheUserName();

int HttpCall(CString ServerAddress,
	int ServerPort,
	CString Process,
	CString Query,
	CString DataBlock);

class CHttpSession : public CInternetSession
{
	// Construction
public:
	CHttpSession(LPCTSTR szAgentName,
		DWORD dwContext,
		DWORD dwAccessType);

	virtual ~CHttpSession();
	void CommonConstruct();

	// Operations
public:
	void SetStatus(LPCTSTR fmt, ...);
	void SetStatusWnd(CWnd* pWnd) { m_pStatusWnd = pWnd; }

	// Overrides
public:
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInformation,
		DWORD dwStatusInformationLength);


// Attributes
protected:
	CWnd* m_pStatusWnd;
};

#endif