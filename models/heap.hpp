typedef struct _minHeapNode {
    int v;
    double dist;
} MinHeapNode;

typedef struct _minHeap {
    int size;
    int capacity;
    int *position;
    MinHeapNode **array;
} MinHeap;