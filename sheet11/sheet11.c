#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    FILE *file = NULL;
    file = fopen(argv[1], "r");

    if(!file){
        printf("Error reading file %s\n", argv[1]);
        return -1;
    }

    // read the dimension and amount of equations from the file
    int n, k;
    fscanf(file, "%d %d", &n, &k);

    printf("%dx%d-Matrix\n", n, n);
    printf("%d equations to solve\n\n", k);

    // read the matrix from the file
    int *matrix = malloc(sizeof(int) * n * n);
    for(int i = 0; i < n*n; i++){
        fscanf(file, "%d", matrix+i);
    }

    // print the matrix to the console
    printf("Input Matrix:\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%4d", *(matrix+(i*n)+j));
        }
        printf("\n");
    }
    printf("\n");


    // read the equations from the file
    long *equations = malloc(sizeof(long) * n *k);
    for(int i = 0; i < n*k; i++){
        fscanf(file, "%d", equations+i);
    }

    fclose(file);

    // create the LU decomposition
    for(int i = 0; i < n-1; i++){
        for(int l = i+1; l < n; l++){
            if(*(matrix+(i*n)+i)){
                *(matrix+(l*n)+i) = *(matrix+(l*n)+i) / *(matrix+(i*n)+i);
                for(int j = i+1; j < n; j++){
                    *(matrix+(l*n)+j) = *(matrix+(l*n)+j) - *(matrix+(l*n)+i) * *(matrix+(i*n)+j);
                }
            }
        }
    }

    // print the LU-matrix to the console
    printf("LU-Matrix:\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%4d", *(matrix+(i*n)+j));
        }
        printf("\n");
    }
    printf("\n");

    // solving the equations
    for(int i = 0; i < k; i++){
        // print the input b to the console
        printf("Equation %d:\n", i+1);
        for(int j = 0; j < n; j++){
            printf("%4d", *(equations+(n*i)+j));
        }
        printf("\n");

        // solve L * y = b for y
        for(int j = 0; j < n; j++){
            for(int l = 0; l < j; l++){
                *(equations+(n*i)+j) -= *(equations+(n*i)+l) * *(matrix+(j*n)+l);
            }
        }

        // solve U * x = y for x
        for(int j = n-1; j >= 0; j--){
            if(*(matrix+(j*n)+j)){
                for(int l = j+1; l < n; l++){
                    *(equations+(n*i)+j) -= *(matrix+(n*j)+l) * *(equations+(n*i)+l);
                }
                *(equations+(n*i)+j) /= *(matrix+(j*n)+j);
            }
        }

        // print the solution x to the console
        printf("Solution %d:\n", i+1);
        for(int j = 0; j < n; j++){
            printf("%4d", *(equations+(n*i)+j));
        }
        printf("\n\n");
    }

    // write solutions to file
    file = fopen(argv[1], "a");

    fprintf(file, "\n\nSolutions:\n");
    for(int i = 0; i < k; i++){
        fprintf(file, "\n");
        for(int j = 0; j < n; j++){
            fprintf(file, "%d ", *(equations+(n*i)+j));
        }
        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}
