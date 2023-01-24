project('game', 'c', 'cpp')

sdl = dependency('SDL2', required : true)

# copy important assets to the build folder
run_command('cp', '-r', 'gameclient/gfx', 'build', check: true)
run_command('cp', '-r', 'gameclient/data', 'build', check: true)
run_command('cp', '-r', 'gameclient/music', 'build', check: true)
run_command('cp', '-r', 'gameclient/sound', 'build', check: true)

executable(
  'game',
  [
      'gameclient/src/block.c',
      'gameclient/src/camera.c',
      'gameclient/src/common.h',
      'gameclient/src/defs.h',
      'gameclient/src/draw.c',
      'gameclient/src/entities.c',
      'gameclient/src/init.c',
      'gameclient/src/input.c',
      'gameclient/src/io.c',
      'gameclient/src/main.c',
      'gameclient/src/map.c',
      'gameclient/src/pizza.c',
      'gameclient/src/platform.c',
      'gameclient/src/player.c',
      'gameclient/src/sound.c',
      'gameclient/src/stage.c',
      'gameclient/src/structs.h',
      'gameclient/src/text.c',
      'gameclient/src/textures.c',
      'gameclient/src/util.c',
      'gameclient/src/sockets.c',
      'gameclient/src/sockets.h',
      'gameclient/src/server.h',
      'gameclient/src/server.c',
      'gameclient/src/misc.h',
      'gameclient/src/misc.c',
  ],

  c_args: [
      '-g',
      '-Wall',
      '-Wempty-body',
      '-Werror',
      '-Werror=maybe-uninitialized',
      '-Warray-bounds',
      '-D_REENTRANT',
      '-D_GNU_SOURCE=1',
      '--std=c2x',
      '-lpthread'
      ],

  link_args: [
    '-lm',
    '-lSDL2_mixer',
    '-lSDL2_image',
    '-lpthread'
    ],

  include_directories: include_directories('gameclient/src'),
  dependencies: [sdl]
)

executable(
  'server',

  [
    'server/src/main.cpp',
    'server/libsockets/sockets.cpp',
    'server/libsockets/sockets.hpp'
    ],

  c_args: [
    '-g',
    '-Wall',
    '-fsanitize=address'
    ],

  link_args: [
    '-fsanitize=address'
    ],

  include_directories: include_directories('server/libsockets')
)

executable(
  'libsocket_test_server',

  [
    'server/tests/libsocket_test_server.cpp',
    'server/libsockets/sockets.cpp',
    'server/libsockets/sockets.hpp'
    ],

  c_args: [
    '-g',
    '-Wall',
    '-fsanitize=address'
    ],

  link_args: [
    '-fsanitize=address'
    ],

  include_directories: include_directories('server/libsockets')
)


executable(
  'libsocket_test_client',

  [
    'server/tests/libsocket_test_client.cpp',
    'server/libsockets/sockets.hpp',
    'server/libsockets/sockets.cpp'
    ],

  c_args: [
    '-g',
    '-Wall',
    '-fsanitize=address'
    ],

  link_args: [
    '-fsanitize=address'
    ],

  include_directories: include_directories('server/libsockets')
)


