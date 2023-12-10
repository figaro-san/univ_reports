#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define N 30

#define boolean int
#define ture 1
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

//辺の追加, 頂点番号srcからdestへ向かう
void add_edge(graph *g, vindex src, vindex dest) {
        edgecell *edge = (edgecell *)malloc(sizeof(edgecell));
        edge->destination = dest;
        edge->next = g->vtop[src]; 
        g->vtop[src] = edge;
}

void translate_into_graph(adjmatrix max, graph *g) {
        int edge_num = 0;
        vindex src, dest;

        for(src = 0; src < g->vertex_num; src++) {
                //g->vtop[src]->next = NULL;
                for (dest = 0; dest < g->vertex_num; dest++) {
                        if (max[src][dest]) {
                                add_edge(g, src, dest);
                                edge_num++;
                        }

                }
        }

        g->edge_num = edge_num;
}

//これを標準出力からリダイレクトにつないてファイルへ書き込む
void print_graph(graph *g) {
        vindex v;
        printf("digraph G {\n");
        printf("  size=\"11.5.8\"; node[fontsize=10,height=0.01,width=0.01]; edge[len=3.0];\n");
        for (v = 0; v < g->vertex_num; v++) {
                edgecell *edge;
                for (edge = g->vtop[v]; edge != NULL; edge = edge->next) {
                        printf("  %d -> %d;\n", v+1, edge->destination+1);
                }
        }
        printf("}\n");
        
}

void free_graph(graph *g) {
        vindex v;

        for (v = 0; v < g->vertex_num; v++) {
                edgecell *edge, *next_edge;
                for (edge = g->vtop[v]; edge != NULL; edge = next_edge) {
                        next_edge = edge->next;
                        free(edge);
                }
        }
}

//大域変数でgraph構造体gを宣言すると、初期化されるため
//vtop[0]->nextの値が0x0になるためNULLとして判定できる
//大域変数として宣言しなかった場合、vtop[0]->nextにゴミが入ってNULL判定できず
//そのままアクセスしようとしてセグフォになる
//gdbで確認すると0x8000へアクセスしようとしてエラーとなった
graph g;
int main(int argc, char *argv[]) {
        char *datafile;
        adjmatrix matrix;
        //graph g;

        if (argc <= 1) {
                fprintf(stderr, "[+] Error: ファイル名を指定してください。\n");
                exit(1);
        }

        datafile = argv[1];
        g.vertex_num = read_adjacency_matrix(datafile, matrix);
        translate_into_graph(matrix, &g);
        print_graph(&g); 
        free_graph(&g);
        return 0;
}
