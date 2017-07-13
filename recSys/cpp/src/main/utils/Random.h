#ifndef RANDOM
#define RANDOM

#include <gtest/gtest_prod.h>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Random{
  public:
    Random(){sample_num = vector <int> (5581365,0);}
    Random(int seed){
      state=seed;
      sample_num = vector <int> (5581365,0);
    }
    void set(int seed){state=seed;}
    int get(int max){step(); return state/(mod + 1.0)*max;}
    double get(){step(); return state/(mod + 1.0);}
    int get_linear(int max); 
    int get_geometric(int length,double prob, int threshold); 
    int get_geometric(double prob, int length); 
    double get_linear();
    double get_arctg(double y);
    int get_arctg(double y, int max);
    int operator()(int max){return get(max);}
    void write(ofstream& file){file.write(reinterpret_cast<char*>(&state), sizeof state);}
    void read(ifstream& file){file.read(reinterpret_cast<char*>(&state), sizeof state);}
    vector <int> sample_num;   
  private:
    long long state=67439852;
    static const int multiplier = 48271;
    static const int mod = 2147483647;
    void step(){
      state = state*multiplier % mod;
    };

};



#endif
