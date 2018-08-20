#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <cmath>
#include <vector>

using namespace std::chrono;

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
std::vector< long double > resultsArray;

int main(int argc, char const *argv[])
{    
    long double pi;
    long double total = 0.0;
    int cThreads = 1;
    int division;
    



    do {

        std::cout << "How many threads (1 ~ 256) to execute Nilakantha's Algorithm ?" << std::endl;
        scanf("%d", &cThreads);

        if(cThreads > 256){
            std::cout << "\n| OUT OF RANGE! - Choose a number between 1 and 256! |\n" << std::endl;
        }

    } while (cThreads > 256);

    division = 1e9 / cThreads;


    auto r0 = rdtscp();
    auto t0 = steady_clock::now();
               

    for(int i=0; i<cThreads; ++i){
        vt[i] = std::thread(calculation, i*division, (i+1)*division, i);
    }

    for(int i=0; i<cThreads; ++i) {
        vt[i].join();
        total += resultsArray[i];
    }

    pi = 3 + 4 * total;

    auto d = steady_clock::now() - t0;
    auto dc = rdtscp() - r0;

    std::cout << "pi: " << pi << std::endl;
    std::cout << duration_cast<milliseconds>(d).count() << "ms" << std::endl;
    std::cout << dc << " clock" << std::endl;

    return 0;
}


void calculation(double start, double limit, int id){

    long double exponent = 1;
    long double sum;

    for(double i=start; i < limit; i++) {        
        sum += (exponent/((2*i+2)*(2*i+3)*(2*i+4)));
        exponent = exponent * -1;
    }

    resultsArray.push_back(sum);
    
    return;
}