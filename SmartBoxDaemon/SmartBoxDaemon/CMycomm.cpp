// CMyComm.cpp: 구현 파일
//

#include "pch.h"
#include "SmartBoxDaemon.h"
#include "afxdialogex.h"
#include "SmartBoxDaemonDlg.h"
#include "CMycomm.h"

extern CSmartBoxDaemonDlg* MainDlg;
extern HWND hCommWnd;

CMycomm::CMycomm()
{
}


int CMycomm::open(CString _ComPortName)
{
	if(CommOpened)
	{
		//MainDlg->SetStatusBarText(_T("Open Error : COM Port is already opened!!\r\n"));
		return -1;
	}
	DCB    dcb = { sizeof(DCB) };
	DWORD			dwThreadID;

	// overlapped structure 변수 초기화.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read 이벤트 생성에 실패..
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}


	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write 이벤트 생성에 실패..
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}
	m_sPortName.Format(_T("\\\\.\\%s"), _ComPortName);

	m_hComm = CreateFile(m_sPortName,
		GENERIC_READ | GENERIC_WRITE,								// 접근모드 읽기쓰기인가 읽기 전용인가
		0,															// 다른 프로그램과 공유를 할것인가 아닌가.
		NULL,                                                       // 보안에 관한 속성
		OPEN_EXISTING,												// 어떻게 열것인가
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,				// 파일 속성(보통 파일이면서 overlapped 되게 한다.)
		NULL);														// 템플릿 파일 핸들러
	m_sPortName = _ComPortName;
	if (m_hComm == (HANDLE)-1)
	{
		//MainDlg->SetStatusBarText(_T("Open Error : COM Port Connect Fail!!\r\n"));
		return -1;
	}

	// EV_RXCHAR event 설정
	SetCommMask(m_hComm, EV_RXCHAR);// EV_BREAK | EV_ERR |  | EV_TXEMPTY);


	// InQueue, OutQueue 크기 설정.
	SetupComm(m_hComm, BUFF_SIZE, BUFF_SIZE);


	// 포트 비우기.
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);




	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2 * 2000000 / 921600;
	timeouts.WriteTotalTimeoutConstant = 0;
	if (!SetCommTimeouts(m_hComm, &timeouts))
		return -1;

	// dcb 설정
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb); // 예전 값을 읽음.
	dcb.BaudRate = 2000000;      //BaudRate 비트/초			//CBR_115200
	dcb.ByteSize = 8;               //데이터비트
	dcb.Parity = NOPARITY;          // 패리티
	dcb.StopBits = ONESTOPBIT;      // 정지비트

	dcb.fOutX = true;
	dcb.fInX = true;
	dcb.fOutxCtsFlow = false;
	dcb.fOutxDsrFlow = false;
	dcb.fDsrSensitivity = false;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
