#include "stdafx.h"
#include "log.h"


static CRITICAL_SECTION g_log_cs;

void   LogInit()
{
	InitializeCriticalSection(&g_log_cs);
}	


void   Log(const TCHAR * format,...)
{
	va_list	args;
	va_start(args, format);

	int len = _vsctprintf(format, args);
	if (len > 0) 
	{
		len += (1 + 2);
		PTCHAR buf = (PTCHAR) malloc(sizeof(TCHAR)*len);
		if (buf) 
		{
			len = _vstprintf_s(buf, len, format, args);
			if (len > 0) 
			{
				while (len && _istspace(buf[len-1])) len--;
				buf[len++] = _T('\r');
				buf[len++] = _T('\n');
				buf[len] = 0;

				EnterCriticalSection(&g_log_cs);
				OutputDebugString(buf);
				LeaveCriticalSection(&g_log_cs);
			}
			free(buf);
		}
		va_end(args);
	}
}