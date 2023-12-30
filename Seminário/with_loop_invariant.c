#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 666

int main() {
    clock_t begin = clock();

    int gerador = 222, de = 222, codigo = 222;
    int ***matriz_grande = malloc(N * sizeof(int**));
    for (int i = 0; i < N; i++) {
        matriz_grande[i] = (int**) malloc(N * sizeof(int*));
        for (int j = 0; j < N; j++) {
            matriz_grande[i][j] = (int*) malloc(N * sizeof(int));
        }
    }

    int expressao1 = gerador + de + codigo;
    int expressao2 = gerador - de - codigo;
    int expressao3 = gerador * de * codigo;
    int expressao4 = gerador / de / codigo;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                matriz_grande[i][j][k] = expressao1;
                matriz_grande[i][j][k] = expressao2;
                matriz_grande[i][j][k] = expressao3;
                matriz_grande[i][j][k] = expressao4;
            }
        }
    }

    printf("Time elapsed with loop invariant is %f seconds\n", (double)(clock() - begin) / CLOCKS_PER_SEC);

    return 0;
}