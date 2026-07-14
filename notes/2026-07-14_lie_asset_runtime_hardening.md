# 2026-07-14 Lie asset deployment and runtime hardening

## Lie template validation

- Verified the real source asset before copying: `F:\OneDrive\Software\tmp\Lie.bmp`.
- Copied it to the exact path used by every active variant: `C:\sptool\Lie.bmp`.
- Source/destination SHA-256: `6169EBCB9757F6B01E75D86A759F29E1DC587C97E41B2B020ACAA054F8E505D2`.
- Image metadata: 83 x 13 pixels, 24-bit BMP.
- Tested against 82 complete positive screenshots under `F:\OneDrive\Software\tmp\lie` using the same top-left/half-window search policy.
- Positive `TM_CCOEFF_NORMED` result: 82/82 passed at `0.98`; minimum score `0.9940`, maximum `0.9995`, mean `0.9978`.
- Six visually confirmed negative full-size images scored from `0.2664` to `0.4179`, all far below `0.98`.
- Based on the real asset and complete screenshots, Lie similarity was restored from `0.96` to `0.98` to reduce false-positive risk without losing the tested positives.

## Runtime hardening

- `sptool::IsBind` now validates the HWND and its process ID, not only numeric HWND equality.
- Monitor paths reject a replaced/reused window binding before running image checks; CarenV performs an equivalent direct HWND/PID check.
- Repeated `gMonitorCheck` exceptions are rate-limited to one log per worker slot every five seconds.
- After a forced `TerminateThread`, cleanup clears the shared `dm` pointer but intentionally skips refcount/destructor cleanup, preferring a rare leak over running a destructor on potentially inconsistent state.
- Normal cleanup clears the shared `dm` pointer before `ReleaseRef`/destruction, reducing the stale-pointer visibility window.
- Disconnect screenshot path conversion now uses `WideToAnsiLocal` instead of truncating `wchar_t` values through iterator conversion.
- Fixed a critical bug inside `WideToAnsiLocal`: it previously allocated `size - 1` bytes but passed `size` to `WideCharToMultiByte`, allowing the terminating NUL to write one byte past the string buffer. It now allocates the full reported size, validates the converted count, and then removes the terminator with `resize(converted - 1)`.

## Verification

- Required `build_all_versions.bat` run: PASS for all nine active V repositories.
- Compiler warnings in the final build logs: 0 for all nine repositories.
- Runtime dependency/plugin packaging check: PASS for all nine repositories.
- `git diff --check` and modified-file byte sanity: PASS.
- OneDrive machine-suffixed conflict scan: 0 items.

## Remaining limits

- C++ `/EHsc` catches still cannot recover from Windows fail-fast, heap corruption, or access violations.
- Forced-thread cleanup can intentionally leak one `dm` object; this occurs only after the existing 30-second graceful-stop timeout.
- The available negative corpus contains only six full-size images. A larger labeled negative gameplay corpus would provide stronger false-positive confidence.
