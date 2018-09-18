#ifndef RANDOM
#define RANDOM

//SIP_AUTOCONVERT

#include <gtest/gtest_prod.h>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Random{
  public:
    Random(){}
    Random(int seed){
      state=seed;
    }
    void set(int seed){state=seed;}
    int get(int max){step(); return state/(mod + 1.0)*max;}
    double get(){step(); return state/(mod + 1.0);}
    bool get_boolean(double prob);
    int get_linear(int max); 
    int get_geometric(double prob, int length); 
    double get_linear();
    double get_arctg(double y);
    int get_arctg(double y, int max);
    int get_discrete(vector<double>& distribution);
    int operator()(int max){return get(max);}
    void write(ofstream& file){file.write(reinterpret_cast<char*>(&state), sizeof state);}
    void read(ifstream& file){file.read(reinterpret_cast<char*>(&state), sizeof state);}
    template<class RandomIt >
    void random_shuffle( RandomIt first, RandomIt last ){
      typename std::iterator_traits<RandomIt>::difference_type i, n;
      n = last - first;
      for (i = n-1; i > 0; --i) {
        using std::swap;
        swap(first[i], first[(*this)(i+1)]);
      }
    };
  private:
    unsigned long long state=67439852;
    static const int multiplier = 48271;
    static const int mod = 2147483647;
    void step(){
      state = state*multiplier % mod;
    };

};



#endif
