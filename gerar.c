#include <stdio.h>
#include "compartilhado.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> // Para a função access()

// Função para calcular o dígito verificador do EAN-8
int calcular_digito_verificador(const char *codigo) {
    int soma = 0;

    for (int i = 0; i < 7; i++) {
        int digito = codigo[i] - '0';
        soma += (i % 2 == 0) ? 3 * digito : digito;
    }

    return (10 - (soma % 10)) % 10;
}

// Gera o código EAN-8 completo
void gerar_codigo_ean8(const char *codigo, CodigoDeBarras codigos[], char *resultado) {
    strcpy(resultado, "101");

    for (int i = 0; i < 4; i++) {
        int digito = codigo[i] - '0';
        strcat(resultado, codigos[digito].codigo_inicial);
    }

    strcat(resultado, "01010");

    for (int i = 4; i < 8; i++) {
        int digito = codigo[i] - '0';
        strcat(resultado, codigos[digito].codigo_final);
    }

    strcat(resultado, "101");
}

// Função para validar o código EAN-8
int validar_codigo_ean8(const char *codigo) {
    if (strlen(codigo) != 8) {
        printf("Erro: O codigo deve conter exatamente 8 digitos.\n");
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(codigo[i])) {
            printf("Erro: O codigo deve conter apenas numeros.\n");
            return 0;
        }
    }

    int digito_verificador_calculado = calcular_digito_verificador(codigo);
    int digito_verificador_informado = codigo[7] - '0';

    if (digito_verificador_calculado != digito_verificador_informado) {
        printf("Erro: Digito verificador do codigo invalido.\n");
        return 0;
    }

    return 1;
}

// Cria uma imagem PBM com o código EAN-8
void criar_imagem_pbm(const char *codigo_completo, int margem, int altura, int largura_barra, const char *nome_arquivo) {
    int largura_total = strlen(codigo_completo) * largura_barra + 2 * margem;
    int altura_total = altura + 2 * margem;

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo PBM");
        return;
    }

    fprintf(arquivo, "P1\n%d %d\n", largura_total, altura_total);

    for (int i = 0; i < margem; i++) {
        for (int j = 0; j < largura_total; j++) {
            fputc('0', arquivo);
            fputc(' ', arquivo);
        }
        fputc('\n', arquivo);
    }

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < margem; j++) {
            fputc('0', arquivo);
            fputc(' ', arquivo);
        }

        for (int j = 0; j < strlen(codigo_completo); j++) {
            char bit = codigo_completo[j];
            for (int k = 0; k < largura_barra; k++) {
                fputc(bit, arquivo);
                fputc(' ', arquivo);
            }
        }

        for (int j = 0; j < margem; j++) {
            fputc('0', arquivo);
            fputc(' ', arquivo);
        }

        fputc('\n', arquivo);
    }

    for (int i = 0; i < margem; i++) {
        for (int j = 0; j < largura_total; j++) {
            fputc('0', arquivo);
            fputc(' ', arquivo);
        }
        fputc('\n', arquivo);
    }

    fclose(arquivo);
    printf("Imagem PBM criada: %s\n", nome_arquivo);
}

int main(int argc, char *argv[]) {
    CodigoDeBarras codigos[10];
    inicializar_codigos(codigos);

    if (argc < 2 || argc > 6) {
        printf("Uso: %s <codigo_ean8> [margem=4] [largura_barra=3] [altura=50] [nome_arquivo=auto]\n", argv[0]);
        return 1;
    }

    char *codigo = argv[1];

    if (!validar_codigo_ean8(codigo)) {
        return 1;
    }

    int margem = (argc > 2) ? atoi(argv[2]) : 4;
    int largura_barra = (argc > 3) ? atoi(argv[3]) : 3;
    int altura = (argc > 4) ? atoi(argv[4]) : 50;
    
    char nome_arquivo[100];
    if (argc > 5) {
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.pbm", argv[5]);
    } else {
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.pbm", codigo);
    }

    // Verifica se o arquivo já existe
    if (access(nome_arquivo, F_OK) == 0) {
        printf("O arquivo %s ja existe. Deseja sobrescreve-lo? (s/n): ", nome_arquivo);
        char resposta;
        scanf(" %c", &resposta);
        if (resposta != 's' && resposta != 'S') {
            printf("Arquivo resultante ja existe\n");
            return 0;
        }
    }

    if (margem < 0 || altura <= 0 || largura_barra <= 0) {
        printf("Erro: Margem, altura e largura devem ser maiores ou iguais a zero.\n");
        return 1;
    }

    char resultado[100] = "";
    gerar_codigo_ean8(codigo, codigos, resultado);

    criar_imagem_pbm(resultado, margem, altura, largura_barra, nome_arquivo);

    return 0;
}
