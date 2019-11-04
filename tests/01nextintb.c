//
// Created by chorm on 2019-11-03.
//

#include <Random.h>
#include <stdio.h>
#include <math.h>

#define SIZE 500000
#define BOUND 5000

int main(){
    int encounters[BOUND] = {};
    double frequency[BOUND];
    double freqErr;
    double distribution;
    double medianFreq;
    double maxFrequency = -INFINITY, minFrequency = INFINITY;
    int maxOccurancesVal, minOccurancesVal;
    Random* rand = Random_new();
    Random_seed(rand);
    printf("Testing distribution, frequency, and occurances of %d integers less than %d\n",SIZE,BOUND);
    for(int i = 0;i<SIZE;i++) {
        int val = Random_nextIntb(rand, BOUND);
        if(val>=BOUND){
            printf("Bounded nextInt generated number %d that is out of bounds",val);
            return -1;
        }
        encounters[val]++;
    }

    for(int i = 0;i<BOUND;i++) {

        double freq = ((double) encounters[i]) / SIZE;
        if(freq<minFrequency){
            minFrequency = freq;
            minOccurancesVal = i;
        }
        if(freq>maxFrequency){
            maxFrequency = freq;
            maxOccurancesVal = i;
        }
        frequency[i] = freq;
    }
    medianFreq = (maxFrequency+minFrequency)/2;
    distribution = medianFreq-minFrequency;
    freqErr = maxFrequency-minFrequency;
    printf("Maximum Frequency: %.3e\n",maxFrequency);
    printf("Minimum Frequency: %.3e\n",minFrequency);
    printf("Frequency Error: %.3e\n",freqErr);
    printf("Distribution: %.3e\n",distribution);
    printf("Most Frequeny Value: %d\n",maxOccurancesVal);
    printf("Lease Frequent Value: %d\n",minOccurancesVal);
    Random_free(rand);
}