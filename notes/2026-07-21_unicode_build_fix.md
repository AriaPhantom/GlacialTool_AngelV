# 2026-07-21 Unicode build fix

## Problem

- The Unicode project configuration exposed narrow ASCII key strings passed to `TCHAR` input helpers as C2664 errors.
- `findArrowDirection` returned a narrow empty string to `CString`, producing C2440.

## Change

- Added local narrow-string overloads for `keyDown`, `keyUp`, `holdKey`, and `press` that decode input as UTF-8 before forwarding to the existing Unicode implementations.
- Replaced the narrow empty `CString` return with `_T("")`.
- The input and task-state control flow is unchanged, and the conversion does not depend on the machine ANSI code page.

## Validation

- `build_all_versions.bat`: PASS (`Release|x64`, NL / Angel / Adele, exit code 0).
- `git diff --check`: PASS.
