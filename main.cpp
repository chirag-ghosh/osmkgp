// system includes
#include <string.h>

#include <ios>
#include <iostream>
#include <limits>
#include <vector>

// models
#include "graph.hpp"
#include "heap.hpp"
#include "node.hpp"
#include "way.hpp"

// helpers
#include "helper.cpp"

//heap methods
#include "minHeap.cpp"

//dijkstra methods
#include "dijkstra.cpp"

// rapidxml parser
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

// namespaces
using namespace std;
using namespace rapidxml;

int getNodeCount(xml_node<> *rootNode, const char *nodeName) {
    int nodeCount = 0;
    for (xml_node<> *myNode = rootNode->first_node(nodeName); myNode; myNode = myNode->next_sibling()) {
        if (strcmp(myNode->name(), nodeName) == 0)
            nodeCount++;
    }
    return nodeCount;
}

Node *getNodeList(xml_node<> *rootNode, int count) {
    static Node *nodeList = (Node *)malloc(count * sizeof(Node));

    int tempCount = 0;

    for (xml_node<> *myNode = rootNode->first_node("node"); myNode; myNode = myNode->next_sibling()) {
        if (strcmp(myNode->name(), "node") != 0) continue;
        Node newNode;
        newNode.ID = stoll(myNode->first_attribute("id")->value());
        newNode.visible = myNode->first_attribute("visible")->value();
        newNode.version = atoi(myNode->first_attribute("version")->value());
        newNode.changeset = atoi(myNode->first_attribute("changeset")->value());
        newNode.timestamp = myNode->first_attribute("timestamp")->value();
        newNode.user = myNode->first_attribute("user")->value();
        newNode.uid = atoi(myNode->first_attribute("uid")->value());
        newNode.latitude = atof(myNode->first_attribute("lat")->value());
        newNode.longitude = atof(myNode->first_attribute("lon")->value());

        bool foundName = false;
        for (xml_node<> *tagNode = myNode->first_node("tag"); tagNode; tagNode = tagNode->next_sibling()) {
            if (strcmp(tagNode->name(), "tag") != 0) continue;
            if (strcmp(tagNode->first_attribute("k")->value(), "name") == 0) {
                newNode.name = tagNode->first_attribute("v")->value();
                foundName = true;
            }
        }
        if (!foundName) newNode.name = "";

        nodeList[tempCount++] = newNode;
    }

    return nodeList;
}

Way *getWayList(xml_node<> *rootNode, int count) {
    static Way *wayList = (Way *)malloc(count * sizeof(Way));

    int tempCount = 0;

    for (xml_node<> *myNode = rootNode->first_node("way"); myNode; myNode = myNode->next_sibling()) {
        if (strcmp(myNode->name(), "way") != 0) continue;
        Way newNode;
        newNode.ID = atoi(myNode->first_attribute("id")->value());
        newNode.visible = (char *)malloc((strlen(myNode->first_attribute("visible")->value()) + 1) * sizeof(char));
        newNode.visible = myNode->first_attribute("visible")->value();
        newNode.version = atoi(myNode->first_attribute("version")->value());
        newNode.changeset = atoi(myNode->first_attribute("changeset")->value());
        newNode.timestamp = myNode->first_attribute("timestamp")->value();
        newNode.user = myNode->first_attribute("user")->value();
        newNode.uid = atoi(myNode->first_attribute("uid")->value());
        for (xml_node<> *tagNode = myNode->first_node("nd"); tagNode; tagNode = tagNode->next_sibling()) {
            if (strcmp(tagNode->name(), "nd") != 0) continue;
            newNode.nodeRefList.push_back(stoll(tagNode->first_attribute("ref")->value()));
        }

        bool foundName = false;
        for (xml_node<> *tagNode = myNode->first_node("tag"); tagNode; tagNode = tagNode->next_sibling()) {
            if (strcmp(tagNode->name(), "tag") == 0 && strcmp(tagNode->first_attribute("k")->value(), "name") == 0) {
                newNode.name = tagNode->first_attribute("v")->value();
                foundName = true;
            }
        }
        if (!foundName) newNode.name = "";

        wayList[tempCount++] = newNode;
    }

    return wayList;
}

void getNodeByName(Node *nodeList, int nodeCount) {
    string nameSearch;
    cout << "Enter a name to search : ";
    getline(cin, nameSearch);

    for (int i = 0; i < nodeCount; i++) {
        if (isSubstring(nodeList[i].name, nameSearch))
            cout << nodeList[i].ID << " " << nodeList[i].name << endl;
    }
}

Node getNodeFromUser(Node *nodeList, int nodeCount) {
    long long int ID;
    cout << "Enter a Node ID : ";
    cin >> ID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < nodeCount; i++) {
        if (nodeList[i].ID == ID) return nodeList[i];
    }

    cout << "No such ID found" << endl;
    return {0, NULL, 0, 0, NULL, 0, 0, 0, NULL, NULL};
}

