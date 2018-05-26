# add source dir
aux_source_directory(. SRC_LIST_FUNCTIONAL)

# add executable
add_executable(Byte2String ${SRC_LIST_FUNCTIONAL})
