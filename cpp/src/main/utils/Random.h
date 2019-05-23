#ifndef RANDOM_H
#define RANDOM_H

//SIP_AUTOCONVERT

#include <gtest/gtest_prod.h>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Random{
/**
    This class implements a pseudorandom generator.

    The next state is computed as `state = state*multiplier % mod` where
    `multiplier = 48271` and `mod = 2147483647`.

    The initial state value can be set through the parameter of the constructor,
    or using the :py:meth:`set()` function.

    Most of the functions are available without the max parameter providing a
    double value between [0,1) with a similar distribution as the discrete
    functions.
*/
  public:
    Random(){}
    Random(int seed){
      state=seed;
    }
    void set(int seed){state=seed;}
    /**
      set(int seed)
      Set the state of the random generator.
    */
    int get(int max){step(); return state/(mod + 1.0)*max;}
    /**
      get(int max)
      Get a uniform pseudorandom value between 0 and max-1.

      Largest possible value is max-1.

      Parameters
      ----------
      max : int
          The upper bound of the random value.

      Returns
      -------
      int
          The pseudorandom value.
    */
    double get(){step(); return state/(mod + 1.0);}
    ///**
    //  Get a uniform pseudorandom value from [0,1).

    //  Largest possible value is less than 1.

    //  Returns
    //  -------
    //  double
    //      The pseudorandom value.
    //*/
    bool get_boolean(double prob);
    /**
      get_boolean(double prob)
      Get a pseudorandom true-or-false value.

      Parameters
      ----------
      prob : double
          The probability of the true value.

      Returns
      -------
      bool
          The pseudorandom value.
    */
    int get_linear(int max); 
    /**
      get_linear(int max)
      Get a pseudorandom value between 0 and max-1 with linear distribution.

      Probability of smaller values is smaller. The largest possible value is max-1.

      Parameters
      ----------
      max : int
          The upper bound of the random value.

      Returns
      -------
      int
          The pseudorandom value.
    */
    int get_geometric(double prob, int max); 
    /**
      get_geometric(double prob, int max)
      Get a pseudorandom value between 0 and max-1 with geometric distribution.

      Probability of smaller values is larger. The largest possible value is max-1.
      The probability of value i is proportional to (1-prob)*prob^i`.

      Parameters
      ----------
      prob : double
          The parameter of the distribution.
      max : int
          The upper bound of the random value.

      Returns
      -------
      int
          The pseudorandom value.
    */
    double get_linear();
    ///**
    //  Get a pseudorandom value between 0 and 1 with linear distribution.

    //  Probability of smaller values is smaller. The largest possible value is smaller than 1.

    //  Returns
    //  -------
    //  double
    //      The pseudorandom value.
    //*/
    double get_arctg(double y);
    ///**
    //  get_arctg(double y)
    //  Get a pseudorandom value between 0 and 1 with decaying distribution.

    //  Probability of smaller values is larger. The largest possible value is smaller than 1.

    //  Parameters
    //  ----------
    //  y : double
    //      The parameter of the distribution.

    //  Returns
    //  -------
    //  double
    //      The pseudorandom value.
    //*/
    int get_arctg(double y, int max);
    /**
      get_arctg(double y, int max)
      Get a pseudorandom value between 0 and max-1 with decaying distribution.

      Probability of smaller values is larger. The largest possible value is max-1.

      Parameters
      ----------
      y : double
          The parameter of the distribution.
      max : int
          The upper bound of the random value.

      Returns
      -------
      double
          The pseudorandom value.
    */
    int get_discrete(vector<double>& distribution);
    /**
      get_discrete(vector<double>& distribution)
      Get a pseudorandom value following a given discrete distribution.

      The sum of the values in the given vector should be 1.
      If the sum is more or less, the probability of the largest value(s) will differ from the specified probability.
      The values should be non-negative.

      Parameters
      ----------
      distribution : vector<double>&
          The probability of output value i is distribution[i].

      Returns
      -------
      int
          The pseudorandom value.
    */
    int operator()(int max){return get(max);}
    void write(ofstream& file){file.write(reinterpret_cast<char*>(&state), sizeof state);}
    void read(ifstream& file){file.read(reinterpret_cast<char*>(&state), sizeof state);}
    bool self_test(){
      bool ok = true;
      if (state==0) ok = false;
      return ok;
    }
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



#endif /* RANDOM_H */
