// O(nlogn)
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

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;

}

void sift(int *data_p, int i, int n) {
	int k = i;

	//j != k を満たすため
	int j = k+1;

	while (j != k) {
		j = k;
		
		//左の子が親より大きいなら、後に入れ替える
		if (2*j+1 < n && data_p[2*j+1] > data_p[k]) {
			k = 2*j+1;
		}

		//右の子が親より大きいなら、後に入れ替える
		if (2*j+2 < n && data_p[2*j+2] > data_p[k]) {
			k = 2*j+2;
		}

		//子と親の大小関係が正しくないなら入れ替える。大小関係が正しいなら、自分自身と入れ替える(実質何もしないのと同じ)
		//大小関係が正しいならj==kなのでループが終了。
		swap(&data_p[j], &data_p[k]);
	}
}

void make_heap(int *data_p, int n) {
 	 // 親の数は要素数/2。ヒープの特性上、配列の先頭から要素数/2までが親の要素となる。c言語は0から始まるので(要素数/2)-1が実際の配列のインデックスとなる。
 	// 一番下(グラフ上では一番下かつ最も左)の親から、ヒープの条件を満たすように並べ替えていく
	for (int i = n/2-1 ; i >= 0; i--) {
		sift(data_p, i, n);
	}
}

void heap_sort(int *data_p, int n) {
	make_heap(data_p, n);

	for (int i = n-1; i >= 1; i--) {
		//ヒープであるように並べ替えられると、配列の先頭が最も大きい値を持っているので、それを配列の最後と交換する
		swap(&data_p[0], &data_p[i]);

		// 入れ替え後に、次に大きい値を根へ持ってくる
		sift(data_p, 0, i);
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
        
         
        //ソートの実行と、実行時間の計測
        time_start = gettime();
        heap_sort(data, n);
        time_end = gettime();
        
        // 計測結果とソートされた配列の出力
        fprintf(stderr, "ヒープソートの実行時間 = %1f[秒]\n", time_end - time_start);
        /*for (i = 0; i < n; i++) 
                printf("%d\n", data[i]);
        */

        free(data);

        return 0;

}
