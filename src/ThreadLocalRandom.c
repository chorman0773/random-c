#include <Random.h>
#ifndef __STDC_NO_THREADS__
#include <threads.h>

static tss_t tlr;
static once_flag tlr_flag = ONCE_FLAG_INIT;

static void tlrDtor(void* v){
    Random_free((Random*)v);
}
static void tlrInit(){
    tss_create(&tlr,tlrDtor);
}

Random* getThreadLocalRandom(){
    call_once(&tlr_flag,tlrInit);
    Random* rand = (Random*) tss_get(tlr);
    if(!rand){
        rand = Random_new();
        Random_seed(rand);
        tss_set(tlr,rand);
    }
    return rand;
}
#else
#include <pthread.h>

static pthread_key_t tlr;
static pthread_once_t tlr_flag = PTHREAD_ONCE_INIT;

static void tlrDtor(void* v){
    Random_free((Random*)v);
}
static void tlrInit(){
    pthread_key_create(&tlr,tlrDtor);
}

Random* getThreadLocalRandom(){
    pthread_once(&tlr_flag,tlrInit);
    Random* rand = (Random*) pthread_getspecific(tlr);
    if(!rand){
        rand = Random_new();
        Random_seed(rand);
        pthread_setspecific(tlr,rand);
    }
    return rand;
}

#endif
