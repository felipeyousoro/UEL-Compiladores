#ifndef COMPILADORES_GRAPH_H
#define COMPILADORES_GRAPH_H

#include <stdlib.h>

typedef struct graph {
    int size;
    void **adjacencies;

} graph;

graph* grf_create_graph(int size);

void grf_add_vertex(graph *g, int vertex, void *data);

void* grf_get_vertex(graph *g, int vertex);
int grf_get_size(graph *g);
void grf_add_edge(graph *g, int v1, int v2, void(*add_adjacency)(void *, void *));

void free_graph(graph *g);

#endif //COMPILADORES_GRAPH_H
