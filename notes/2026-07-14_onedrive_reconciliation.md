# 2026-07-14 OneDrive reconciliation and hardening

## Scope

- Reconciled the reviewed OneDrive conflict-copy changes into the canonical sources rather than keeping machine-suffixed copies.
- Hardened the monitoring path where applicable: null `dm` checks, mini-map snapshot validity checks, guarded Lie-window coordinates/results, and a `gMonitorCheck` exception boundary.
- The initial reconciliation kept Lie detection local at `0.96`; after the exact asset and 82 complete screenshots were found, the follow-up hardening restored it to the validated `0.98` threshold. The global external `FindPic` behavior remains unchanged.
- Cached the Netch toggle in an atomic value so worker threads no longer read a Qt widget directly.
- Ensured the Netch widget is constructed and connected before it is inserted into the layout.
- Made the build choose the available VS18/VS2019 toolset and `C:\vcpkg`/`F:\vcpkg` installation, with project paths based on `$(VcpkgRoot)`.
- Fixed the trailing-backslash quoting of the PowerShell post-build `-OutDir` argument. Before this fix, the closing quote could be swallowed and the explicit `-InstalledBin` value could be ignored.
- Removed 18 reviewed machine-suffixed OneDrive source/build artifact(s) from this repository after semantic comparison.

## Validation

- Required command: `cmd /c "set CI=1&& call build_all_versions.bat <nul"`.
- Repository build entry: `build_all_versions.bat`.
- Result on 2026-07-14: **PASS** (`Release|x64`, exit code 0).
- The final all-V run reported no `vcpkg.applocal.log not found and applocal.ps1 is missing` warning.
- Runtime output verification found the common CPR/OpenCV/Qt DLLs, Wuya DLLs, `skin/main.xml`, `qwindows.dll`, and `qjpeg.dll` in every active variant output.
- `git diff --check`, scripted-edit escape checks, Netch create-before-add checks, and `git fsck --full --no-dangling` were included in the reconciliation gate.

## Lie asset deployment

The exact mapped asset `C:\sptool\Lie.bmp` was deployed on 2026-07-14 from `F:\OneDrive\Software\tmp\Lie.bmp`. The copied file is an 83 x 13, 24-bit BMP with SHA-256 `6169EBCB9757F6B01E75D86A759F29E1DC587C97E41B2B020ACAA054F8E505D2`.

## Residual limitations

- A null check cannot prove that a non-null `dm` pointer is still alive; full protection against a stale/dangling pointer requires synchronized ownership or generation validation.
- The `/EHsc` C++ exception boundary does not catch Windows fail-fast (`0xc0000409`) or general structured exceptions/access violations.
- Persistent monitor exceptions are currently logged and retried; there is no rate limit or automatic circuit breaker.
- Build auto-detection intentionally covers the currently used VS18/VS2019 and `C:\vcpkg`/`F:\vcpkg` layouts, not every possible Visual Studio/vcpkg installation.

## Explicit non-goal

Per user instruction, `.git`, `x64`, and `Release` directories were **not** moved out of the OneDrive synchronization scope.
