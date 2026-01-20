#include "stdafx.h"
#include "script.h"
#include "thread_control.h"
#include "log.h"
#include "gMonitor.h"
#include "boHandler.h"


void DoWork(long index);
unsigned WINAPI SubThread(PVOID pParam);
unsigned WINAPI MiaoThread(PVOID pParam);

// 我们做暂停和恢复操作,就主要靠这个延时函数,要求脚本所有用到延时的地方，全部用这个，这样我们可以有很多机会去暂停线程
void ScriptDelay(long index,long time)
{
	// 判断是否有暂停和结束标记
	if (g_info[index].is_stop)
	{
		// 直接退出当前线程,退出前一定要在本线程内解绑,因为绑定模式0和2在线程外解绑的话会失败,导致解绑失败.
		if (g_info[index].dm)
		{
			g_info[index].dm->UnBindWindow();
		}
		_endthreadex(0);
	}

	if (g_info[index].is_pause)
	{
		g_info[index].thread_state = State_Pause;
		ThreadNotifyUI_Post(NOTIFY_UPDATE,index);

		// 如果你想要在暂停时让用户可以操作,那么可以调用EnableBind,但是不要去调用LockInput,LockInput不是用来解除后台的,具体参考LockInput的说明
		if (g_info[index].dm)
		{
			
		}

		// 我们暂停的方法是死循环,然后延时,而不是调用系统的接口
		// 这样开销最小,并且效率也还不错
		while (1)
		{
			if (!g_info[index].is_pause)
			{
				g_info[index].thread_state = State_Runing;
				ThreadNotifyUI_Post(NOTIFY_UPDATE,index);
				
				// 开启后台
				if (g_info[index].dm)
				{
					//g_info[index].dm->EnableBind(1);
				}
				break;
			}

			if (g_info[index].is_stop)
			{
				// 直接退出当前线程,退出前一定要在本线程内解绑,因为绑定模式0和2在线程外解绑的话会失败,导致解绑失败.
				if (g_info[index].dm)
				{
					g_info[index].dm->UnBindWindow();
				}
				_endthreadex(0);
			}

			Sleep(1);
		}
	}

	// 可能暂停,恢复时会让状态错乱,这里再判断一次
	if (g_info[index].thread_state != State_Runing)
	{
		g_info[index].thread_state = State_Runing;
		ThreadNotifyUI_Post(NOTIFY_UPDATE,index);

		// 开启后台
		if (g_info[index].dm)
		{
			g_info[index].dm->EnableBind(1);
		}
	}

	Sleep(time);
}

void SetTaskState(long index,const TCHAR * state)
{
	_tcscpy(g_info[index].task_state,state);
	ThreadNotifyUI_Post(NOTIFY_UPDATE,index);
}

// 脚本主线程,脚本的主要逻辑在这里处理
unsigned WINAPI MainThread(PVOID pParam)
{
	long index = (long)(DWORD_PTR)pParam;
	sptool* dm;

	// 初始化当前线程com组件为MTA模式
	CoInitializeEx (NULL,0);

	g_info[index].thread_state = State_Runing;
	ThreadNotifyUI_Post(NOTIFY_UPDATE,index);

	// 创建对象
	g_info[index].dm = new sptool;
	dm = g_info[index].dm;

	// 检测对象是否创建成功,虽然这个一般不会失败,但为了程序健壮性考虑还是加上,如果内存吃紧，还是可能会失败
	if (dm == NULL || dm->Ver().GetLength() == 0)
	{
		Log(_T("对象创建失败"));
		ThreadNotifyUI_Post(NOTIFY_STOP,index);
		return 0;
	}

	// 开启全局共享字库
	dm->EnableShareDict(1);

	// 其他设置,比如路径等等
	dm->SetPath(_T("c:\\sptool"));
	dm->EnableRealMouse(2, 6, 62);
	dm->SetShowErrorMsg(0);

	// 强制解绑
	// dm->ForceUnBindWindow(dm->IsBind(g_info[index].hwnd));

	// 开始绑定,主绑定一定要第一个绑定,并且主绑定所在的线程绝对不能结束,否则会造成绑定失效
	dm->SetWindowState(g_info[index].hwnd, 12);
	ScriptDelay(index, 50);
	//long dm_ret = dm->BindWindowEx(g_info[index].hwnd, _T("dx.graphic.3d.10plus"), _T("dx.mouse.position.lock.api|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor"),
	//	_T("dx.keypad.state.api|dx.keypad.api"), _T("dx.public.active.api|dx.public.active.message"), 0);
	long dm_ret = dm->BindWindowEx(g_info[index].hwnd, _T("normal"), _T("normal"), _T("normal"), _T(""), 0);

	if (dm_ret != 1)
	{
		Log(_T("主:绑定失败，错误码:%d"),dm->GetLastError());
		// 通知主线程进行结束操作(释放资源)
		ThreadNotifyUI_Post(NOTIFY_STOP,index);
		return 0;
	}

	// 禁止输入
	/*dm->LockInput(4);*/

	// 我们可以创建副线程了
	// 更新副线程信息
	g_info[index+MAX_HWND].thread_state = State_Starting;

	// 通知UI,副线程开始创建了
	ThreadNotifyUI_Post(NOTIFY_UPDATE,index+MAX_HWND);

	// 创建副线程
	g_info[index+MAX_HWND].handle = (HANDLE)_beginthreadex(0, 0, SubThread, (PVOID)(DWORD_PTR)(index+MAX_HWND), 0, 0);
	if (g_info[index+MAX_HWND].handle == NULL)
	{
		// 这里必须要解绑,因为模式0和2不在当前线程解绑的话,会导致无法解绑
		dm->UnBindWindow();

		Log(_T("创建副线程失败"));
		// 通知主线程进行结束操作(释放资源)
		ThreadNotifyUI_Post(NOTIFY_STOP,index);
		return 0;
	}

	// 更新喵码线程信息
	g_info[index + MAX_HWND * 2].thread_state = State_Starting;

	// 通知UI,副线程开始创建了
	ThreadNotifyUI_Post(NOTIFY_UPDATE, index + MAX_HWND);

	// 创建喵码线程
	g_info[index + MAX_HWND * 2].handle = (HANDLE)_beginthreadex(0, 0, MiaoThread, (PVOID)(DWORD_PTR)(index + MAX_HWND * 2), 0, 0);
	if (g_info[index + MAX_HWND * 2].handle == NULL)
	{
		// 这里必须要解绑,因为模式0和2不在当前线程解绑的话,会导致无法解绑
		dm->UnBindWindow();

		Log(_T("创建副线程失败"));
		// 通知主线程进行结束操作(释放资源)
		ThreadNotifyUI_Post(NOTIFY_STOP, index);
		return 0;
	}


	startBo(index);
	
	/* 
	while (1)
	{
		SetTaskState(index,_T("开始"));
		//DoWork(index);
		startBot(index);
		SetTaskState(index,_T("完成"));
		ScriptDelay(index,1000);
	} 
	*/
}

