#include <stdio.h>
#include "heap.h"

// Inicializa o heap
void initHeap(MaxHeap *heap) {
    heap->size = 0;
}

// Calcula o índice do pai
int parent(int i) {
    return (i - 1) / 2;
}

// Calcula o índice do filho esquerdo
int leftChild(int i) {
    return 2 * i + 1;
}

// Calcula o índice do filho direito
int rightChild(int i) {
    return 2 * i + 2;
}

// Troca dois nós no heap
void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// "Sobe" um elemento para restaurar a propriedade do heap
void siftUp(MaxHeap *heap, int i) {
    while (i > 0 && heap->data[parent(i)].value < heap->data[i].value) {
        swap(&heap->data[parent(i)], &heap->data[i]);
        i = parent(i);
    }
}

// "Desce" um elemento para restaurar a propriedade do heap
void siftDown(MaxHeap *heap, int i) {
    int maxIndex = i;
    int l = leftChild(i);
    if (l < heap->size && heap->data[l].value > heap->data[maxIndex].value) {
        maxIndex = l;
    }
    int r = rightChild(i);
    if (r < heap->size && heap->data[r].value > heap->data[maxIndex].value) {
        maxIndex = r;
    }
    if (i != maxIndex) {
        swap(&heap->data[i], &heap->data[maxIndex]);
        siftDown(heap, maxIndex);
    }
}

// Insere um elemento no heap
void insert(MaxHeap *heap, int id, int value) {
    if (heap->size == MAX_HEAP_SIZE) {
        printf("Erro: Heap cheia!\n");
        return;
    }
    heap->data[heap->size].id = id;
    heap->data[heap->size].value = value;
    heap->size++;
    siftUp(heap, heap->size - 1);
}

// Obtém o elemento máximo (raiz) do heap
HeapNode getMax(MaxHeap *heap) {
    if (heap->size == 0) {
        printf("Erro: Heap vazia!\n");
        return (HeapNode){-1, -1}; // Retorna um nó inválido
    }
    return heap->data[0];
}

// Remove o elemento máximo (raiz) do heap
HeapNode extractMax(MaxHeap *heap) {
    if (heap->size == 0) {
        printf("Erro: Heap vazia!\n");
        return (HeapNode){-1, -1}; // Retorna um nó inválido
    }
    HeapNode result = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    siftDown(heap, 0);
    return result;
}

// Imprime o heap
void printHeap(MaxHeap *heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("(ID: %d, Valor: %d) ", heap->data[i].id, heap->data[i].value);
    }
    printf("\n");
}
