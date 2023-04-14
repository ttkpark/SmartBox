// CMyComm.cpp: ���� ����
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

	// overlapped structure ���� �ʱ�ȭ.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read �̺�Ʈ ������ ����..
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}


	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write �̺�Ʈ ������ ����..
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}
	m_sPortName.Format(_T("\\\\.\\%s"), _ComPortName);

	m_hComm = CreateFile(m_sPortName,
		GENERIC_READ | GENERIC_WRITE,								// ���ٸ�� �б⾲���ΰ� �б� �����ΰ�
		0,															// �ٸ� ���α׷��� ������ �Ұ��ΰ� �ƴѰ�.
		NULL,                                                       // ���ȿ� ���� �Ӽ�
		OPEN_EXISTING,												// ��� �����ΰ�
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,				// ���� �Ӽ�(���� �����̸鼭 overlapped �ǰ� �Ѵ�.)
		NULL);														// ���ø� ���� �ڵ鷯
	m_sPortName = _ComPortName;
	if (m_hComm == (HANDLE)-1)
	{
		//MainDlg->SetStatusBarText(_T("Open Error : COM Port Connect Fail!!\r\n"));
		return -1;
	}

	// EV_RXCHAR event ����
	SetCommMask(m_hComm, EV_RXCHAR);// EV_BREAK | EV_ERR |  | EV_TXEMPTY);


	// InQueue, OutQueue ũ�� ����.
	SetupComm(m_hComm, BUFF_SIZE, BUFF_SIZE);


	// ��Ʈ ����.
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);




	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2 * 2000000 / 921600;
	timeouts.WriteTotalTimeoutConstant = 0;
	if (!SetCommTimeouts(m_hComm, &timeouts))
		return -1;

	// dcb ����
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb); // ���� ���� ����.
	dcb.BaudRate = 2000000;      //BaudRate ��Ʈ/��			//CBR_115200
	dcb.ByteSize = 8;               //�����ͺ�Ʈ
	dcb.Parity = NOPARITY;          // �и�Ƽ
	dcb.StopBits = ONESTOPBIT;      // ������Ʈ

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
	//--> ��Ʈ ���� ������ ����.
	m_hThreadWatchComm = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadWatchComm,
		this, 0, &dwThreadID);

	//--> ������ ������ �����ϸ�..
	if (!m_hThreadWatchComm)
	{
		//--> ���� ��Ʈ�� �ݰ�..
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

	//--> ��Ʈ�� ������� ���� �����̸�..
	if (!CommOpened)
	{
		MainDlg->SetStatusBarText(_T("Write Error : COM Port is not connected!!\r\n"));
		return -1;
	}


	//--> ���ڷ� ���� ������ ������ WriteData ��ŭ ����.. �� ������.,dwWritten �� �ѱ�.
	if (!WriteFile(m_hComm, WriteData, Length, &dwWritten, &m_osWrite))
	{
		//--> ���� ������ ���ڰ� ������ ���..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeouts�� ������ �ð���ŭ ��ٷ��ش�.
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



	//--> ���� ��Ʈ�� ������ ������ ����..
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

	//--- system queue�� ������ byte���� �̸� �д´�.
	ClearCommError(m_hComm, &dwErrorFlags, &comstat);

	//--> �ý��� ť���� ���� �Ÿ��� ������..
	dwRead = comstat.cbInQue;
	if (dwRead > 0)
	{

		//--> ���ۿ� �ϴ� �о���̴µ�.. ����..�о���ΰ��� ���ٸ�..
		if (!ReadFile(m_hComm, ReadData, Length, &dwRead, &m_osRead))
		{
			//--> ���� �Ÿ��� ��������..
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts�� ������ �ð���ŭ ��ٷ��ش�.
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


	//--> ���� �о���� ������ ����.
	return dwRead;
}
CMycomm::~CMycomm()
{
}


// ��Ʈ�� �����ϰ�, ���� ������ ������
// m_ReadData�� ������ �ڿ� MainWnd�� �޽����� ������ Buffer�� ������
// �о��� �Ű��Ѵ�.
DWORD	ThreadWatchComm(CMycomm* pComm)
{
	DWORD           dwEvent;
	OVERLAPPED      os;
	BOOL            bOk = TRUE;
	BYTE            buff[204800];      // �б� ����
	DWORD           dwRead;  // ���� ����Ʈ��.


							 // Event, OS ����.
	memset(&os, 0, sizeof(OVERLAPPED));

	//--> �̺�Ʈ ����..
	if (!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		bOk = FALSE;
	}

	//--> �̺�Ʈ ����ũ..
	if (!SetCommMask(pComm->m_hComm, EV_RXCHAR))
	{
		bOk = FALSE;
	}

	//--> �̺�Ʈ��..����ũ ������ ������..
	if (!bOk)
	{
		AfxMessageBox(_T("Error while creating ThreadWatchComm, ") + pComm->m_sPortName);
		return FALSE;
	}

	while (pComm->CommOpened)//��Ʈ�� ����Ǹ� ���� ������ ��
	{
		dwEvent = 0;

		// ��Ʈ�� ���� �Ÿ��� �ö����� ��ٸ���.
		WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);


		//--> �����Ͱ� ���ŵǾ��ٴ� �޼����� �߻��ϸ�..
		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			//--> buff �� �޾Ƴ���..
			do
			{


				dwRead = pComm->read(buff, 2048); //���� ������ �о� ���� 
				if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for (WORD i = 0; i < dwRead; i++)
					{
						pComm->m_QueueRead.PutByte(buff[i]);//ť ���ۿ�  ���� ������ �ֱ� 
					}
				}
				else
					AfxMessageBox(_T("buff full")); //ť������ ũ�⸦ �ʰ��ϸ� ��� �޽��� ����
			} while (dwRead);
			::PostMessage(hCommWnd, WM_COMM_READ, 0, 0);
			//CSerialComDlg�� �����Ͱ� ���Դٴ� �޽����� ����
		}
		Sleep(30);	// ���� �����͸� ȭ�鿡 ������ �ð��� ���� ����.
					// �����͸� �������� ������ cpu�������� 100%�� �Ǿ� ȭ�鿡 �ѷ��ִ� �۾��� �� �ȵǰ�. ��������� 
					// ť ���ۿ� �����Ͱ� ���̰� ��
	}

	CloseHandle(os.hEvent);

	//--> ������ ���ᰡ �ǰ���?
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