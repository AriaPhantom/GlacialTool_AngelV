# 2026-05-12 Friend/Guild minimap threshold raise

## Goal
Test whether false positives in friend/guild minimap detection come from the snapshot/template path being too permissive at 0.95.

## Change
- In `boHandler.cpp`, raised both friend and guild minimap snapshot matches from `0.95` to `0.99`.
- Scope limited to the friend/guild branch only.

## Validation
Built with:
- `cmd /c "set CI=1&& call build_all_versions.bat <nul>"`

Successful outputs:
- `x64\Release\Angi_NL.exe`
- `x64\Release\Angi_Angel.exe`
- `x64\Release\Angi_Adele.exe`
