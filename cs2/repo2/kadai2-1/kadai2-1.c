#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define N 30

#define boolean int
#define true 1
#define false 0

typedef boolean adjmatrix[N][N];
typedef int vindex;


typedef struct edgecell {
        vindex destination;
        struct edgecell *next;
} edgecell;

typedef edgecell *vertices[N];

typedef struct {
        int vertex_num;
        int edge_num;
        vertices vtop;
} graph;

boolean debug = false;

//-------------------------------//

//隣接行列をファイルから読み込む
int read_adjacency_matrix(char *datafile, adjmatrix matrix) {
        FILE *fp;
        int vertex_num; //頂点数
        vindex src, dest;

        if ((fp = fopen(datafile, "r")) == NULL) {
                fprintf(stderr, "[+] Error: ファイルのオープンに失敗しました。\n");
                exit(1);
        }
        
        fscanf(fp, "%d\n", &vertex_num); 
        
        if (vertex_num > N) {
                fprintf(stderr, "[+] Error: このプログラムが扱えるのは超点数が%dまでのグラフです。\n", N);
                exit(1);
        }

        //隣接行列の要素を読み込む
        for (src = 0; src < vertex_num; src++) {
                for (dest = 0; dest < vertex_num; dest++) {
                        fscanf(fp, "%d\n", &matrix[src][dest]);
                }
        }

        fclose(fp);

        return vertex_num;
}

void print_matrix(adjmatrix matrix, int vertex_num) {
        for (int i = 0; i < vertex_num; i++) {
                for (int j = 0; j < vertex_num; j++) {
                        printf("%2d ", matrix[i][j]);
                }
                puts("");
        }
}

void copy_matrix(adjmatrix dest, adjmatrix src, int vertex_num) {
        for (int i = 0; i < vertex_num; i++) {
                for (int j = 0; j < vertex_num; j++) {
                        dest[i][j] = src[i][j];
                }
        } 
}

void multi_matrix(adjmatrix ans, adjmatrix matrix_1, adjmatrix matrix_2, int vertex_num) {
        int i, j, k;
        adjmatrix tmp = {0};
        for(i = 0; i < vertex_num; i++){
                for(j = 0; j < vertex_num; j++){
                        for(k = 0; k < vertex_num; k++){
                                tmp[i][j] += matrix_1[i][k] * matrix_2[k][j];
                        }
                }
        }

        // ans = tmp
        copy_matrix(ans, tmp, vertex_num);
}

void add_matrix(adjmatrix ans, adjmatrix matrix_1, adjmatrix matrix_2, int vertex_num) {
        for (int i = 0; i < vertex_num; i++) {
                for (int j = 0; j < vertex_num; j++) {
                        ans[i][j] = matrix_1[i][j] + matrix_2[i][j];
                }
        }
}

boolean is_strongly_connected(adjmatrix matrix, int vertex_num) {
        adjmatrix pwrd_matrix = {0};
        adjmatrix target_matrix = {0};

        // pwrd_matrix = matrix^1
        copy_matrix(pwrd_matrix, matrix, vertex_num);
        // target_matrix = matrix^1
        copy_matrix(target_matrix, matrix, vertex_num);

        if (vertex_num <= 1) {
                fprintf(stderr, "[+] Error: 強連結を判定するには頂点数が少なすぎます。\n");
                exit(1);
        }

        // target_matrix = matrix^2 + matrix^3 + ... + matrix^(vertex_num)
       for (int exponent = 2; exponent <= vertex_num; exponent++) { // vertex_num -> vertex_num + 1
                // pwrd_matrix = pwrd_matrix * matrix
                multi_matrix(pwrd_matrix, pwrd_matrix, matrix, vertex_num);

                // target_matrix = target_matrix + pwrd_matrix
                add_matrix(target_matrix, target_matrix, pwrd_matrix, vertex_num); 
        }

        /* 
         * 強連結の実際の判定
         * [0][0] [1][1] [2][2] ... [vertex_num-1-1][vertex_num-1] 以外でパスが無い(数値が0の)場合、強連結ではない 
         */
        for (int i = 0; i < vertex_num; i++) {
                for (int j = 0; j < vertex_num; j++) {
                        if (target_matrix[i][j] == 0) { 
                                return false;
                        }
                }
        }

        return true;
}

int main(int argc, char *argv[]) {
        char *datafile;
        adjmatrix matrix;
        int vertex_num;

        if (argc <= 1) {
                fprintf(stderr, "[+] Error: ファイル名を指定してください。\n");
                exit(1);
        }

        datafile = argv[1];
        vertex_num = read_adjacency_matrix(datafile, matrix);

        if (is_strongly_connected(matrix, vertex_num)) 
                printf("強連結である。\n");
        else {
                printf("強連結でない。\n");
        }

        return 0;
}