void DoWork(long index)
{
	//sptool* dm = g_info[index].dm;


	//dm->KeyPressChar(_T("alt"));
	//dm->KeyPressChar("s");
	//goTo(index, 82, 118, 2);
	ScriptDelay(index, 1000);
	//goTo(index, 166, 126, 2);
	ScriptDelay(index, 5000);

}

void SetExcepState(long index,const TCHAR * state)
{
	_tcscpy(g_info[index].excep_state,state);
	ThreadNotifyUI_Post(NOTIFY_STOP,index);
}

// 脚本副线程,用于检测异常等
unsigned WINAPI SubThread(PVOID pParam)
{
	long index = (long)(DWORD_PTR)pParam;

	sptool* dm;

	// 初始化当前线程com组件为MTA模式
	CoInitializeEx (NULL,0);

	g_info[index].thread_state = State_Runing;
	ThreadNotifyUI_Post(NOTIFY_UPDATE,index);

	// 创建对象
	g_info[index].dm = new sptool;
	dm = g_info[index].dm;

	// 检测对象是否创建成功,虽然这个一般不会失败,但为了程序健壮性考虑还是加上,如果内存吃紧，还是可能会失败
	if (dm == NULL || dm->Ver().GetLength() == 0)
	{
		Log(_T("对象创建失败"));
		SetExcepState(index,_T("对象创建失败"));
		return 0;
	}

	// 开启全局共享字库
	dm->EnableShareDict(1);

	// 其他设置,比如路径等等
	dm->SetPath(_T("c:\\sptool"));
	dm->SetShowErrorMsg(0);

	dm->SetWindowState(g_info[index].hwnd, 12);
	//long dm_ret = dm->BindWindowEx(g_info[index].hwnd, _T("dx.graphic.3d.10plus"), _T("dx.mouse.position.lock.api|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor"),
	//	_T("dx.keypad.state.api|dx.keypad.api"), _T("dx.public.active.api|dx.public.active.message"), 0);
	long dm_ret = dm->BindWindowEx(g_info[index].hwnd, _T("normal"), _T("normal"), _T("normal"), _T(""), 0);
	//64位
	//long dm_ret = dm->BindWindowEx(g_info[index].hwnd, _T("dx2"), _T("dx.mouse.position.lock.api|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor"),
	//	_T("dx.keypad.input.lock.api|dx.keypad.state.api|dx.keypad.api"), _T("dx.public.active.api|dx.public.active.message"), 11);

	if (dm_ret != 1)
	{
		Log(_T("副:绑定失败，错误码:%d"),dm->GetLastError());
		// 通知主线程进行结束操作(释放资源)
		SetExcepState(index,_T("副:绑定失败"));
		return 0;
	}

	dm->EnableRealMouse(2, 6, 62);

	long count = 0;
	while (1)
	{
		// 检测一些异常,比如突然弹出的对话框，目标窗口被关闭或者掉线等突发情况
		// 比如检测到掉线，可考虑通知UI,然后重新运行
		gMonitorCheck(index, count);
		ScriptDelay(index,10);
		count++;
		if (count > 20) { count = 0; }
	}
}

unsigned WINAPI MiaoThread(PVOID pParam) {
	long index = (long)(DWORD_PTR)pParam;

	sptool* dm;

	// 初始化当前线程com组件为MTA模式
	CoInitializeEx(NULL, 0);

	g_info[index].thread_state = State_Runing;
	ThreadNotifyUI_Post(NOTIFY_UPDATE, index);

	// 创建对象
	g_info[index].dm = new sptool;
	dm = g_info[index].dm;

	// 检测对象是否创建成功,虽然这个一般不会失败,但为了程序健壮性考虑还是加上,如果内存吃紧，还是可能会失败
	if (dm == NULL || dm->Ver().GetLength() == 0)
	{
		Log(_T("对象创建失败"));
		SetExcepState(index, _T("对象创建失败"));
		return 0;
	}

	startMiao(index);

}