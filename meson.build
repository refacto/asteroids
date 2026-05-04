project(
  'Asteroids',
  'c',
  default_options: {
    'buildtype': 'debugoptimized',
    'warning_level': '2',
    'werror': true,
    'b_pie': true,
    'c_std': 'c23',
    'b_lundef': false, # meson says sanitizers can't handle this
  },
  meson_version: '>=1.10.0',
)
cc = meson.get_compiler('c')

# sanitizers are not supported on Windows or Emscripten; leak sanitizer is Linux-only
if host_machine.system() != 'windows' and host_machine.system() != 'emscripten'
  sanitize_flags = ['-fsanitize=address,undefined']
  if host_machine.system() != 'darwin'
    sanitize_flags += ['-fsanitize=leak']
  endif
  add_project_arguments(sanitize_flags, language: 'c')
  add_project_link_arguments(sanitize_flags, language: 'c')
endif

if get_option('buildtype') == 'release'
  add_project_arguments(
    cc.get_supported_arguments(
      [
        '-U_FORTIFY_SOURCE', # first undefine it since it is already defined on macOS
        '-D_FORTIFY_SOURCE=3', # incompatible with sanitizers, so do it in the release build
      ],
    ),
    language: 'c',
  )
endif

if get_option('debug-ship')
  add_project_arguments(
    [
      '-DDEBUG_SHIP',
    ],
    language: 'c',
  )
endif

# Link: https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++
# Link: https://github.com/intel/igsc/blob/master/meson.build
add_project_arguments(
  cc.get_supported_arguments(
    [
      '-Wformat',
      '-Wformat=2',
      '-Wformat-security',
      '-Wimplicit-fallthrough',
      '-Wvla',
      '-Wconversion',
      '-fstack-protector-strong',
      # fix insane behavior that we do not want
      '-fstrict-flex-arrays=3',
      '-fno-delete-null-pointer-checks',
      '-fno-strict-aliasing',
      '-fwrapv',
    ],
    checked: 'warn',
  ),
  language: 'c',
)

raylib = dependency('raylib')

srcs = [
  'main.c',
  'game.c',
  'input.c',
  'player.c',
  'asteroid.c',
  'screenController.c',
  'screenDimensions.c',
  'shot.c',
  'fontLoader.c',
  'objects.c',
  'title.c',
  'malloc.c',
  'blink.c',
  'asteroidShower.c',
  'globalActions.c',
  'soundFx.c',
]

if host_machine.system() == 'emscripten'
  web_link_args = [
    '-sUSE_GLFW=3',
    '-sASYNCIFY',
    '-sTOTAL_MEMORY=64MB',
    '-sFORCE_FILESYSTEM=1',
    '-O3',
  ]

  executable(
    'asteroids',
    srcs,
    dependencies: [raylib],
    link_args: web_link_args,
    name_suffix: 'html',
  )
else
  executable('asteroids', srcs, install: true, dependencies: [raylib])
endif
