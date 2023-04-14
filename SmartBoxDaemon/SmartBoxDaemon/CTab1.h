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
	CMFCPropertyGridProperty* m_PropStartDate;  // ��������
	CMFCPropertyGridProperty* m_PropWarningmin; // ���Ⱓ
	CMFCPropertyGridProperty* m_PropDangermin;  // ����Ⱓ
	CMFCPropertyGridProperty* m_PropSavetotmin; // �����Ⱓ
	CMFCPropertyGridProperty* m_PropProduct;    // ��ǰ��
	CMFCPropertyGridProperty* m_PropRACLL;      // RAC����(LL)
	CMFCPropertyGridProperty* m_PropRACMM;      // RAC����(MM)
	CMFCPropertyGridProperty* m_PropRACSS;      // RAC����(SS)
	CMFCPropertyGridProperty* m_PropSendTime;   // �����ֱ�

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
