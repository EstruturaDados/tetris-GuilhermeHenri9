#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_MAX 5 

typedef struct {
    char nome;  
    int id;     
} Peca;


typedef struct {
    Peca itens[TAMANHO_MAX];
    int inicio; 
    int fim;    
    int total;  
} Fila;


int id_sequencial = 0;

void inicializarFila(Fila *f);
Peca gerarPeca();
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void inserirPeca(Fila *f, Peca p); 
void jogarPeca(Fila *f);           
void exibirFila(Fila *f);
void exibirMenu();

int main() {
    Fila filaDePecas;
    int opcao;
    
    srand(time(NULL));

    printf("=== INICIALIZANDO TETRIS STACK ===\n");
    inicializarFila(&filaDePecas);

    printf("Gerando peças iniciais...\n");
    while (!filaCheia(&filaDePecas)) {
        inserirPeca(&filaDePecas, gerarPeca());
    }

    do {
        exibirFila(&filaDePecas);
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                jogarPeca(&filaDePecas);
                break;
            case 2:
                if (!filaCheia(&filaDePecas)) {
                    Peca nova = gerarPeca();
                    inserirPeca(&filaDePecas, nova);
                    printf(">> Nova peca gerada e inserida: [%c %d]\n", nova.nome, nova.id);
                } else {
                    printf(">> ATENCAO: A fila esta cheia! Nao e possivel adicionar mais pecas.\n");
                }
                break;
            case 0:
                printf("Saindo do Tetris Stack... Ate logo!\n");
                break;
            default:
                printf(">> Opcao invalida! Tente novamente.\n");
        }
        
        printf("\n------------------------------------------------\n"); 
        
    } while (opcao != 0);

    return 0;
}

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};

    int indice = rand() % 4;
    
    p.nome = tipos[indice];
    p.id = id_sequencial;
    id_sequencial++; 
    
    return p;
}

int filaCheia(Fila *f) {
    return (f->total >= TAMANHO_MAX);
}

int filaVazia(Fila *f) {
    return (f->total == 0);
}

void inserirPeca(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAMANHO_MAX; 
    f->total++;
}

void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf(">> Erro: Nao ha pecas para jogar!\n");
        return;
    }
    
    Peca p = f->itens[f->inicio];
    printf(">> Voce jogou a peca: [%c %d]\n", p.nome, p.id);

    f->inicio = (f->inicio + 1) % TAMANHO_MAX;
    f->total--;
}

void exibirFila(Fila *f) {
    printf("\nEstado atual:\n");
    printf("Fila de pecas: ");
    
    if (filaVazia(f)) {
        printf("[ VAZIA ]");
    } else {
        int i, idx;
        for (i = 0; i < f->total; i++) {
            idx = (f->inicio + i) % TAMANHO_MAX;
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }
    printf("\n");
}

void exibirMenu() {
    printf("\nOpcões de acao:\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
}
