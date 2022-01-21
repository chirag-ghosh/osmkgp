#include <iostream>

using namespace std;

int getParent(int index) {
    return (index - 1) / 2;
}

int getLeftChild(int index) {
    return (2 * index + 1);
}

int getRightChild(int index) {
    return (2 * index + 2);
}

int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

bool isInMinHeap(MinHeap* minHeap, int v) {
    return minHeap->position[v] < minHeap->size;
}

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

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int index) {
    int min, left, right;

    min = index;
    left = getLeftChild(index);
    right = getRightChild(index);

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[min]->dist)
        min = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[min]->dist)
        min = right;

    if (min != index) {
        minHeap->position[minHeap->array[min]->v] = index;
        minHeap->position[minHeap->array[index]->v] = min;

        swapMinHeapNode(&minHeap->array[min], &minHeap->array[index]);

        minHeapify(minHeap, min);
    }
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap)) return NULL;

    MinHeapNode* root = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    minHeap->position[minHeap->array[0]->v] = minHeap->size - 1;
    minHeap->position[minHeap->array[minHeap->size - 1]->v] = 0;

    minHeap->size--;

    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap* minHeap, int v, double dist) {
    int i = minHeap->position[v];
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[getParent(i)]->dist) {
        minHeap->position[minHeap->array[i]->v] = getParent(i);
        minHeap->position[minHeap->array[getParent(i)]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[getParent(i)]);
        i = getParent(i);
    }
}