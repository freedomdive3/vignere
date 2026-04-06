#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10000

void cifrar(char mensagem[], char chave[], char resultado[]) {
    int i, j = 0;
    int tam_chave = strlen(chave);

    for (i = 0; mensagem[i] != '\0'; i++) {
        if (isalpha(mensagem[i])) {
            char m = toupper(mensagem[i]);
            char k = toupper(chave[j % tam_chave]);

            resultado[i] = ((m - 'A' + k - 'A') % 26) + 'A';
            j++;
        } else {
            resultado[i] = mensagem[i];
        }
    }
    resultado[i] = '\0';
}

void decifrar(char criptograma[], char chave[], char resultado[]) {
    int i, j = 0;
    int tam_chave = strlen(chave);

    for (i = 0; criptograma[i] != '\0'; i++) {
        if (isalpha(criptograma[i])) {
            char c = toupper(criptograma[i]);
            char k = toupper(chave[j % tam_chave]);

            resultado[i] = ((c - 'A' - (k - 'A') + 26) % 26) + 'A';
            j++;
        } else {
            resultado[i] = criptograma[i];
        }
    }
    resultado[i] = '\0';
}

int main() {
    FILE *entrada, *saida;

    char nome_entrada[100];
    char nome_saida[100];
    char chave[100];

    char mensagem[MAX];
    char resultado[MAX];

    int opcao;

    printf("1 - Cifrar\n2 - Decifrar\nEscolha: ");
    scanf("%d", &opcao);
    getchar(); // limpar buffer

    printf("Digite o nome do arquivo de entrada: ");
    fgets(nome_entrada, 100, stdin);
    nome_entrada[strcspn(nome_entrada, "\n")] = '\0';

    printf("Digite o nome do arquivo de saída: ");
    fgets(nome_saida, 100, stdin);
    nome_saida[strcspn(nome_saida, "\n")] = '\0';

    printf("Digite a chave: ");
    fgets(chave, 100, stdin);
    chave[strcspn(chave, "\n")] = '\0';

    entrada = fopen(nome_entrada, "r");
    if (entrada == NULL) {
        printf("Erro ao abrir arquivo de entrada.\n");
        return 1;
    }

    size_t len = fread(mensagem, 1, MAX - 1, entrada);
    mensagem[len] = '\0';
    fclose(entrada);

    if (opcao == 1) {
        cifrar(mensagem, chave, resultado);
        printf("Arquivo cifrado com sucesso.\n");
    } else if (opcao == 2) {
        decifrar(mensagem, chave, resultado);
        printf("Arquivo decifrado com sucesso.\n");
    } else {
        printf("Opcao invalida.\n");
        return 1;
    }

    saida = fopen(nome_saida, "w");
    if (saida == NULL) {
        printf("Erro ao abrir arquivo de saída.\n");
        return 1;
    }

    fprintf(saida, "%s", resultado);
    fclose(saida);

    return 0;
}
