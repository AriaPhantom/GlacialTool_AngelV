# 2026-06-08 AngelV task status encoding

## Change
- Synced the KannaV task-status text bridge hardening into AngelV.
- The final TCHAR* -> QString conversion now accepts either UTF-8 task-state bytes or legacy local-8-bit task-state bytes.
- Gameplay/thread flow is unchanged; only the UI text conversion used by the status display was touched.

## Verification
- git diff --check passed.
- Attempted `build_all_versions.bat` with `CI=1`; local script stopped before compile because VS2019 MSBuild was not found at `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe`.

