#pragma once

class CTab0;
class CSmartBoxDaemonDlg;

// CTab1 dialog

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab1();

	CSmartBoxDaemonDlg* p_parent;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	CMFCPropertyGridCtrl m_ManualData;

	CMFCPropertyGridProperty* m_PropAPPKEY;		// APPKEY
	CMFCPropertyGridProperty* m_PropStartDate;  // 시작일자
	CMFCPropertyGridProperty* m_PropWarningmin; // 경고기간
	CMFCPropertyGridProperty* m_PropDangermin;  // 위험기간
	CMFCPropertyGridProperty* m_PropSavetotmin; // 보관기간
	CMFCPropertyGridProperty* m_PropProduct;    // 상품명
	CMFCPropertyGridProperty* m_PropRACLL;      // RAC정보(LL)
	CMFCPropertyGridProperty* m_PropRACMM;      // RAC정보(MM)
	CMFCPropertyGridProperty* m_PropRACSS;      // RAC정보(SS)
	CMFCPropertyGridProperty* m_PropSendTime;   // 전송주기

protected:
	afx_msg LRESULT OnAfxWmPropertyChanged(WPARAM wParam, LPARAM lParam);
public:
	void UpdateVals();
	void SendVals();
	CEdit m_SensorCnt;
	afx_msg void OnEnChangeSensorcnt();
	afx_msg void OnBnClickedSnreplacechk();
	CButton m_SensorReplaceChk;
	CEdit m_toSerialEdit;
	CEdit m_frSerialEdit;
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRepbtn();
	CButton m_ReplaceUpdateBtn;
	void UpdateReplacingData();
	afx_msg void OnEnChangeFrserialedit();
	afx_msg void OnEnChangeToserialedit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_SendButton;
	CStatic m_Tab1ICON;
	afx_msg void OnBnClickedSend();
};
