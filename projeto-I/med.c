#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "heap.h"

#define MAX_LINHA 256

typedef struct { int pacientes, salas, cardio, neuro, outros; } Config;
typedef struct { char nome[50], especialidade[50]; int faltas, ocup, hrs; } Medico;
typedef struct { int id, prioridade, idade, faltas, volta; char especialidade[50], nome[50], tel[15], sintoma[100], med[100]; float peso, altura; } Cliente;

void parseConfig(char *l, Config *c) {
    sscanf(l, "Pacientes: %d, Salas: %d, Cardiologistas: %d, Neurologistas: %d, Outros Especialistas: %d",
           &c->pacientes, &c->salas, &c->cardio, &c->neuro, &c->outros);
}
void parseMedico(char *l, Medico *m) {
    sscanf(l, "Nome: %49[^,], Especialidade: %49[^,], Faltas: %d", m->nome, m->especialidade, &m->faltas);
    m->ocup = 0;
    m->hrs = 0;
}
void parseCliente(char *l, Cliente *c) {
    sscanf(l, "id: %d, prioridade: %d, Especialidade: %49[^,], Nome: %49[^,], Idade: %d, Tel: %14[^,], Peso: %f, Altura: %f, Sintoma: %99[^,], Medicações: %99[^,], Faltas: %d",
           &c->id, &c->prioridade, c->especialidade, c->nome, &c->idade, c->tel, &c->peso, &c->altura, c->sintoma, c->med, &c->faltas);
    c->volta = 0;
}

void printLine(int len) { printf("+"); for (int i = 0; i < len; i++) printf("-"); printf("+\n"); }

