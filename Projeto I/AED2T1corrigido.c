#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 256
#define MAX_CLIENTES 100

typedef struct {
    int id;
    int prioridade;
    char especialidade[50];
    char nome[50];
    int idade;
    char telefone[15];
    float peso;
    float altura;
    char sintoma[100];
    char medicacoes[100];
} Cliente;

// Função para remover o \n
void removeBarraN(char *linha) {
    size_t len = strlen(linha);
    if (len > 0 && linha[len - 1] == '\n') {
        linha[len - 1] = '\0';
    }
}

// Função para processar uma linha do arquivo e preencher um struct Cliente
void processaLinha(char *linha, Cliente *cliente) {
    char *token;

    // Extrai cada campo da linha
    token = strtok(linha, "/");
    cliente->id = atoi(token);

    token = strtok(NULL, "/");
    cliente->prioridade = atoi(token);

    token = strtok(NULL, "/");
    strcpy(cliente->especialidade, token);

    token = strtok(NULL, "/");
    strcpy(cliente->nome, token);

    token = strtok(NULL, "/");
    cliente->idade = atoi(token);

    token = strtok(NULL, "/");
    strcpy(cliente->telefone, token);

    token = strtok(NULL, "/");
    cliente->peso = atof(token);

    token = strtok(NULL, "/");
    cliente->altura = atof(token);

    token = strtok(NULL, "/");
    strcpy(cliente->sintoma, token);

    token = strtok(NULL, "/");
    strcpy(cliente->medicacoes, token);
}

int main() {
    FILE *arquivo;
    char linha[MAX_LINHA];
    Cliente clientes[MAX_CLIENTES];
    int count = 0;

    // Abre o arquivo para leitura
    arquivo = fopen("dados.txt", "r");

    // Lê o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        removeBarraN(linha);  // Remove o caractere de nova linha
        if (count < MAX_CLIENTES) {
            processaLinha(linha, &clientes[count]);
            count++;
        } else {
            printf("Limite de clientes excedido.\n");
            break;
        }
    }

    fclose(arquivo);

    // Exibe os dados lidos
    for (int i = 0; i < count; i++) {
        printf("Cliente %d:\n", i + 1);
        printf("  ID: %d\n", clientes[i].id);
        printf("  Prioridade: %d\n", clientes[i].prioridade);
        printf("  Especialidade: %s\n", clientes[i].especialidade);
        printf("  Nome: %s\n", clientes[i].nome);
        printf("  Idade: %d\n", clientes[i].idade);
        printf("  Telefone: %s\n", clientes[i].telefone);
        printf("  Peso: %.2f\n", clientes[i].peso);
        printf("  Altura: %.2f\n", clientes[i].altura);
        printf("  Sintoma: %s\n", clientes[i].sintoma);
        printf("  Medicacoes: %s\n", clientes[i].medicacoes);
        printf("\n");
    }

    return 0;
}
