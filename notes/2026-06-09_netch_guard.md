# Netch SOCKS 2801 external disconnect guard

Date: 2026-06-09

## Behavior
- Adds a `Netch Guard` toggle in the options UI.
- When enabled, the existing AutoLogin disconnect watcher probes an external endpoint through the Netch SOCKS5 proxy at `127.0.0.1:2801` once per second.
- The probe uses libcurl through SOCKS5 to `http://1.1.1.1/` with short timeouts (`800ms` connect, `1500ms` total). This is intentionally **not** a localhost-only port check: Netch must be able to reach the outside network for the guard to consider the path healthy.
- This follows the same principle as maple.watch (`https://maple.690420.xyz/#GMS-Scania`): test remote reachability through the network path instead of only checking whether a local listener exists.
- If the external SOCKS path is down for 5 consecutive seconds, it calls `subSoftPause()` and sets task state to `NETCH PAUSE`.
- If it recovers after the 5s soft-pause window but before hard close, it calls `subSoftStart()` and sets task state to `NETCH OK`.
- If the external SOCKS path stays down for 10 seconds, it keeps soft-paused, kills MapleStory, marks a forced relaunch, and waits for the path to recover before triggering AutoLogin.
- While the guard is holding a soft pause/hard close, the regular disconnect watcher check is skipped to avoid relogging while the proxy path is still down.

## Touchpoints
- `AutoLogin.cpp`: Netch guard state machine and external SOCKS probe integrated into `DisconnectWatcherLoop`.
- `exampleDlg.cpp` / `exampleDlg.h`: persisted `Netch Guard` toggle and getter.

## Verification
- `cmd /c "set CI=1&& call build_all_versions.bat <nul"` passed on 2026-06-09.
- Static sanity checks passed on 2026-06-09: no literal PowerShell escape artifacts (`` `r`n`` / `` `t``) and `git diff --check` clean.
