#pragma once
#include "SmartBox MasterKey Application.h"
#include "SmartBoxMasterKeyAppDlg.h"

// CTab3 dialog

class CTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab3)

public:
	CTab3(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab3();
	CSmartBoxMasterKeyAppDlg* p_parent;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 교체 장치
	CEdit m_Edit30;
	// 교체 BOX NO
	CEdit m_Edit31;
	// 교체 상품명
	CEdit m_Edit32;
	// 교체 보관시간
	CEdit m_Edit33;
	// 교체 경고시간
	CEdit m_Edit34;
	// 교체 위험시간
	CEdit m_Edit35;
	// 교체 RAC 대분류
	CEdit m_Edit37;
	// 교체 RAC 중분류
	CEdit m_Edit38;
	// 교체 RAC 소분류
	CEdit m_Edit39;
	// 교체할 센서
	CEdit m_Edit3A;
	// 교체 버튼
	CButton m_Button3A;
	afx_msg void OnBnClickedButton3a();
	virtual BOOL OnInitDialog();
	int ValUpdate();
	int ReplaceDBClicked();
	void ChangeCaption();
	CString GetCaption(int id);
};
