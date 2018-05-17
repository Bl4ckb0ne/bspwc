bspwc_src = files(
    'backend.c',
    'config.c',
	'cursor.c',
	'desktop.c',
    'input.c',
    'main.c',
    'output.c',
    'server.c',
    'window.c',
    'xdg_surface_v6.c',
)

bspwc_deps = [
    wayland_server,
    wlroots,
    pixman,
]

executable(
    'bspwc',
    bspwc_src,
    include_directories: [inc_folder],
    dependencies: [bspwc_deps, wlr_protos],
    install: true
)
