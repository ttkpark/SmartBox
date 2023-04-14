#pragma once

class CTab1;
class CSmartBoxDaemonDlg;

// CTab0 dialog

class CTab0 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab0)

public:
	CTab0(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab0();

	CSmartBoxDaemonDlg* p_parent;

	CWinThread* DataListDlgThread;
	bool isWorkingDataListDlg;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB0 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int isAutoQuery;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedComconnect();
	afx_msg void OnBnClickedComauto();
	CStatic m_COMStateIcon;
	CButton m_SensorTableBtn;
	CButton m_AutoQueryData;
	afx_msg void OnBnClickedApiquery();
	CStatic m_NumofListSensor;
	afx_msg void OnEnChangeApiurl();
//	afx_msg void OnEnChangeComedit();
//	CEdit m_COMEdit;
	CEdit m_ApiURLEdit;
	afx_msg void OnCbnEditchangeComcombo();
	CComboBox m_COMCombo;
	afx_msg void OnCbnSelchangeComcombo();
	CButton m_COMConnectBtn;
	CButton m_AutoQueryChk;
	afx_msg void OnBnClickedTray();
	afx_msg void OnBnClickedSetablebtn();
	void SensorListChanged();
	afx_msg void OnBnClickedAutodata();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
};
