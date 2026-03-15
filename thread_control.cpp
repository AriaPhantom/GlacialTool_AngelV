#include "stdafx.h"
#include "thread_control.h"
#include "log.h"
#include "script.h"
#include "AutoLogin.h"
#include "SPUtils.h"

extern sptool* g_dm;
extern HWND   g_main_hwnd;
extern void UpdateUI(long index,long action);

ThreadInfo g_info[MAX_HWND * 3];
static HANDLE g_threadStateEvents[MAX_HWND * 3] = { 0 };

static HANDLE GetThreadStateEvent(long index)
{
	if (index < 0 || index >= MAX_HWND * 3)
	{
		return NULL;
	}

	if (g_threadStateEvents[index] == NULL)
	{
		g_threadStateEvents[index] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	return g_threadStateEvents[index];
}


static void ThreadReset(long index)
{
	if (index < 0 || index >= MAX_HWND * 3)
	{
		return;
	}
	ZeroMemory(&g_info[index],sizeof(ThreadInfo));
	g_info[index].thread_state = State_Inactive;
	_tcscpy(g_info[index].task_state ,_T(""));
	_tcscpy(g_info[index].excep_state,_T("无异常"));
}

static long ThreadGetPos()
{
	for (int i = 0;i < MAX_HWND;++i)
	{
		if (g_info[i].handle == NULL)
		{
			return i;
		}
	}

	return -1;
}

static BOOL ThreadIsStart(long hwnd)
{
	for (int i = 0;i < MAX_HWND;++i)
	{
		if (g_info[i].hwnd == hwnd)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void   ThreadInit()
{
	for (int i = 0;i < MAX_HWND * 3;++i)
	{
		ThreadReset(i);
		HANDLE ev = GetThreadStateEvent(i);
		if (ev) ResetEvent(ev);
	}
}


BOOL   ThreadStart(long hwnd)
{
	if (hwnd == 0)
	{
		Log(_T("无效的窗口句柄"));
		return FALSE;
	}

	if (ThreadIsStart(hwnd))
	{
		Log(_T("已经启动了"));
		return TRUE;
	}

	long index = ThreadGetPos();
	if (index == -1)
	{
		Log(_T("没有多余位置了"));
		return FALSE;
	}

	// 更新线程信息,注意这里创建对象的操作不能在这里做
	// 之前提到了,对象的线程模型等于创建时所在线程的线程模型
	// 由于UI线程不一定是MTA模型,所以我们把创建对象的操作放到线程内部去
	g_info[index].handle = NULL;
	g_info[index].hwnd = hwnd;
	g_info[index].pid = g_dm->GetWindowProcessId(hwnd);
	g_info[index].is_pause = FALSE;
	g_info[index].is_stop = FALSE;
	g_info[index].thread_state = State_Starting;

	g_info[index+MAX_HWND].handle = NULL;
	g_info[index+MAX_HWND].hwnd = hwnd;
	g_info[index+MAX_HWND].pid = g_dm->GetWindowProcessId(hwnd);
	g_info[index+MAX_HWND].is_pause = FALSE;
	g_info[index+MAX_HWND].is_stop = FALSE;
	g_info[index+MAX_HWND].thread_state = State_Inactive;

	g_info[index + MAX_HWND * 2].handle = NULL;
	g_info[index + MAX_HWND * 2].hwnd = hwnd;
	g_info[index + MAX_HWND * 2].pid = g_dm->GetWindowProcessId(hwnd);
	g_info[index + MAX_HWND * 2].is_pause = FALSE;
	g_info[index + MAX_HWND * 2].is_stop = FALSE;
	g_info[index + MAX_HWND * 2].thread_state = State_Inactive;

	// 通知UI更新
	UpdateUI(index,UI_ADD);

	// 创建线程,这里我们只创建主线程,副线程交给主线程去创建
	g_info[index].handle = (HANDLE)_beginthreadex(0, 0, MainThread, (PVOID)(DWORD_PTR)index, 0, 0);
	if (g_info[index].handle == NULL)
	{
		UpdateUI(index,UI_DELETE);
		ThreadReset(index);
		ThreadReset(index + MAX_HWND);
		ThreadReset(index + MAX_HWND * 2);
		Log(_T("创建线程失败"));
		return FALSE;
	}

	Log(_T("启动 hwnd = %d,主序号:%d"),hwnd,index);
	return TRUE;
}

BOOL ThreadStartPendingAutoLogin()
{
	long index = ThreadGetPos();
	if (index == -1)
	{
		Log(_T("No free slot for cold-start auto login"));
		return FALSE;
	}

	g_info[index].handle = NULL;
	g_info[index].hwnd = 0;
	g_info[index].pid = 0;
	g_info[index].dm = NULL;
	g_info[index].is_pause = FALSE;
	g_info[index].is_stop = FALSE;
	g_info[index].thread_state = State_Starting;

	g_info[index+MAX_HWND].handle = NULL;
	g_info[index+MAX_HWND].hwnd = 0;
	g_info[index+MAX_HWND].pid = 0;
	g_info[index+MAX_HWND].dm = NULL;
	g_info[index+MAX_HWND].is_pause = FALSE;
	g_info[index+MAX_HWND].is_stop = FALSE;
	g_info[index+MAX_HWND].thread_state = State_Inactive;

	g_info[index + MAX_HWND * 2].handle = NULL;
	g_info[index + MAX_HWND * 2].hwnd = 0;
	g_info[index + MAX_HWND * 2].pid = 0;
	g_info[index + MAX_HWND * 2].dm = NULL;
	g_info[index + MAX_HWND * 2].is_pause = FALSE;
	g_info[index + MAX_HWND * 2].is_stop = FALSE;
	g_info[index + MAX_HWND * 2].thread_state = State_Inactive;

	UpdateUI(index,UI_ADD);

	g_info[index].handle = (HANDLE)_beginthreadex(0, 0, MainThread, (PVOID)(DWORD_PTR)index, 0, 0);
	if (g_info[index].handle == NULL)
	{
		UpdateUI(index,UI_DELETE);
		ThreadReset(index);
		ThreadReset(index + MAX_HWND);
		ThreadReset(index + MAX_HWND * 2);
		Log(_T("Create cold-start auto-login thread failed"));
		return FALSE;
	}

	Log(_T("Start cold-start auto-login flow, main index:%d"),index);
	return TRUE;
}

void ThreadPause(long index)
{
	if (index < 0 || index >= MAX_HWND)
	{
		return;
	}

	if (g_info[index].handle == NULL)
	{
		return;
	}

	// 主
	if (g_info[index].thread_state == State_Runing || g_info[index].thread_state == State_Resuming)
	{
		g_info[index].is_pause = TRUE;
		g_info[index].thread_state = State_Pausing;
	}

	// 副
	if (g_info[index+MAX_HWND].thread_state == State_Runing || g_info[index+MAX_HWND].thread_state == State_Resuming)
	{
		g_info[index+MAX_HWND].is_pause = TRUE;
		g_info[index+MAX_HWND].thread_state = State_Pausing;
	}

	// Mark pause first to minimize the race window where a script can still inject one more key.
	SPUtils::ReleaseAllKeysFastKeyboardOnlySkipEnter();

	Log(_T("暂停 主序号:%d"),index);
	ThreadSignalStateChanged(index);
	ThreadSignalStateChanged(index + MAX_HWND);

	UpdateUI(index,UI_UPDATE);
}

void ThreadResume(long index)
{
	if (index < 0 || index >= MAX_HWND)
	{
		return;
	}

	if (g_info[index].handle == NULL)
	{
		return;
	}


	// 主
	if (g_info[index].thread_state == State_Pause || g_info[index].thread_state == State_Pausing)
	{
		g_info[index].is_pause = FALSE;
		g_info[index].thread_state = State_Resuming;
	}

	// 副
	if (g_info[index+MAX_HWND].thread_state == State_Pause || g_info[index+MAX_HWND].thread_state == State_Pausing)
	{
		g_info[index+MAX_HWND].is_pause = FALSE;
		g_info[index+MAX_HWND].thread_state = State_Resuming;
	}

	Log(_T("恢复 主序号:%d"),index);
	ThreadSignalStateChanged(index);
	ThreadSignalStateChanged(index + MAX_HWND);

	UpdateUI(index,UI_UPDATE);
}

void ThreadSetExitState(long index)
{
	if (index < 0 || index >= MAX_HWND)
	{
		return;
	}

	if (g_info[index].handle == NULL)
	{
		return;
	}


	// 主
	g_info[index].is_stop = TRUE;
	g_info[index].thread_state = State_Stoping;
	AutoLogin_StopDisconnectWatcher(index);

	// 副
	g_info[index+MAX_HWND].is_stop = TRUE;
	g_info[index+MAX_HWND].thread_state = State_Stoping;

	// 喵
	g_info[index + MAX_HWND * 2].is_stop = TRUE;
	g_info[index + MAX_HWND * 2].thread_state = State_Stoping;

	// 插件同样也设置结束标记
	if (g_info[index].dm)
	{
		g_info[index].dm->SetExitThread(1);
	}

	if (g_info[index+MAX_HWND].dm)
	{
		g_info[index+MAX_HWND].dm->SetExitThread(1);
	}

	if (g_info[index + MAX_HWND * 2].dm)
	{
		g_info[index + MAX_HWND * 2].dm->SetExitThread(1);
	}
	ThreadSignalStateChanged(index);
	ThreadSignalStateChanged(index + MAX_HWND);
	ThreadSignalStateChanged(index + MAX_HWND * 2);

	
	UpdateUI(index,UI_UPDATE);
}

// 这个接口不给外面调用
static void ThreadInternalStop(long index)
{
	Log(_T("结束 主序号:%d"),index);

	ThreadSetExitState(index);

	DWORD wait_time = 30 * 1000;

	// 主
	if (g_info[index].handle)
	{
		if (WaitForSingleObject(g_info[index].handle,wait_time) != 0)
		{
			// 等待线程自然结束失败了,逼不得已我们使用强制结束线程
			Log(_T("主:等待线程失败,强制结束线程"));
			TerminateThread(g_info[index].handle,0);
		}
		CloseHandle(g_info[index].handle);
	}

	// 副
	if (g_info[index+MAX_HWND].handle)
	{
		if (WaitForSingleObject(g_info[index+MAX_HWND].handle,wait_time) != 0)
		{
			// 等待线程自然结束失败了,逼不得已我们使用强制结束线程
			Log(_T("副:等待线程失败,强制结束线程"));
			TerminateThread(g_info[index+MAX_HWND].handle,0);
		}
		CloseHandle(g_info[index+MAX_HWND].handle);
	}

	// 喵码
	if (g_info[index + MAX_HWND * 2].handle)
	{
		if (WaitForSingleObject(g_info[index + MAX_HWND * 2].handle, wait_time) != 0)
		{
			// 等待线程自然结束失败了,逼不得已我们使用强制结束线程
			Log(_T("副:等待线程失败,强制结束线程"));
			TerminateThread(g_info[index + MAX_HWND * 2].handle, 0);
		}
		CloseHandle(g_info[index + MAX_HWND * 2].handle);
	}


	// 释放对象
	// 由于我们使用了SetExitThread，另外也可能调用了强制结束线程,还有我们在脚本内部有对象复制的操作
	// 这都会导致对象内部引用计数错误,导致对象无法被释放掉
	// 所以我们在释放对象前,强制把引用计数释放到正确的值
	if (g_info[index].dm)
	{
		g_info[index].dm->ReleaseRef();
		delete g_info[index].dm;
		g_info[index].dm = NULL;
	}

	if (g_info[index+MAX_HWND].dm)
	{
		g_info[index+MAX_HWND].dm->ReleaseRef();
		delete g_info[index+MAX_HWND].dm;
		g_info[index+MAX_HWND].dm = NULL;
	}

	if (g_info[index + MAX_HWND * 2].dm)
	{
		g_info[index + MAX_HWND * 2].dm->ReleaseRef();
		delete g_info[index + MAX_HWND * 2].dm;
		g_info[index + MAX_HWND * 2].dm = NULL;
	}
}


void   ThreadStop(long index)
{
	if (index < 0 || index >= MAX_HWND)
	{
		return;
	}

	if (g_info[index].handle == NULL)
	{
		return;
	}

	

ThreadInternalStop(index);

	ThreadReset(index);
	ThreadReset(index + MAX_HWND);
	ThreadReset(index + MAX_HWND * 2);

	UpdateUI(index,UI_DELETE);

	Log(_T("结束成功 主序号:%d"),index);
}


BOOL   ThreadRestart(long index)
{
	if (index < 0 || index >= MAX_HWND)
	{
		return FALSE;
	}

	if (g_info[index].handle == NULL)
	{
		return FALSE;
	}

	// 先结束
	ThreadInternalStop(index);

	AutoLogin_StopDisconnectWatcher(index);
	g_info[index].handle = NULL;
	g_info[index].is_pause = FALSE;
	g_info[index].is_stop = FALSE;
	g_info[index].thread_state = State_Starting;

	g_info[index+MAX_HWND].handle = NULL;
	g_info[index+MAX_HWND].is_pause = FALSE;
	g_info[index+MAX_HWND].is_stop = FALSE;
	g_info[index+MAX_HWND].thread_state = State_Inactive;

	UpdateUI(index,UI_UPDATE);

	// 创建线程,这里我们只创建主线程,副线程交给主线程去创建
	g_info[index].handle = (HANDLE)_beginthreadex(0, 0, MainThread, (PVOID)(DWORD_PTR)index, 0, 0);
	if (g_info[index].handle == NULL)
	{
		UpdateUI(index,UI_DELETE);
		ThreadReset(index);
		ThreadReset(index+MAX_HWND);
		Log(_T("创建线程失败"));
		return FALSE;
	}
	
	Log(_T("重新运行 hwnd = %d,主序号:%d"),g_info[index].hwnd,index);
	return TRUE;
}

void  ThreadNotifyUI_Post(long notify_code,long index)
{
	PostMessage(g_main_hwnd,WM_NOTIFY_UI,(WPARAM)notify_code,(LPARAM)index);
}


void  ThreadNotifyUI_Send(long notify_code,long index)
{
	SendMessage(g_main_hwnd,WM_NOTIFY_UI,(WPARAM)notify_code,(LPARAM)index);
}
void ThreadSignalStateChanged(long index)
{
	HANDLE ev = GetThreadStateEvent(index);
	if (ev)
	{
		SetEvent(ev);
	}
}

bool ThreadWaitStateSignal(long index, DWORD timeout_ms)
{
	HANDLE ev = GetThreadStateEvent(index);
	if (ev == NULL)
	{
		if (timeout_ms != INFINITE && timeout_ms > 0)
		{
			Sleep(timeout_ms);
		}
		return false;
	}

	DWORD waitRet = WaitForSingleObject(ev, timeout_ms);
	return waitRet == WAIT_OBJECT_0;
}