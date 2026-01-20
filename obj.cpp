#include "stdafx.h"
#include "atlbase.h"
#include "obj.h"

class MyDispatchDriver
{
public:
    IDispatch* p;
    MyDispatchDriver()
    {
        p = NULL;
    }
    MyDispatchDriver(IDispatch* lp)
    {
        if ((p = lp) != NULL)
            p->AddRef();
    }
    ~MyDispatchDriver() { if (p) p->Release(); }
    HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
    {
        HRESULT hr = -1;
        if (p == NULL) return hr;
        return p->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
    }
    HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL)
    {
        HRESULT hr = -1;
        DISPPARAMS dispparams = { NULL, NULL, 0, 0};
        if (p == NULL) return hr;
        return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
    {
        HRESULT hr = -1;
        DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
        if (p == NULL) return hr;
        return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
    }
};

sptool::sptool()
{
    CLSID clsid;
    IUnknown * pUnknown = NULL;
    HRESULT hr;

    obj = NULL;
    hr = ::CLSIDFromProgID( L"sp.sptool", &clsid );
    if (FAILED(hr))
    {
        return;
    }

    hr = ::CoCreateInstance( clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&pUnknown);
    if (FAILED(hr))
    {
        return;
    }

    pUnknown->QueryInterface(IID_IDispatch, (void **)&obj);
    if (pUnknown) pUnknown->Release();
}

sptool::~sptool()
{
        if (obj) obj->Release();
}

