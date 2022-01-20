typedef struct _node {
    long long int ID;
    char* visible;
    int version;
    int changeset;
    char* user;
    int uid;
    double latitude;
    double longitude;
    char* timestamp;
    char* name;
} Node;