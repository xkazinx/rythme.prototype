# continue.md (rythme, main)

## Repos

| Repo | Commit | State |
|---|---|---|
| rythme | main, this file's commit | pushed |
| kazilib (`src/dep/kazilib`) | master, the pointer in this commit | pushed |

## Last work (2026-11-06)

- **Port (from 2026-10-18, committed now):** the 2022 code onto SFML 3.1 and kazilib, FASTBuild, Release x86.
- **Web:** `build_web.ps1`, deployed at https://kazin.dev/projects/rythme/ (`deploy_ubuntu.ps1`, static, Caddy snippet `kazin.dev.d/rythme.caddy`, `/opt/rythme/web`).
- **Bug fixed (kazilib):** `rang::init()` wasn't inline: every file including `rang.hpp` defined it, and the web link refused the duplicates.
- **Added:** the console line `Game ready: 10 frets` and the controls hint on the page (web only), the web test, README with the screenshot.

## Tested

| Test | Result |
|---|---|
| `src\tests\web\run_web_test.ps1`, local build | passed |
| same, `-Url https://kazin.dev/projects/rythme/` | passed |
| `deploy\rythme\rythme.exe` Release x86 | builds; started and ran 3 s; not played by hand |

Known gaps: only headless Edge with software WebGL; see `to-resolve.md`.

## Next steps

- `to-resolve.md`: the manual checks and the question about a game with no notes yet.

## The user's prompts (this work)

1. "https://github.com/xkazinx/rythme.prototype compile for web upload to projects"
2. "update readme put photo"
