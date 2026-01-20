#include "stdafx.h"
#include "example.h"
#include "exampleDlg.h"
#include "thread_control.h"
#include "log.h"
#include "boHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern sptool * g_dm;


HWND   g_main_hwnd = NULL;
CexampleDlg * g_main_cwnd = NULL;

void UpdateUI(long index,long action);
void UpdateCoords(int* nums);

int resumeCount = 0;

int SplitString(LPCTSTR lpszStr, LPCTSTR lpszSplit, CStringArray& rArrString, BOOL bAllowNullString)
{
	rArrString.RemoveAll();
	CString szStr = lpszStr;
	szStr.TrimLeft();
	szStr.TrimRight();

	if (szStr.GetLength() == 0)
	{
		return 0;
	}

	CString szSplit = lpszSplit;

	if (szSplit.GetLength() == 0)
	{
		rArrString.Add(szStr);
		return 1;
	}

	CString s;
	int n;

	do
	{
		n = szStr.Find(szSplit);

		if (n > 0)
		{
			rArrString.Add(szStr.Left(n));
			szStr = szStr.Right(szStr.GetLength() - n - szSplit.GetLength());
			szStr.TrimLeft();
		}
		else if (n == 0)
		{
			if (bAllowNullString)
			{
				rArrString.Add(_T(""));
			}

			szStr = szStr.Right(szStr.GetLength() - szSplit.GetLength());
			szStr.TrimLeft();
		}
		else
		{
			if ((szStr.GetLength() > 0) || bAllowNullString)
			{
				rArrString.Add(szStr);
			}

			break;
		}
	}
	while (1);

	return (int)rArrString.GetSize();
}

CexampleDlg::CexampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CexampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);



}

void CexampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_list);
	DDX_Control(pDX, IDC_CHECKRUNE, autoRuneSolver);
	DDX_Control(pDX, IDC_CHECKOTHERS, friendPlayerNotification);
	DDX_Control(pDX, IDC_CHECKHUANGMEN, huangMen);
	DDX_Control(pDX, IDC_CHECK4, expPot);
	DDX_Control(pDX, IDC_CHECK5, EXP_PARK);

	DDX_Control(pDX, IDC_RADIO1, hunt);
	DDX_Control(pDX, IDC_COMBO1, mapList);
	DDX_Control(pDX, IDC_CHECK_TAI, EXP_MVP);
	DDX_Control(pDX, IDC_CHECK1, autoWealth);
	DDX_Control(pDX, IDC_CHECK2, autoOil);
	DDX_Control(pDX, IDC_CHECK6, bless);
	DDX_Control(pDX, IDC_CHECK7, EXP15M);
	DDX_Control(pDX, IDC_CHECK8, EXP30M);
	DDX_Control(pDX, IDC_CHECK9, whiteDetect);
}

BEGIN_MESSAGE_MAP(CexampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_StopAll, &CexampleDlg::OnBnClickedButtonStopall)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CloseAllWin, &CexampleDlg::OnBnClickedButtonCloseallwin)
	ON_BN_CLICKED(IDC_BUTTON_StartAll, &CexampleDlg::OnBnClickedButtonStartall)
	ON_BN_CLICKED(IDC_BUTTON_PauseAll, &CexampleDlg::OnBnClickedButtonPauseall)
	ON_BN_CLICKED(IDC_BUTTON_ResumeAll, &CexampleDlg::OnBnClickedButtonResumeall)
	ON_BN_CLICKED(IDC_BUTTON_SOFT, &CexampleDlg::OnBnClickedButtonSoft)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	//ON_MESSAGE(WM_HOTKEY, OnHotKey2)
END_MESSAGE_MAP()


// CexampleDlg 消息处理程序

