#pragma once
#include <stdint.h>

#define WM_COMM_READ (WM_USER +80)
#define BUFF_SIZE			4096
// ��Ʈ���� �б� :
//   ��Ʈ�� �� �Ŀ� ��Ʈ�� �ڷᰡ �����ϸ� WM_COMM_READ �޽����� ���� 
//   �����쿡 ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿�, �Լ��� �����ϰ�
//   m_ReadData String�� ����� �����͸� �̿� �б�
// ��Ʈ�� ���� : WriteComm(buff, 30)�� ���� ���ۿ� �� ũ�⸦ �ǳ׸� �ȴ�.
class CQueue
{
public:
	BYTE buff[BUFF_SIZE];//ť ���� ������� 4192�� ���� 
	int m_iHead, m_iTail;//ť ���ۿ� �����͸� �ְ� �� �ö� ����� ���� 
	CQueue();
	void Clear();//���۸� �ʱ�ȭ ��Ŵ
	int GetSize();//���� ���ۿ� ����ִ� �������� size�� ����
	BOOL PutByte(BYTE b);//ť���ۿ� 1����Ʈ�� ����
	BOOL GetByte(BYTE *pb);//ť���ۿ��� 1����Ʈ�� �� ��
};


class CMycomm:
	public CCmdTarget
{
public:
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	CQueue      m_QueueRead;			//ť����
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	bool CommOpened = false;

	int open(CString _ComPortName);
	int close();
	int write(void* WriteData, int _Length);
	int read(void* ReadData, DWORD Length);
	bool RTS_DTR_Control(uint8_t RTS, uint8_t DTR);//0:Low 1:high 2:not change//1:Succeed 0:Failed
	CMycomm();
	~CMycomm();
};
// Thread�� ����� �Լ� 
DWORD	ThreadWatchComm(CMycomm* pComm);
