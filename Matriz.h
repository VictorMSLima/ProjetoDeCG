#include <vector>
#include <iostream>

using namespace std;

class Matriz
{
    private:
    int largura;
    int altura;
    int valorMaximo;
    vector<vector<int>> matriz;

    public:
    Matriz(int largura, int altura, int valorMaximo, vector<vector<int>> &matriz)
    {
        this->largura = largura;
        this->altura = altura;
        this->valorMaximo = valorMaximo;
        this->matriz = matriz;
    }

    int getValorPosicao(int x, int y)
    {
        if(x >= 0 && x < this->largura && y >= 0 && y < this->altura) return this->matriz[x][y];
        else {
            cout << "Posição fora dos limites da matriz.\n" << endl;
            exit(1);
        }
    }

    int getLargura() { return this->largura; }

    int getAltura() { return this->altura; }

    int getValorMaximo() { return this->valorMaximo; }

    vector<vector<int>> getMatriz() { return this->matriz; }
};