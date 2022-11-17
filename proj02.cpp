#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <cstdio>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
using namespace std;
mutex myMutex;

void monteCarlo(int& counter, unsigned long long beg, unsigned long long end)
{
   double z;
   double x;
   double y;
   int local_count = 0;
   //This for loop allocates the randomization into the different slices for each thread
   for (auto it= beg; it < end; ++it)
   {         
      // calculating whether or not we count the product
      x = ((double)rand())/((double)RAND_MAX);
      y = ((double)rand())/((double)RAND_MAX);
      z = sqrt((x*x)+(y*y));
      //If it's < z 1, we increase the counter
      if (z<=1)
      {
         ++local_count;
      }
   }
   lock_guard<mutex> myLock(myMutex);
   //cout << "Local counter: " << local_count << endl;
   counter += local_count;
   //cout << "Global Count: " << counter << endl;
}

int main (int argc,  char *argv[]) 
{
   double ratio;
   double pi;
   double delta;
   int points;
   int threads;

   //We make sure the user's input has 3 arguments
   if (argc == 3)
   {
      points = stoi(argv[2]);
      //cout << points << endl;
      threads = stoi(argv[1]);
      //cout << threads << endl;
   }

   //Otherwise, we tell the user the correct syntax
   else
   {
      cout << "Invalid input ";
      cout << "usage: "<< argv[0] << " [1...10] [10...1000000] " << endl;
      return -1;
   }
   //Random Number Generation Seed
   srand((unsigned)(time(nullptr))); 
   mt19937 engine (0);
   //Initializing the counter for use outside of the monteCarlo function
   int counter;
   auto start = chrono::system_clock::now();
   thread t[threads];
   long long slice = points / threads;
   int startIdx=0;
   for (int i = 0; i < threads; ++i) 
   {
      cout << "Thread[" << i << "] - slice [" << startIdx << ":" << startIdx+slice-1 << "]" << endl;
      t[i] = thread(monteCarlo, ref(counter), startIdx, startIdx+slice-1);
      startIdx += slice;
      //cout << "Global Count: " << counter << endl;
   }

   for (int i = 0; i < threads; ++i)
   {
      t[i].join();
   }

   chrono::duration<double> dur= chrono::system_clock::now() - start;
   cout << "Time for combining " << dur.count() << " seconds" << endl;
   ratio = (double)counter/(double)points;
   pi = 4*ratio;
   delta = M_PI - pi;
   printf("Ratio = %f Pi (theoretical) = %f Delta = %f \n", ratio, pi, delta); 


}