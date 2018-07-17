
class Random{
  %TypeHeaderCode
  #include "utils/Random.h"
  %End
  public:
    Random();
    Random(int seed);
    void set(int seed);
    int get(int max);
    double get();
    bool get_boolean(double prob);
    int get_linear(int max); 
    int get_geometric(double prob, int length); 
    double get_linear();
    double get_arctg(double y);
    int get_arctg(double y, int max);
    int get_discrete(std::vector<double>& distribution);
    int operator()(int max);
//    void write(ofstream& file);
//    void read(ifstream& file);
};

