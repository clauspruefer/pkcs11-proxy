# cmake settings
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

# set version
set (TLSProxy_VERSION_MAJOR 0)
set (TLSProxy_VERSION_MINOR 1)

# project settings
project(wcdx-pkcs11-tls-proxy)

# switch build type here
SET(CMAKE_BUILD_TYPE DEBUG)

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

# add subdirs
add_subdirectory(lib)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(test)
