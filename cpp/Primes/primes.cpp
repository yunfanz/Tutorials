// Contains prototypes for cout & cin
#include <iostream>
#include <array>
#include <fstream>
#include <omp.h>
#include <iterator>
#include <cmath>
#define BINS 200
// Prototype for IsPrime function.
bool IsPrime(int);

// main function is always run first in a
// C++ program.

int main()
{	//Given:   nothing.
	//Results: Accepts a number and
	//		  indicates if it is prime.
    using namespace std;
    int ceiling;
    cout<<"Enter integer ceiling: "<<endl;
    cin>>ceiling;
    int step = ceiling/BINS;
    array<int,BINS> hist; int hist_count;
    #pragma omp parallel for private(hist_count)
    for (int b=0; b<BINS; ++b)
    {
    	int tid = omp_get_thread_num();
        int n = b*step, p1=2, p2=3;
        hist_count=0;
        while (n<(b+1)*step)
        {
        	if (IsPrime(n))
        	{
        		p2 = n;
        		if (p2-p1==2){ hist_count++;}
        		p1 = p2;
        	}
        	n++;
        }
        hist[b] = hist_count;
    }
    ofstream outfile ("twin_hist.dat");
    std::ostream_iterator<int> output_iterator(outfile, "\n");
    std::copy(hist.begin(), hist.end(), output_iterator); 
    cout<<"Done!"<<endl;
    return 0;
}
// bool is_prime(int n)
// {
//   if (n == 2 || n == 3) {return true}
//   if n < 2 or n%2 == 0) {return false}
//   if n < 9: return True
//   if n%3 == 0: return False
//   r = int(n**0.5)
//   f = 5
//   while f <= r:
//     #print '\t',f
//     if n%f == 0: return False
//     if n%(f+2) == 0: return False
//     f +=6
//   return true;
// }
bool IsPrime(int number)
{	// Given:   num an integer > 1
	// Returns: true if num is prime
	// 			false otherwise.
	
	int i;
	if (number<2) {return false;}
	for (i=2; i<number; i++)
	{
		if (number % i == 0)
		{
			return false;
		}
	}
	
	return true;	
}
