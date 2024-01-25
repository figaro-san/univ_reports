#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SMALL_N 50
int gamm[SMALL_N];
int debug = 0;



double gettime()
{
        struct timeval tp;
        double ret;
        gettimeofday(&tp, NULL);
        ret = (double)(tp.tv_sec & 0x00ffffff) + (double)tp.tv_usec / 1000000;
        return ret;
}



void bubble(int *a, int n)
{
        int i, j;

        for (i = 1; i <= n-1; i++) {
                for (j = n-1; j >= i; j--) {
                        if(a[j-1] > a[j]) {
                                int x = a[j-1];
                                a[j-1] = a[j];
                                a[j] = x;
                        }
                }

                if (debug) {
                        for (j = 0; j < n; j++) {
                                printf("%d\n", a[j]);
                        }
                        printf("\n");
                }
        }
}



int l_select(int *a, int n, int k)
{
	int i, m, ans;

	// 数が少ないならそのままソート
	if (n < SMALL_N) {
		bubble(a, n);
		return a[k-1];
	}

	// 配列を5個づつソートする
	for (i = 0; i < n/5; i++) {
		bubble(a+i*5, 5);
	}

	//5個のソートされた要素を持つグループたちから中央値をそれぞれ持ってくる
	int *medians = (int *)malloc((n/5) * sizeof(int));
	for (i = 0; i < n/5; i++) {
		medians[i] = a[i*5+2];
	}

	// n/5   はソートする中央値の数(個数)			n/5
	// |M|/2 はソートする中央地の数を半分に割ったもの	n/5*1/2 = n/10
	// 中央値から中央値を選ぶ
	m = l_select(medians, n/5, n/10);
	free(medians);

	// mより大きい、小さい、等しいクラスに分ける
	int *big_class = NULL, *small_class = NULL, *equal_class = NULL;
	int bsize = 0, ssize = 0, esize = 0;

	for (i = 0; i < n; i++) {
		if (a[i] < m) {
			ssize++;
			small_class = (int *)realloc(small_class, ssize * sizeof(int));
			small_class[ssize-1] = a[i];

		} else if (a[i] == m) {
			esize++;
			equal_class = (int *)realloc(equal_class, esize * sizeof(int));
			equal_class[esize-1] = a[i];

		} else {
			bsize++;
			big_class = (int *)realloc(big_class, bsize * sizeof(int));
			big_class[bsize-1] = a[i];
		}
	}

	if (ssize >= k) {
		ans = l_select(small_class, ssize, k);

		free(small_class);
		free(equal_class);
		free(big_class);

		return ans;
	} else if (ssize+esize >= k) {

		free(small_class);
		free(equal_class);
		free(big_class);

		return m;
	} else {
		ans = l_select(big_class, bsize, k-ssize-esize);

		free(small_class);
		free(equal_class);
		free(big_class);

		return ans;
	}
}


int main(int argc, char *argv[])
{
        char *datafile;                         // 入力データのファイル名
        FILE *fp;                               // 入力データのファイルポインタ
        int n;                                  // 入力データ数
        int *data;                              // 入力データの格納先
        int k;					// 入力データの中から何番目に小さい数を選ぶのか
        int i;
        double time_start, time_end;
        int answer;

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
        
        if (argc <= 3) {
        	fprintf(stderr, "#### 何番目のデータを選ぶのかを指定してください\n");
        	return 1;
        }
        k = atoi(argv[3]);
        
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
        
        // selectの実行と、実行時間の計測
        time_start = gettime();
        answer = l_select(data, n, k);
        time_end = gettime();
        
        printf("答え = %d\n", answer);
        fprintf(stderr, "k番目選択の実行時間 = %1f[秒]\n", time_end - time_start);

        free(data);
        return 0;

}
