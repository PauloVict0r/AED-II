#ifndef HEAP_H
#define HEAP_H

#define MAX_HEAP_SIZE 100

typedef struct {
    int id;     // Campo para o ID
    int value;  // Valor associado ao ID
} HeapNode;

typedef struct {
    HeapNode data[MAX_HEAP_SIZE];
    int size;
} MaxHeap;

// Declarações das funções do heap
void initHeap(MaxHeap *heap);
void insert(MaxHeap *heap, int id, int value);
HeapNode getMax(MaxHeap *heap);
HeapNode extractMax(MaxHeap *heap);
void printHeap(MaxHeap *heap);

#endif // HEAP_H
