#include <iostream>
#include <string>

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