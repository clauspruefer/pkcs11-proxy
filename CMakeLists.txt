# cmake settings
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

# set version
set (TLSProxy_VERSION_MAJOR 0)
set (TLSProxy_VERSION_MINOR 1)

# project settings
project(wcdx-pkcs11-tls-proxy)

# switch build type here
SET(CMAKE_BUILD_TYPE RELEASE)

# find openssl package
find_package(OpenSSL)
if (OpenSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR}/openssl)
    MESSAGE(STATUS "FOUND OPENSSL INCLUDE DIR: " ${OPENSSL_INCLUDE_DIR})
endif (OpenSSL_FOUND)

# find libxml2 package
find_package(LibXml2)
if (LibXml2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIR})
    MESSAGE(STATUS "FOUND LIBXML2 INCLUDE DIR: " ${LIBXML2_INCLUDE_DIR})
endif (LibXml2_FOUND)

# libuuid.so ubuntu x86_64
if (EXISTS "/usr/lib/x86_64-linux-gnu/libuuid.so")
    set(UUID_LIBRARY /usr/lib/x86_64-linux-gnu/libuuid.so)
endif()

# libuuid.so gentoo x86_64
if (EXISTS "/usr/lib64/libuuid.so")
    set(UUID_LIBRARY /usr/lib64/libuuid.so)
endif()

# libuuid.so gentoo x86_64 user install
if (EXISTS "/usr/lib64/libuuid.so")
    set(UUID_LIBRARY /usr/local/lib64/libuuid.so)
endif()

# find opensc/openssl libraries
find_file(OPENSSL_OPENSC_ENGINE_PKCS11_LIBRARY NAMES libpkcs11.so engine_pkcs11.so PATHS /usr/lib/engines /usr/local/lib/engines)
find_file(OPENSC_ENGINE_PKCS11_LIBRARY opensc-pkcs11.so PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu)

MESSAGE(STATUS "FOUND OPENSSL OPENSC ENGINE SHARED OBJECT: " ${OPENSSL_OPENSC_ENGINE_PKCS11_LIBRARY})
MESSAGE(STATUS "FOUND OPENSC PKCS11 SHARED OBJECT: " ${OPENSC_ENGINE_PKCS11_LIBRARY})

configure_file( lib/Constant.hpp.in lib/Constant.hpp )

# set gcc/g++ compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -Wl,-z,relro,-z,now -O2 -fno-exceptions -fno-rtti -fomit-frame-pointer -fstack-protector -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0 -fno-stack-protector -std=c++11 -pthread")

# add subdirs
add_subdirectory(lib)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(test)
