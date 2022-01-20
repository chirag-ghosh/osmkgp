#include <iostream>

using namespace std;

MinHeapNode* newMinHeapNode(int v, double dist) {
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->position = (int*)malloc(
        capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a,
                     MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist <
            minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist <
            minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        MinHeapNode* smallestNode =
            minHeap->array[smallest];
        MinHeapNode* idxNode =
            minHeap->array[idx];

        minHeap->position[smallestNode->v] = idx;
        minHeap->position[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;
    MinHeapNode* root =
        minHeap->array[0];
    MinHeapNode* lastNode =
        minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->position[root->v] = minHeap->size - 1;
    minHeap->position[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap* minHeap,
                 int v, double dist) {
    int i = minHeap->position[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist <
                    minHeap->array[(i - 1) / 2]->dist) {
        minHeap->position[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->position[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(MinHeap* minHeap, int v) {
    if (minHeap->position[v] < minHeap->size)
        return true;
    return false;
}