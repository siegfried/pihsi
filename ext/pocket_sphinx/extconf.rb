require 'mkmf'

unless find_header 'pocketsphinx_export.h', '/usr/local/include/pocketsphinx', '/usr/include/pocketsphinx'
  abort 'cannot find pocketsphinx, please install it.'
end
unless find_header 'cmd_ln.h', '/usr/local/include/sphinxbase', '/usr/include/sphinxbase'
  abort 'cannot find sphinxbase, please install it.'
end
find_library 'pocketsphinx', 'ps_init'
find_library 'sphinxbase', 'cmd_ln_init'

create_makefile 'pocket_sphinx/pocket_sphinx'
