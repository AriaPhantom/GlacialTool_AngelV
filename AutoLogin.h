#pragma once

bool AutoLogin_IsActive(long index);
bool AutoLogin_RunStartup(long index);
void AutoLogin_CheckAndTrigger(long index);
long AutoLogin_GetLoginPendingMs(long index);
void AutoLogin_ClearLoginPending(long index);

void AutoLogin_StartDisconnectWatcher(long index);
void AutoLogin_StopDisconnectWatcher(long index);