int main() {
    srand(time(NULL));

    // Criar ou abrir o arquivo para salvar a saída
    FILE *resultado = fopen("resultado.txt", "w");
    if (!resultado) return perror("Erro ao criar arquivo"), 1;

    // Redirecionar stdout para o arquivo
    freopen("resultado.txt", "w", stdout);

    FILE *f = fopen("dados.txt", "r");
    if (!f) return perror("Erro ao abrir arquivo"), 1;

    Config cfg;
    Medico *medicos = malloc(10 * sizeof(Medico));
    Cliente *clientes = malloc(10 * sizeof(Cliente));
    int countMed = 0, countCli = 0, capMed = 10, capCli = 10, min = 10;

    char linha[MAX_LINHA], secao[MAX_LINHA] = "";
    while (fgets(linha, MAX_LINHA, f)) {
        linha[strcspn(linha, "\n")] = 0;
        if (linha[0] == '[') { strcpy(secao, linha); continue; }
        if (!linha[0]) continue;

        if (!strcmp(secao, "[CONFIGURACOES]")) parseConfig(linha, &cfg);
        else if (!strcmp(secao, "[MEDICOS]")) {
            if (countMed == capMed) medicos = realloc(medicos, (capMed *= 2) * sizeof(Medico));
            parseMedico(linha, &medicos[countMed++]);
        } else if (!strcmp(secao, "[PACIENTES]")) {
            if (countCli == capCli) clientes = realloc(clientes, (capCli *= 2) * sizeof(Cliente));
            parseCliente(linha, &clientes[countCli++]);
        }
    }
    fclose(f);

    // Verificar se o número de médicos é suficiente
    int totalMedicos = cfg.cardio + cfg.neuro + cfg.outros;
    if (totalMedicos < cfg.salas) {
        printf("Erro: quantidade de salas (%d) superior à quantidade de médicos (%d).\n", cfg.salas, totalMedicos);
        free(medicos), free(clientes);
        return 1;
    }

    printf("Configurações:\n");
    printLine(28);
    printf("| %-20s | %3d |\n| %-20s | %3d |\n| %-20s | %3d |\n| %-20s | %3d |\n| %-20s | %3d |\n",
           "Pacientes", cfg.pacientes, "Salas", cfg.salas, "Cardiologistas", cfg.cardio, "Neurologistas", cfg.neuro, "Outros Especialistas", cfg.outros);
    printLine(28);

    printf("\nMédicos:\n");
    printLine(56);
    printf("| %-20s | %-20s | %s |\n", "Nome", "Especialidade", " Faltas ");
    printLine(56);
    for (int i = 0; i < countMed; i++) printf("| %-20s | %-20s | %4d\t |\n", medicos[i].nome, medicos[i].especialidade, medicos[i].faltas);
    printLine(56);

    printf("\nPacientes:\n");
    printLine(109);
    printf("| %-5s | %-12s | %-25s | %-5s | %-12s | %-6s | %-6s | %-15s |\n",
           "ID", "Prioridade", "Nome", "Idade", "Telefone", "Peso", "Altura", "Especialidade");
    printLine(109);
    for (int i = 0; i < countCli; i++) printf("| %-5d | %-12d | %-25s | %-5d | %-12s | %-6.2f | %-6.2f | %-15s |\n",
           clientes[i].id, clientes[i].prioridade, clientes[i].nome, clientes[i].idade, clientes[i].tel, clientes[i].peso, clientes[i].altura, clientes[i].especialidade);
    printLine(109);

    printf("\nDetalhes adicionais:\n");
    printLine(66);
    printf("| %-25s | %-25s | %s |\n", "Sintoma", "Medicações", " Faltas ");
    printLine(66);
    for (int i = 0; i < countCli; i++) printf("| %-25s | %-25s | %4d     |\n", clientes[i].sintoma, clientes[i].med, clientes[i].faltas);
    printLine(66);

    MaxHeap heap;
    MaxHeap heapMed;
    HeapNode max;
    HeapNode extracted;
    initHeap(&heap);

    for (int i = 0; i < countCli; i++) insert(&heap, clientes[i].id, clientes[i].prioridade);

    printf("\nHeap atual: ");
    printHeap(&heap);

    max = getMax(&heap);
    printf("\nElemento máximo: (ID: %d, Valor: %d)\n", max.id, max.value);

    int sal_ocup = 0;
    int hora_sala = 8;
    int dia_sala = 1;

    while (heap.size > 0) {
        int paciente_atendido = 0;

        if (rand() % 100 < 5) {
            extracted = extractMax(&heap);
            max = getMax(&heap);
            clientes[extracted.id - 1].faltas++;
            if (clientes[extracted.id - 1].faltas != 2) {
                insert(&heap, extracted.id, max.value - 1);
            }        
        }
        
        for (int i = 0; i < totalMedicos; i++) {
            if (strcmp(medicos[i].especialidade, clientes[max.id - 1].especialidade) == 0 && medicos[i].ocup == 0) {
                sal_ocup++;
                medicos[i].ocup = 1;
                medicos[i].hrs++;
                if (clientes[max.id - 1].volta > 0) printf("Retorno - "); 
                
                printf("O paciente de id: %d nome: %s, está em na sala: %d com o médico: %s no dia: %d hora: %d:00hrs\n\n", 
                    max.id, clientes[max.id - 1].nome, sal_ocup, medicos[i].nome, dia_sala, hora_sala);

                if (rand() % 100 < 30) {
                    max = getMax(&heap);
                    if (clientes[max.id - 1].volta == 0) {
                        insert(&heap, max.id, min + (rand() % max.value));
                        clientes[max.id - 1].volta++;
                    }
                }

                extracted = extractMax(&heap);
                printf("Extraindo elemento máximo: (ID: %d, Valor: %d)\n\n", extracted.id, extracted.value);
                if (heap.size > 0) max = getMax(&heap);
                paciente_atendido = 1;
                break;
            }
        }

        if (!paciente_atendido) {
            sal_ocup = 0;
            hora_sala++;
            if (hora_sala > 16) {
                dia_sala++;
                hora_sala = 8;
            }
            for (int i = 0; i < totalMedicos; i++) medicos[i].ocup = 0;
        }
    }

    for (int i = 0; i < totalMedicos; i++) insert(&heapMed, i, medicos[i].hrs);
    for (int i = 0; i < totalMedicos; i++) {
        extracted = extractMax(&heapMed);
        printf("O médico: %s, trabalhou: %d horas.\n", medicos[extracted.id].nome, medicos[extracted.id].hrs);
    }
    
    int hor = 0;
    for (int i = 1; i <= cfg.pacientes; i++) {
        if (i % cfg.salas == 0) hor++;
        if (i == cfg.pacientes && (hor * cfg.salas) % cfg.pacientes != 0) hor++;
    }
    
    printf("\nhoras: %d\n\n", hor);

    int ceil = 0;
    for (int i = 1; i <= cfg.pacientes; i++) {
        if (i == 1) ceil = 1;
        if (i % (cfg.salas * 9) == 0) ceil++;
    }

    int ceilS = 0;
    for (int i = 1; i <= ceil; i++) {
        if (i == 1) ceilS = 1;
        if (i % 5 == 0) ceilS++;
    }

    if (ceil < 5) {
        printf("Vamos precisar de %d dias.\n\n", ceil);
    } else {
        printf("Vamos precisar de %d dias que equivale +/- a %d semanas.\n\n", ceil, (ceil + 6) / 7);
    }

    free(medicos), free(clientes);
    return 0;
}
