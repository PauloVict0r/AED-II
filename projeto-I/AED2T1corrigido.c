#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 1024
#define MAX_MEDICOS 50
#define MAX_CLIENTES 100

// Estrutura para armazenar as configurações gerais
typedef struct {
    int quantidadeDePacientes;
    int quantidadeDeSalas;
    int quantidadeDeCardiologistas;
    int quantidadeDeNeurologistas;
    int quantidadeDeOutrosEspecialistas;
} Configuracoes;

// Estrutura para armazenar informações de cada médico
typedef struct {
    char nome[50];
    char especialidade[50];
    int faltas;
} Medico;

// Estrutura para armazenar informações de cada cliente (paciente)
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
    int falta;  // Número de faltas
} Cliente;

// Função para processar as configurações gerais
void processaConfiguracoes(char *dados, Configuracoes *config) {
    char *token = strtok(dados, "/");
    config->quantidadeDePacientes = atoi(token);

    token = strtok(NULL, "/");
    config->quantidadeDeSalas = atoi(token);

    token = strtok(NULL, "/");
    config->quantidadeDeCardiologistas = atoi(token);

    token = strtok(NULL, "/");
    config->quantidadeDeNeurologistas = atoi(token);

    token = strtok(NULL, "/");
    config->quantidadeDeOutrosEspecialistas = atoi(token);
}

// Função para processar os médicos
int processaMedicos(char *dados, Medico *medicos) {
    int count = 0;
    char *token = strtok(dados, ",");
    while (token != NULL && count < MAX_MEDICOS) {
        sscanf(token, "%49[^/]/%49[^/]/%d", 
               medicos[count].nome, 
               medicos[count].especialidade, 
               &medicos[count].faltas);
        count++;
        token = strtok(NULL, ",");
    }
    return count;
}

// Função para processar os pacientes
int processaClientes(char *dados, Cliente *clientes) {
    int count = 0;
    char *token = strtok(dados, ",");
    while (token != NULL && count < MAX_CLIENTES) {
        sscanf(token, "%d/%d/%49[^/]/%49[^/]/%d/%14[^/]/%f/%f/%99[^/]/%99[^/]/%d", 
               &clientes[count].id,
               &clientes[count].prioridade,
               clientes[count].especialidade,
               clientes[count].nome,
               &clientes[count].idade,
               clientes[count].telefone,
               &clientes[count].peso,
               &clientes[count].altura,
               clientes[count].sintoma,
               clientes[count].medicacoes,
               &clientes[count].falta);
        count++;
        token = strtok(NULL, ",");
    }
    return count;
}

int main() {
    FILE *arquivo;
    char linha[MAX_LINHA];
    Configuracoes config;
    Medico medicos[MAX_MEDICOS];
    Cliente clientes[MAX_CLIENTES];
    int countMedicos = 0, countClientes = 0;

    // Abre o arquivo para leitura
    arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê a linha completa do arquivo
    if (fgets(linha, sizeof(linha), arquivo)) {
        // Dividir a linha em seções: configurações, médicos e pacientes
        char *configuracoes = strtok(linha, "|");
        char *medicosStr = strtok(NULL, "|");
        char *clientesStr = strtok(NULL, "|");

        // Criar buffers independentes para evitar interferência
        char bufferConfig[MAX_LINHA];
        char bufferMedicos[MAX_LINHA];
        char bufferClientes[MAX_LINHA];

        strcpy(bufferConfig, configuracoes);
        strcpy(bufferMedicos, medicosStr);
        strcpy(bufferClientes, clientesStr);

        // Processa cada seção
        processaConfiguracoes(bufferConfig, &config);
        countMedicos = processaMedicos(bufferMedicos, medicos);
        countClientes = processaClientes(bufferClientes, clientes);
    }

    fclose(arquivo);

    // Exibe as configurações gerais
    printf("Configurações gerais:\n");
    printf("  Quantidade de Pacientes: %d\n", config.quantidadeDePacientes);
    printf("  Quantidade de Salas: %d\n", config.quantidadeDeSalas);
    printf("  Quantidade de Cardiologistas: %d\n", config.quantidadeDeCardiologistas);
    printf("  Quantidade de Neurologistas: %d\n", config.quantidadeDeNeurologistas);
    printf("  Quantidade de Outros Especialistas: %d\n\n", config.quantidadeDeOutrosEspecialistas);

    // Exibe os médicos
    printf("Médicos:\n");
    for (int i = 0; i < countMedicos; i++) {
        printf("  Nome: %s\n", medicos[i].nome);
        printf("  Especialidade: %s\n", medicos[i].especialidade);
        printf("  Faltas: %d\n\n", medicos[i].faltas);
    }

    // Exibe os pacientes
    printf("Pacientes:\n");
    for (int i = 0; i < countClientes; i++) {
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
        printf("  Faltas: %d\n\n", clientes[i].falta);
    }

    return 0;
}