int getNodeIndexFromID(Node *nodeList, int nodeCount, long long int ID) {
    for (int i = 0; i < nodeCount; i++) {
        if (nodeList[i].ID == ID) return i;
    }

    return -1;
}

vector<NodeWithDistance> getNodeDistanceVector(Node a, Node *nodeList, int nodeCount) {
    static vector<NodeWithDistance> distanceList;
    for (int i = 0; i < nodeCount; i++) {
        distanceList.push_back({nodeList[i], getDisplacement(a, nodeList[i])});
    }
    return distanceList;
}

Way getWayByName(Way *wayList, int wayCount) {
    string nameSearch;
    cout << "Enter a name to search : ";
    getline(cin, nameSearch);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < wayCount; i++) {
        if (isSubstring(wayList[i].name, nameSearch))
            cout << wayList[i].ID << " " << wayList[i].name << endl;
    }
    return wayList[0];
}

Graph *makeGraphFromWays(Way *wayList, int wayCount, Node *nodeList, int nodeCount) {
    Graph *G = (Graph *)malloc(sizeof(Graph));
    G = createGraph(nodeCount);

    //the dijkstra code will use index of node in nodeList
    for (int i = 0; i < wayCount; i++) {
        for (int j = 0; j < wayList[i].nodeRefList.size() - 1; j++) {
            int srcIndex = getNodeIndexFromID(nodeList, nodeCount, wayList[i].nodeRefList[j]);
            int destIndex = getNodeIndexFromID(nodeList, nodeCount, wayList[i].nodeRefList[j + 1]);
            addEdge(G, srcIndex, destIndex, getDisplacement(nodeList[srcIndex], nodeList[destIndex]));
        }
    }

    return G;
}

int main(int argc, char const *argv[]) {
    // get xml root node from file
    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile("map.osm");
    doc.parse<0>(xmlFile.data());
    xml_node<> *rootNode = doc.first_node("osm");
    cout << "OSM file found." << endl;

    int nodeCount = getNodeCount(rootNode, "node");
    int wayCount = getNodeCount(rootNode, "way");

    cout << endl
         << "Total nodes : " << nodeCount << endl;
    cout << "Total ways : " << wayCount << endl;

    Node *nodeList = (Node *)malloc(nodeCount * sizeof(Node));
    Way *wayList = (Way *)malloc(wayCount * sizeof(Way));
    nodeList = getNodeList(rootNode, nodeCount);
    wayList = getWayList(rootNode, wayCount);

    cout << endl
         << "Loaded nodes and ways with attributes to memory" << endl
         << endl;

    cout << "Creating graph from ways" << endl;
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph = makeGraphFromWays(wayList, wayCount, nodeList, nodeCount);
    cout << "Created graph" << endl
         << endl;

    cout << "Welcome to main-menu" << endl;
    int choice = 0, subChoice = 0;
    do {
        cout << "1. Search a node or way by name" << endl;
        cout << "2. Find k-closest nodes to a given node (crow-fly distance)" << endl;
        cout << "3. Find the shortest path between 2 nodes via ways" << endl;
        cout << "Enter options 1-3. Enter -1 to exit. Enter anything else to repeat this message" << endl
             << endl;

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "Enter 1 to search nodes and 2 to search ways." << endl;
                cin >> subChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (subChoice == 1)
                    getNodeByName(nodeList, nodeCount);
                else if (subChoice == 2)
                    getWayByName(wayList, wayCount);
                else
                    cout << "Invalid Choice" << endl;
                break;

            case 2: {
                Node selectedNode = getNodeFromUser(nodeList, nodeCount);
                if (selectedNode.ID != 0) {
                    vector<NodeWithDistance> nodeWithDistanceList = getNodeDistanceVector(selectedNode, nodeList, nodeCount);
                    nodeWithDistanceList = sortNodeWithDistanceList(nodeWithDistanceList);
                    cout << "Enter the number of closest nodes you need : ";
                    int k;
                    cin >> k;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "ID\tDistance\tName" << endl;
                    for (int i = 1; i <= k; i++) {
                        cout << nodeWithDistanceList[i].node.ID << "\t" << nodeWithDistanceList[i].distance << "\t" << nodeWithDistanceList[i].node.name << endl;
                    }
                }
                break;
            }

            case 3: {
                cout << "Select a source node" << endl;
                Node srcNode = getNodeFromUser(nodeList, nodeCount);
                cout << "Select a destination node" << endl;
                Node destNode = getNodeFromUser(nodeList, nodeCount);

                cout << "Using Djikstra Shortest path algorithm to find the shortest path for you....." << endl;
                double distance = dijkstra(graph, getNodeIndexFromID(nodeList, nodeCount, srcNode.ID), getNodeIndexFromID(nodeList, nodeCount, destNode.ID));

                if (distance == -1)
                    cout << "Sorry. No road connects these IDs" << endl;
                else
                    cout << "The shortest distance is : " << distance << endl;
            }

            default:
                break;
        }
    } while (choice != -1);

    cout << "Exiting. Have a nice day" << endl;

    return 0;
}