/*
	dcb.fOutX = false;
	dcb.fInX = false;
	dcb.fOutxCtsFlow = true;
	dcb.fOutxDsrFlow = true;
	dcb.fDsrSensitivity = true;
	dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;*/
	
	dcb.fParity = FALSE;
	dcb.fNull = FALSE;

	if (!SetCommState(m_hComm, &dcb))
	{
		MainDlg->SetStatusBarText(_T("Open Error : COM Port Connect Setting Fail!!\r\n"));
		CommOpened = true; close();
		return -1;
	}

	CommOpened = true;
	//--> 포트 감시 쓰레드 생성.
	m_hThreadWatchComm = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadWatchComm,
		this, 0, &dwThreadID);

	//--> 쓰레드 생성에 실패하면..
	if (!m_hThreadWatchComm)
	{
		//--> 열린 포트를 닫고..
		close();
		return FALSE;
	}

	return 0;
}
int CMycomm::close() {
	if (!CommOpened || m_hComm == NULL)
	{
		MainDlg->SetStatusBarText(_T("Open Error : COM Port is NOT opened!!\r\n"));
		return -1;
	}
	SetCommMask(m_hComm, 0);
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	CloseHandle(m_hComm);
	CommOpened = false;
	return 0;
}
bool CMycomm::RTS_DTR_Control(uint8_t RTS, uint8_t DTR) {
	DWORD dwFunc = 0;
	if (RTS == 0)dwFunc |= 1 << CLRRTS;
	else if(RTS == 1)dwFunc |= 1 << SETRTS;
	if (DTR == 0)dwFunc |= 1 << CLRDTR;
	else if (DTR == 1)dwFunc |= 1 << SETDTR;
	return EscapeCommFunction(m_hComm, dwFunc);
}
int CMycomm::write(void* WriteData, int _Length = -1) {
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;
	int Length;
	if (_Length == -1)Length = strlen((const char*)WriteData);
	else Length = _Length;

	//--> 포트가 연결되지 않은 상태이면..
	if (!CommOpened)
	{
		MainDlg->SetStatusBarText(_T("Write Error : COM Port is not connected!!\r\n"));
		return -1;
	}


	//--> 인자로 들어온 버퍼의 내용을 WriteData 만큼 쓰고.. 쓴 갯수를.,dwWritten 에 넘김.
	if (!WriteFile(m_hComm, WriteData, Length, &dwWritten, &m_osWrite))
	{
		//--> 아직 전송할 문자가 남았을 경우..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
			while (!GetOverlappedResult(m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError(m_hComm, &dwErrorFlags, &comstat);
		}
	}



	//--> 실제 포트로 쓰여진 갯수를 리턴..
	return dwWritten;
}
int CMycomm::read(void* ReadData, DWORD Length) {
	if (!CommOpened)
	{
		MainDlg->SetStatusBarText(_T("Read Error : COM Port is not connected!!\r\n"));
		return -1;
	}
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT comstat;

	//--- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError(m_hComm, &dwErrorFlags, &comstat);

	//--> 시스템 큐에서 읽을 거리가 있으면..
	dwRead = comstat.cbInQue;
	if (dwRead > 0)
	{

		//--> 버퍼에 일단 읽어들이는데.. 만일..읽어들인값이 없다면..
		if (!ReadFile(m_hComm, ReadData, Length, &dwRead, &m_osRead))
		{
			//--> 읽을 거리가 남았으면..
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				while (!GetOverlappedResult(m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError(m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}


	//--> 실제 읽어들인 갯수를 리턴.
	return dwRead;
}
CMycomm::~CMycomm()
{
}


// 포트를 감시하고, 읽힌 내용이 있으면
// m_ReadData에 저장한 뒤에 MainWnd에 메시지를 보내어 Buffer의 내용을
// 읽어가라고 신고한다.
DWORD	ThreadWatchComm(CMycomm* pComm)
{
	DWORD           dwEvent;
	OVERLAPPED      os;
	BOOL            bOk = TRUE;
	BYTE            buff[204800];      // 읽기 버퍼
	DWORD           dwRead;  // 읽은 바이트수.


							 // Event, OS 설정.
	memset(&os, 0, sizeof(OVERLAPPED));

	//--> 이벤트 설정..
	if (!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		bOk = FALSE;
	}

	//--> 이벤트 마스크..
	if (!SetCommMask(pComm->m_hComm, EV_RXCHAR))
	{
		bOk = FALSE;
	}

	//--> 이벤트나..마스크 설정에 실패함..
	if (!bOk)
	{
		AfxMessageBox(_T("Error while creating ThreadWatchComm, ") + pComm->m_sPortName);
		return FALSE;
	}

	while (pComm->CommOpened)//포트가 연결되면 무한 루프에 들어감
	{
		dwEvent = 0;

		// 포트에 읽을 거리가 올때까지 기다린다.
		WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);


		//--> 데이터가 수신되었다는 메세지가 발생하면..
		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// 포트에서 읽을 수 있는 만큼 읽는다.
			//--> buff 에 받아놓고..
			do
			{


				dwRead = pComm->read(buff, 2048); //들어온 데이터 읽어 오기 
				if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for (WORD i = 0; i < dwRead; i++)
					{
						pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기 
					}
				}
				else
					AfxMessageBox(_T("buff full")); //큐버퍼의 크기를 초과하면 경고 메시지 보냄
			} while (dwRead);
			::PostMessage(hCommWnd, WM_COMM_READ, 0, 0);
			//CSerialComDlg로 데이터가 들어왔다는 메시지를 보냄
		}
		Sleep(30);	// 받은 데이터를 화면에 보여줄 시간을 벌기 위해.
					// 데이터를 연속으로 받으면 cpu점유율이 100%가 되어 화면에 뿌려주는 작업이 잘 안되고. 결과적으로 
					// 큐 버퍼에 데이터가 쌓이게 됨
	}

	CloseHandle(os.hEvent);

	//--> 쓰레드 종료가 되겠죠?
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;

}


void CQueue::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff, 0, BUFF_SIZE);
}
CQueue::CQueue()
{
	Clear();
}
int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}
BOOL CQueue::PutByte(BYTE b)
{
	if (GetSize() == (BUFF_SIZE - 1)) return FALSE;
	buff[m_iHead++] = b;
	m_iHead %= BUFF_SIZE;
	return TRUE;
}
BOOL CQueue::GetByte(BYTE *pb)
{
	if (GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= BUFF_SIZE;
	return TRUE;
}