#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 10000
#define MAX_KEY 20

double freq_pt[26] = {
    14.63,1.04,3.88,4.99,12.57,1.02,1.30,1.28,6.18,
    0.40,0.02,2.78,4.74,5.05,10.73,2.52,1.20,6.53,
    7.81,4.34,4.63,1.67,0.01,0.21,0.01,0.47
};

double freq_en[26] = {
    8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,
    0.153,0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,
    6.327,9.056,2.758,0.978,2.360,0.150,1.974,0.074
};

// ponteiro global para a tabela escolhida
double *freq;
// limpar texto
int limpar_texto(char entrada[], char saida[]) {
    int j = 0;
    for (int i = 0; entrada[i]; i++) {
        if (isalpha(entrada[i])) {
            saida[j++] = toupper(entrada[i]);
        }
    }
    saida[j] = '\0';
    return j;
}

// calcula score (quanto menor melhor)
double calcular_score(int contagem[], int total) {
    double score = 0;

    for (int i = 0; i < 26; i++) {
        double esperado = freq[i] * total / 100.0;
        double diff = contagem[i] - esperado;
        score += diff * diff / (esperado + 1); // chi-square simplificado
    }

    return score;
}

// melhor deslocamento para uma posição
int melhor_shift(char texto[], int inicio, int passo, int tamanho) {
    int melhor = 0;
    double melhor_score = 1e9;

    for (int shift = 0; shift < 26; shift++) {
        int contagem[26] = {0};
        int total = 0;

        for (int i = inicio; i < tamanho; i += passo) {
            int letra = (texto[i] - 'A' - shift + 26) % 26;
            contagem[letra]++;
            total++;
        }

        double score = calcular_score(contagem, total);

        if (score < melhor_score) {
            melhor_score = score;
            melhor = shift;
        }
    }

    return melhor;
}

// descobrir chave
void descobrir_chave(char texto[], int tamanho, char chave[], int tam_chave) {
    for (int i = 0; i < tam_chave; i++) {
        int shift = melhor_shift(texto, i, tam_chave, tamanho);
        chave[i] = shift + 'A';
    }
    chave[tam_chave] = '\0';
}

// decifrar
void decifrar(char texto[], char chave[], char resultado[]) {
    int i, j = 0;
    int tam_chave = strlen(chave);

    for (i = 0; texto[i]; i++) {
        if (isalpha(texto[i])) {
            char c = toupper(texto[i]);
            char k = chave[j % tam_chave];

            resultado[i] = ((c - 'A' - (k - 'A') + 26) % 26) + 'A';
            j++;
        } else {
            resultado[i] = texto[i];
        }
    }
    resultado[i] = '\0';
}

int main() {
    FILE *entrada, *saida;

    char nome_entrada[100], nome_saida[100];
    char bruto[MAX], limpo[MAX], resultado[MAX];
    char chave[MAX_KEY + 1];

    int idioma;

    printf("Escolha o idioma:\n1 - Portugues\n2 - Ingles\nOpcao: ");
    scanf("%d", &idioma);
    getchar();

    if (idioma == 1) {
        freq = freq_pt;
    } else if (idioma == 2) {
        freq = freq_en;
    } else {
        printf("Opcao invalida.\n");
        return 1;
    }

    printf("Arquivo criptografado: ");
    fgets(nome_entrada, 100, stdin);
    nome_entrada[strcspn(nome_entrada, "\n")] = '\0';

    printf("Arquivo de saída: ");
    fgets(nome_saida, 100, stdin);
    nome_saida[strcspn(nome_saida, "\n")] = '\0';

    entrada = fopen(nome_entrada, "r");
    if (!entrada) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    size_t len = fread(bruto, 1, MAX - 1, entrada);
    bruto[len] = '\0';
    fclose(entrada);

    int tamanho = limpar_texto(bruto, limpo);

    int melhor_tam = 1;
    double melhor_score = 1e9;

    // testar tamanhos de chave
    for (int tam = 1; tam <= MAX_KEY; tam++) {
        char chave_temp[MAX_KEY + 1];
        descobrir_chave(limpo, tamanho, chave_temp, tam);

        char teste[MAX];
        decifrar(limpo, chave_temp, teste);

        int contagem[26] = {0};
        for (int i = 0; teste[i]; i++) {
            contagem[teste[i] - 'A']++;
        }

        double score = calcular_score(contagem, tamanho);

        if (score < melhor_score) {
            melhor_score = score;
            melhor_tam = tam;
        }
    }

    descobrir_chave(limpo, tamanho, chave, melhor_tam);

    printf("Tamanho da chave: %d\n", melhor_tam);
    printf("Chave encontrada: %s\n", chave);

    decifrar(bruto, chave, resultado);

    saida = fopen(nome_saida, "w");
    fprintf(saida, "%s", resultado);
    fclose(saida);

    printf("Decifrado com sucesso.\n");

    return 0;
}
