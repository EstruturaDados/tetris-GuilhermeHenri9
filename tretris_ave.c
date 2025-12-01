#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   
#define TAM_PILHA 3  

typedef struct {
    char nome;
    int id; 
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

int id_global = 0;

Peca gerarPeca();
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
void reabastecerFila(Fila *f); 

int filaVazia(Fila *f);
void enqueue(Fila *f, Peca p); 
Peca dequeue(Fila *f);         

int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void push(Pilha *p, Peca item);
Peca pop(Pilha *p);

void exibirEstado(Fila *f, Pilha *p);
void exibirMenu();

int main() {
    Fila filaPecas;
    Pilha pilhaReserva;
    int opcao;

    srand(time(NULL)); 

    inicializarFila(&filaPecas);
    inicializarPilha(&pilhaReserva);

    printf("=== TETRIS STACK: SISTEMA DE FILA E RESERVA ===\n");
    printf("Gerando pecas iniciais...\n");
    
    while (filaPecas.total < TAM_FILA) {
        enqueue(&filaPecas, gerarPeca());
    }

    do {
        exibirEstado(&filaPecas, &pilhaReserva);
        exibirMenu();
        printf("Opcao: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: 
                {
                    Peca jogada = dequeue(&filaPecas);
                    printf(">> ACAO: Voce jogou a peca [%c %d] no tabuleiro.\n", jogada.nome, jogada.id);
                    reabastecerFila(&filaPecas);
                }
                break;

            case 2: 
                {
                    if (pilhaCheia(&pilhaReserva)) {
                        printf(">> AVISO: A Pilha de Reserva esta cheia! Nao e possivel reservar.\n");
                    } else {
                        Peca p = dequeue(&filaPecas);
                        push(&pilhaReserva, p);
                        printf(">> ACAO: Peca [%c %d] movida para a Reserva.\n", p.nome, p.id);
                        reabastecerFila(&filaPecas);
                    }
                }
                break;

            case 3: 
                {
                    if (pilhaVazia(&pilhaReserva)) {
                        printf(">> AVISO: A Reserva esta vazia!\n");
                    } else {
                        Peca usada = pop(&pilhaReserva);
                        printf(">> ACAO: Voce resgatou a peca [%c %d] da Reserva.\n", usada.nome, usada.id);
                    }
                }
                break;

            case 0:
                printf("Encerrando simulacao...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }
        printf("--------------------------------------------------\n");

    } while (opcao != 0);

    return 0;
}


Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id_global++;
    return p;
}

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1; 
}

void reabastecerFila(Fila *f) {
    if (f->total < TAM_FILA) {
        Peca nova = gerarPeca();
        enqueue(f, nova);
        printf(">> SISTEMA: Nova peca [%c %d] entrou na fila.\n", nova.nome, nova.id);
    }
}

void enqueue(Fila *f, Peca p) {
    if (f->total >= TAM_FILA) return; 
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA; 
    f->total++;
}

Peca dequeue(Fila *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA; 
    f->total--;
    return p;
}

int pilhaCheia(Pilha *p) {
    return (p->topo >= TAM_PILHA - 1);
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

void push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = item;
}

Peca pop(Pilha *p) {
    Peca item = p->itens[p->topo];
    p->topo--;
    return item;
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    
    printf("Fila de pecas: ");
    int i, idx;
    for (i = 0; i < f->total; i++) {
        idx = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[ VAZIA ]");
    } else {
        for (i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

void exibirMenu() {
    printf("\nOpcoes de Acao:\n");
    printf("1 - Jogar peca (Dequeue Fila)\n");
    printf("2 - Reservar peca (Dequeue Fila -> Push Pilha)\n");
    printf("3 - Usar peca reservada (Pop Pilha)\n");
    printf("0 - Sair\n");
}