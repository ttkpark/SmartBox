
// SmartBoxDaemonDlg.h : header file
//
#include "CMycomm.h"
#include "json.h"
#include <afxtempl.h>
#pragma once

#define WM_TAB1_UPDATE_VAL (WM_USER+1)
#define WM_STATBARTXT (WM_USER+2)

struct SensorData
{
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
};
struct SensorDataList
{
	CString serialno;     // 상품명
	CString appkey;

	CString StartDate;
	CString WarningDate;
	CString DangerDate;
	CString LastDate;

	CString alertstatus;

	CString savetotmin;
	CString warningmin;
	CString dangermin;

	CString GoodsName;
	CString RACLL;
	CString RACMM;
	CString RACSS;
	CString SendTimeSec;
	CString Revision;
};

class CTab0;
class CTab1;
class CDataTableDlg;
class CBoxManageDlg;

class DataList {
	void** data;
	int* Size;
	int contents;
	int focus;
public:
	void initContents(int);
	void initData(int, void*, int);
	void DeleteData(int);
	void* WhereisData(int, int&);
	int AlldataSize();
	void DeleteContents();
	int getfocusID();
	int setfocusID(int);
	int getendID();
	~DataList();
};


class COMPort {
public:
	int number;
	char Descreption[30];
	wchar_t* GetPortNameW();
	char* GetPortName();
};

class SystemConst
{
public:
	int d_isAutoQuery;
	CString m_ApiURL;
};

template <> UINT AFXAPI HashKey<CString>(CString str);

// CSmartBoxDaemonDlg dialog
class CSmartBoxDaemonDlg : public CDialogEx
{
// Construction
public:
	CSmartBoxDaemonDlg(CWnd* pParent = nullptr);	// standard constructor

	CTab0* pDlg0;
	CTab1* pDlg1;
	CDataTableDlg *pDataDlg;
	CBoxManageDlg* pBoxDlg;

	SensorData MannualData;
	SensorData TransmitData;
	int TxPacketLen;
	uint8_t* TxPacket;
	char PacketBuf[2048];
	int  PacketBuflen;


	NOTIFYICONDATA nid;

	CString idxClient;

	//from Tab1
	int m_iSensorCnt;
	BOOL m_isSensorReplace;
	CString m_frSerialNo;
	CString m_toSerialNo;

	//from Tab0
	CString m_ApiURL;

	//about SerialCom
	CString m_COMPort;
	int m_COMConnected;
	CMycomm ComPort;
	int AutoDetectReadWaiting = false;
	uint8_t m_InterfaceStr[512];
	uint8_t ReadString[BUFF_SIZE];
	int ReadWaiting = false;
	int lastfoc = 0;
	int ReadStringLength = 0;
	bool Hex = 0;
	DataList ActualSerialPorts;
	int m_SensorAccessed;
	uint8_t Accessreadbyte, Accessfocus;

	CWinThread* SensorAccessThread;
	bool isWorkingAccess;
	bool isReceivedRTNNDT;
	
	bool isFinishedValue;//is ready to send the data?
	
	//for SensorList
	int m_ListNumSensor;
	CMap<CString, CString, SensorDataList, SensorDataList&> SensorList;
	int SensordatasINITED;

	SystemConst Systemconst;


	//Box Processing

	CString StrBoxCode;
	CString StridxGoods;
	DataList Goods;
	///api/BOX/  equip_sn, BoxCode, idxGoods, savetotmin, warningmin, dangermin
	// RacNo, startdate[단, No Touch LCD]
	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMARTBOXDAEMON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	CStatusBar m_statusBar;
	CStatic m_statBarIcon;
	CStatic m_statBarText;
	int SetStatusBarText(CString Text);
//	CEdit m_EditText;
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetCOMICON();
	void COMConnect();
	int COMDisconnect();
	void COMAutoDetect();
	long OnCommunication(WPARAM wParam, LPARAM lParam);
	int BTTray();
	afx_msg void OnTrayicon32771();
	afx_msg void OnTrayicon32772();
	afx_msg LRESULT CSmartBoxDaemonDlg::OnTrayNotifycation(WPARAM wParam, LPARAM lParam);
	int COMPortSeek();
//	int AutoDetect();
	CString AutoDetect();
//	int Serial_Write();
	int Serial_Write(int _Length);
	int Serial_Read(DWORD Length);
	int SerialConnect(CString port);
	int SerialDisconnect();
	int Serial_Detect();
protected:
	afx_msg LRESULT OnCommRead(WPARAM wParam, LPARAM lParam);
public:
	int SensorAccess(char* serialnum);
	CString HTTPGet(CString URL, CString* RetVal);
	CString Encoding(CString data, CString* ret);
	CString Base64Encoding(CString data, CString* ret);
	int GetAPISLR(CString idxClient, CString admin = _T("admin"), DataList* SensorData = NULL);
	int SystemConst_Store();
	int SystemConst_Read();
//	int SystemConst_Open(Json::Value* out);
	void SystemConst_Reset();
	int DataReady(CString str);
	int SETICON(HICON hIcon);
	void SensorDataList2SensorData(SensorDataList& in, SensorData& out);
	int isNDT00;
	virtual BOOL DestroyWindow();
protected:
	afx_msg LRESULT OnTab1UpdateVal(WPARAM wParam, LPARAM lParam);
public:
	int PacketData();
protected:
	afx_msg LRESULT OnStatbartxt(WPARAM wParam, LPARAM lParam);
public:
	int Process_Replace();
	CString QueryAPI(CString apin, CString Msg);
};
