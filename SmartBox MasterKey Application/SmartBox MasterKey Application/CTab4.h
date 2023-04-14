#pragma once
#include "SmartBox MasterKey Application.h"
#include "SmartBoxMasterKeyAppDlg.h"

// CTab4 dialog

class CTab4 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab4)

public:
	CTab4(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab4();
	CSmartBoxMasterKeyAppDlg* p_parent;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// //BUTTON41 : 끊음/연결 버튼
	void UpdateButton41(int m_connected);
	CComboBox m_Combo40;
	// //Language setting
	CComboBox m_Combo41;
	afx_msg void OnBnClickedButton40();
	afx_msg void OnBnClickedButton41();
	//"자동검색"
	CButton m_Button40;
	//"연결"
	CButton m_Button41;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo41();
	afx_msg void OnCbnEditchangeCombo40();
	afx_msg void OnCbnSelchangeCombo40();
	afx_msg void OnBnClickedApiretry();
	int updatecaption();
	void ChangeCaption();
	CString GetCaption(int id);
};
