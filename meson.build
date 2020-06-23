project('fudel', 'cpp',
 version : '0.1',
 default_options : ['warning_level=3', 'cpp_std=c++17'])

sqlite3_dep = dependency('sqlite3')
gtkmm_dep = dependency('gtkmm-3.0')
notify_dep = dependency('libnotify')


executable('fudel', 'fudel.cpp', 'gui.cpp', dependencies : [sqlite3_dep, gtkmm_dep, notify_dep], install : false)
