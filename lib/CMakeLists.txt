# cmake settings
cmake_minimum_required(VERSION 2.8)

# include dirs
include_directories(/usr/include/libxml2)

# add custom targets
add_custom_target(LibConstant SOURCES ./Constant.hpp.in)
add_custom_target(LibDebug SOURCES ./Debug.cpp)
add_custom_target(LibFiledescriptor SOURCES ./Filedescriptor.cpp)
add_custom_target(LibFiledescriptorHeader SOURCES ./Filedescriptor.hpp)
add_custom_target(LibSocket SOURCES ./Socket.cpp)
add_custom_target(LibSocketHeaderClient SOURCES ./Socket.hpp)
add_custom_target(LibSSLSocket SOURCES ./SSLSocket.cpp)
add_custom_target(LibSSLSocketHeader SOURCES ./SSLSocket.hpp)
add_custom_target(LibSSLHandshake SOURCES ./SSLHandshake.cpp)
add_custom_target(LibSSLHandshakeHeader SOURCES ./SSLHandshake.hpp)
add_custom_target(LibConnection SOURCES ./Connection.cpp)
add_custom_target(LibConnectionHeader SOURCES ./Connection.hpp)
add_custom_target(LibConfig SOURCES ./Config.cpp)
add_custom_target(LibConfigHeader SOURCES ./Config.hpp)
add_custom_target(LibTimeout SOURCES ./Timeout.cpp)
add_custom_target(LibTimeoutHeader SOURCES ./Timeout.hpp)
add_custom_target(LibHelper SOURCES ./Helper.hpp)
