#pragma once

#define RES_OK  0
#define RES_ERR 1
#define NULLSTRING _T("")
#define Lengthis0 0

#define WM_TAB2_UPDATE_VAL	(WM_USER+1)
#define WM_STATBARTXT		(WM_USER+2)
#define WM_LOG				(WM_USER+3)

#define WM_TRAY_NOTIFICATION WM_APP + 1
#define SMARTBOXAPPLICATION_TRAY 84935

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

struct SensorRange
{ 
	CString sensortype;//온도:TP 습도:HU...
	float d01fr;//위험 범위 1
	float d01to;//
	float d02fr;//위험 범위 2
	float d02to;//
	float w01fr;//경고 범위 1
	float w01to;//
	float w02fr;//경고 범위 2
	float w02to;//
};


struct SensorInitData
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

	CString m_BoxNum;		 // 박스 번호
	CString work_zone_num;	 // 워크존 번호
	CString work_zone_name;	 // 워크존 이름
	CString m_idxWE;		 // idxWE
	SensorRange range01;
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

	CString idxGoods;
	CString RACLL;
	CString RACMM;
	CString RACSS;
	CString SendTimeSec;
	CString Revision;

	CString BoxNum;

	CString work_zone_num;
	CString work_zone_name;	 // 워크존 이름
	CString idxWE;

	SensorRange range01;
};