BOOL CexampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	g_main_hwnd = GetSafeHwnd();
	g_main_cwnd = this;

	SetTimer(0,300,NULL);

	// 初始化列表
	m_list.InsertColumn(0,_T("窗口句柄"),LVCFMT_LEFT,70);
	m_list.InsertColumn(1,_T("PID"),LVCFMT_LEFT,55);
	m_list.InsertColumn(2,_T("主线程序号"),LVCFMT_LEFT,86);
	m_list.InsertColumn(3,_T("副线程序号"),LVCFMT_LEFT,86);
	m_list.InsertColumn(4,_T("主线程状态"),LVCFMT_LEFT,87);
	m_list.InsertColumn(5,_T("副线程状态"),LVCFMT_LEFT,87);
	m_list.InsertColumn(6,_T("任务状态"),LVCFMT_LEFT,84);
	m_list.InsertColumn(7,_T("异常状态"),LVCFMT_LEFT,74);

	m_list.SetExtendedStyle(m_list.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER);

	autoRuneSolver.SetCheck(1);
	friendPlayerNotification.SetCheck(1);
	whiteDetect.SetCheck(1);
	hunt.SetCheck(1);

	EXP15M.SetCheck(0);
	expPot.SetCheck(0);
	autoWealth.SetCheck(0);
	autoOil.SetCheck(0);

	// 地图添加
	mapList.AddString(_T("甲板上层1"));
	mapList.AddString(_T("树3"));
	mapList.SetCurSel(0);

	RegisterHotKey(m_hWnd, IDC_BUTTON_SOFT, MOD_CONTROL,76);
	RegisterHotKey(m_hWnd, IDC_BUTTON_PauseAll, MOD_CONTROL, 80);

	return TRUE; 
}

void CexampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


HCURSOR CexampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexampleDlg::CoordUpdate(int* nums)
{
	CString tips;
	tips.Format(_T("（%d,%d)"), *nums, *(nums + 1));

	GetDlgItem(IDC_UserCoo)->SetWindowText(tips);
	}




void CexampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
	{
		CString tips;
		tips.Format(_T("当前对象数量:%d     插件版本:%s"),g_dm->GetDmCount(),g_dm->Ver());

		GetDlgItem(IDC_EDIT_TIP)->SetWindowText(tips);
	}
	CDialog::OnTimer(nIDEvent);
}

long CexampleDlg::GetListIndex(long index)
{
	long item_count = m_list.GetItemCount();
	if (item_count == 0)
	{
		return -1;
	}

	if (index < 0 || index >= MAX_HWND * 3)
	{
		return -1;
	}

	if (index >= MAX_HWND * 2)
	{
		index = index - MAX_HWND * 2;
	}
	else if (index >= MAX_HWND) {
		index = index - MAX_HWND;
	}

	for (int i = 0;i < item_count;++i)
	{
		long index_tmp = _tstoi(m_list.GetItemText(i,2));

		if (index == index_tmp)
		{
			return i;
		}
	}
	return -1;
}


CString CexampleDlg::ThreadStateToString(ThreadState state)
{
	CString ret;

	switch (state)
	{
	case State_Inactive:
		ret = _T("未启动");
		break;
	case State_Starting:
		ret = _T("正在启动..");
		break;
	case State_Runing:
		ret = _T("正在运行");
		break;
	case State_Pausing:
		ret = _T("正在暂停");
		break;
	case State_Pause:
		ret = _T("暂停");
		break;
	case State_Resuming:
		ret = _T("正在恢复");
		break;
	case State_Stoping:
		ret = _T("正在停止");
		break;
	default:
		Log(_T("未定义的线程状态:%d"),state);
		break;
	}


	return ret;
}

