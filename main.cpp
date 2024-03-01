#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Matriz {
private:
  int largura;
  int altura;
  int valorMaximo;
  std::vector<std::vector<int>> matriz;

public:
  Matriz(int larg, int alt, int maxVal,
         const std::vector<std::vector<int>> &mat)
      : largura(larg), altura(alt), valorMaximo(maxVal), matriz(mat) {}

  int getLargura() const { return largura; }
  int getAltura() const { return altura; }
  int getValorMaximo() const { return valorMaximo; }
  const std::vector<std::vector<int>> &getMatriz() const { return matriz; }
};

class ImageLoader {
private:
  // Método para ler o cabeçalho da imagem
  void lerCabecalho(FILE *arquivo, int &largura, int &altura,
                    int &valorMaximo) {
    char tipo[20];
    if (fgets(tipo, 20, arquivo) == NULL) {
      printf("Erro ao ler o cabeçalho da imagem.\n");
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo) && linha[0] == '#')
      ;

    if (sscanf(linha, "%d %d", &largura, &altura) != 1) {
      // printf("Erro ao ler o tamanho da imagem.\n");
    };

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
      printf("Erro !\n");
    };
    if (sscanf(linha, "%d", &valorMaximo) != 1) {
      printf("Erro ao ler o valor máximo da imagem.\n");
    };
  }

  // Método para ler os dados da matriz da imagem
  std::vector<std::vector<int>> lerMatriz(FILE *arquivo, int largura,
                                          int altura) {
    std::vector<std::vector<int>> matriz(altura, std::vector<int>(largura));

    for (int i = 0; i < altura; ++i) {
      for (int j = 0; j < largura; ++j) {
        if (fscanf(arquivo, "%d", &matriz[i][j]) != 1) {
          printf("Erro ao ler pixel");
        };
      }
    }

    return matriz;
  }

public:
  // Método para carregar a matriz de pixels a partir de um arquivo
  Matriz load(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (!arquivo) {
      fprintf(stderr, "Erro ao abrir o arquivo.\n");
      exit(1);
    }

    int largura, altura, valorMaximo;
    lerCabecalho(arquivo, largura, altura, valorMaximo);
    std::vector<std::vector<int>> matriz = lerMatriz(arquivo, largura, altura);

    fclose(arquivo);

    return Matriz(largura, altura, valorMaximo, matriz);
  }
};

int main() {
  const char *nomeArquivo = "imagem.pgm";

  ImageLoader loader;
  Matriz matriz = loader.load(nomeArquivo);

  // Obtendo a matriz de pixels
  const std::vector<std::vector<int>> &pixels = matriz.getMatriz();

  printf("%d x %d\n", matriz.getLargura(), matriz.getAltura());
  printf("%d\n", matriz.getValorMaximo());

  // Imprimindo a matriz
  printf("Matriz:\n");
  for (const auto &linha : pixels) {
    for (int valor : linha) {
      printf("%d ", valor);
    }
    printf("\n");
  }

  return 0;
}