CString sptool::GetWordResultStr(const TCHAR * str,long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(str);
    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ejTb",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarUnlock(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HmeqvGhJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DeleteIniPwd(const TCHAR * section,const TCHAR * key,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FSQBffgZfmVgik",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetClipboard()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FbzBT",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetWordLineHeight(long line_height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(line_height);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FKcuVj",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindDataEx(long hwnd,const TCHAR * addr_range,const TCHAR * data,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(addr_range);
    pn[3] = COleVariant(data);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"zSskKDTBKDvdl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::WriteDoubleAddr(long hwnd,LONGLONG addr,double v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rtPciwLgJW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableKeypadPatch(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XwqYhMUCz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetAero(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LnhPZbTZbeGAK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindInputMethod(const TCHAR * id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kKPjJh",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetShowAsmErrorMsg(long show)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(show);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"abAeAofixUe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetWordResultCount(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"UBBpgqLSRzHK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DmGuardExtract(const TCHAR * tpe,const TCHAR * path)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(path);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"EcLFrUnSGVNZoh",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetWordResultPos(const TCHAR * str,long index,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(index);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YVwrVWivJKkGtIs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetLocale()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ADgg",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableFontSmooth()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ruubIq",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ClearDict(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"QjWrEtAXtkC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetExactOcr(long exact_ocr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(exact_ocr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"QHekYPSqyt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableMouseSync(long en,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xpUHz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteFloat(long hwnd,const TCHAR * addr,float v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fJbUXR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DeleteFolder(const TCHAR * folder_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(folder_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"zpuPYDEa",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetExitThread(long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HGLsoCn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::KeyUp(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BhhWFVjVlbN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindWindow(const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WxAD",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetColorBGR(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rBpUyvgohy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::Capture(long x1,long y1,long x2,long y2,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FTPzTTwW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::MoveR(long rx,long ry)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(rx);
    pn[0] = COleVariant(ry);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xGvhW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetColor(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BkEEyVyxRVGwPhR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetColorNum(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HgoBVrlVUo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG sptool::VirtualAllocEx(long hwnd,LONGLONG addr,long size,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"KGpTXMeBURLWyta",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long sptool::SendString2(long hwnd,const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AbMPFSgqz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LockDisplay(long locks)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(locks);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"wYjXbNk",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetDisplayRefreshDelay(long t)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(t);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"oioUnnZA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindStrE(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"uKZZsLFZiEmNhM",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetNetTimeByIp(const TCHAR * ip)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(ip);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"hpuZfri",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::OpenProcess(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ehYacvkT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AsmAdd(const TCHAR * asm_ins)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(asm_ins);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"Jseg",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::OcrEx(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kpiaylxkavfbkrY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::CopyFile(const TCHAR * src_file,const TCHAR * dst_file,long over)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(src_file);
    pn[1] = COleVariant(dst_file);
    pn[0] = COleVariant(over);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LUqeJyIpb",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicExS(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CAsFapssU",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindDouble(long hwnd,const TCHAR * addr_range,double double_value_min,double double_value_max)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(double_value_min);
    pn[0] = COleVariant(double_value_max);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"PLmywTjjsfDENy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindStr(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MKEpUsrT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::MiddleClick()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"oAHxDVFZjI",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetRowGapNoDict(long row_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(row_gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MAQhPTNemAhuK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindMulColor(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fXKwERGd",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteFloatAddr(long hwnd,LONGLONG addr,float v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ijmfHEquoFgwSs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LeftDoubleClick()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IgNzffzQ",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LoadPicByte(long addr,long size,const TCHAR * name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XWDPWcMXWGAl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CmpColor(long x,long y,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yPUg",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG sptool::GetModuleBaseAddr(long hwnd,const TCHAR * module_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(module_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"cpdmkzwNWtUBTn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long sptool::GetClientRect(long hwnd,long * x1,long * y1,long * x2,long * y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;
    CComVariant t0,t1,t2,t3;

    VariantInit(&t0);
    VariantInit(&t1);
    VariantInit(&t2);
    VariantInit(&t3);

    pn[4] = COleVariant(hwnd);
    pn[3].vt = VT_BYREF|VT_VARIANT;
    pn[3].pvarVal = &t0;
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t1;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t2;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t3;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rvSfCmC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        *x1 = t0.lVal;
        *y1 = t1.lVal;
        *x2 = t2.lVal;
        *y2 = t3.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicSimEx(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,long sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"jXuqqVlixPefKgf",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::ReadFile(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"DeyjxKoraQdgyl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SendString(long hwnd,const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"djJg",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindMultiColor(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(first_color);
    pn[4] = COleVariant(offset_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WTEdnQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumIniKey(const TCHAR * section,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(section);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FyBdweaKaGlC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::DisableFontSmooth()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CCQyIihg",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LeaveCri()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YZdMTqz",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetMinRowGap(long row_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(row_gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"hFwtyCKmNCq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarSetFont(long hwnd,const TCHAR * font_name,long size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yuZMRmHu",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarTextPrintDir(long hwnd,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AkmvtDhLMu",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetFindPicMultithreadCount(long count)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(count);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LbYvtESPnS",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarSetTrans(long hwnd,long trans,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(trans);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ijjCdxf",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CapturePre(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"jkSRbSj",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableIme(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NESrKmRS",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumProcess(const TCHAR * name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YSoEaXdoX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FaqIsPosted()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nPRUclNiebqoxM",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::BindWindow(long hwnd,const TCHAR * display,const TCHAR * mouse,const TCHAR * keypad,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(display);
    pn[2] = COleVariant(mouse);
    pn[1] = COleVariant(keypad);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VmBNDG",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::IsBind(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JGUxorEI",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetLocale()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"aAljX",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::ExcludePos(const TCHAR * all_pos,long tpe,long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(all_pos);
    pn[4] = COleVariant(tpe);
    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JBFuDSlHH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindInt(long hwnd,const TCHAR * addr_range,LONGLONG int_value_min,LONGLONG int_value_max,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(addr_range);
    pn[2] = COleVariant(int_value_min);
    pn[1] = COleVariant(int_value_max);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"wDxgegubLq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::UseDict(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dlysDdc",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG sptool::ReadInt(long hwnd,const TCHAR * addr,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CIAyJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long sptool::EnableGetColorByCapture(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nsCvhMopFIZeMhs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindWindowByProcessId(long process_id,const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(process_id);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ERkKAHVkxmFH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteDataFromBin(long hwnd,const TCHAR * addr,long data,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(data);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"axUaDjBXK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ReleaseRef()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pJJFulmFbJb",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DisableScreenSave()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YDpcjnZtPe",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetCpuUsage()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dJIxUCDZdqY",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CheckInputMethod(long hwnd,const TCHAR * id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"zIzgGRVqR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetRealPath(const TCHAR * path)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(path);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"apCB",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::Stop(long id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AUpNjnvqefBSJdQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteIni(const TCHAR * section,const TCHAR * key,const TCHAR * v,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(v);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"GopSwbGLxD",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindIntEx(long hwnd,const TCHAR * addr_range,LONGLONG int_value_min,LONGLONG int_value_max,long tpe,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(addr_range);
    pn[5] = COleVariant(int_value_min);
    pn[4] = COleVariant(int_value_max);
    pn[3] = COleVariant(tpe);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"gzNpWGpeLNJrVka",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::RGB2BGR(const TCHAR * rgb_color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(rgb_color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"RShPN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SendStringIme(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fYJJPs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindFloat(long hwnd,const TCHAR * addr_range,float float_value_min,float float_value_max)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(float_value_min);
    pn[0] = COleVariant(float_value_max);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"giItwIQhtNMFDI",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetClientSize(long hwnd,long * width,long * height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"iMaheIrwSHdhuq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *width = t0.lVal;
        *height = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindStrFastExS(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"QKIks",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetCursorSpot()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"luBpvTEUlbjeWm",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindStrFastE(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XQiMlQTXCpEZUn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::VirtualFreeEx(long hwnd,LONGLONG addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rLzP",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetShowErrorMsg(long show)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(show);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"uSgZRBtZ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SendStringIme2(long hwnd,const TCHAR * str,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(str);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ZUNrdiaGAn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindData(long hwnd,const TCHAR * addr_range,const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr_range);
    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rEEqGdGzhbYWvtR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::RightUp()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WEcLXYwtcG",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LoadAiMemory(long addr,long size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"DQHbtv",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LoadPic(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nUsW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CaptureJpg(long x1,long y1,long x2,long y2,const TCHAR * file_name,long quality)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(quality);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"gBhqY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::UnLoadDriver()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nPLJbjIitjFVsA",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetWindowText(long hwnd,const TCHAR * text)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(text);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xrtzuWwlClxy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetScreenWidth()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ksGaeDflRnLarY",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SwitchBindWindow(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AaFMIX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DisablePowerSave()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pUXS",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::MoveDD(long dx,long dy)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(dx);
    pn[0] = COleVariant(dy);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TEdSDXXZQH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LeftDown()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"sTRPtn",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableDisplayDebug(long enable_debug)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(enable_debug);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ywNkUjscEuXsJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::RightDown()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"aFphNMmiT",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetWindowState(long hwnd,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xWmMKIZwviG",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteDataAddr(long hwnd,LONGLONG addr,const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"Cjtz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloSetModel(long index,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(index);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"Mhfah",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::KeyPress(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"saxJbCyuYxk",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetDictPwd(const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MAiJxlBY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CheckFontSmooth()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LxCgeJW",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::Play(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BMGKkdBte",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DownCpu(long tpe,long rate)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(rate);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rsvaWa",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::AiYoloObjectsToString(const TCHAR * objects)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(objects);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rlpzN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FaqSend(const TCHAR * server,long handle,long request_type,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(server);
    pn[2] = COleVariant(handle);
    pn[1] = COleVariant(request_type);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JMuo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::Delay(long mis)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mis);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"zjPVlWFJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetMemoryUsage()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kiGCIl",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FaqGetSize(long handle)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(handle);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IwvZq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::RunApp(const TCHAR * path,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(path);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ucuuHmQJfN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::MoveToEx(long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TeiSNjryZRdX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarTextLineGap(long hwnd,long gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VPWliGJDv",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetPath(const TCHAR * path)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(path);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"RerbjVZ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AsmSetTimeout(long time_out,long param)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(time_out);
    pn[0] = COleVariant(param);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CXMwwomSdXgq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetDictInfo(const TCHAR * str,const TCHAR * font_name,long font_size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"UdvJqUN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::EnumWindowSuper(const TCHAR * spec1,long flag1,long type1,const TCHAR * spec2,long flag2,long type2,long sort)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(spec1);
    pn[5] = COleVariant(flag1);
    pn[4] = COleVariant(type1);
    pn[3] = COleVariant(spec2);
    pn[2] = COleVariant(flag2);
    pn[1] = COleVariant(type2);
    pn[0] = COleVariant(sort);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kcDvLSoktGRdDC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::ReadStringAddr(long hwnd,LONGLONG addr,long tpe,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FBiPFpgDxTvyo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetCursorShapeEx(long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"Kxex",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindShape(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(offset_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nDhkQhXHDvuhV",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetWindowProcessPath(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"SzqtVKnGVsw",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarDrawText(long hwnd,long x,long y,long w,long h,const TCHAR * text,const TCHAR * color,long align)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(x);
    pn[5] = COleVariant(y);
    pn[4] = COleVariant(w);
    pn[3] = COleVariant(h);
    pn[2] = COleVariant(text);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(align);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"StoEBfijAfe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::AiYoloDetectObjects(long x1,long y1,long x2,long y2,float prob,float iou)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(prob);
    pn[0] = COleVariant(iou);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"mbEGhNIt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetWindowTransparent(long hwnd,long v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"SBiRRXhwy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::IsDisplayDead(long x1,long y1,long x2,long y2,long t)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(t);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qoZVetSBACoH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

double sptool::ReadDoubleAddr(long hwnd,LONGLONG addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VREyxSTjYaa",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.dblVal;
    }
    return 0.0;
}

CString sptool::GetCommandLine(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YTdvSelYekebKr",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetForegroundWindow()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"cYJYZCjJ",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CreateFolder(const TCHAR * folder_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(folder_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ipQNFlcl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetDmCount()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YbKhvmN",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FaqFetch()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"EARbzHXVgKm",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetMachineCode()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"beVbM",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FetchWord(long x1,long y1,long x2,long y2,const TCHAR * color,const TCHAR * word)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(word);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"KeCspu",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::MatchPicName(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"afURCIyCDbdx",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::Ocr(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ySyvHkzrUy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetBindWindow()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"djdEKBJsSuYk",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SaveDict(long index,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"biarQTWku",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetResultPos(const TCHAR * str,long index,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(index);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LiGye",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::FaqPost(const TCHAR * server,long handle,long request_type,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(server);
    pn[2] = COleVariant(handle);
    pn[1] = COleVariant(request_type);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VfNmPpaFgh",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicSimMemEx(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,long sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ovnXoYje",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetWindowState(long hwnd,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"DtpmAVjybDtdwHR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetDPI()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YzzEgnIH",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumWindowByProcessId(long pid,const TCHAR * title,const TCHAR * class_name,long filter)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(pid);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LCRqjX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindShapeE(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"lcUBhiNvr",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetMouseDelay(const TCHAR * tpe,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fwUJFwCgmGcY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG sptool::AsmCallEx(long hwnd,long mode,const TCHAR * base_addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(mode);
    pn[0] = COleVariant(base_addr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XfPDMY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long sptool::SetExcludeRegion(long tpe,const TCHAR * info)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(info);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"GVPzATuorYbLHdG",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetScreen(long width,long height,long depth)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(width);
    pn[1] = COleVariant(height);
    pn[0] = COleVariant(depth);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VaDceqicVKgaD",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::DisAssemble(const TCHAR * asm_code,LONGLONG base_addr,long is_64bit)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(asm_code);
    pn[1] = COleVariant(base_addr);
    pn[0] = COleVariant(is_64bit);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BRtasHfm",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindPicSimMem(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,long sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_info);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AmDLJXS",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::MoveWindow(long hwnd,long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IzhlrnLF",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::Md5(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CzNf",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::EnableRealKeypad(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YEvcTFWLw",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CreateFoobarRoundRect(long hwnd,long x,long y,long w,long h,long rw,long rh)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(x);
    pn[4] = COleVariant(y);
    pn[3] = COleVariant(w);
    pn[2] = COleVariant(h);
    pn[1] = COleVariant(rw);
    pn[0] = COleVariant(rh);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BgJKxCt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::MoveFile(const TCHAR * src_file,const TCHAR * dst_file)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(src_file);
    pn[0] = COleVariant(dst_file);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rystDZVsZAhuKq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetFindPicMultithreadLimit(long limit)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(limit);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BaWPFMlEYfFdS",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindMultiColorE(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(first_color);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"arieUclXlmEmq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetAveHSV(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ZPTdXdkqRInerYk",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindPicSim(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,long sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BNjqY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetDict(long index,long font_index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(font_index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"RIJjXX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindString(long hwnd,const TCHAR * addr_range,const TCHAR * string_value,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(string_value);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HtxwwrFvAbkzw",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::CheckUAC()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"KqVMLSDoajxsbM",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ScreenToClient(long hwnd,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    t0.vt = VT_I4;
    t0.lVal = *x;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    t1.vt = VT_I4;
    t1.lVal = *y;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TkeZz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::ForceUnBindWindow(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fzBj",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloUseModel(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ZohFlXhrmIM",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindPic(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"wDtT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindStrWithFont(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[12];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[11] = COleVariant(x1);
    pn[10] = COleVariant(y1);
    pn[9] = COleVariant(x2);
    pn[8] = COleVariant(y2);
    pn[7] = COleVariant(str);
    pn[6] = COleVariant(color);
    pn[5] = COleVariant(sim);
    pn[4] = COleVariant(font_name);
    pn[3] = COleVariant(font_size);
    pn[2] = COleVariant(flag);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xNil",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,12,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableSpeedDx(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"uyDgukm",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteData(long hwnd,const TCHAR * addr,const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BbfTHu",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::BindWindowEx(long hwnd,const TCHAR * display,const TCHAR * mouse,const TCHAR * keypad,const TCHAR * public_desc,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(display);
    pn[3] = COleVariant(mouse);
    pn[2] = COleVariant(keypad);
    pn[1] = COleVariant(public_desc);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"vEwIacPmQe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetSpecialWindow(long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"hYbdzusXKMY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetClipboard(const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WqUDQWJJXhkgML",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetCpuType()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tVpmoLKhHieu",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteIniPwd(const TCHAR * section,const TCHAR * key,const TCHAR * v,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(section);
    pn[3] = COleVariant(key);
    pn[2] = COleVariant(v);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dTaBJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableFindPicMultithread(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CgbJHcjF",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AddDict(long index,const TCHAR * dict_info)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_info);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NEbtGPng",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindStrWithFontE(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(str);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"DaQrYd",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FloatToData(float float_value)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(float_value);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dBnesRCX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindStrFastEx(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"vXuzJqlwtZR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::Beep(long fre,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(fre);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"EpzDVDh",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableMouseMsg(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qswkFL",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FaqCapture(long x1,long y1,long x2,long y2,long quality,long delay,long time)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(quality);
    pn[1] = COleVariant(delay);
    pn[0] = COleVariant(time);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"eDMF",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ReadDataToBin(long hwnd,const TCHAR * addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pdamzYRVSKCJNqp",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetNetTime()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BfPcvda",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::Reg(const TCHAR * code,const TCHAR * ver)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(code);
    pn[0] = COleVariant(ver);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"mnLIzhMMeFUt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CaptureGif(long x1,long y1,long x2,long y2,const TCHAR * file_name,long delay,long time)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(delay);
    pn[0] = COleVariant(time);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"cXXXBmCT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetEnumWindowDelay(long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qcjKPMuHYCelE",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::IsFolderExist(const TCHAR * folder)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(folder);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tFAa",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::AiYoloSortsObjects(const TCHAR * objects,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(objects);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"QfdyqxoCcFEdDeV",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetPicPwd(const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"oUuShbSxHcWI",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetNowDict()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NZqGUZpGSUFyalR",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarLock(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"mhWiFsxubJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

float sptool::ReadFloatAddr(long hwnd,LONGLONG addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"QQpuGTUqlibsa",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.fltVal;
    }
    return 0.0f;
}

long sptool::LeftUp()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kxHor",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumIniSection(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"PYQsYMLgBt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetDiskSerial(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AXFLVcpVfgV",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::RegEx(const TCHAR * code,const TCHAR * ver,const TCHAR * ip)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(code);
    pn[1] = COleVariant(ver);
    pn[0] = COleVariant(ip);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yWLksU",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindColorBlockEx(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long count,long width,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(count);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XwIrMiFLBt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetColGapNoDict(long col_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(col_gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MmSePwkqi",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetDictCount(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"DdpbkvucYGgKW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetDiskReversion(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"PwwkFn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::ReadFileData(const TCHAR * file_name,long start_pos,long end_pos)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(start_pos);
    pn[0] = COleVariant(end_pos);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"UggsfPLubK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetWordsNoDict(long x1,long y1,long x2,long y2,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nradKkDzF",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarClearText(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ipJogTGTCBborX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::ReadDataAddr(long hwnd,LONGLONG addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HshbdQB",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindStrS(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TacPrzWXjrdne",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::EnterCri()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"vIcx",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetSimMode(long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yJuGFwyuBnbL",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindWindowEx(long parent,const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(parent);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"RSQqrYQQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetParam64ToPointer()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"anEgYBbxgcGDCHh",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetWindow(long hwnd,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IFSKXzQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindDoubleEx(long hwnd,const TCHAR * addr_range,double double_value_min,double double_value_max,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(double_value_min);
    pn[3] = COleVariant(double_value_max);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"sChdikdCNSGv",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::Is64Bit()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FLNdASaaVUDgW",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::KeyPressStr(const TCHAR * key_str,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(key_str);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yDnDkbsC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetWindowClass(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CfYSPiXogpfkA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::TerminateProcess(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"atnMywrk",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteInt(long hwnd,const TCHAR * addr,long tpe,LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ammFBz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::RegNoMac(const TCHAR * code,const TCHAR * ver)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(code);
    pn[0] = COleVariant(ver);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"cJwVntlPv",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WheelDown()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FCLoxcm",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetPointWindow(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NYuEKDMouIHS",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumIniKeyPwd(const TCHAR * section,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XZfpIrSKUwjlCL",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::RegExNoMac(const TCHAR * code,const TCHAR * ver,const TCHAR * ip)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(code);
    pn[1] = COleVariant(ver);
    pn[0] = COleVariant(ip);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dpJZI",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG sptool::GetRemoteApiAddress(long hwnd,LONGLONG base_addr,const TCHAR * fun_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(base_addr);
    pn[0] = COleVariant(fun_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"phgNgHpvqWHs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

CString sptool::FindStrFastS(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IVzl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::EncodeFile(const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"Dhuvdkdx",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::ReadIni(const TCHAR * section,const TCHAR * key,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(key);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FBZkSLrlQat",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetFps()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"bKzAMLyLsrU",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::Delays(long min_s,long max_s)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(min_s);
    pn[0] = COleVariant(max_s);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"GTbVdZEeXwRn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::MoveTo(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nBWKmFzmAA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::IsSurrpotVt()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fsleczp",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetMachineCodeNoMac()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"Wavh",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindPicSimMemE(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,long sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xjLyCTFcCqt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetForegroundFocus()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MBmI",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetDict(long index,const TCHAR * dict_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LJJo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetOsBuildNumber()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pgBIp",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarFillRect(long hwnd,long x1,long y1,long x2,long y2,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"UHQSxvI",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetMinColGap(long col_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(col_gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tLBhqrQnZzPLYpg",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetResultCount(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ocBxd",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::UnBindWindow()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tNiwCxgGe",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarDrawPic(long hwnd,long x,long y,const TCHAR * pic,const TCHAR * trans_color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(pic);
    pn[0] = COleVariant(trans_color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"GdhYstPznbkTscn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::ExecuteCmd(const TCHAR * cmd,const TCHAR * current_dir,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(cmd);
    pn[1] = COleVariant(current_dir);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tmzxuGcK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::SelectDirectory()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YjET",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::MiddleUp()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"gudjNVKJr",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetProcessInfo(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fUuRrIsnqarmyyW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetMouseSpeed()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"psoZvIlwRW",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetDisplayInput(const TCHAR * mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MBEe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::Assemble(LONGLONG base_addr,long is_64bit)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(base_addr);
    pn[0] = COleVariant(is_64bit);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kBIIp",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarSetSave(long hwnd,const TCHAR * file_name,long en,const TCHAR * header)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(en);
    pn[0] = COleVariant(header);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tFGHTLajJl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

float sptool::ReadFloat(long hwnd,const TCHAR * addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FGQZHHi",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.fltVal;
    }
    return 0.0f;
}

long sptool::ShowTaskBarIcon(long hwnd,long is_show)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(is_show);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ZMSihRX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindStrEx(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pnIWbHGdALYWl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::VirtualProtectEx(long hwnd,LONGLONG addr,long size,long tpe,long old_protect)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(addr);
    pn[2] = COleVariant(size);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(old_protect);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CNfLQQzkHz",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ReadDataAddrToBin(long hwnd,LONGLONG addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yoZhnnDHnV",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableKeypadSync(long en,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"cJUNkUgasRL",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DecodeFile(const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"buWlefhfFoCZV",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetSystemInfo(const TCHAR * tpe,long method)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(method);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LYucwnXzamM",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::DmGuardParams(const TCHAR * cmd,const TCHAR * sub_cmd,const TCHAR * param)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(cmd);
    pn[1] = COleVariant(sub_cmd);
    pn[0] = COleVariant(param);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rLIEg",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::ReadIniPwd(const TCHAR * section,const TCHAR * key,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"jSgPinfwsJM",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetModuleSize(long hwnd,const TCHAR * module_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(module_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FFouqzkWo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloDetectObjectsToDataBmp(long x1,long y1,long x2,long y2,float prob,float iou,long * data,long * size,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(prob);
    pn[3] = COleVariant(iou);
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t0;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t1;
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VUAyaFfrMbvrX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *data = t0.lVal;
        *size = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetColorHSV(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YunGryiRK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::WriteDouble(long hwnd,const TCHAR * addr,double v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JYXAw",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::RightClick()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XYEvAb",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarStopGif(long hwnd,long x,long y,const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(x);
    pn[1] = COleVariant(y);
    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"skesyxNBn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetOsType()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"mxqE",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindColor(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IvHhcGkpq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::IsFileExist(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IvIhnEWb",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableKeypadMsg(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ZUjnggkSBPjh",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteStringAddr(long hwnd,LONGLONG addr,long tpe,const TCHAR * v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tazCIAwVIGmJV",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindNearestPos(const TCHAR * all_pos,long tpe,long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(all_pos);
    pn[2] = COleVariant(tpe);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tIBoBncnwiGHMWA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarUpdate(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xGKAJunusjKCo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::DoubleToData(double double_value)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(double_value);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WehXjplFY",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::DmGuard(long en,const TCHAR * tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YedDYWFr",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetScreenDataBmp(long x1,long y1,long x2,long y2,long * data,long * size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"muzQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        *data = t0.lVal;
        *size = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::LockMouseRect(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dUfedgPe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindFloatEx(long hwnd,const TCHAR * addr_range,float float_value_min,float float_value_max,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(float_value_min);
    pn[3] = COleVariant(float_value_max);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"umLysntMD",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetMac()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dmUuMrXGsTsG",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetDictMem(long index,long addr,long size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(index);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yikcLWuXwzgNXCx",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumWindow(long parent,const TCHAR * title,const TCHAR * class_name,long filter)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(parent);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"aHRIUpW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetMousePointWindow()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YqqGZvclAbKwNxq",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetAveRGB(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tjjdU",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindColorEx(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CAyWzKb",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::ImageToBmp(const TCHAR * pic_name,const TCHAR * bmp_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(pic_name);
    pn[0] = COleVariant(bmp_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qMETYoSlh",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ShowScrMsg(long x1,long y1,long x2,long y2,const TCHAR * msg,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(msg);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YDEpVDnubB",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::IntToData(LONGLONG int_value,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(int_value);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"heQQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindPicSimE(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,long sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XxmuxWQbiKtLZfl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindShapeEx(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WwGZM",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::Ver()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"echgGQNZJdIhxlx",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarDrawLine(long hwnd,long x1,long y1,long x2,long y2,const TCHAR * color,long style,long width)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(style);
    pn[0] = COleVariant(width);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"EiLhFfhiQdvIDc",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicE(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pdsPGcRACCykUA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindColorE(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"uKDyDo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::OcrInFile(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(pic_name);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WoPYdNbCEksCM",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::CapturePng(long x1,long y1,long x2,long y2,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"gHsIYdFkNfaUJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CreateFoobarCustom(long hwnd,long x,long y,const TCHAR * pic,const TCHAR * trans_color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x);
    pn[3] = COleVariant(y);
    pn[2] = COleVariant(pic);
    pn[1] = COleVariant(trans_color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XrdGIdCgdATvUTF",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableFakeActive(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TYoWPPyATzdl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::TerminateProcessTree(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TerminateProcessTree",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetLastError()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CaBMTRim",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::SelectFile()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LgLlyZhYHpHcb",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetCursorPos(long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"eDSDHIXJBxEpNA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::MiddleDown()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"DbwPQzy",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetKeyState(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"xgnrduCo",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FreePic(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HciH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FreeProcessMemory(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yUxtsNmcsYIXRy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarTextRect(long hwnd,long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"vsFTyGIqMmTleIR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DisableCloseDisplayAndSleep()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BoTCfJ",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnablePicCache(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VIJbs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloFreeModel(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FTaJZzwope",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetWindowThreadId(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NQUTUEl",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicEx(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"veNrqhCaVzJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetWindowProcessId(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"jfhLgRYm",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloSetModelMemory(long index,long addr,long size,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(index);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LXBrwlACrvLGphU",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetExportDict(long index,const TCHAR * dict_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XFjoAdJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetBasePath()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tNMSDSZzoY",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetMemoryHwndAsProcessId(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tSJLyNcxnFHHjt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::VirtualQueryEx(long hwnd,LONGLONG addr,long pmbi)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(pmbi);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HleSLsdWag",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetPicSize(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"waNPACYH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::ReadData(long hwnd,const TCHAR * addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"lIFMscK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetDisplayAcceler(long level)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(level);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ustRN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetWords(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FNXduwgyAxBlnJT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::KeyPressChar(const TCHAR * key_str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(key_str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"uxtgFnWC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WheelUp()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nbFVKcKumTeL",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetUAC(long uac)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(uac);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kHwKFQdC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetWordGapNoDict(long word_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(word_gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qCIoKli",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetDisplayInfo()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kIRqdVeGpjsqj",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindStrFast(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"oFBMLMUGSwhs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarStartGif(long hwnd,long x,long y,const TCHAR * pic_name,long repeat_limit,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x);
    pn[3] = COleVariant(y);
    pn[2] = COleVariant(pic_name);
    pn[1] = COleVariant(repeat_limit);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pTEDuaZSH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindMultiColorEx(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(first_color);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"tvptiyWx",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindStrWithFontEx(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(str);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dnpmzBbMQFxi",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetScreenHeight()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yRUICTDcAglcBz",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DeleteIni(const TCHAR * section,const TCHAR * key,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(key);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JbQEhpaIs",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WaitKey(long key_code,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(key_code);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NWxSEWhGYvcFbDZ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FaqCaptureString(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"EFzyFVpwnqL",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FindColorBlock(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long count,long width,long height,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[11];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[10] = COleVariant(x1);
    pn[9] = COleVariant(y1);
    pn[8] = COleVariant(x2);
    pn[7] = COleVariant(y2);
    pn[6] = COleVariant(color);
    pn[5] = COleVariant(sim);
    pn[4] = COleVariant(count);
    pn[3] = COleVariant(width);
    pn[2] = COleVariant(height);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"IbMVTZb",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,11,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::ClientToScreen(long hwnd,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    t0.vt = VT_I4;
    t0.lVal = *x;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    t1.vt = VT_I4;
    t1.lVal = *y;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"watWCmiBF",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::InitCri()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BvVGTYrqaLSIqN",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::KeyDownChar(const TCHAR * key_str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(key_str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"RMUKYsAIu",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

double sptool::ReadDouble(long hwnd,const TCHAR * addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VJwWsKJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.dblVal;
    }
    return 0.0;
}

CString sptool::GetPath()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fVHP",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetMemoryFindResultToFile(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"bBdxWDhfZYhNd",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AsmClear()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WgbnrYSKbxLpLH",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetMouseSpeed(long speed)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(speed);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"zqtkjqdhbGqnsRi",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::KeyDown(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WrKsDBw",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetWindowRect(long hwnd,long * x1,long * y1,long * x2,long * y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;
    CComVariant t0,t1,t2,t3;

    VariantInit(&t0);
    VariantInit(&t1);
    VariantInit(&t2);
    VariantInit(&t3);

    pn[4] = COleVariant(hwnd);
    pn[3].vt = VT_BYREF|VT_VARIANT;
    pn[3].pvarVal = &t0;
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t1;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t2;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t3;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"vAzEgfdiyQ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        *x1 = t0.lVal;
        *y1 = t1.lVal;
        *x2 = t2.lVal;
        *y2 = t3.lVal;
        return vResult.lVal;
    }
    return 0;
}

long sptool::DownloadFile(const TCHAR * url,const TCHAR * save_file,long timeout)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(url);
    pn[1] = COleVariant(save_file);
    pn[0] = COleVariant(timeout);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"XDlZAAXvxicE",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DeleteFile(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FZvamoNZdfxnU",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableMouseAccuracy(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"quJZG",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteDataAddrFromBin(long hwnd,LONGLONG addr,long data,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(data);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"bHEhRD",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SendPaste(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kTgUGAihRKD",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetWindowSize(long hwnd,long width,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"viCQrHrS",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ExitOs(long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"iQTLTNsaFen",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::AppendPicAddr(const TCHAR * pic_info,long addr,long size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(pic_info);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"khnBSemBFlrIaJf",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::HackSpeed(double rate)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(rate);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"geknyFW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG sptool::AsmCall(long hwnd,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YspHhTWJ",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long sptool::EnableBind(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YisswwuVTy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetNetTimeSafe()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"BWNUkD",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetKeypadDelay(const TCHAR * tpe,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ezXSizDsqmG",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::Hex32(long v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"uyWx",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindPicS(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ngJASUP",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetID()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nBeeXrGs",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetTime()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"eFSU",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FaqCaptureFromFile(long x1,long y1,long x2,long y2,const TCHAR * file_name,long quality)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(quality);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TdqR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LoadAi(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qPuVabejWpB",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteString(long hwnd,const TCHAR * addr,long tpe,const TCHAR * v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"VQTobwtTiCf",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FoobarClose(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FUMDBJjPSzzti",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::ActiveInputMethod(long hwnd,const TCHAR * id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"sHYQzcFDFcc",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloDetectObjectsToFile(long x1,long y1,long x2,long y2,float prob,float iou,const TCHAR * file_name,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(prob);
    pn[2] = COleVariant(iou);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NxGnHTdYdc",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableShareDict(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"SAizTowuEQjeLPK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::ReadString(long hwnd,const TCHAR * addr,long tpe,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"avkCA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SpeedNormalGraphic(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"duVlcmSIf",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetWindowTitle(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"CXJVPItGzvubq",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FoobarPrintText(long hwnd,const TCHAR * text,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(text);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"jWEhKktJli",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::FaqCancel()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"KLTDByBfLYlpALI",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::AiYoloSetVersion(const TCHAR * ver)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(ver);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"bnKpWmyJuhn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicMemEx(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"KhEamKr",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindWindowByProcess(const TCHAR * process_name,const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(process_name);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"QbteoVgDC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::KeyUpChar(const TCHAR * key_str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(key_str);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"jwHBt",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindStringEx(long hwnd,const TCHAR * addr_range,const TCHAR * string_value,long tpe,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(string_value);
    pn[3] = COleVariant(tpe);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"oMMT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::BGR2RGB(const TCHAR * bgr_color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(bgr_color);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"TgRLe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::WriteFile(const TCHAR * file_name,const TCHAR * content)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(content);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"WDYVGLnjhNfX",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetInputDm(long input_dm,long rx,long ry)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(input_dm);
    pn[1] = COleVariant(rx);
    pn[0] = COleVariant(ry);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"GXuVGEk",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::DmGuardLoadCustom(const TCHAR * tpe,const TCHAR * path)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(path);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"AWTibSE",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LockInput(long locks)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(locks);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"YnINoCkCC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetWordGap(long word_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(word_gap);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"FPLmfkA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumIniSectionPwd(const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"PmwB",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::StringToData(const TCHAR * string_value,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(string_value);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"yUjDvpoRkUGyn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindPicMem(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_info);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"HhrJJH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetCursorShape()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"nFqcmBTBJvkwV",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::SortPosDistance(const TCHAR * all_pos,long tpe,long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(all_pos);
    pn[2] = COleVariant(tpe);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"gFKlH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetDisplayDelay(long t)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(t);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NLiXC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::OcrExOne(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JIRZkfRqZaC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::SetWordLineHeightNoDict(long line_height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(line_height);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"dDGCDRkrdAErR",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CreateFoobarEllipse(long hwnd,long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ahFnRuIFCdapP",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::EnumWindowByProcess(const TCHAR * process_name,const TCHAR * title,const TCHAR * class_name,long filter)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(process_name);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"fmAwbPW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::GetDiskModel(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NReRZAe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::FindWindowSuper(const TCHAR * spec1,long flag1,long type1,const TCHAR * spec2,long flag2,long type2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(spec1);
    pn[4] = COleVariant(flag1);
    pn[3] = COleVariant(type1);
    pn[2] = COleVariant(spec2);
    pn[1] = COleVariant(flag2);
    pn[0] = COleVariant(type2);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"qpVNGLrPvlApyT",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::Int64ToInt32(LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"wxfDe",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::EnableRealMouse(long en,long mousedelay,long mousestep)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(en);
    pn[1] = COleVariant(mousedelay);
    pn[0] = COleVariant(mousestep);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"NuvbizBhijakgL",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::LeftClick()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"kUBqJMfR",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::SetClientSize(long hwnd,long width,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"wdhFgHH",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::Hex64(LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"MbVtWcCbxDpn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

LONGLONG sptool::ReadIntAddr(long hwnd,LONGLONG addr,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"ddVishK",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long sptool::GetFileLength(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"mjhWtqxFsN",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::WriteIntAddr(long hwnd,LONGLONG addr,long tpe,LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"pTtA",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::CreateFoobarRect(long hwnd,long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"LsHhDeMcnGSXyn",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::FindPicMemE(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"sYtPlykW",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString sptool::FindStrExS(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"JRhQBC",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long sptool::GetScreenDepth()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"rnSquek",&dispatch_id);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long sptool::GetScreenData(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"lscnBQIIeBrLbb",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString sptool::GetDir(long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(L"GJobguMsupiGZFy",&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

