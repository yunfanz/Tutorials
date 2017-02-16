// Contains prototypes for cout & cin
#include <iostream>
#include <array>
#include <fstream>
#include <omp.h>
#include <iterator>
#include <cmath>
#define BINS 200
#define SM 200
// Prototype for IsPrime function.
bool IsPrime(long long int, std::array<int, SM>);
bool is_prime_alg(int);

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
    array<int, SM> small_primes;
    int p = 2, i = 0;
    do {
        if (is_prime_alg(p)) {
            small_primes[i] = p;
            i++;
        }
        p++;
    } while (i<SM);
    // std::ostream_iterator<int> cout_iterator(cout, " ");
    // std::copy(small_primes.begin(), small_primes.end(), cout_iterator); 

    long long int step = ceiling/BINS;
    array<int,BINS> hist; int hist_count;

    #pragma omp parallel for private(hist_count)
    for (int b=0; b<BINS; ++b)
    {
    	int tid = omp_get_thread_num();
        long long int n = b*step, p1=2, p2=3;
        hist_count=0;
        while (n<(b+1)*step)
        {
        	if (IsPrime(n, small_primes))
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
    std::ostream_iterator<int> fout_iterator(outfile, "\n");
    std::copy(hist.begin(), hist.end(), fout_iterator); 
    cout<<"Done!"<<endl;
    return 0;
}

bool is_prime_alg(int n)
{
  if (n == 2 || n == 3) {return true;}
  if (n < 2 || n%2 == 0) {return false;}
  if (n < 9) {return true;}
  if (n%3 == 0) {return false;}
  int f = 5;
  while (f*f <= n) {
    //print '\t',f
    if (n%f == 0) {return false;}
    if (n%(f+2) == 0) {return false;}
    f +=6;
    }
  return true;
}

bool IsPrime(long long int number, std::array<int, SM> small_primes)
{	// Given:   num an integer > 1
	// Returns: true if num is prime
	// 			false otherwise.
	
	for (int p : small_primes)
	{
        if (p == number) { return false; }
		if (number % p == 0)
		{
			return false;
		}
	}
	
	return is_prime_alg(number);	
}
