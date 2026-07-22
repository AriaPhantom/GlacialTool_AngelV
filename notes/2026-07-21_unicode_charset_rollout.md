# Unicode charset rollout

## Scope

- The Release|x64 configuration uses CharacterSet=Unicode and /utf-8.
- Legacy CP936 C++/header/resource inputs in this project were converted to UTF-8 with BOM.
- The task-state path remains wide through TCHAR storage, SetTaskState, and Qt wide-string conversion.

## Compatibility

- UTF-8 BOM makes Chinese task-state literals independent of the machine ACP, including US Windows installations.
- The CarenV OK-dialog retry strategy is already present in the sync CarenV OK detection strategy commit; no gameplay flow was changed here.

## Validation

- Static XML/UTF-8/RC checks and `git diff --check`: PASS.
- `build_all_versions.bat` in CI mode using Release|x64: PASS for the charset/resource rollout. Parallel AutoLogin edits are intentionally excluded from this commit and need their own rebuild.
