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



int selectk(int *a, int n, int k)
{
	int i, p, used;
	int u, v;
	int *U, *V;
	int answer;

	if (n < SMALL_N) {
		m_sort(a, 0, n-1);
		return a[k-1];
	}

	p = a[random() % n];
	u = 0;
	v = 0;

	for (i = 0; i < n; i++) {
		if (a[i] < p)
			u++;
		if (a[i] <= p)
			v++;
	}

	if (k <= u) {
		U = (int *)malloc(u * sizeof(int));
		used = 0;

		for (i = 0; i < n; i++) {
			if (a[i] < p)
				U[used++] = a[i];
		}
		answer = selectk(U, u, k);
		free(U);
		return answer;
	}

	if (k <= v)
		return p;

	V = (int *)malloc((n-v) * sizeof(int));
	used = 0;
	for (i = 0; i < n; i++) {
		if (a[i] > p)
			V[used++] = a[i];
	}

	answer = selectk(V, n-v, k-v);
	free(V);
	return answer;
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
        answer = selectk(data, n, k);
        time_end = gettime();
        
        printf("答え = %d\n", answer);
        fprintf(stderr, "k番目選択の実行時間 = %1f[秒]\n", time_end - time_start);

        free(data);
        return 0;

}
