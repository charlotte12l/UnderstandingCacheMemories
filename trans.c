/*
 * student ID: 516021910154
 * student NMAE: Xingyu Liu
 */
/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 *
 */
#include <stdio.h>
#include <stdbool.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void transpose_32(int M, int N, int A[N][M], int B[M][N]);
void transpose_64(int M, int N, int A[N][M], int B[M][N]);
void transpose_61(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
	switch(N) {
    	case 32:	// 32x32 matrix
    		transpose_32(M, N, A, B);
        	break;
    	case 64:	// 64x64 matrix
    		transpose_64(M, N, A, B);
      		break;
      	default:	// 61x67 matrix
        	transpose_61(M, N, A, B);
        	break;
    }
}

/* 32x32 matrix use 8x8 as a unit to decrease misses
 * I use blocking and defers assigning the diagonals until the end of the block.
 * The diagonal deferring allows there to be less conflict misses.
 */
char transpose_32_desc[] = "Transpose a 32x32 matrix";
void transpose_32(int M, int N, int A[N][M], int B[M][N]){
    int diagonal = 0;
    int diagonal_index = 0;
    int x_block = 8; // Set the size of the unit
    int y_block = 8;
    for (int i = 0; i < N; i += x_block) {
        for (int j = 0; j < M; j += y_block) {
            for (int k = i; k < i + x_block && k < N; ++k) {
                for (int l = j; l < j + y_block && l < M; ++l) {
                    if (l != k) { // Defer elements on the diagonal
                        B[l][k] = A[k][l];
                    } else if (l == k) {
                        diagonal = A[l][l];
                        diagonal_index = l;
                    }
                }
                if (i == j && k < N) {// deal with diagonal elements
                    B[diagonal_index][diagonal_index] = diagonal;
                }
            }
        }
    }
}

// The process is illustrated detailly in the report's figure.
char transpose_64_desc[] = "Transpose a 64x64 matrix";
void transpose_64(int M, int N, int A[N][M], int B[M][N]){
    int i, j, k;
    int a1, a2, a3, a4, a5, a6, a7, a8;
        for(i=0; i<N; i+=8) {
            for(j=0; j<M; j+=8) {
/*1.deal with the top half of A line by line and transfer it to the top half of B*/
                for(k=j; k<j+4; ++k) {
                    a1=A[k][i];
                    a2=A[k][i+1];
                    a3=A[k][i+2];
                    a4=A[k][i+3];
                    a5=A[k][i+4];
                    a6=A[k][i+5];
                    a7=A[k][i+6];
                    a8=A[k][i+7];

                    B[i][k]=a1;
                    B[i+1][k]=a2;
                    B[i+2][k]=a3;
                    B[i+3][k]=a4;
                    B[i][k+4]=a5;
                    B[i+1][k+4]=a6;
                    B[i+2][k+4]=a7;
                    B[i+3][k+4]=a8;
                }
/*
 *2. Load the upper-right row of B into the intermediate variables a1-a4,
 *   load the lower-left col of A into the intermediate variables a5-a8,
 *   transfer a5-a8 to the upper-right row of B.
 *   transfer a1-a4 to the lower-left row of B,
 */
                for(k=i; k<i+4; ++k) {
                    a1=B[k][j+4];
                    a2=B[k][j+5];
                    a3=B[k][j+6];
                    a4=B[k][j+7];
                    a5=A[j+4][k];
                    a6=A[j+5][k];
                    a7=A[j+6][k];
                    a8=A[j+7][k];


                    B[k][j+4]=a5;
                    B[k][j+5]=a6;
                    B[k][j+6]=a7;
                    B[k][j+7]=a8;
                    B[k+4][j]=a1;
                    B[k+4][j+1]=a2;
                    B[k+4][j+2]=a3;
                    B[k+4][j+3]=a4;
                }
/*
 * 3. Transfer the lower-right col of A to the lower-right row of B
 */
                for(k=i+4; k<i+8; ++k) {
                    a1=A[j+4][k];
                    a2=A[j+5][k];
                    a3=A[j+6][k];
                    a4=A[j+7][k];

                    B[k][j+4]=a1;
                    B[k][j+5]=a2;
                    B[k][j+6]=a3;
                    B[k][j+7]=a4;
                }
            }
        }
}

/*
 * 61x67 matrix use 14x14 as a unit to decrease misses
 * I use blocking and defers assigning the diagonals until the end of the block.
 * The diagonal deferring allows there to be less conflict misses.
 */
char transpose_61_desc[] = "Transpose a 61x67 matrix";
void transpose_61(int M, int N, int A[N][M], int B[M][N]){
    int diagonal = 0;
    int diagonal_index = 0;
    int x_block = 14; // Set the unit size
    int y_block = 14;
    for (int i = 0; i < N; i += x_block) {
        for (int j = 0; j < M; j += y_block) {
            for (int k = i; k < i + x_block && k < N; ++k) {
                for (int l = j; l < j + y_block && l < M; ++l) {
                    if (l != k) { // Defer elements on the diagonal
                        B[l][k] = A[k][l];
                    } else if (l == k) {
                        diagonal = A[l][l];//defer
                        diagonal_index = l;
                    }
                }
                if (i == j && k < N) {// deal with diagonal elements
                    B[diagonal_index][diagonal_index] = diagonal;

                }
            }
        }
    }
}
/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";

void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
     registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
