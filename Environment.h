#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


class Environment
{
    public:
        Environment();
        Environment(int gradeDimensionX, int gradeDimensionY);

        // construtor usando dados armazenados em um arquivo

        // Metodos

            //  adicionar obstaculos a partir de um arquivo - (Flavio)
            //  imprimir o ambiente no terminal - ( Duda)
            //  armazenar o ambiente no um arquivo

        // Metodos - Aguardando resposta

            //  adicionar um obstaculo ao ambiente (celula)
            //  adicionar um obstaculo ao ambiente (retangulo)

    private:

    // Var
    int _gradeDimensionX;
    int _gradeDimensionY;
    int **_grade;

    // Methodes


};

#endif // ENVIRONMENT_H
