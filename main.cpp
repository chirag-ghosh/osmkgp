#include <iostream>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace std;
using namespace rapidxml;

int getNodeCount(xml_node<> *rootNode, char *nodeName) {
    int nodeCount = 0;
    for (xml_node<> *myNode = rootNode->first_node(nodeName); myNode; myNode = myNode->next_sibling()) {
        nodeCount++;
    }
    return nodeCount;
}

int main(int argc, char const *argv[]) {
    rapidxml::file<> xmlFile("map.osm");
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    xml_node<> *root_node;
    root_node = doc.first_node("osm");

    cout << endl
         << "Total nodes : " << getNodeCount(root_node, "node") << endl;
    cout << endl
         << "Total ways : " << getNodeCount(root_node, "way") << endl;
    return 0;
}
