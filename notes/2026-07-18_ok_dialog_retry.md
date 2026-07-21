# 2026-07-18 OK dialog retry hardening

## Problem

- Useful-item handling performed a single OK-image lookup immediately after the key hold.
- A delayed dialog or a missed first Esc press could leave the OK dialog open.

## Change

- `OKDetector` now polls up to 12 times at 150 ms intervals.
- After finding the dialog, it presses Esc once and waits 400 ms. If the OK image is still detected, it clicks once at the newly detected button coordinates as a mouse-only fallback; it never sends a second Esc.
- Null plugin pointers and failed window-rectangle queries are rejected before scanning where applicable.
- The ignite/oil branches, where present, now wait 400-500 ms after the key hold and run the same detector.
- 2026-07-21 rollout: this detector now follows the CarenV dynamic-window and 0.90 second-pass strategy while preserving AngelV's existing ROI.

## Validation

- `build_all_versions.bat`: PASS (`Release|x64`, all variants, exit code 0).
- `git diff --check`: PASS.
