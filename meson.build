project(
	'mathulator',
	'c',
	default_options: {
		'c_std': 'c23',
		'buildtype': 'debugoptimized',
		'warning_level': '3',
	},
)

executable(
	'mathulator',
	'src/evaluate.c',
	'src/main.c',
	'src/tree.c',
	'src/utils.c',
	c_args: [
		'-D_DEFAULT_SOURCE',
		'-D_POSIX_C_SOURCE=200809L',
		'-DVERSION="@0@"'.format(meson.project_version()),
		'-Wno-empty-translation-unit',
		'-Wno-pragma-once-outside-header',
	],
	link_args: [],
	dependencies: [],
	install: true,
)
