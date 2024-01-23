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
        ret = (double)(tp.tv_sec & 0x00ffffff) + (double)tp.tv_usec / 1000000;
        return ret;
}

void shellA(int *a, int n)
{
	int h[4] = {9, 5, 3, 1};
	int i, j, k, m, x;

	for (m = 0; m < 4; m++) {
		k = h[m];

		for (int i = k; i < n; i++) {
			x = a[i];
			j = i - k;

			while (j >= 0 && x < a[j]) {
				a[j+k] = a[j];
				j = j - k;
			}

			a[j + k] = x;
		}
	}
}

int main(int argc, char *argv[])
{
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
        
        // ファイルオープンとエラー処理
        if ((fp = fopen(datafile, "r")) == NULL) {
                fprintf(stderr, "##### ファイルのオープンに失敗しました。");
                return 1;
        }

        // データ格納先の確保と、ファイルから格納先への読み出し
        data = (int *)malloc(n*sizeof(int));
        for (i = 0; i < n; i++) 
                fscanf(fp, "%d", &data[i]);
        fclose(fp);
        
        // シェルソートの実行と、実行時間の計測
        time_start = gettime();
        shellA(data, n);
        time_end = gettime();

        // 計測結果とソートされた配列の出力
        fprintf(stderr, "シェルソートAの実行時間 = %1f[秒]\n", time_end - time_start);
        for (i = 0; i < n; i++) 
                printf("%d\n", data[i]);

        free(data);
        return 0;

}
