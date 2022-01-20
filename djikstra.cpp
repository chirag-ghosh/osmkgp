#include <climits>
#include <iostream>

using namespace std;

AdjListNode* newAdjListNode(
    int dest, double weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->ID = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->adjList = (AdjList*)
        malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->adjList[i].head = NULL;

    return graph;
}

void addEdge(Graph* graph, int src, int dest, double weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode = newAdjListNode(dest, weight);
    newNode->next = graph->adjList[src].head;
    graph->adjList[src].head = newNode;
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->adjList[dest].head;
    graph->adjList[dest].head = newNode;
}

double dijkstra(Graph* graph, int src, int dest) {
    int V = graph->V;
    double dist[V];
    MinHeap* minHeap = createMinHeap(V);
    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->position[v] = v;
    }
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->position[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;
    while (!isEmpty(minHeap)) {
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        AdjListNode* pCrawl =
            graph->adjList[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->ID;
            if (isInMinHeap(minHeap, v) &&
                dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    cout << "Hello" << endl;

    return dist[dest] == INT_MAX ? -1 : dist[dest];
}