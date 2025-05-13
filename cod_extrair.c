#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compartilhado.h"

// Função para verificar se o arquivo é um PBM válido
int verificar_pbm(FILE *arquivo) {
    char formato[3];
    if (fscanf(arquivo, "%2s", formato) != 1 || strcmp(formato, "P1") != 0) {
        printf("Erro: Arquivo PBM invalido.\n");
        return 0;
    }
    return 1;
}


// Função para verificar se uma coluna é composta apenas por 0s
int verificar_coluna_zeros(int **imagem, int altura, int coluna) {
    for (int i = 0; i < altura; i++) {
        if (imagem[i][coluna] != 0) {  // Se algum valor na coluna não for 0
            return 0;  // Retorna 0 se a coluna contiver valores diferentes de 0
        }
    }
    return 1;  // Retorna 1 se todos os valores da coluna forem 0
}


// Função para comparar código
int comparar_codigo(const char *codigo_binario, const char *codigo_comparar) {
    return strncmp(codigo_binario, codigo_comparar, 7) == 0;
}


// Função para verificar se uma linha é composta apenas por 0s
int verificar_linha_zeros(int *linha, int largura) {
    for (int i = 0; i < largura; i++) {
        if (linha[i] != 0) {  // Se algum valor na linha não for 0
            return 0;  // Retorna 0 se a linha contiver valores diferentes de 0
        }
    }
    return 1;  // Retorna 1 se todos os valores da linha forem 0
}


int main(int argc, char *argv[]) {
    int invalido = 0;
    // Verificar se o nome do arquivo foi fornecido
    if (argc != 2) {
        printf("Uso: %s <nome_arquivo.pbm>\n", argv[0]);
        return 1;
    }
    const char *nome_arquivo = argv[1];

    CodigoDeBarras codigos[10];
    inicializar_codigos(codigos); 
    
    // Abrir o arquivo PBM
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Verificar se é um PBM válido
    if (!verificar_pbm(arquivo)) {
        fclose(arquivo);
        return 1;
    }

    // Ler dimensões da imagem
    int largura, altura;
    if (fscanf(arquivo, "%d %d", &largura, &altura) != 2) {
        printf("Erro: Dimensoes invalidas no arquivo PBM.\n");
        fclose(arquivo);
        return 1;
    }

    // Ler o conteúdo do PBM
    int **imagem = malloc(altura * sizeof(int *));
    for (int i = 0; i < altura; i++) {
        imagem[i] = malloc(largura * sizeof(int));
        for (int j = 0; j < largura; j++) {
            if (fscanf(arquivo, "%d", &imagem[i][j]) != 1) {
                printf("Erro: Dados invalidos no arquivo PBM.\n");
                for (int k = 0; k <= i; k++) {
                    free(imagem[k]);
                }
                free(imagem);
                fclose(arquivo);
                return 1;
            }
        }
    }
    fclose(arquivo);

    // Contar as margens compostas apenas por 0s
    int margens_superior_zeros = 0;
    int margens_inferior_zeros = 0;
    int margens_esquerda_zeros = 0;
    int margens_direita_zeros = 0;

    // Verificar a margem superior
    for (int i = 0; i < altura; i++) {
        if (verificar_linha_zeros(imagem[i], largura)) {
            margens_superior_zeros++;
        } else {
            break;  // Interrompe assim que encontrar uma linha não composta só por 0s
        }
    }

    // Verificar a margem inferior
    for (int i = altura - 1; i >= 0; i--) {
        if (verificar_linha_zeros(imagem[i], largura)) {
            margens_inferior_zeros++;
        } else {
            break;  // Interrompe assim que encontrar uma linha não composta só por 0s
        }
    }

    // Verificar a margem esquerda
    for (int j = 0; j < largura; j++) {
        if (verificar_coluna_zeros(imagem, altura, j)) {
            margens_esquerda_zeros++;
        } else {
            break;  // Interrompe assim que encontrar uma coluna não composta só por 0s
        }
    }

    // Verificar a margem direita
    for (int j = largura - 1; j >= 0; j--) {
        if (verificar_coluna_zeros(imagem, altura, j)) {
            margens_direita_zeros++;
        } else {
            break;  // Interrompe assim que encontrar uma coluna não composta só por 0s
        }
    }

    // Subtrair o número das margens e dividir por 67 apenas para a largura
    int largura_resultado = (largura - margens_esquerda_zeros - margens_direita_zeros) / 67;

    int j;
     char resultado[largura];  // Tamanho máximo da string (ajuste conforme necessário)
    int pos = 0;  // Posição no resultado

    // Laço para pegar os valores das colunas
    for (int j = margens_esquerda_zeros; j < largura - margens_direita_zeros; j += largura_resultado) {
        // Armazena o valor como um caractere na string (0 ou 1)
        resultado[pos++] = '0' + imagem[0 + margens_superior_zeros][j]; // '0' + valor para transformar em caractere
    }
    // Adicionando o terminador nulo para formar uma string
    resultado[pos] = '\0';
     // Verificar se a string tem 67 caracteres
   if (strlen(resultado) == 67) {
        // Verificar se começa com "101"
        if (strncmp(resultado, "101", 3) == 0) {
            // Verificar se termina com "101"
            if (strcmp(resultado + 64, "101") == 0) {
                // Verificar se tem "01010" a partir do 32º caractere
                if (strncmp(resultado + 31, "01010", 5) == 0) {
                    printf("Codigo de barras EAN-8 valido.\n");
                } else {
                    printf("Codigo de barras EAN-8 invalido.\n");
                    invalido = 1;
                }
            } else {
                printf("Codigo de barras EAN-8 invalido.\n");
                invalido = 1;
            }
        } else {
            printf("Codigo de barras EAN-8 invalido.\n");
            invalido = 1;
        }
    } else {
        printf("Codigo de barras EAN-8 invalido.\n");
        invalido = 1;
    }
    int a, i;
    if(invalido == 0){
        printf("O identificador de codigo de barras EAN-8 extraido:");
    }
    for(a=3; a<33; a+=7){
        for (i = 0; i < 10; i++) {
            // Comparando com código inicial
            if (comparar_codigo(&resultado[a], codigos[i].codigo_inicial) && invalido == 0) {
                printf("%d", codigos[i].numero);
            }
        }
    }    
    for(a=36; a<64; a+=7){
        for (i = 0; i < 10; i++) {
            // Comparando com código inicial
            if (comparar_codigo(&resultado[a], codigos[i].codigo_final) && invalido == 0) {
                printf("%d", codigos[i].numero);
            }
        }
    }    
    
    // Liberar memória da imagem
    for (int i = 0; i < altura; i++) {
        free(imagem[i]);
    }
    free(imagem);

    return 0;
}

