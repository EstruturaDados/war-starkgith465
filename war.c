// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.

    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.




// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 4
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de setup e gerenciamento de memória
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa, char* missaoJogador);

// Funções de interface com o usuário
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(const char* missao);
void limparBufferEntrada();

// Funções de lógica principal do jogo
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* corJogador);
int sortearMissao();
int verificarVitoria(const char* missao, const Territorio* mapa, const char* corJogador);

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup)
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }
    inicializarTerritorios(mapa);

    char corJogador[TAM_COR] = "Azul";
    int idMissao = sortearMissao();
    char* missaoJogador = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memória para a missão.\n");
        liberarMemoria(mapa, NULL);
        return 1;
    }

    const char* missoes[NUM_MISSOES] = {
        "Conquistar todos os territorios do mapa.",
        "Ter 5 ou mais tropas em 3 territorios.",
        "Conquistar 5 ou mais territorios.",
        "Eliminar todas as tropas da cor Vermelha."
    };
    
    strcpy(missaoJogador, missoes[idMissao]);

    int escolha;
    int vitoria = 0;

    printf("Bem-vindo ao War Estruturado!\n");
    printf("Você joga com a cor %s.\n", corJogador);
    exibirMissao(missaoJogador);
    
    // 2. Laço Principal do Jogo (Game Loop)
    do {
        exibirMapa(mapa);
        exibirMenuPrincipal();
        scanf("%d", &escolha);
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                vitoria = verificarVitoria(missaoJogador, mapa, corJogador);
                if (vitoria) {
                    printf("\nParabéns! Você cumpriu sua missão e venceu o jogo!\n");
                } else {
                    printf("\nSua missão ainda não foi cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

        if (escolha != 0 && vitoria == 0) {
            // Verifica a vitória automaticamente após cada turno
            vitoria = verificarVitoria(missaoJogador, mapa, corJogador);
            if (vitoria) {
                 printf("\nParabéns! Você cumpriu sua missão e venceu o jogo!\n");
                 break;
            }
        }
        
    } while (escolha != 0 && !vitoria);

    // 3. Limpeza
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa(): Aloca dinamicamente a memória para o vetor de territórios.
Territorio* alocarMapa() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

// inicializarTerritorios(): Preenche os dados iniciais do mapa.
void inicializarTerritorios(Territorio* mapa) {
    // Configura 5 territórios para cada jogador para o exemplo
    strcpy(mapa[0].nome, "Brasil"); strcpy(mapa[0].cor, "Azul"); mapa[0].tropas = 3;
    strcpy(mapa[1].nome, "Argentina"); strcpy(mapa[1].cor, "Azul"); mapa[1].tropas = 2;
    strcpy(mapa[2].nome, "Chile"); strcpy(mapa[2].cor, "Azul"); mapa[2].tropas = 1;
    strcpy(mapa[3].nome, "Peru"); strcpy(mapa[3].cor, "Azul"); mapa[3].tropas = 2;
    strcpy(mapa[4].nome, "Colombia"); strcpy(mapa[4].cor, "Azul"); mapa[4].tropas = 2;

    strcpy(mapa[5].nome, "Espanha"); strcpy(mapa[5].cor, "Vermelha"); mapa[5].tropas = 3;
    strcpy(mapa[6].nome, "Franca"); strcpy(mapa[6].cor, "Vermelha"); mapa[6].tropas = 2;
    strcpy(mapa[7].nome, "Alemanha"); strcpy(mapa[7].cor, "Vermelha"); mapa[7].tropas = 3;
    strcpy(mapa[8].nome, "Italia"); strcpy(mapa[8].cor, "Vermelha"); mapa[8].tropas = 1;
    strcpy(mapa[9].nome, "Inglaterra"); strcpy(mapa[9].cor, "Vermelha"); mapa[9].tropas = 2;
}

// liberarMemoria(): Libera a memória alocada.
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (missaoJogador != NULL) {
        free(missaoJogador);
    }
}

// exibirMenuPrincipal(): Imprime o menu de ações.
void exibirMenuPrincipal() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Atacar um territorio\n");
    printf("2. Verificar vitoria\n");
    printf("0. Sair do jogo\n");
    printf("Escolha sua acao: ");
}

// exibirMapa(): Mostra o estado atual dos territórios.
void exibirMapa(const Territorio* mapa) {
    printf("\n--- Mapa do Mundo ---\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%d. %s (Cor: %s, Tropas: %d)\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// exibirMissao(): Exibe a missão do jogador.
void exibirMissao(const char* missao) {
    printf("\n--- Sua Missao Secreta ---\n");
    printf("%s\n\n", missao);
}

// limparBufferEntrada(): Limpa o buffer para evitar erros.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// faseDeAtaque(): Gerencia a interação para o ataque.
void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int idAtacante, idDefensor;

    printf("\n--- Fase de Ataque ---\n");
    printf("Escolha o territorio para atacar (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &idAtacante);
    limparBufferEntrada();
    idAtacante--; // Ajusta para o índice do array

    printf("Escolha o territorio para defender (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &idDefensor);
    limparBufferEntrada();
    idDefensor--; // Ajusta para o índice do array

    // Validação
    if (idAtacante < 0 || idAtacante >= NUM_TERRITORIOS || idDefensor < 0 || idDefensor >= NUM_TERRITORIOS) {
        printf("IDs de territorios invalidos.\n");
        return;
    }
    if (strcmp(mapa[idAtacante].cor, corJogador) != 0) {
        printf("Voce so pode atacar com seus territorios!\n");
        return;
    }
    if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
        printf("Nao e possivel atacar um territorio do seu proprio exercito!\n");
        return;
    }

    simularAtaque(&mapa[idAtacante], &mapa[idDefensor], corJogador);
}

// simularAtaque(): Executa a lógica de batalha.
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* corJogador) {
    if (atacante->tropas <= 1) {
        printf("\nVocê precisa de no mínimo 2 tropas para atacar.\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--> Batalha: %s (%s) vs %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Tropas - Atacante: %d | Defensor: %d\n", atacante->tropas, defensor->tropas);
    printf("Dados - Atacante: %d | Defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA! O territorio %s foi conquistado!\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
    } else {
        printf("DERROTA! O ataque falhou.\n");
        atacante->tropas--;
    }
}

// sortearMissao(): Sorteia um ID de missão.
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

// verificarVitoria(): Verifica se a missão foi cumprida.
int verificarVitoria(const char* missao, const Territorio* mapa, const char* corJogador) {
    int territoriosJogador = 0;
    int territoriosInimigos = 0;
    int tropasInimigas = 0;
    int tropasFortes = 0;
    
    char corInimigo[TAM_COR];
    if (strcmp(corJogador, "Azul") == 0) {
        strcpy(corInimigo, "Vermelha");
    } else {
        strcpy(corInimigo, "Azul");
    }

    // Contagem para as missões
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosJogador++;
            if (mapa[i].tropas >= 5) {
                tropasFortes++;
            }
        } else {
            territoriosInimigos++;
            tropasInimigas += mapa[i].tropas;
        }
    }

    // Lógica das missões
    if (strstr(missao, "Conquistar todos os territorios") != NULL) {
        return (territoriosJogador == NUM_TERRITORIOS);
    }
    if (strstr(missao, "5 ou mais tropas") != NULL) {
        return (tropasFortes >= 3);
    }
    if (strstr(missao, "Conquistar 5 ou mais territorios") != NULL) {
        return (territoriosJogador >= 5);
    }
    if (strstr(missao, "Eliminar todas as tropas") != NULL) {
        return (tropasInimigas == 0);
    }
    
    return 0;
}