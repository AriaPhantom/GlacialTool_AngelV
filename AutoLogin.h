#pragma once

bool AutoLogin_IsActive(long index);
void AutoLogin_RecordWhiteIconSeen(long index);
bool AutoLogin_RunStartup(long index);
void AutoLogin_CheckAndTrigger(long index, unsigned long long generation = 0);
long AutoLogin_GetLoginPendingMs(long index);
void AutoLogin_ClearLoginPending(long index);

void AutoLogin_StartDisconnectWatcher(long index);
void AutoLogin_StopDisconnectWatcher(long index);
