#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 行列のサイズを定義（例として500x500とします）
#define N 500

/**
 * @brief 行列Aと行列Bの積を計算し、結果を行列Cに格納。
 * ループ順序はi -> j -> k。
 */
void matrix_multiply_ijk(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // C[i][j]の初期化
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // 乱数の種を初期化
    srand(time(NULL));

    // 行列を定義し、ランダムな要素で初期化
    static int A[N][N];
    static int B[N][N];
    static int C[N][N]; // 結果を格納する行列

    // 行列AとBにランダムな値を設定（0から9まで）
    printf("Initializing matrices A and B with size %d x %d...\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
    printf("Initialization complete.\n");

    // 時間計測開始
    clock_t start_time = clock();

    // 行列Aと行列Bの積を計算（ループ順序: i -> j -> k）
    printf("Starting matrix multiplication (i -> j -> k order)...\n");
    matrix_multiply_ijk(A, B, C);
    printf("Matrix multiplication complete.\n");

    // 時間計測終了
    clock_t end_time = clock();

    // 経過時間を計算
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // 結果の表示 (Nが大きい場合は省略し、検証用に左上の4x4のみ表示)
    printf("\nMatrix C (Result of A * B, top-left 4x4 for verification):\n");
    int display_size = (N < 4) ? N : 4;
    for (int i = 0; i < display_size; i++) {
        for (int j = 0; j < display_size; j++) {
            printf("%d ", C[i][j]);
        }
        printf("...\n");
    }
    printf("...\n");

    // 計算時間を出力
    printf("\n--- Performance ---\n");
    printf("Matrix Size: %d x %d\n", N, N);
    printf("Elapsed Time: %f seconds\n", elapsed_time);

    return 0;
}
/*AVX2
#include <immintrin.h> // AVX用ヘッダ

void matrix_multiply_avx2(int A[N][N], int B[N][N], int C[N][N]) {
    // 結果行列の初期化
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) C[i][j] = 0;
    }

    // ikj順序でキャッシュ効率を最大化し、SIMD化
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            // A[i][k] をレジスタにロードし、8要素分にコピー
            __m256i a_val = _mm256_set1_epi32(A[i][k]);

            int j = 0;
            // 8要素ずつ並列処理
            for (; j <= N - 8; j += 8) {
                __m256i b_row = _mm256_loadu_si256((__m256i*)&B[k][j]);
                __m256i c_row = _mm256_loadu_si256((__m256i*)&C[i][j]);

                // c_row += a_val * b_row
                __m256i prod = _mm256_mullo_epi32(a_val, b_row);
                c_row = _mm256_add_epi32(c_row, prod);

                _mm256_storeu_si256((__m256i*)&C[i][j], c_row);
            }
            // 端数処理 (Nが8の倍数でない場合)
            for (; j < N; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}*/

/*AVX512
#include <immintrin.h> // AVX用ヘッダ

void matrix_multiply_avx2(int A[N][N], int B[N][N], int C[N][N]) {
    // 結果行列の初期化
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) C[i][j] = 0;
    }

    // ikj順序でキャッシュ効率を最大化し、SIMD化
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            // A[i][k] をレジスタにロードし、8要素分にコピー
            __m256i a_val = _mm256_set1_epi32(A[i][k]);

            int j = 0;
            // 8要素ずつ並列処理
            for (; j <= N - 8; j += 8) {
                __m256i b_row = _mm256_loadu_si256((__m256i*)&B[k][j]);
                __m256i c_row = _mm256_loadu_si256((__m256i*)&C[i][j]);

                // c_row += a_val * b_row
                __m256i prod = _mm256_mullo_epi32(a_val, b_row);
                c_row = _mm256_add_epi32(c_row, prod);

                _mm256_storeu_si256((__m256i*)&C[i][j], c_row);
            }
            // 端数処理 (Nが8の倍数でない場合)
            for (; j < N; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}*/
