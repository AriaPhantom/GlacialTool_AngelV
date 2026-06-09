# VS2026 + C:\vcpkg build configuration

Date: 2026-06-08

## Scope

- Align local Release x64 builds with VS2026 (`v145`) and the C-drive vcpkg install.
- Keep runtime packaging pointed at `C:\vcpkg\installed\x64-windows\bin` and Qt plugin paths under `C:\vcpkg`.
- Preserve the existing multi-version build flow; no gameplay/runtime logic was changed.

## Changes

- Updated build_all_versions.bat to call VS2026 MSBuild directly.
- Added MSBuild properties: `WindowsTargetPlatformVersion=10.0.26100.0`, `PlatformToolset=v145`, `VcpkgRoot=C:\vcpkg\`, and `VcpkgInstalledDir=C:\vcpkg\installed\`.
- Replaced legacy `F:\vcpkg` project and post-build copy paths with `C:\vcpkg` paths in the active project/runtime copy files.

## Verification

- Command: `cmd /c "set CI=1&& call build_all_versions.bat <nul"`
- Result: exit code `0`.
- Outputs: `Angi_Adele.exe, Angi_Angel.exe, Angi_NL.exe`.
- Log: `C:\Users\Pai\AppData\Local\Temp\maple_vs2026_build_logs\GlacialTool_AngelV_20260608_073004.log`.
- Warnings: Build succeeded with existing non-fatal warnings: OpenCV C4819 under CP936 and wchar_t-to-char C4244.