void CexampleDlg::UpdateList(long index,long action)
{
	if (action == UI_ADD)
	{
		long list_index = m_list.InsertItem(m_list.GetItemCount(), _T(""));

		CString tmp;

		tmp.Format(_T("%d"),g_info[index].hwnd);
		m_list.SetItemText(list_index, 0, tmp);

		tmp.Format(_T("%d"),g_info[index].pid);
		m_list.SetItemText(list_index, 1, tmp);

		tmp.Format(_T("%d"),index);
		m_list.SetItemText(list_index, 2, tmp);
		
		tmp.Format(_T("%d"),index+MAX_HWND);
		m_list.SetItemText(list_index, 3, tmp);

		UpdateUI(index,UI_UPDATE);
		return;
	}
	else if (action == UI_DELETE)
	{
		long list_index = GetListIndex(index);

		if (list_index == -1)
		{
			return;
		}

		m_list.DeleteItem(list_index);
		m_list.Invalidate();
		m_list.UpdateWindow();
		return;
	}
	else if (action == UI_UPDATE)
	{
		long list_index = GetListIndex(index);

		if (list_index == -1)
		{
			return;
		}

		// 先更新主
		m_list.SetItemText(list_index, 4, ThreadStateToString(g_info[index].thread_state));
		m_list.SetItemText(list_index, 6, g_info[index].task_state);
		// 再更新副
		m_list.SetItemText(list_index, 5, ThreadStateToString(g_info[index+MAX_HWND].thread_state));
		m_list.SetItemText(list_index, 7, g_info[index+MAX_HWND].excep_state);
		m_list.Invalidate();
		m_list.UpdateWindow();
		return;
	}
}

LRESULT CexampleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NOTIFY_UI)
	{
		long list_index = GetListIndex((long)lParam);

		if (list_index >= 0)
		{
			long notify_code = (long)wParam;
			long index = (long)lParam;

			if (index >= MAX_HWND)
			{
				index = index - MAX_HWND;
			}

			switch(notify_code)
			{
			case NOTIFY_UPDATE:
				UpdateUI(index,UI_UPDATE);
				break;
			case NOTIFY_STOP:
				ThreadStop(index);
				break;
			case NOTIFY_RESTART:
				ThreadRestart(index);
				break;
			default:
				break;
			}
		}
		else
		{
			Log(_T("索引找不到 序号:%d"),lParam);
		}
	}
	else if (message == WM_CLOSE)
	{
		OnBnClickedButtonStopall();
		OnBnClickedButtonCloseallwin();
	}

	return CDialog::WindowProc(message,wParam,lParam);
}


void UpdateUI(long index,long action)
{
	g_main_cwnd->UpdateList(index,action);
}

void UpdateCoords(int* nums)
{
	g_main_cwnd->CoordUpdate(nums);
}

int GetautoRuneSolver() {
	return g_main_cwnd->autoRuneSolver.GetCheck();
}

int GetfriendPlayerNotification() {
	return g_main_cwnd->friendPlayerNotification.GetCheck();
}

int GethuangMen() {
	return g_main_cwnd->huangMen.GetCheck();
}

int GetexpPot() {
	return g_main_cwnd->expPot.GetCheck();
}

int GetEXP_PARK() {
	return g_main_cwnd->EXP_PARK.GetCheck();
}

int Gethunt() {
	return g_main_cwnd->hunt.GetCheck();
}

int Getmap() {
	return g_main_cwnd->mapList.GetCurSel();
}

int Getkuxing() {
	return g_main_cwnd->EXP_MVP.GetCheck();
}

int GetautoWealth() {
	return g_main_cwnd->autoWealth.GetCheck();
}

int GetautoOil() {
	return g_main_cwnd->autoOil.GetCheck();
}

int Getignite() {
	return g_main_cwnd->bless.GetCheck();
}

int GetExp10() {
	return g_main_cwnd->EXP15M.GetCheck();
}

int GetExp30() {
	return g_main_cwnd->EXP30M.GetCheck();
}

int GetWhiteDetect() {
	return g_main_cwnd->whiteDetect.GetCheck();
}

void CexampleDlg::OnBnClickedButtonCloseallwin()
{
	
}

