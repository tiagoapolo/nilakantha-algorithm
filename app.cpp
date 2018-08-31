#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <cmath>
#include <vector>

using namespace std;
using namespace chrono;

// Windows
#ifdef _WIN32
#include <intrin.h>
uint64_t rdtscp()
{
    unsigned int aux;
    return __rdtscp(&aux);
}
#else
// Linux/macOS
uint64_t rdtscp(){
    unsigned int lo,hi;
    asm volatile ("rdtscp" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
} 
#endif

void calculation(double start, double limit, int id);

union Cake {
    long double cakeSlice;
    char pool [64];
};

vector< Cake > resultsArray;

int main(int argc, char const *argv[])
{    
    long double pi;
    long double total = 0.0;
    int cThreads = 1;
    int division;
    thread * vt[256];

    do {

        cout << "How many threads (1 ~ 256) to execute Nilakantha's Algorithm ?" << endl;
        scanf("%d", &cThreads);

        if(cThreads > 256){
            cout << "\n| OUT OF RANGE! - Choose a number between 1 and 256! |\n" << endl;
        }

    } while (cThreads > 256);

    division = 1e9 / cThreads;


    auto r0 = rdtscp();
    auto t0 = steady_clock::now();
               

    for(int i=0; i<cThreads; ++i){
        vt[i] = new thread(calculation, i*division, (i+1)*division, i);
    }

    for(int i=0; i<cThreads; ++i) {

        vt[i]->join();
        total += resultsArray[i].cakeSlice;
    }

    pi = 3 + 4 * total;

    auto d = steady_clock::now() - t0;
    auto dc = rdtscp() - r0;

    cout << "pi: " << pi << endl;
    cout << duration_cast<milliseconds>(d).count() << "ms" << endl;
    cout << dc << " clock" << endl;

    return 0;
}


void calculation(double start, double limit, int id){

    long double exponent = 1;
    long double sum;

    Cake cheesecake;

    for(double i=start; i < limit; i++) {        
        sum += (exponent/((2*i+2)*(2*i+3)*(2*i+4)));
        // cheesecake.cakeSlice += (exponent/((2*i+2)*(2*i+3)*(2*i+4)));
        exponent = exponent * -1;
    }

    cheesecake.cakeSlice = sum;

    // cout << "\nID: "<< id << endl;
    // cout << "SUM: "<< sum << endl;

    resultsArray.push_back(cheesecake);
    
    return;
}