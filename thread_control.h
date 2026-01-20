#ifndef __INCLUDE_THREAD_H__
#define __INCLUDE_THREAD_H__

#include "obj.h"

// 最大支持的窗口个数
#define  MAX_HWND        2

// 通知UI的消息
#define WM_NOTIFY_UI      (WM_USER + 0x555)
// 通知类型
#define NOTIFY_UPDATE    0
#define NOTIFY_STOP      1
#define NOTIFY_RESTART   2

// 通知UI的类型
#define UI_ADD             0
#define UI_DELETE          1
#define UI_UPDATE          2

// 线程状态
enum ThreadState
{
	State_Inactive = -1,           // 未启动
	State_Starting = 0,            // 正在启动
	State_Runing,                  // 正在运行
	State_Pausing,                 // 正在暂停
	State_Pause,                   // 暂停
	State_Resuming,                // 正在恢复
	State_Stoping,                 // 正在停止
};
// 多线程数据
struct ThreadInfo
{
	HANDLE   handle;          // 线程句柄
	long     hwnd;            // 窗口句柄
	long     pid;             // 进程pid
	sptool *dm;
	BOOL     is_pause;        // 暂停标记
	BOOL     is_stop;         // 结束标记
	ThreadState thread_state; // 线程状态
	                          /*
							   * 记录当前线程的状态,这里为何要用整数，而不直接用文本来标识线程状态，是因为这个结构体中的数值都是可能被多个线程同时访问的，
							   * 如果使用了字符串类型，那么在赋值和访问的瞬间，可能会有数据冲突，因为字符串类型的数据内部会涉及到内存重新分配.
							   */
	TCHAR  task_state[100];    // 任务状态
	                          /*
							   * 那么与上面的对应的，这里我们采用了字符串类型，为了避免在多个线程同时访问此数据造成内存访问异常，我们这里固定使用100个字节
							   */
	TCHAR  excep_state[100];   // 异常状态
	                          /*
							   * 那么与上面的对应的，这里我们采用了字符串类型，为了避免在多个线程同时访问此数据造成内存访问异常，我们这里固定使用100个字节
							   */
};


extern ThreadInfo g_info[MAX_HWND * 3];

// 多线程控制接口
void   ThreadInit();

// 启动
BOOL   ThreadStart(long hwnd);

// 暂停
void   ThreadPause(long index);

// 恢复
void   ThreadResume(long index);

// 设置线程标记(用在全部停止)
void   ThreadSetExitState(long index);

// 结束
void   ThreadStop(long index);

// 重新运行
BOOL   ThreadRestart(long index);

// 异步通知UI
// 这个接口是唯一的一个多线程安全的接口,一般用来脚本线程给UI通知信息，这个是异步接口，调用后立刻返回,不等待UI处理完毕
void   ThreadNotifyUI_Post(long notify_code,long index);

// 同步通知UI
// 这个接口是唯一的一个多线程安全的接口,一般用来脚本线程给UI通知信息，这个是同步接口，调用后一直等待,直到UI处理完毕
void   ThreadNotifyUI_Send(long notify_code,long index);

#endif