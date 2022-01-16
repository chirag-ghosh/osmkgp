#include <vector>

typedef struct _way {
    int ID;
    char* visible;
    int version;
    int changeset;
    char* timestamp;
    char* user;
    int uid;
    std::vector<int> nodeRefList;
    char* name;
} Way;