# cmake settings
cmake_minimum_required(VERSION 2.8)

# set version
set (TLSProxy_VERSION_MAJOR 0)
set (TLSProxy_VERSION_MINOR 1)

# project settings
project(wcdx-pkcs11-tls-proxy-client)
aux_source_directory(. SRC_LIST)

# set g++ compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O2 -fomit-frame-pointer -fstack-protector -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0 -fno-stack-protector -std=c++11 -pthread")

# add executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# libuuid.so ubuntu x86_64
if (EXISTS "/usr/lib/x86_64-linux-gnu/libuuid.so")
    set(UUID_LIBRARY /usr/lib/x86_64-linux-gnu/libuuid.so)
endif()

# libuuid.so gentoo x86_64
if (EXISTS "/usr/lib64/libuuid.so")
    set(UUID_LIBRARY /usr/lib64/libuuid.so)
endif()

# set libs
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES} ${LIBXML2_LIBRARIES} ${UUID_LIBRARY})
