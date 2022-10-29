project('korai', 'cpp')
gtkdep = [dependency('gtkmm-3.0'), dependency('webkit2gtk-4.0'), dependency('libarchive')]
vtedep = [dependency('vte-2.91')]
saveposdep = [dependency('webkit2gtk-web-extension-4.0')]
executable('korai', 'src/main.cpp', dependencies : gtkdep + vtedep, install : true, install_dir : '/usr/bin/')
#TODO setup the compiler flag for the less-featured korai-nodownload version
#It is the -DNODOWNLOAD flag
#executable('korai-nodownload', 'src/main.cpp', dependencies : gtkdep)

library('savepos', 'src/extensions/pos_save.cpp', dependencies : saveposdep, install : true, install_dir : '/usr/share/korai/korai-extensions/')