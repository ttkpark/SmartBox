#pragma once
#include <stdint.h>

#define WM_COMM_READ (WM_USER +80)
#define BUFF_SIZE			4096
// 포트에서 읽기 :
//   포트를 연 후에 포트에 자료가 도착하면 WM_COMM_READ 메시지가 메인 
//   윈도우에 전달된다. ON_MESSAGE 매크로를 이용, 함수를 연결하고
//   m_ReadData String에 저장된 데이터를 이용 읽기
// 포트에 쓰기 : WriteComm(buff, 30)과 같이 버퍼와 그 크기를 건네면 된다.
class CQueue
{
public:
	BYTE buff[BUFF_SIZE];//큐 버퍼 사이즈는 4192로 만듬 
	int m_iHead, m_iTail;//큐 버퍼에 데이터를 넣고 빼 올때 사용할 변수 
	CQueue();
	void Clear();//버퍼를 초기화 시킴
	int GetSize();//현재 버퍼에 들어있는 데이터의 size를 리턴
	BOOL PutByte(BYTE b);//큐버퍼에 1바이트를 넣음
	BOOL GetByte(BYTE *pb);//큐버퍼에서 1바이트를 빼 옴
};


class CMycomm:
	public CCmdTarget
{
public:
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	CQueue      m_QueueRead;			//큐버퍼
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	bool CommOpened = false;

	int open(CString _ComPortName);
	int close();
	int write(void* WriteData, int _Length);
	int read(void* ReadData, DWORD Length);
	bool RTS_DTR_Control(uint8_t RTS, uint8_t DTR);//0:Low 1:high 2:not change//1:Succeed 0:Failed
	CMycomm();
	~CMycomm();
};
// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CMycomm* pComm);
