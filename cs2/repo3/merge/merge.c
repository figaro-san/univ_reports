#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/time.h>

int debug = 0;

double gettime()
{
        struct timeval tp;
        double ret;
        gettimeofday(&tp, NULL);
        ret = (double)(tp.tv_sec & 0x00ffffff) + (double)tp.tv_sec / 1000000;
        return ret;
}

int *gamm;      // γ: 一時的なデータ置き場

int *merge(int *alpha, int n, int *beta, int m)
{
        int gamm_used = 0;
        int i, j;

        if (debug) {
                printf("α[] = \n");
                for (i = 0; i < n; i++)
                        printf("%d\n", alpha[i]);
                printf("\n");

                printf("β[] = \n");
                for (j = 0; j < m; j++)
                        printf("%d\n", beta[j]);
                printf("\n");
        }

        i = 0;
        j = 0;

        while(i < n && j < m) {
                if (alpha[i] <= beta[j])
                        gamm[gamm_used++] = alpha[i++];
                else 
                        gamm[gamm_used++] = beta[j++];
        }

        if (i < n && j >= m) {
                while (i < n)
                        gamm[gamm_used++] = alpha[i++];
        }

        if (i >= n && j< m) {
                while (j < m)
                        gamm[gamm_used++] = beta[j++];
        }

        for (i = 0; i < n; i++)
                alpha[i] = gamm[i];

        for (j = 0; j < m; j++) 
                beta[j] = gamm[n+j];

        if (debug) {
                printf("γ[] = \n");
                for (i = 0; i < n+m; i++)
                        printf("%d\n", gamm[i]);
                printf("\n");
        }

        return alpha;
}

int * m_sort(int *a, int i, int j) {
        int k;

        if (i == j) 
                return &a[i];

        k = (i+j-1) / 2;
        return merge(m_sort(a, i, k), k-i+1, m_sort(a, k+1, j), j-(k+1)+1);
}

int main(int argc, char *argv[]) {
        char *datafile;                         // 入力データのファイル名
        FILE *fp;                               // 入力データのファイルポインタ
        int n;                                  // 入力データ数
        int *data;                              // 入力データの格納先
        int i;
        double time_start, time_end;

        if (argc <= 1) {
                fprintf(stderr, "##### ファイル名を指定してください\n");
                return 1;
        }
        datafile = argv[1];

        if (argc <= 2) {
                fprintf(stderr, "##### データ数を指定してください\n");
                return 1;
        }
        n = atoi(argv[2]);                      // データ数の取得
        
        data = (int *)malloc(n * sizeof(int));  // データ格納場所の確保
        gamm = (int *)malloc(n * sizeof(int));  // 一時的なデータ置き場の確保
        fp = fopen(datafile, "r");              // ファイルを開く
        
        // データを一つずつ読み込む
        for (i = 0; i < n; i++) {
                fscanf(fp, "%d", &data[i]);
        }
        fclose(fp);

        time_start = gettime();
        m_sort(data, 0, n-1);
        time_end = gettime();

        // fprintf(stderr, "マージソートの実行時間 = %1f[秒]\n", time_end-time_start);
        for (i = 0; i < n; i++)
                printf("%d\n", data[i]);

        free(data);
        free(gamm);
        
        return 0;
}
