
// SmartBoxMasterKeyAppDlg.h : header file
//

#pragma once
#include "stdafx.h"
#include "CMycomm.h"
#include "json.h"
#include "sqlite3/sqlite3.h"


struct SensorInitData;
struct SensorDataList;
class CTab1;
class CTab2;
class CTab3;
class CTab4;


template <> UINT AFXAPI HashKey<CString>(CString str);

int JsonfromCString(Json::Value& val, CString& str);
int JsonDatatoString(Json::Value& val, CString& str);
// CSmartBoxMasterKeyAppDlg dialog
class CSmartBoxMasterKeyAppDlg : public CDialogEx
{
// Construction
public:
	CSmartBoxMasterKeyAppDlg(CWnd* pParent = nullptr);	// standard constructor

	//탭 관리
	CTab1* pTab1;
	CTab2* pTab2;
	CTab3* pTab3;
	CTab4* pTab4;

	//마스터키 통신
	SensorInitData TransmitData;
	int TxPacketLen;
	uint8_t* TxPacket;
	char PacketBuf[2048];
	int  PacketBuflen;

	CString idxClient;
	CString work_zone_num;
	CString Token;
	CString ClientName;
	CString work_zone_name;

	int captionavail;
	sqlite3 *pCaptionDB;
	CString CaptionDBpath;
	CMap<int, int, CString, CString&> CaptionList;

	//API 기본 연결
	CString m_ApiURL;

	//센서
	CString m_frSerialNo;
	CString m_toSerialNo;
	SensorInitData replaceData;

	//COM포트 통신
	CString m_COMPort;
	int m_COMConnected;
	CMycomm ComPort;
	int AutoDetectReadWaiting = false;
	uint8_t m_InterfaceStr[512];
	uint8_t ReadString[BUFF_SIZE];
	uint8_t* ReadBuffer;//ReadString 정리 함수.
	int ReadWaiting = false;
	int lastfoc = 0;
	int ReadStringLength = 0;

	int isNDT00;
	bool isReceivedNDT = 0;

	DataList ActualSerialPorts;
	int m_SensorAccessed;
	uint8_t Accessreadbyte, Accessfocus;



	//센서 접근 처리 (스레드)
	CWinThread* SensorAccessThread;
	bool isWorkingAccess;
	volatile bool isReceivedRTNNDT;
	volatile bool isFinishedValue;//is ready to send the data?
	int m_isSensorReplace;
	SensorInitData AccessData;
	CString m_AccessedSerial;
	CString m_BoxSerial;

	//for SensorList
	int m_ListNumSensor;
	CMap<CString, CString, SensorDataList, SensorDataList&> SensorList;
	int SensordatasINITED;

	CString inifile;
	CString herePath;

	//언어설정
	int nowLang;
	CMap<int, int, CString, CString&> LangTable;
	CString LangfilePath;
	CString Langpath;

	//Log 관련
	CString Logroot;

	//Box Processing
	CString StrBoxCode;
	CString StridxGoods;
	CMap<int, int, CString, CString&> Goods;
	
	CWinThread* CallMessageThread;
	CString MessagetoPrint;

	NOTIFYICONDATA nid;

	__time64_t timediff;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMARTBOXAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//Form Control
	CTabCtrl m_Tab;
	CStatusBar m_statusBar;
	CStatic m_statBarIcon;
	CStatic m_statBarText;
	int SetStatusBarText(CString Text);
	void SetCOMICON();
	int SETICON(HICON hIcon);

	//WinAPI-based Code
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();

	//TrayICON
	int TrayButtonEvent();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTrayiconMenu1();
	afx_msg void OnTrayiconMenu2();

	//COM interface
	long OnCommunication(WPARAM wParam, LPARAM lParam);
	void COMConnect();
	int COMDisconnect();

	void COMAutoDetect();
	int COMPortSeek();
	CString AutoDetect();
	int Serial_Write(int _Length);
	int Serial_Read(DWORD Length);
	int SerialConnect(CString port);
	int SerialDisconnect();
	int Serial_Detect();

	int SystemConst_Store();
	int SystemConst_Read();
	void SystemConst_Reset();

	__time64_t GetGMTTimetoLocalTime();

	void SensorDataList2SensorData(SensorDataList& in, SensorInitData& out);

	void CallMessageBox(CString Msg);


protected:
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTab2UpdateVal(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStatbartxt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommRead(WPARAM wParam, LPARAM lParam);
public:
	int isSensorWorking(SensorDataList& sdt);

	//센서등록
	int SensorAccess(char* serialnum);
	int DataReady(CString str);
	int PacketData();

	//Encode
	CString Encoding100(CString data, CString* ret);
	CString Encoding800(CString data, CString* ret);
	CString Base64Encoding(CString data, CString* ret);

	//API 
	CString QueryAPI(CString apin, CString Msg);
	int APIInitalUpdate();
	int APISendEncoding100RetJson(CString APIname, CString DatatoSend, Json::Value& jsonRet);
	void APIShowResultError(CString APIname, Json::Value& jsonIn);
	void ShowAPIError(CString ErrorCode, CString Error);
	int GetAPIBox();
	int GetAPIGOD();
	int GetAPISYS();
	int GetAPISDI(CString equip_sn, SensorDataList& sdt);
	int GetAPISLR(CString idxClient, CString admin = _T("admin"), DataList* SensorData = NULL);
	int Process_Replace();

	//HTTP
	CString HTTPGet(CString URL, CString* RetVal);

	// 로그 출력 함수
	int MyLog_Add(CString logData,int type);
	int My_Log(CString Log, int type);

	// 언어에 따른 Caption을 얻는함수.
	int GetCaption(int id, CString& out);
	CString GetCaption(int id);
	int SetLang(int nLang);
	void ChangeCaption();
	int KeyProcess(CString str);
	void ReadStr_Process();
};
