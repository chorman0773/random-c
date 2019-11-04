//
// Created by chorm on 2019-11-03.
//

#include <Random.h>
#include <stdio.h>
#include <limits.h>

#define SIZE 50000

int main(){
    Random* rand = Random_new();

    int  min = INT_MAX, max = INT_MIN;
    long long sum = 0;
    double median = 0, mean = 0;
    Random_seed(rand);
    printf("Testing Distribution of next int over %d values\n",SIZE);
    for(int i =0;i<SIZE;i++){
        int val = Random_nextInt(rand);
        sum += val;
        if(val<min)
            min = val;
        if(val>max)
            max = val;
    }
    median = (max+min)/2.0;
    mean = ((double)sum)/SIZE;
    printf("Min value: %d\n",min);
    printf("Max value: %d\n",max);
    printf("Median: %.1f\n",median);
    printf("Mean: %.3f\n",mean);
    Random_free(rand);
}
