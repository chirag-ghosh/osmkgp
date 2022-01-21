typedef struct _adjListNode {
    long long int index;
    double weight;
    struct _adjListNode* next;
} AdjListNode;

typedef struct _adjList {
    AdjListNode* head;
} AdjList;

typedef struct _graph {
    int V;
    AdjList* adjList;
} Graph;