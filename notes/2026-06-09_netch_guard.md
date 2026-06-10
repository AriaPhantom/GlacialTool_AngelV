# Netch SOCKS 2801 Scania disconnect guard

Date: 2026-06-09

## Behavior
- Adds a compact `Netch` / `Netch Guard` toggle in the options UI.
- When enabled, the existing AutoLogin disconnect watcher probes real MapleStory Scania game-server endpoints through the Netch SOCKS5 proxy at `127.0.0.1:2801` once per second.
- The probe uses libcurl `CONNECT_ONLY` through SOCKS5 to Scania Channel 9 (`34.215.85.101:8585`) and Channel 11 (`54.191.254.95:8585`); either endpoint succeeding counts as healthy.
- This is intentionally **not** a localhost-only port check: Netch must be able to reach an actual MapleStory channel endpoint for the guard to consider the path healthy.
- Endpoint selection follows the same principle as maple.watch (`https://maple.690420.xyz/#GMS-Scania`): test remote MapleStory reachability through the network path instead of only checking whether a local listener exists.
- If both Scania endpoints are unreachable for 5 consecutive seconds, it calls `subSoftPause()` and sets task state to `NETCH PAUSE`.
- If either endpoint recovers after the 5s soft-pause window but before hard close, it calls `subSoftStart()` and sets task state to `NETCH OK`.
- If both endpoints stay unreachable for 10 seconds, it keeps soft-paused, kills MapleStory, marks a forced relaunch, and waits for the path to recover before triggering AutoLogin.
- While the guard is holding a soft pause/hard close, the regular disconnect watcher check is skipped to avoid relogging while the proxy path is still down.

## Touchpoints
- UI note (2026-06-10): non-Universal variants place the compact `Netch` toggle in the existing login-mode row instead of adding a new Detect grid row, preventing the page from being squeezed/clipped.
- `AutoLogin.cpp`: Netch guard state machine and Scania SOCKS probe integrated into `DisconnectWatcherLoop`.
- `exampleDlg.cpp` / `exampleDlg.h`: persisted `Netch Guard` toggle and getter.

## Verification
- `cmd /c "set CI=1&& call build_all_versions.bat <nul"` passed on 2026-06-10 after the UI layout fix.
- Static sanity checks passed on 2026-06-10: no literal PowerShell escape artifacts (`` `r`n`` / `` `t``) and `git diff --check` clean.
