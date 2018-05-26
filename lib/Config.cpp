#include "Config.hpp"

using namespace std;

string Config::Type = "client";
string Config::ListenHost = "";
uint16_t Config::ListenPort = 0;
string Config::RemoteHost = "";
uint16_t Config::RemotePort = 0;
string Config::ProxyHost = "";
uint16_t Config::ProxyPort = 0;
string Config::CACertFile = "";
string Config::CertFile = "";
string Config::KeyFile = "";
string Config::CardSlot = "";
string Config::ContainerId = "";

void Config::setup()
{
    xmlDocPtr XMLDocument = NULL;

    xmlKeepBlanksDefault(0);

    XMLDocument = xmlParseFile(CONFIG_FILE);

    if (XMLDocument == NULL) {
        cout << "Config file not readable" << endl;
        exit(EXIT_FAILURE);
    }

    xmlNodePtr Element = NULL;

    Element = xmlDocGetRootElement(XMLDocument);

    if (Element == NULL) {
        cout << "No Root XML Element found" << endl;
        exit(EXIT_FAILURE);
    }

    Element = Element->xmlChildrenNode;

    while (Element != NULL) {

        if (xmlStrcmp(Element->name, (const xmlChar*)"client") == 0) {
            if (Element != NULL) {
                Config::parseClientSettings(XMLDocument, Element);
            }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"server") == 0) {
                if (Element != NULL) {
                    Config::parseServerSettings(XMLDocument, Element);
                }
        }

        Element = Element->next;

    }

    //- release document mem structures
    xmlFreeDoc(XMLDocument);

    //- force return kernel memory
    malloc_trim(0);
}

void Config::parseClientSettings(xmlDocPtr XMLDocument, xmlNodePtr Element)
{
    Type = "client";

    Element = Element->xmlChildrenNode;

    while (Element != NULL) {

        char const *res = "";

        if (xmlStrcmp(Element->name, (const xmlChar*)"ListenHost") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ListenHost = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"ListenPort") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ListenPort = atoi(res); }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"RemoteHost") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { RemoteHost = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"RemotePort") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { RemotePort = atoi(res); }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"CACertFile") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { CACertFile = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"CertFile") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { CertFile = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"KeyFile") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { KeyFile = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"CardSlot") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { CardSlot = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"ContainerId") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ContainerId = res; }
        }

        Element = Element->next;

    }

}

void Config::parseServerSettings(xmlDocPtr XMLDocument, xmlNodePtr Element)
{
    Type = "server";

    Element = Element->xmlChildrenNode;

    while (Element != NULL) {

        char const *res = "";

        if (xmlStrcmp(Element->name, (const xmlChar*)"ListenHost") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ListenHost = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"ListenPort") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ListenPort = atoi(res); }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"ProxyHost") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ProxyHost = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"ProxyPort") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ProxyPort = atoi(res); }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"CACertFile") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { CACertFile = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"CertFile") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { CertFile = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"KeyFile") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { KeyFile = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"CardSlot") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { CardSlot = res; }
        }

        if (xmlStrcmp(Element->name, (const xmlChar*)"ContainerId") == 0) {
            res = (char*)xmlNodeListGetString(XMLDocument, Element->xmlChildrenNode, 1);
            if (res != NULL) { ContainerId = res; }
        }

        Element = Element->next;

    }

}
