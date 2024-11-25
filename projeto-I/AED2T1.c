#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 256

typedef struct {
    int quantidadeDePacientes;
    int quantidadeDeSalas;
    int quantidadeDeCardiologistas;
    int quantidadeDeNeurologistas;
    int quantidadeDeOutrosEspecialistas;
} Configuracoes;

typedef struct {
    char nome[50];
    char especialidade[50];
    int faltas;
} Medico;

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
    int falta;
} Cliente;

// Funções auxiliares para leitura das informações
void processaConfiguracoes(char *linha, Configuracoes *config) {
    sscanf(linha, "%d/%d/%d/%d/%d",
           &config->quantidadeDePacientes,
           &config->quantidadeDeSalas,
           &config->quantidadeDeCardiologistas,
           &config->quantidadeDeNeurologistas,
           &config->quantidadeDeOutrosEspecialistas);
}

void processaMedico(char *linha, Medico *medico) {
    sscanf(linha, "%49[^/]/%49[^/]/%d", 
           medico->nome, medico->especialidade, &medico->faltas);
}

void processaPaciente(char *linha, Cliente *cliente) {
    sscanf(linha, "%d/%d/%49[^/]/%49[^/]/%d/%14[^/]/%f/%f/%99[^/]/%99[^/]/%d",
           &cliente->id,
           &cliente->prioridade,
           cliente->especialidade,
           cliente->nome,
           &cliente->idade,
           cliente->telefone,
           &cliente->peso,
           &cliente->altura,
           cliente->sintoma,
           cliente->medicacoes,
           &cliente->falta);
}

int main() {
    FILE *arquivo = fopen("dados.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Configuracoes config;
    Medico *medicos = NULL;
    Cliente *clientes = NULL;
    int countMedicos = 0, countClientes = 0, capacidadeMedicos = 10, capacidadeClientes = 10;

    medicos = malloc(capacidadeMedicos * sizeof(Medico));
    clientes = malloc(capacidadeClientes * sizeof(Cliente));

    if (!medicos || !clientes) {
        perror("Erro ao alocar memória");
        fclose(arquivo);
        return 1;
    }

    char linha[MAX_LINHA];
    char secaoAtual[MAX_LINHA] = "";

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove o caractere de nova linha, se existir
        linha[strcspn(linha, "\n")] = '\0';

        // Verifica se a linha é uma seção
        if (linha[0] == '[' && linha[strlen(linha) - 1] == ']') {
            strcpy(secaoAtual, linha);
            continue;
        }

        // Ignora linhas vazias
        if (strlen(linha) == 0) continue;

        // Processa cada seção
        if (strcmp(secaoAtual, "[CONFIGURACOES]") == 0) {
            processaConfiguracoes(linha, &config);
        } else if (strcmp(secaoAtual, "[MEDICOS]") == 0) {
            if (countMedicos == capacidadeMedicos) {
                capacidadeMedicos *= 2;
                medicos = realloc(medicos, capacidadeMedicos * sizeof(Medico));
                if (!medicos) {
                    perror("Erro ao realocar memória para médicos");
                    fclose(arquivo);
                    return 1;
                }
            }
            processaMedico(linha, &medicos[countMedicos++]);
        } else if (strcmp(secaoAtual, "[PACIENTES]") == 0) {
            if (countClientes == capacidadeClientes) {
                capacidadeClientes *= 2;
                clientes = realloc(clientes, capacidadeClientes * sizeof(Cliente));
                if (!clientes) {
                    perror("Erro ao realocar memória para clientes");
                    fclose(arquivo);
                    return 1;
                }
            }
            processaPaciente(linha, &clientes[countClientes++]);
        }
    }

    fclose(arquivo);

    // Exibe as configurações
    printf("Configurações:\n");
    printf("  Pacientes: %d, Salas: %d, Cardiologistas: %d, Neurologistas: %d, Outros Especialistas: %d\n\n",
           config.quantidadeDePacientes, config.quantidadeDeSalas,
           config.quantidadeDeCardiologistas, config.quantidadeDeNeurologistas,
           config.quantidadeDeOutrosEspecialistas);

    // Exibe os médicos
    printf("Médicos:\n");
    for (int i = 0; i < countMedicos; i++) {
        printf("  Nome: %s, Especialidade: %s, Faltas: %d\n",
               medicos[i].nome, medicos[i].especialidade, medicos[i].faltas);
    }

    // Exibe os pacientes com todas as informações
    printf("\nPacientes:\n");
    for (int i = 0; i < countClientes; i++) {
        printf("  ID: %d, Prioridade: %d, Nome: %s, Idade: %d, Telefone: %s\n", 
               clientes[i].id, clientes[i].prioridade, clientes[i].nome, clientes[i].idade, clientes[i].telefone);
        printf("  Peso: %.2f, Altura: %.2f, Especialidade: %s\n", 
               clientes[i].peso, clientes[i].altura, clientes[i].especialidade);
        printf("  Sintoma: %s, Medicacoes: %s, Falta: %d\n\n", 
               clientes[i].sintoma, clientes[i].medicacoes, clientes[i].falta);
    }

    // Libera memória
    free(medicos);
    free(clientes);

    return 0;
}
