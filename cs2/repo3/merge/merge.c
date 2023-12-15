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
        }

        return alpha;
}
