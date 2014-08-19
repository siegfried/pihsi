require 'mkmf'

dir_config 'pocketsphinx', ['/usr/local/include/pocketsphinx'], ['/usr/local/lib'] # a workaround for http://stackoverflow.com/questions/25388652/makemakefile-cannot-find-headers

find_header 'pocketsphinx.h', '/usr/local/include/pocketsphinx' # if this work, #dir_config is not needed.
find_header 'cmd_ln.h', '/usr/local/include/sphinxbase'
find_library 'pocketsphinx', 'ps_init'
find_library 'sphinxbase', 'cmd_ln_init'

create_makefile 'pocket_sphinx/pocket_sphinx'
