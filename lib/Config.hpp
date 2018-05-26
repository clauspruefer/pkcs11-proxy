#ifndef Config_hpp
#define Config_hpp

#include <stdio.h>
#include <malloc.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string>
#include <iostream>

using namespace std;

class Config
{

 public:
    static void setup();

    static string Type;
    static string ListenHost;
    static uint16_t ListenPort;
    static string RemoteHost;
    static uint16_t RemotePort;
    static string ProxyHost;
    static uint16_t ProxyPort;
    static string CACertFile;
    static string CertFile;
    static string KeyFile;
    static string CardSlot;
    static string ContainerId;

 private:
    static void parseClientSettings(xmlDocPtr, xmlNodePtr);
    static void parseServerSettings(xmlDocPtr, xmlNodePtr);

};

#endif
