#ifndef CODIGO_DE_BARRAS_H
#define CODIGO_DE_BARRAS_H

typedef struct {
    int numero;
    char codigo_inicial[8];
    char codigo_final[8];
} CodigoDeBarras;

// Inicializa os códigos de barras
void inicializar_codigos(CodigoDeBarras codigos[]) {
    const char *padroes_iniciais[] = {
        "0001101", "0011001", "0010011", "0111101",
        "0100011", "0110001", "0101111", "0111011",
        "0110111", "0001011"
    };
    const char *padroes_finais[] = {
        "1110010", "1100110", "1101100", "1000010",
        "1011100", "1001110", "1010000", "1000100",
        "1001000", "1110100"
    };

    for (int i = 0; i < 10; i++) {
        codigos[i].numero = i;
        snprintf(codigos[i].codigo_inicial, sizeof(codigos[i].codigo_inicial), "%s", padroes_iniciais[i]);
        snprintf(codigos[i].codigo_final, sizeof(codigos[i].codigo_final), "%s", padroes_finais[i]);
    }
}

#endif
