// 平均O(nlogn)
// 最悪O(N^2)
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

void subsort(int *data_p, int n, int s, int t) {
	int i = s;
	int j = t;
	int x = data_p[(s+t)/2]; //配列の大体中心の要素を選ぶ

	//i > jになるまで実行することで、渡された配列中を、基準を元に2分割する
	while (i <= j) {
		//配列[0]から調べて、xより小さいなら、そのまま次の要素を調べに行く。大きいなら、その要素が格納されている場所のインデックスで止まる
		while (data_p[i] < x) 
			i++;

		//配列[n-1](最後尾)から調べて、xより大きいなら次の要素を調べに行く。小さいなら、その要素が格納されている場所のインデックスで止まる
		while (x < data_p[j])
			j--;

		//基準より小さい要素を、基準が格納されている場所より前、大きい要素を後ろに配置したいので
		//上記で止まった場所(それぞれ、小さい要素のエリアに大きい要素が見つかった場所のインデックス、と大きい要素のエリアに小さい要素が見つかった場所のインデックス)
		//を元にして入れ替える
		if (i <= j) {
			swap(&data_p[i], &data_p[j]);
			i++;
			j--;
		}
	}

	//この時点で、jが基準が格納される場所よりより一つ下の場所を指し、iが基準の場所より一つ上の場所を指す
	//基準より小さい要素のエリアについてソートする
	if (s < j)
		subsort(data_p, n, s, j);

	//基準より大きい要素のエリアについてソートする
	if (i < t) 
		subsort(data_p, n, i, t);
}

void quick_sort(int *data_p, int n) {
	subsort(data_p, n, 0, n-1);	
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
        quick_sort(data, n);
        time_end = gettime();
        
        // 計測結果とソートされた配列の出力
        fprintf(stderr, "クイックソートの実行時間 = %1f[秒]\n", time_end - time_start);
        /*for (i = 0; i < n; i++) 
                printf("%d\n", data[i]);*/
        
        free(data);

        return 0;

}
