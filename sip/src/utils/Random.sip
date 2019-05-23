
class Random{
  %TypeHeaderCode
  #include "utils/Random.h"
  %End
  %Docstring
    This class implements a pseudorandom generator.

    The next state is computed as `state = state*multiplier % mod` where
    `multiplier = 48271` and `mod = 2147483647`.

    The initial state value can be set through the parameter of the constructor,
    or using the :py:meth:`set()` function.

    Most of the functions are available without the max parameter providing a
    double value between [0,1) with a similar distribution as the discrete
    functions.
  %End
  public:
    Random();
    Random(int seed);
    void set(int seed);
  %Docstring
      set(int seed)
      Set the state of the random generator.
  %End
    int get(int max);
  %Docstring
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
  %End
    double get();

    bool get_boolean(double prob);
  %Docstring
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
  %End
    int get_linear(int max); 
  %Docstring
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
  %End
    int get_geometric(double prob, int max); 
  %Docstring
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
  %End
    double get_linear();

    double get_arctg(double y);

    int get_arctg(double y, int max);
  %Docstring
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
  %End
    int get_discrete(std::vector<double>& distribution);
  %Docstring
      get_discrete(std::vector<double>& distribution)
      Get a pseudorandom value following a given discrete distribution.

      The sum of the values in the given vector should be 1.
      If the sum is more or less, the probability of the largest value(s) will differ from the specified probability.
      The values should be non-negative.

      Parameters
      ----------
      distribution : std::vector<double>&
          The probability of output value i is distribution[i].

      Returns
      -------
      int
          The pseudorandom value.
  %End
    int operator()(int max);
//    void write(ofstream& file);
//    void read(ifstream& file);
    bool self_test();
};

