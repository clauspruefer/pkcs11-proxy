# add source dir
aux_source_directory(. SRC_LIST_CONNECTION)

# add executable
add_executable(Connection ${SRC_LIST_CONNECTION})

set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O2 -fomit-frame-pointer -fstack-protector -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0 -fno-stack-protector -std=c++11 -pthread")
