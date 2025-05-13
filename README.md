# 🧾 Gerador e Leitor de Códigos de Barras EAN-8 (PBM) em C

Este projeto implementa dois programas em C para **gerar** e **extrair** imagens de **códigos de barras EAN-8** no formato **PBM (Portable Bitmap)**.

---

## 📁 Estrutura do Projeto

- `gerar.c`: Gera uma imagem `.pbm` contendo um código de barras EAN-8 a partir de um identificador numérico.
- `extrair.c`: Lê uma imagem `.pbm`, verifica se há um código de barras EAN-8 e extrai o identificador, se possível.
- `compartilhado.h`: Define structs e funções auxiliares utilizadas por ambos os programas, incluindo manipulação de imagem e validação de identificadores.

---

## 🔧 Compilação

Use `gcc` para compilar os arquivos:

```bash
gcc gerar.c -o gerar
gcc extrair.c -o extrair