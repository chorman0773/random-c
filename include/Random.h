/*
 * Random.h
 *
 *  Created on: Sep 18, 2019
 *      Author: chorm
 * Copyright (C) 2019, Connor Horman. 
 * Released under the terms of the MIT License, see the acompanying LICENSE 
 */

#ifndef INCLUDE_RANDOM_H_
#define INCLUDE_RANDOM_H_

#ifdef __cplusplus
extern"C"{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

///
//The implementation of Random here is according to the LCLib Random Specification
//The implementation is defined by Oracle, under java.util.Random, which is adopted
// as standard by LCLib.
//The implementation is according to https://docs.oracle.com/javase/8/docs/api/java/util/Random.html
// Pursuant to that documentation, this code comes with the following warnings
//
/// Thread-safety
// If this file is compiled with the macro TIGAME_MULTITHREAD_SUPPORT,
// then instances of Random are thread safe. However, use of
// a single instance of Random across multiple threads may
// encounter contention, and therefore is disabled.
// If thread-safety is not required, the macro can be left undefined,
// and use of Random will be generally faster.
//
/// Security
// This Random Number Generator is NOT Cryptographically Strong and should NOT be used
// for Security Purposes.
/// Expects clauses
// Functions may have expects clauses.
// Such a clause defines a precondition of that function. The behavior of violating an Expects clause is undefined. 
typedef struct Random Random;




/**
 * Creates a new, uninitialized Pseudo-random Number Generator according to the LCLib Random Specification.
 * The Random Instance, will be given a unique seed, as though by `Random_seed(rand)` the first time its used, if neither Random_seed nor Random_setSeed is called on it prior.
 * Random_new allocates the instance via a call to malloc. If malloc fails, Random_new returns NULL.
 * However, it may make additional allocations or initializations.
 * If these additional allocations or initializations fail,
 *  any which are completed are deallocated and/or deinitialized in the reverse order,
 *  then the instance is deallocated using free, and the call returns NULL.
 * The instance must be destroyed using Random_free. If the instance is freed in any other way, the behavior is undefined.
 *
 * The algorithms defined below are defined in terms of 3 fields for exposition only, a uint64_t field called seed, 
 *  a double field called nextNextGaussian, and a _Bool field called haveNextNextGaussian. 
 * Implementations need not provide these fields. Consumers cannot access fields of this instance. 
 * If an instance of type Random is examined or written to as an array of char or unsigned char, the behavior is undefined. 
 */
Random* Random_new();

/**
 * Destroys the given instance of Random.
 * All allocations and initializations peformed by Random_new are reversed in the reverse order they were performed.
 * rand MUST be a valid random instance that was returned by Random_new() and has not been freed, or the behavior is undefined
 */
void Random_free(Random* rand);

/**
* Automatically initializes the instance of random with a seed that is almost guaranteed (with overwealming probability), 
* to be unique from any other invocation of this function.
*
* The algorithm which is used to perform this task is unspecified, but is guaranteed to be thread-safe.
* 
* If neither this method nor Random_setSeed is called on an instance prior to any call to a generator function,
*  the instance is automatically initialized as though by this function.
* After this call, haveNextNextGaussian shall be false.
* Expects: rand shall have been returned by Random_new(), and must not have been freed.
*/
void Random_seed(Random* rand);



/**
* Initializes the generator to the state indicated by seed. 
* This function operates as though by setting seed to (seed ^ 0x5DEECE66DL) & ((1L << 48) - 1)
* If the library is compiled using TIGAME_MULTITHREAD_SUPPORT, the above operation shall be atomic.
* Additionally, this function clears haveNextNextGaussian.
* Expects: rand shall have been returned by Random_new(), and must not have been freed.
*/
void Random_setSeed(Random* rand,uint64_t seed);

/**
* An exposition only function which generates a random integer up to bits.
* This function operates as though by updating seed to (seed * 0x5DEECE66DuLL + 0xBuLL) & ((1uLL << 48) - 1),
*  then returns (uint32_t)(seed >> (48 - bits)). 
* If the library is compiled using TIGAME_MULTITHREAD_SUPPORT, both operations shall be atomic, and the composite shall be atomic.
* 
* An implementation shall not provide an accessible function called Random_next.
* Expects: rand shall have been returned by Random_new(), and must not have been freed. This is shared by all Random_next*(rand) functions.
*/
/*uint32_t Random_next(Random* rand,int bits);*/


/**
* Returns the result of (int32_t)(Random_next(rand,32));
*/
int32_t Random_nextInt(Random* rand);

/**
* Generates a random integer in [0,bound), as though by the following algorithm:
   if ((bound & -bound) == bound)  // i.e., bound is a power of 2
     return (int32_t)((bound * (uint64_t)Random_next(rand,31)) >> 31);

   int32_t bits, val;
   do {
       bits = Random_next(31);
       val = bits % bound;
   } while (bits - val + (bound-1) < 0);
   return val;
   
* Documentation copied and adapted from the Random#nextInt(int) function.
* Expects: In addition to the general requirements for all Random_next* functions, bound MUST not be negative.
* Api Note: The function accepts a signed bound, as the algorithm specified can only output positive signed values. 
*  It made little sense to allow unsigned values greater than INT32_MAX. 
*  The return type, as well as the bound to an extent, is also a legacy choice, as prior C++ implementations of java.util.Random returned signed int32_t. 
*/
int32_t Random_nextIntb(Random* rand,int32_t bound);
/**
* Returns the result of (int64_t)(((uint64_t)Random_next(rand,32))<<32)|Random_next(rand,32));
* Note: as the generator deals with 48-bit seeds, this function will not generate all possible long values.
*/
int64_t Random_nextLong(Random* rand);
/**
* Generates a Random floating point number in [0,1). 
* Returns the result of ((float)Random_next(rand,24))/(1<<24).
*/
float Random_nextFloat(Random* rand);
/**
* Generates a Random double-precision floating point number in [0,1)
* Returns the result of ((double)(((uint64_t)Random_next(rand,26))<<27)|Random_next(rand,27)))/1uLL<<53;
*/
double Random_nextDouble(Random* rand);

/**
* Generates a Random Gaussian distributed double value with mean 0 and standard distribution 1. 
* If the exposition-only flag haveNextNextGaussian is set, clears haveNextNextGaussian and returns nextNextGaussian. 
* Otherwise, generates two such double values as though by the following algorithm:
    double v1, v2, s;
     do {
       v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
       v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
       s = v1 * v1 + v2 * v2;
     } while (s >= 1 || s == 0);
     double multiplier = sqrt(-2 * log(s)/s);
     rand->nextNextGaussian = v2 * multiplier;
     rand->haveNextNextGaussian = true;
     return v1 * multiplier;
* Documentation copied and adapted from Random#nextGaussian()
* 
*/
double Random_nextGaussian(Random* rand);

/**
* Generates a random boolean value, with approximately equal probability of generating either true or false.
* Returns the result of Random_next(rand,1)!=0
*/
bool Random_nextBoolean(Random* rand);
/**
* Fills the output array with size random bytes, as though by the following algorithm:
  for (int i = 0; i < size; )
     for (int rnd = Random_nextInt(rand), n = min(size - i, 4);
          n-- > 0; rnd >>= 8)
       ((char*)out)[i++] = (char)rnd;
  Where min(a,b) is an exposition only function-like macro which is defined as ((a)<(b)?(a):(b))
* Documentation copied and adapted from Random#nextBytes(byte[])
* Expects: out must point to an object, which 
*/
void Random_nextBytes(Random* rand,void* out,size_t size);

/**
* Generates len random ints as though by Random_nextInt(rand), and stores them in [out,out+len).
* The function is not constrained to write the output values in any particular order.
* Expects: the range [out,out+len) must be a valid, continuous, and writeable range. 
*/
void Random_ints(Random* rand,int* out,size_t len);
/**
* Generates len random double values as though by Random_nextDouble(rand), and stores them in [out,out+len)
* The function is not constrained to write the output values in any particular order.
* Expects: the range [out,out+len) must be a valid, continuous, and writeable range. 
*/
void Random_doubles(Random* rand,double* out,size_t len);

/**
* Generates len random ints as though by Random_nextIntb(rand,max-min)+min, and stores them in [out,out+len).
* The function is not constrained to write the output values in any particular order.
* Expects: the range [out,out+len) must be a valid, continuous, and writeable range. 
*/
void Random_intsb(Random* rand,int* out,size_t len,int min,int max);
/**
* Generates len random double values in [min,max) as though by Random_nextDouble(rand)*(max-min)+min, and stores them in [out,out+len)
* The function is not constrained to write the output values in any particular order.
* Expects: the range [out,out+len) must be a valid, continuous, and writeable range. 
*/
void Random_doublesb(Random* rand,double* out,size_t len,double min,double max);

#ifdef __cplusplus
};
#endif

#endif /* INCLUDE_RANDOM_H_ */
