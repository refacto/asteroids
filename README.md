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

## Building

```sh
meson setup build
meson compile -C build
```

The compiled binary will be at `build/asteroids`.

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
clang-format -i main.c
```

To check formatting without modifying files (as CI does):
```sh
clang-format --dry-run --Werror main.c
```

### Git hook

A pre-commit hook is included in `.githooks/` that automatically formats staged `.c`/`.h` files before each commit. To install it:

```sh
git config core.hooksPath .githooks
```
