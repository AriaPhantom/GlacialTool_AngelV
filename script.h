#ifndef __INCLUDE_SCRIPT_H__
#define __INCLUDE_SCRIPT_H__


unsigned WINAPI MainThread(PVOID pParam);


void ScriptDelay(long index, long time);
void SetTaskState(long index, const TCHAR* state);

#endif