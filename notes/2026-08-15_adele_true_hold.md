# Adele simultaneous true hold

The Adele routine buff now presses `飞剑` and `轰炸` once, holds both for 3000 ms, and releases both once.

- Removed the former 133 ms repeated `KeyDownChar` loop.
- Preserved simultaneous-key behavior and the existing buff timeout/rest timing.
- The shared `holdKey` implementation also uses one key-down, one hold delay, and one key-up.