void CexampleDlg::OnBnClickedButtonStartall()
{
	CString hwnds = g_dm->EnumWindowByProcess("MapleStory.exe", NULL, "MapleStoryClass", 1 + 8 + 16);
	if (hwnds.GetLength() == 0)
	{
		hwnds = g_dm->EnumWindow(0, "MapleStory", "MapleStoryClass", 1 + 8);
	}
	if (hwnds.GetLength() == 0)
	{
		::MessageBox(NULL, _T("枚举窗口失败，DM未找到"), _T("错误"), MB_OK);
		return;
	}

	//CStringArray hwnd_array;
	//long count = SplitString(hwnds,_T(","),hwnd_array,FALSE);

	//for (int i = 0;i < count;++i)
	//{
	//	long hwnd = _tstoi(hwnd_array[i]);
	//	hwnd = g_dm->GetWindow(hwnd,1);
	//	if (!ThreadStart(hwnd))
	//	{
	//		g_dm->TerminateProcess(g_dm->GetWindowProcessId(hwnd));
	//	}
	//	
	//}
	
	long hwnd = _tstoi(hwnds);
	if (!ThreadStart(hwnd))
	{
		g_dm->TerminateProcess(g_dm->GetWindowProcessId(hwnd));
	}

	subSoftStart();
	detectionStart();
}


void CexampleDlg::OnBnClickedButtonStopall()
{
	long item_count = m_list.GetItemCount();

	if (item_count == 0)
	{
		Log(_T("没有窗口"));
		return;
	}

	subSoftPause();

	// 先全部设置结束标记,这样可加快结束的速度
	for (int i = 0;i < item_count;++i)
	{
		long index = _tstoi(m_list.GetItemText(i,2));
		ThreadSetExitState(index);
	}

	// 这里我们从最后一个开始，因为结束时，会删除表项
	long list_index = item_count - 1;
	while (list_index >= 0)
	{
		long index = _tstoi(m_list.GetItemText(list_index,2));
		ThreadStop(index);
		--list_index;
	}
}
void CexampleDlg::OnBnClickedButtonPauseall()
{
	long item_count = m_list.GetItemCount();

	if (item_count == 0)
	{
		Log(_T("没有窗口"));
		return;
	}


	long list_index = item_count - 1;
	while (list_index >= 0)
	{
		long index = _tstoi(m_list.GetItemText(list_index,2));
		ThreadPause(index);
		--list_index;
	}
}

void CexampleDlg::OnBnClickedButtonResumeall()
{
	long item_count = m_list.GetItemCount();

	if (item_count == 0)
	{
		Log(_T("没有窗口"));
		return;
	}


	long list_index = item_count - 1;
	while (list_index >= 0)
	{
		long index = _tstoi(m_list.GetItemText(list_index,2));
		ThreadResume(index);
		--list_index;
	}
}

void CexampleDlg::OnBnClickedButtonSoft()
{
	Log(_T("软启动/停止"));
	detectWhiteIcon();
	subSoftStartPause();
}

LRESULT CexampleDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	UINT Mod = (UINT)LOWORD(lParam); // key-modifier flags 
	UINT uVirtKey = (UINT)HIWORD(lParam); // virtual-key code 
	//判断响应了什么热键 
	if (MOD_CONTROL == Mod && 76 == uVirtKey)
	{
		CexampleDlg::OnBnClickedButtonSoft();
	}
	else if (MOD_CONTROL == Mod && 80 == uVirtKey)
	{
		if (resumeCount % 2 == 0) {
			Log(_T("暂停所有"));
			CexampleDlg::OnBnClickedButtonPauseall();
		}
		else {
			Log(_T("恢复所有"));
			CexampleDlg::OnBnClickedButtonResumeall();
		}
		resumeCount++;

	}
	//else if (MOD_CONTROL == Mod && 79 == uVirtKey)
	//{
	//	Log(_T("恢复所有"));
	//	CexampleDlg::OnBnClickedButtonResumeall();
	//}
	return 0;
}
