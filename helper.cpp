#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#define PI 3.141592653589793238462643383279
#define EARTH_RADIUS 6371

using namespace std;

// checks if b is the substring of a
bool isSubstring(string a, string b) {
    // b is larger. not possible if substring
    if (b.length() > a.length()) return false;

    for (int i = 0; i <= a.length() - b.length(); i++) {
        if (a.substr(i, b.length()).compare(b) == 0) return true;
    }

    return false;  // no match found
}

// function that converts degree to radians.
double toRadian(double deg) { return deg * PI / 180; }

// return crow fly distance between two nodes
double getDisplacement(Node a, Node b) {
    double latA = toRadian(a.latitude);
    double lonA = toRadian(a.longitude);
    double latB = toRadian(b.latitude);
    double lonB = toRadian(b.longitude);

    double dlat = latA - latB;
    double dlon = lonA - lonB;
    double k = pow(sin(0.5 * dlat), 2);
    double l = cos(latA) * cos(latB) * pow(sin(0.5 * dlon), 2);
    double q = sqrt(k + l);
    double dis = 2 * EARTH_RADIUS * asin(q);
    return dis;
}

bool nodeWithDistanceSorter(NodeWithDistance const& lhs, NodeWithDistance const& rhs) {
    return (lhs.distance < rhs.distance);
}

vector<NodeWithDistance> sortNodeWithDistanceList(vector<NodeWithDistance> distanceList) {
    sort(distanceList.begin(), distanceList.end(), &nodeWithDistanceSorter);
    return distanceList;
}