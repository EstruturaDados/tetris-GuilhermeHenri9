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
    int qtd;
} FilaCircular;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;


int id_sequencial = 0;

void inicializarFila(FilaCircular *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca();
void reabastecerFila(FilaCircular *f);

int filaVazia(FilaCircular *f);
void enqueue(FilaCircular *f, Peca p);
Peca dequeue(FilaCircular *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void push(Pilha *p, Peca p_val);
Peca pop(Pilha *p);

void trocarPecaUnica(FilaCircular *f, Pilha *p);
void trocarBloco(FilaCircular *f, Pilha *p);

void exibirEstado(FilaCircular *f, Pilha *p);
void exibirMenu();

int main() {
    FilaCircular fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    printf("=== TETRIS STACK: SISTEMA AVANCADO ===\n");
    printf("Preenchendo fila inicial...\n");
    
    while (fila.qtd < TAM_FILA) {
        enqueue(&fila, gerarPeca());
    }

    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                {
                    Peca jogada = dequeue(&fila);
                    printf(">> ACAO: Peca [%c %d] foi jogada no tabuleiro.\n", jogada.nome, jogada.id);
                    reabastecerFila(&fila);
                }
                break;

            case 2:
                {
                    if (pilhaCheia(&pilha)) {
                        printf(">> ERRO: Pilha de reserva cheia! Nao e possivel reservar.\n");
                    } else {
                        Peca p = dequeue(&fila);
                        push(&pilha, p);
                        printf(">> ACAO: Peca [%c %d] movida para a Reserva.\n", p.nome, p.id);
                        reabastecerFila(&fila);
                    }
                }
                break;

            case 3:
                {
                    if (pilhaVazia(&pilha)) {
                        printf(">> ERRO: Nao ha pecas na reserva!\n");
                    } else {
                        Peca p = pop(&pilha);
                        printf(">> ACAO: Peca da reserva [%c %d] utilizada.\n", p.nome, p.id);
                    }
                }
                break;

            case 4:
                trocarPecaUnica(&fila, &pilha);
                break;

            case 5:
                trocarBloco(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando sistema Tetris Stack...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
        printf("--------------------------------------------------\n");

    } while (opcao != 0);

    return 0;
}

void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id_sequencial++;
    return p;
}

void reabastecerFila(FilaCircular *f) {
    if (f->qtd < TAM_FILA) {
        Peca nova = gerarPeca();
        enqueue(f, nova);
        printf(">> AUTO: Nova peca [%c %d] gerada na fila.\n", nova.nome, nova.id);
    }
}

void enqueue(FilaCircular *f, Peca p) {
    if (f->qtd >= TAM_FILA) return;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->qtd++;
}

Peca dequeue(FilaCircular *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

int pilhaCheia(Pilha *p) { return p->topo >= TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca p_val) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = p_val;
}

Peca pop(Pilha *p) {
    Peca p_val = p->itens[p->topo];
    p->topo--;
    return p_val;
}

void trocarPecaUnica(FilaCircular *f, Pilha *p) {
    if (f->qtd == 0 || pilhaVazia(p)) {
        printf(">> ERRO: Impossivel trocar. Fila vazia ou Pilha vazia.\n");
        return;
    }

    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    
    printf(">> ACAO: Troca efetuada (Frente Fila <-> Topo Pilha).\n");
}

void trocarBloco(FilaCircular *f, Pilha *p) {
    if (p->topo < 2) { 
        printf(">> ERRO: Pilha precisa estar cheia (3 itens) para troca em bloco.\n");
        return;
    }
    if (f->qtd < 3) {
        printf(">> ERRO: Fila precisa ter pelo menos 3 itens para troca em bloco.\n");
        return;
    }

    int i;
    for (i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % TAM_FILA;
        int idxPilha = p->topo - i;

        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = temp;
    }
    
    printf(">> ACAO: TROCA EM BLOCO realizada com sucesso!\n");
}


void exibirEstado(FilaCircular *f, Pilha *p) {
    printf("\nEstado atual:\n");
    
    printf("Fila de pecas: ");
    int i, idx;
    if (f->qtd == 0) printf("[ VAZIA ]");
    for (i = 0; i < f->qtd; i++) {
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
    printf("\nOpcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
}

