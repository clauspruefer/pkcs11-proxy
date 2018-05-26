#ifndef ConstantSSLServer_hpp
#define ConstantSSLServer_hpp

#define CONFIG_FILE "config.xml"

#define UUID_SIZE 36

#define TMP_BUFFER_SIZE 4096*10

#define MAX_TUNNELED_CONNECTIONS 10

#define PROXY_CMD_NEW_CONNECTION "[SYS_Connection_NEW]"
#define PROXY_CMD_CLOSE_CONNECTION "[SYS_Connection_CLOSE]"
#define PROXY_CMD_KEEPALIVE "[SYS_Connection_KEEPALIVE]"

#define TIMEOUT_WAIT_MSG_SEC -1
#define SOCKET_IDLE_TIMEOUT_SEC 3600
#define SSLSOCKET_KEEPALIVE_PACKAGE_INTERVAL_SEC 60
#define CLIENT_REKEY_INTERVAL_SEC 600
#define SERVER_IDLE_INTERVAL_SEC 180

#define PAYLOAD_MARKER_PREFIX "<SYS_DATA_PROXY_v01_DATA_START_TAG>"
#define PAYLOAD_MARKER_POSTFIX "</SYS_DATA_PROXY_v01_DATA_END_TAG>"

#define LOG_LEVEL 50

#endif