project(
  'Asteroids',
  'c',
  default_options: {
    'buildtype': 'debugoptimized',
    'warning_level': '2',
    'werror': true,
    'b_pie': true,
    'c_std': 'c23',
    'b_sanitize': [
      'address',
      'undefined',
      'leak',
    ],
    'b_lundef': false, # mesons says sanitizers can't handle this
  },
  meson_version: '>=1.10.0',
)
cc = meson.get_compiler('c')

if get_option('buildtype') == 'release'
  add_project_arguments(
    cc.get_supported_arguments(
      [
        '-D_FORTIFY_SOURCE=3', # incompatible with sanitizers, so do it in the release build
      ],
    ),
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
]
executable('asteroids', srcs, install: true, dependencies: [raylib])
