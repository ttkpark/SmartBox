#pragma once#include "SmartBox MasterKey Application.h"
#include "SmartBoxMasterKeyAppDlg.h"

// CTab2 dialog

class CTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab2)

public:
	CTab2(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab2();
	CSmartBoxMasterKeyAppDlg* p_parent;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCombo2();
	// 장치번호
	CEdit m_Edit20;
	// //BOX 번호
	CEdit m_Edit21;
	// "보관시간"
	CEdit m_Edit23;
	// "경고시간"
	CEdit m_Edit24;
	// "위험시간"
	CEdit m_Edit25;
	// RAC NO 대분류
	CEdit m_Edit27;
	// RAC NO 중분류
	CEdit m_Edit28;
	// RAC NO 소분류
	CEdit m_Edit29;
	// 센서/박스 등록 버튼
	CButton m_Button2A;
	// 보관시간 단위
	CComboBox m_Combo2;
	// 경고시간 단위
	CComboBox m_Combo3;
	// 위험시간 단위
	CComboBox m_Combo4;
	afx_msg void OnBnClickedButton2a();
	virtual BOOL OnInitDialog();
	int SensorAccessed();
	int AccessdatOut();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnEnChangeEdit23();
	afx_msg void OnEnChangeEdit24();
	afx_msg void OnEnChangeEdit25();
	CComboBox m_Combo22;
	void ChangeCaption();
	CString GetCaption(int id);
};
