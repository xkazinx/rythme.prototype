# CLAUDE.md (rythme)

- **Build:** `powershell -ExecutionPolicy Bypass -File build.ps1` (`setup.ps1`: kazilib submodule, junction `src\dep\SFML-3.1.0`; then kazilib FASTBuild on `src\fastbuild.json`: kazilib, rythme). Output `deploy\rythme\rythme.exe`. `setup.ps1`'s `submodule update` resets kazilib to the recorded pointer: stage a moved pointer first (`git add src/dep/kazilib`).
- **Web:** `build_web.ps1` (Emscripten `D:\code\tools\emsdk`, SFML web `D:\code\tools\kazilib-web`) → `src\build\web\rythme-build\`; `src\client\CMakeLists.txt` is the web build only. Code that differs: `#ifdef __EMSCRIPTEN__` in `pch.h`, `App.h`, `App.cpp` (no console, no viewport calls, no sleep, no frame limit, the controls hint on the page).
- **Deploy:** `deploy_ubuntu.ps1` → box `kazin-music`, `/opt/rythme/web`, Caddy `kazin.dev/projects/rythme` (`deploy\ubuntu\`); static, no service. Touch only rythme's files there.
- **Tests:** `src\tests\web\run_web_test.ps1` (local build; `-Url https://kazin.dev/projects/rythme/` live), 0 failed, outside the command sandbox; read the shots in `testerino\web\`.
- **Console lines** (`Game ready: 10 frets`, `Pressed <idx> press <true|false>`) are what the web test waits for: writer and steps change together.
- **README:** the original stays at its end (`## Original README`).
- **Kazilib first:** scenes, std names, rang, ImGui from kazilib; reusable code goes there.
