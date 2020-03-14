#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug 0

double *transpose(double *x1, double *x1t, int numHouses, int numAtt) {
    int i = 0;
    int j = 0;
    double num = 0;
    for (i = 0; i < numAtt + 1; i++) {
        for (j = 0; j < numHouses; j++) {
            num = *(x1 + j * (numAtt + 1) + i);
            *(x1t + i * (numHouses) + j) = num;
            ////printf("%f ", *(x1t + i * (numHouses) + j));
        }
        ////printf("\n");
    }
    return x1t;
}

double *multiply(double *x1t, double *x1, double *product, int rows1, int cols1, int rows2, int cols2) {
    int j = 0;
    int k = 0;
    for (j = 0; j < rows1; j++) {
        for (k = 0; k < cols2; k++) {
            int h = 0;
            double sum = 0;
            while (h < cols1) {
                sum += (*(x1t + j * cols1 + h) * *(x1 + h * cols2 + k));
                h++;
            }
            *(product + j * cols2 + k) = sum;
            ////printf("%f ", sum);
        }
        ////printf("\n");
    }
    for (j = 0; j < rows1; j++) {
        for (k = 0; k < cols2; k++) {
            //printf("%f ", *(product + j * cols2 + k));
        }
        //printf("\n");
    }

    return product;
}

double *invert(double *M, double *N, int cols) {
    int p = 0;

    int i = 0;
    double f = 0;
    int j = 0;

    for (p = 0; p < cols; p++) {
        f = *(M + p * cols + p);
        int j = 0;
        for (j = 0; j < cols; j++) {
            *(M + p * cols + j) /= f;
            *(N + p * cols + j) /= f;
        }
        i = p + 1;
        for (i = p + 1; i < cols; i++) {
            f = *(M + i * cols + p);
            j = 0;
            for (j = 0; j < cols; j++) {
                *(M + i * cols + j) -= (*(M + p * cols + j) * f);
                *(N + i * cols + j) -= (*(N + p * cols + j) * f);
            }
        }
    }
    for (p = cols - 1; p >= 0; p--) {
        for (i = p - 1; i >= 0; i--) {
            f = *(M + i * cols + p);
            int j = 0;
            for (j = 0; j < cols; j++) {
                *(M + i * cols + j) -= (*(M + p * cols + j) * f);
                *(N + i * cols + j) -= (*(N + p * cols + j) * f);
            }
        }
    }
    i = 0;
    j = 0;
    for (j = 0; j < cols; j++) {
        for (i = 0; i < cols; i++) {
            //printf("%f ", *(M + j * cols + i));
        }
        //printf("\n");
    }
    for (j = 0; j < cols; j++) {
        for (i = 0; i < cols; i++) {
            //printf("%f ", *(N + j * cols + i));
        }
        //printf("\n");
    }

    return N;
}

int main(int argc, char *argv[]) {
    FILE *f1 = NULL;
    FILE *f2 = NULL;
    if (!debug) {
        f1 = fopen(argv[1], "r");
        f2 = fopen(argv[2], "r");
    } else {
        f1 = fopen("C:\\Users\\djwon\\CLionProjects\\pa2\\train.00.txt", "r");
        f2 = fopen("C:\\Users\\djwon\\CLionProjects\\pa2\\data.00.txt", "r");
    }
    //printf("File loaded \n");
    char word1[20];
    char word2[20];
    fscanf(f1, "%s", word1);
    fscanf(f2, "%s", word2);
    FILE *train = NULL;
    FILE *data = NULL;
    if (!strcmp(word1, "train")) {
        train = f1;
        data = f2;
    } else {
        train = f2;
        data = f1;
    }
    //printf("Words scanned\n");

    int numAtt = 0;
    fscanf(train, "%d", &numAtt);
    int numHouses = 0;
    fscanf(train, "%d", &numHouses);
    int cols = numAtt + 1;
    double *x1 = (double *) (malloc(sizeof(double) * (cols) * numHouses));
    double *y1 = (double *) (malloc(sizeof(double) * numHouses));
    double *x1t = (double *) (malloc(sizeof(double) * (cols) * numHouses));
    int i = 0;
    int j = 0;
    double number = 0;
    //making X matrix
    for (i = 0; i < numHouses; i++) {
        for (j = 0; j < (cols); j++) {
            if (j == 0) {
                *(x1 + i * (cols) + j) = 1;
            } else {
                fscanf(train, "%lf", &number);
                *(x1 + i * (cols) + j) = number;
            }
            //printf("%f ", *(x1 + i * (cols) + j));
        }
        //printf("\n");
        fscanf(train, "%lf", &number);
        *(y1 + i) = number;
    }
    //printf("Matrix made\n");


    x1t = transpose(x1, x1t, numHouses, numAtt);
    double *product = (double *) (malloc(sizeof(double) * (cols) * (cols)));
    product = multiply(x1t, x1, product, cols, numHouses, numHouses, cols);
    //printf("Multiplied\n");
    free(x1);
    x1 = NULL;
    double *identity = (double *) (malloc(sizeof(double) * cols * cols));
    for (i = 0; i < cols; i++) {
        for (j = 0; j < cols; j++) {
            if (i == j)
                *(identity + i * cols + j) = 1;
            else
                *(identity + i * cols + j) = 0;

        }
    }
    identity = invert(product, identity, cols);
    //printf("Inverted\n");
    free(product);
    product = (double *) (malloc(sizeof(double) * cols * numHouses));
    //printf("Malloced product\n");
    product = multiply(identity, x1t, product, cols, cols, cols, numHouses);
    //printf("Producted\n");
    free(x1t);
    x1t = NULL;
    double *W = (double *) (malloc(sizeof(double) * cols));
    W = multiply(product, y1, W, cols, numHouses, numHouses, 1);
    //printf("Found W\n");
    free(x1);
    free(y1);
    free(x1t);
    free(product);
    free(identity);


    fscanf(data, "%d", &numAtt);
    fscanf(data, "%d", &numHouses);
    double *x2 = (double *) (malloc(sizeof(double) * (numAtt+1) * numHouses));
    for (i = 0; i < numHouses; i++) {
        for (j = 0; j < (numAtt + 1); j++) {
            if (j == 0) {
                *(x2 + i * (numAtt+1) + j) = 1;
            } else {
                fscanf(data, "%lf", &number);
                *(x2 + i * (numAtt+1) + j) = number;
            }
            //printf("%f ", *(x2 + i * (numAtt+1) + j));
        }
        //printf("\n");
    }
    //printf("x prime made\n");
    double *y2 = (double *) (malloc(sizeof(double) * numHouses));
    y2 = multiply(x2, W, y2, numHouses, numAtt + 1, cols, 1);
    for (j = 0; j < numHouses; j++) {
        printf("%.0f\n", *(y2 + j));
    }
    free(x2);
    free(W);
    free(y2);
    return 0;
}
