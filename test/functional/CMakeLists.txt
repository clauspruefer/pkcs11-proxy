# add source dir
aux_source_directory(. SRC_LIST_FUNCTIONAL)

# add executable
add_executable(Byte2String Bytes2String.cpp)
add_executable(Timeout Timeout.cpp)
