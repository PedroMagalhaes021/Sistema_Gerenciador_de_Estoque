#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "produtos.dat"
#define TAM_NOME 100
#define TAM_CATEGORIA 50

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char categoria[TAM_CATEGORIA];
    int quantidade;
    float preco;
} Produto;

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void remover_newline(char *texto) {
    texto[strcspn(texto, "\n")] = '\0';
}

int codigo_existe(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Produto produto;

    if (arquivo == NULL) return 0;

    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1) {
        if (produto.codigo == codigo) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

void cadastrar_produto(void) {
    Produto produto;
    FILE *arquivo;

    printf("\n=== CADASTRAR PRODUTO ===\n");

    printf("Codigo: ");
    scanf("%d", &produto.codigo);
    limpar_buffer();

    if (codigo_existe(produto.codigo)) {
        printf("Erro: ja existe um produto com esse codigo.\n");
        return;
    }

    printf("Nome: ");
    fgets(produto.nome, TAM_NOME, stdin);
    remover_newline(produto.nome);

    printf("Categoria: ");
    fgets(produto.categoria, TAM_CATEGORIA, stdin);
    remover_newline(produto.categoria);

    printf("Quantidade em estoque: ");
    scanf("%d", &produto.quantidade);

    printf("Preco unitario: R$ ");
    scanf("%f", &produto.preco);
    limpar_buffer();

    arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fwrite(&produto, sizeof(Produto), 1, arquivo);
    fclose(arquivo);

    printf("Produto cadastrado com sucesso!\n");
}

void listar_produtos(void) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Produto produto;
    int encontrou = 0;

    printf("\n=== LISTA DE PRODUTOS ===\n");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1) {
        encontrou = 1;
        printf("\nCodigo: %d\n", produto.codigo);
        printf("Nome: %s\n", produto.nome);
        printf("Categoria: %s\n", produto.categoria);
        printf("Quantidade: %d\n", produto.quantidade);
        printf("Preco: R$ %.2f\n", produto.preco);
    }

    if (!encontrou) {
        printf("Nenhum produto cadastrado.\n");
    }

    fclose(arquivo);
}

void buscar_produto(void) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Produto produto;
    int codigo;
    int encontrado = 0;

    printf("\n=== BUSCAR PRODUTO ===\n");
    printf("Digite o codigo: ");
    scanf("%d", &codigo);
    limpar_buffer();

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1) {
        if (produto.codigo == codigo) {
            printf("\nProduto encontrado!\n");
            printf("Codigo: %d\n", produto.codigo);
            printf("Nome: %s\n", produto.nome);
            printf("Categoria: %s\n", produto.categoria);
            printf("Quantidade: %d\n", produto.quantidade);
            printf("Preco: R$ %.2f\n", produto.preco);
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("Produto nao encontrado.\n");
    }
}

void calcular_valor_estoque(void) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Produto produto;
    double total = 0.0;

    printf("\n=== VALOR TOTAL DO ESTOQUE ===\n");

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1) {
        total += (double)produto.quantidade * produto.preco;
    }

    fclose(arquivo);

    printf("Valor total do estoque: R$ %.2f\n", total);
}

void atualizar_estoque(void) {
    FILE *arquivo = fopen(ARQUIVO, "r+b");
    Produto produto;
    int codigo;
    int nova_quantidade;
    int encontrado = 0;

    printf("\n=== ATUALIZAR ESTOQUE ===\n");
    printf("Digite o codigo do produto: ");
    scanf("%d", &codigo);

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        limpar_buffer();
        return;
    }

    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1) {
        if (produto.codigo == codigo) {
            printf("Produto: %s\n", produto.nome);
            printf("Quantidade atual: %d\n", produto.quantidade);
            printf("Nova quantidade: ");
            scanf("%d", &nova_quantidade);

            if (nova_quantidade < 0) {
                printf("A quantidade nao pode ser negativa.\n");
                fclose(arquivo);
                limpar_buffer();
                return;
            }

            produto.quantidade = nova_quantidade;

            fseek(arquivo, -(long)sizeof(Produto), SEEK_CUR);
            fwrite(&produto, sizeof(Produto), 1, arquivo);

            printf("Estoque atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);
    limpar_buffer();

    if (!encontrado) {
        printf("Produto nao encontrado.\n");
    }
}

void mostrar_menu(void) {
    printf("\n====================================\n");
    printf("       SISTEMA DE PRODUTOS\n");
    printf("====================================\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Buscar produto\n");
    printf("4 - Calcular valor do estoque\n");
    printf("5 - Atualizar quantidade em estoque\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

int main(void) {
    int opcao;

    do {
        mostrar_menu();
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                cadastrar_produto();
                break;
            case 2:
                listar_produtos();
                break;
            case 3:
                buscar_produto();
                break;
            case 4:
                calcular_valor_estoque();
                break;
            case 5:
                atualizar_estoque();
                break;
            case 0:
                printf("\nPrograma encerrado.\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
