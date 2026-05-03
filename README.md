# Asteroids

An Asteroids clone written in C using [raylib](https://www.raylib.com/).

## Dependencies

- [raylib](https://www.raylib.com/) 5.5+
- [Meson](https://mesonbuild.com/) 1.10+
- [Ninja](https://ninja-build.org/)
- A C23-capable compiler (clang, gcc might work too)

### Installing dependencies

**macOS (Homebrew):**
```sh
brew install raylib meson
```

**Linux:**
```sh
pip install meson
# Also install raylib from source: https://github.com/raysan5/raylib.git
```

**Windows ([MSYS2](https://www.msys2.org/)):**

Install [MSYS2](https://www.msys2.org/), then open the **CLANG64** shell and run:
```sh
pacman -S mingw-w64-clang-x86_64-clang mingw-w64-clang-x86_64-meson mingw-w64-clang-x86_64-raylib
```

## Building

```sh
meson setup build
meson compile -C build
```

The compiled binary will be at `build/asteroids` (or `build/asteroids.exe` on Windows).

To build a release:
```sh
meson setup build --buildtype=release
meson compile -C build
```

## Running

```sh
./build/asteroids
```

## Code formatting

The project uses [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to enforce a consistent code style.

To format all C files:
```sh
clang-format -i *.c *.h
```

To check formatting without modifying files (as CI does):
```sh
clang-format --dry-run --Werror *.c *.h
```

### Git hook

A pre-commit hook is included in `.githooks/` that automatically formats staged `.c`/`.h` files before each commit. To install it:

```sh
git config core.hooksPath .githooks
```

### Sounds

The sounds were made using [rFXGen](https://raylibtech.itch.io/rfxgen). You can find the rFX files in `resources/audio` to load, edit and export them again to modify them.
