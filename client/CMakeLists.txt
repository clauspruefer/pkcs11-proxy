# cmake settings
cmake_minimum_required(VERSION 2.8)

# set version
set (TLSProxy_VERSION_MAJOR 0)
set (TLSProxy_VERSION_MINOR 1)

# project settings
project(wcdx-pkcs11-tls-proxy-client)
aux_source_directory(. SRC_LIST)

# add executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# set libs
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES} ${LIBXML2_LIBRARIES} ${UUID_LIBRARY})
