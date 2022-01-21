#include <climits>
#include <iostream>

using namespace std;

//Creates an adjacency list node using index and distance
AdjListNode* newAdjListNode(
    int index, double weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->index = index;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

//returns a graph after allocating memory according to the number of nodes.
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->adjList = (AdjList*)malloc(V * sizeof(AdjList));

    for (int i = 0; i < V; ++i) graph->adjList[i].head = NULL;

    return graph;
}

//adds an edge to the graph by taking adjacent nodes in a way
void addEdge(Graph* graph, int src, int dest, double weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));

    newNode = newAdjListNode(dest, weight);
    newNode->next = graph->adjList[src].head;
    graph->adjList[src].head = newNode;

    newNode = newAdjListNode(src, weight);
    newNode->next = graph->adjList[dest].head;
    graph->adjList[dest].head = newNode;
}

// the main dijkstra code. takes source and destination index and calculates the minimum distance between them via ways.
double dijkstra(Graph* graph, int src, int dest) {
    int V = graph->V;
    double dist[V];
    MinHeap* minHeap = createMinHeap(V);

    for (int i = 0; i < V; ++i) {
        dist[i] = INT_MAX;
        minHeap->array[i] = newMinHeapNode(i, dist[i]);
        minHeap->position[i] = i;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->position[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;

    while (!isEmpty(minHeap)) {
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        AdjListNode* tempListNode = graph->adjList[u].head;

        while (tempListNode != NULL) {
            int v = tempListNode->index;
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && tempListNode->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + tempListNode->weight;
                decreaseKey(minHeap, v, dist[v]);
            }
            tempListNode = tempListNode->next;
        }
    }

    return dist[dest] == INT_MAX ? -1 : dist[dest];
}