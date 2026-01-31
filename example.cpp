#include "stdafx.h"
#include "example.h"
#include "exampleDlg.h"
#include "log.h"
#include "thread_control.h"
#include "obj.h"
#include "StartupVerify.h"

#include <QtWidgets/QApplication>
#include <QtGui/QFont>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CexampleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CexampleApp::CexampleApp()
{

}

CexampleApp theApp;


// 全局的dm对象
sptool* g_dm = NULL;

/*
 * 写这个例子的原因是看到了很多用户对多线程的处理，处于一知半解的状态
 * 各种奇葩写法层出不穷. 导致的问题也是稀奇古怪
 * 卡死，崩溃简直是家常便饭

 * 为了让用户能更深入的了解多线程的控制和处理,我要在这里写一些原理性的东西
 * 包括插件的设计思想

 * 首先说插件
 * 插件目前是用COM架构设计的,目前的是版本同时支持sta和mta两种线程模型
 * 简单来说,sta就是单线程模型,mta是多线程模型
 * 对于sta来说,所有的com调用(比如你调用了插件的某个接口),都被序列化到某个线程(创建对象的那个线程)内去执行,而不一定在你的调用线程执行
 * 对于mta来说,所有的com调用,都在你的调用线程执行,所以mta的调用效率是最高的.
 * 那么什么时候决定com的线程模型呢? 其实是在对象创建的那个时候决定的
 * 当对象创建时，所在的线程是sta模型,那么这个对象就是sta模型了，无论它在哪个线程里调用,都必定序列化
 * 相反，如果创建时，所在线程是mta模型,那么这个对象就算实在sta模型的线程里调用,它也一定不会被序列化.
 * 所以，我们如果想让某个COM对象以sta模型运行，那么只需要让它在sta模型的线程内创建就好了. 相反mta也如此

 * 对于实际的使用中，我还是建议统统用mta模型。 因为考虑到我们会在线程外去调用接口,为了效率考虑，我们统一使用mta
 * 另外插件设计的所有接口，对于同个对象而言，不是多线程安全的.(也就是同个对象的接口，不能同时在多个线程内调用,会引发崩溃)
 * 在实际使用中，一定要时刻注意这一点. 相反的，如果不是同个对象，那么是多线程安全的.

 * 然后我们说一下系统的多线程
 * 系统的多线程其实就是创建，暂停，恢复，强制结束这4个接口
 * 先告诉大家结论，暂停这个接口,永远不要去调用. 强制结束这个接口要尽可能避免调用.
 * 很多用户为了方便，在暂停某个脚本时，使用了系统提供的暂停接口
 * 结果导致了各种卡死，崩溃。 还不知道咋回事。 感觉明明代码写的没毛病啊
 * 这里我要说一下，系统提供的暂停(SuspendThread)操作,压根就不是给正常用户使用的,这是给调试器使用的
 * 那为何这个暂停操作会导致异常呢？ 实际上,线程在运行当中，它的状态是不可控的
 * 假如你要暂停的线程正好处于某个锁当中(E语言也叫做许可证),万一恰巧这个锁还是个全局的，你说会发生什么事情？
 * 而恰巧的是，系统中使用全局锁的操作很频繁. 比如分配内存/释放内存.
 * 所以我给大家的建议是，永远不要去用暂停这个接口.
 * 至于强制结束线程，也是非常不建议使用，它和暂停线程有相同的作用. 假如结束的线程正好处于锁当中,BOOM!!!
 * 那么今天的例子也就是给大家演示，如何不用这几个要命的接口，如何正确的使用多线程.
 * 至于上面说的暂停，恢复我们有别的方法替代完成.
 * 相信如果你真的理解了这个例子，那么你操作多线程真的会做到游刃有余!
 */

BOOL CexampleApp::InitInstance()
{
	// 初始化COM模型为MTA
	CoInitializeEx(NULL, 0);
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
// 初始化
	LogInit();
	ThreadInit();

	if (!VerifyStartup(nullptr))
	{
		AfxMessageBox(_T("Startup verification failed. Please contact admin."));
		return FALSE;
	}

	// 创建对象,此对象必须全局保持
	g_dm = new sptool;
	if (g_dm == NULL)
	{
		Log(_T("对象创建失败"));
		return FALSE;
	}

	Log(g_dm->Ver());

	// 这里我们设置一下全局资源路径
	g_dm->SetPath(_T("c:\\sptool"));
	g_dm->EnableShareDict(1);
	//g_dm->SetDict(0,_T("1.txt"));

	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	int argc = __argc;
	char** argv = __argv;
	QApplication app(argc, argv);
	app.setFont(QFont(QString::fromWCharArray(L"Microsoft YaHei UI"), 10));

	CexampleDlg mainWindow;
	mainWindow.show();

	app.exec();

	delete g_dm;
	g_dm = NULL;

	return FALSE;
}
