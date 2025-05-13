# Leitor e Gerador de Códigos de Barras em C (PBM)

Este projeto contém dois programas em C (`gerar.c` e `extrair.c`) capazes de **gerar imagens PBM com códigos de barras** e **extrair identificadores a partir dessas imagens**. Ambos compartilham funcionalidades comuns através do arquivo `compartilhado.h`.

---

## 📂 Estrutura do Projeto

- `gerar.c`: Gera uma imagem `.pbm` com um código de barras a partir de um identificador numérico.
- `extrair.c`: Lê uma imagem `.pbm` e extrai o identificador contido no código de barras.
- `compartilhado.h`: Arquivo com structs e funções compartilhadas entre os dois programas.

---

## 🔧 Compilação

Use `gcc` para compilar:

```bash
gcc gerar.c -o gerar
gcc extrair.c -o extrair