#pragma once

bool AutoLogin_IsActive(long index);
void AutoLogin_CheckAndTrigger(long index);
long AutoLogin_GetLoginPendingMs(long index);
void AutoLogin_ClearLoginPending(long index);
