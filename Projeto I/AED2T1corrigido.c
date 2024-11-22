#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estas duas variáveis globáis eu to usando pra criar os vetores que vão armazenar os clientes e a o texto do arquivo em si
#define MAX_LINHA 256
#define MAX_CLIENTES 100

//essa struct Configuracoes ta armazenando as informacoes da primeira linha do arquivo que sao a quantidade de pacientes, de salas, etc
typedef struct {
    int quantidadeDePacientes;
    int quantidadeDeSalas;
    int quantidadeDeCardiologistas;
    int quantidadeDeNeurologistas;
    int quantidadeDeOutrosEspecialistas;
} Configuracoes;

//essa aqui é para os clientes
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

// essa função ta removendo os /n que atrapalham na hora de ler as informacoes do arquivo
void removeBarraN(char *linha) {
    size_t len = strlen(linha);
    if (len > 0 && linha[len - 1] == '\n') {
        linha[len - 1] = '\0';
    }
}

// Função para processar as configurações gerais
void processaConfiguracoes(char *linha, Configuracoes *config) {
    char *token;

    token = strtok(linha, "/");
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
    //aqui eu crio uma variável para o arquivo, uma para o texto do arquivo, outra para as configurações e outra para os clientes e o contador tambem
    FILE *arquivo;
    char linha[MAX_LINHA];
    Configuracoes config;
    Cliente clientes[MAX_CLIENTES];
    int count = 0;

    // Abre o arquivo para leitura
    arquivo = fopen("dados.txt", "r");
    //essa parte eh opcional
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê a primeira linha do arquivo para processar as configurações gerais
    if (fgets(linha, sizeof(linha), arquivo)) {
        removeBarraN(linha);  // Remove o caractere de nova linha
        processaConfiguracoes(linha, &config);
    }

    // Lê as demais linhas do arquivo para processar os clientes
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

    // Exibe as configurações gerais
    printf("Configurações gerais:\n");
    printf("  Quantidade de Pacientes: %d\n", config.quantidadeDePacientes);
    printf("  Quantidade de Salas: %d\n", config.quantidadeDeSalas);
    printf("  Quantidade de Cardiologistas: %d\n", config.quantidadeDeCardiologistas);
    printf("  Quantidade de Neurologistas: %d\n", config.quantidadeDeNeurologistas);
    printf("  Quantidade de Outros Especialistas: %d\n\n", config.quantidadeDeOutrosEspecialistas);

    // Exibe os dados dos clientes
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
