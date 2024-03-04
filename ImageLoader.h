#include <iostream>
#include <vector>
#include "Matriz.h"

using namespace std;

class ImageLoader
{
    private:
    void lerCabecalho(FILE *arquivo, int &largura, int &altura, int &valorMaximo)
    {
        char tipo[20];

        if(fgets(tipo, 20, arquivo) == NULL) cout << "Erro ao ler o cabeçalho da imagem.\n" << endl;

        char linha[256];

        while(fgets(linha, sizeof(linha), arquivo) && linha[0] == '#');

        if(sscanf(linha, "%d %d", &largura, &altura) != 1);

        if(fgets(linha, sizeof(linha), arquivo) == NULL) cout << "Erro!\n" << endl;

        if(sscanf(linha, "%d", &valorMaximo) != 1) cout << "Erro ao ler o valor máximo da imagem.\n" << endl;
    }

    vector<vector<int>> lerMatriz(FILE *arquivo, int largura, int altura)
    {
        vector<vector<int>> matriz(altura, vector<int>(largura));

        for(int i = 0; i < altura; i++)
            for(int j = 0; j < largura; j++)
                if(fscanf(arquivo, "%d", &matriz[i][j]) != 1) cout << "Erro ao ler pixel" << endl;

        return matriz;
    }

    public:
    Matriz load(const char *nomeArquivo)
    {
        FILE *arquivo = fopen(nomeArquivo, "r");

        if(!arquivo)
        {
            cout << "Erro ao abrir o arquivo.\n" << endl;
            exit(1);
        }

        int largura, altura, valorMaximo;

        lerCabecalho(arquivo, largura, altura, valorMaximo);

        vector<vector<int>> matriz = lerMatriz(arquivo, largura, altura);

        fclose(arquivo);

        return Matriz(largura, altura, valorMaximo, matriz);
    }
};