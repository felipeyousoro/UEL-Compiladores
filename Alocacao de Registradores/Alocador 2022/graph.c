#include "graph.h"

graph *grf_create_graph(int size) {
    graph *g = (graph *) malloc(sizeof(graph));
    g->size = size;

    g->adjacencies = (void **) malloc(sizeof(void *) * size);
    for (int i = 0; i < size; i++) {
        g->adjacencies[i] = NULL;
    }

    return g;
}

void grf_add_vertex(graph *g, int vertex, void *data) {
    g->adjacencies[vertex] = data;
}

int grf_get_size(graph *g) {
    return g->size;
}

void *grf_get_vertex(graph *g, int vertex) {
    return g->adjacencies[vertex];
}

void grf_add_edge(graph *g, int v1, int v2, void (*add_adjacency)(void *, void *)) {
    add_adjacency(g->adjacencies[v1], g->adjacencies[v2]);
}

void free_graph(graph *g) {
    free(g->adjacencies);
    free(g);
}