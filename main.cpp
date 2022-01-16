// system includes
#include <string.h>

#include <iostream>
#include <vector>

// models
#include "node.hpp"
#include "way.hpp"

// rapidxml parser
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

// namespaces
using namespace std;
using namespace rapidxml;

xml_node<> *getRootNode(rapidxml::file<> xmlFile) {
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    xml_node<> *rootNode = doc.first_node("osm");

    return rootNode;
}

int getNodeCount(xml_node<> *rootNode, char *nodeName) {
    int nodeCount = 0;
    for (xml_node<> *myNode = rootNode->first_node(nodeName); myNode; myNode = myNode->next_sibling()) {
        nodeCount++;
    }
    return nodeCount;
}

Node *getNodeList(xml_node<> *rootNode, int count) {
    static Node *nodeList = (Node *)malloc(count * sizeof(Node));

    int tempCount = 0;

    for (xml_node<> *myNode = rootNode->first_node("node"); myNode; myNode = myNode->next_sibling()) {
        Node newNode;
        newNode.ID = atoi(myNode->first_attribute("id")->value());
        newNode.visible = myNode->first_attribute("visible")->value();
        newNode.version = atoi(myNode->first_attribute("version")->value());
        newNode.changeset = atoi(myNode->first_attribute("changeset")->value());
        newNode.timestamp = myNode->first_attribute("timestamp")->value();
        newNode.user = myNode->first_attribute("user")->value();
        newNode.uid = atoi(myNode->first_attribute("uid")->value());
        newNode.latitude = atof(myNode->first_attribute("lat")->value());
        newNode.longitude = atof(myNode->first_attribute("lon")->value());

        for (xml_node<> *tagNode = myNode->first_node("tag"); tagNode; tagNode = tagNode->next_sibling()) {
            if (strcmp(tagNode->first_attribute("k")->value(), "name") == 0) {
                newNode.name = tagNode->first_attribute("v")->value();
            }
        }

        nodeList[tempCount++] = newNode;
    }

    return nodeList;
}

Way *getWayList(xml_node<> *rootNode, int count) {
    static Way *wayList = (Way *)malloc(count * sizeof(Way));

    int tempCount = 0;

    for (xml_node<> *myNode = rootNode->first_node("way"); myNode; myNode = myNode->next_sibling()) {
        Way newNode;
        newNode.ID = atoi(myNode->first_attribute("id")->value());
        newNode.visible = myNode->first_attribute("visible")->value();
        newNode.version = atoi(myNode->first_attribute("version")->value());
        newNode.changeset = atoi(myNode->first_attribute("changeset")->value());
        newNode.timestamp = myNode->first_attribute("timestamp")->value();
        newNode.user = myNode->first_attribute("user")->value();
        newNode.uid = atoi(myNode->first_attribute("uid")->value());

        for (xml_node<> *tagNode = myNode->first_node("nd"); tagNode; tagNode = tagNode->next_sibling()) {
            newNode.nodeRefList.push_back(atoi(myNode->first_attribute("ref")->value()));
        }

        for (xml_node<> *tagNode = myNode->first_node("tag"); tagNode; tagNode = tagNode->next_sibling()) {
            if (strcmp(tagNode->first_attribute("k")->value(), "name") == 0) {
                newNode.name = tagNode->first_attribute("v")->value();
            }
        }

        wayList[tempCount++] = newNode;
    }

    return wayList;
}

int main(int argc, char const *argv[]) {
    // get xml root node from file
    xml_node<> *rootNode = getRootNode("map.osm");

    int nodeCount = getNodeCount(rootNode, "node");
    int wayCount = getNodeCount(rootNode, "way");

    Node *nodeList = (Node *)malloc(nodeCount * sizeof(Node));
    Way *wayList = (Way *)malloc(wayCount * sizeof(Way));

    cout << endl
         << "Total nodes : " << getNodeCount(rootNode, "node") << endl;
    cout << endl
         << "Total ways : " << getNodeCount(rootNode, "way") << endl;

    nodeList = getNodeList(rootNode, nodeCount);
    wayList = getWayList(rootNode, wayCount);
    return 0;
}
