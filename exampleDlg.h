// exampleDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "thread_control.h"

// CexampleDlg 对话框
class CexampleDlg : public CDialog
{
// 构造
public:
	CexampleDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXAMPLE_DIALOG };

	void UpdateList(long index,long action);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	CString ThreadStateToString(ThreadState state);

// 实现
protected:
	HICON m_hIcon;

	long GetListIndex(long index);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCloseallwin();

	afx_msg void OnBnClickedButtonStartall();
	afx_msg void OnBnClickedButtonStopall();
	afx_msg void OnBnClickedButtonPauseall();
	afx_msg void OnBnClickedButtonResumeall();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void CoordUpdate(int* nums);
	CListCtrl m_list;
		
	CButton autoRuneSolver;
	CButton friendPlayerNotification;
	CButton huangMen;
	CButton expPot;
	CButton EXP_PARK;
	CButton hunt;
	CComboBox mapList;
	afx_msg void OnBnClickedButtonSoft();

	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

	CButton EXP_MVP;
	CButton autoWealth;
	CButton autoOil;
	CButton bless;
	CButton EXP15M;
	CButton EXP30M;
	CButton whiteDetect;
};

