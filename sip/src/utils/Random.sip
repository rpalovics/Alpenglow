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
    int get_linear(int max); 
    int get_geometric(int length,double prob, int threshold); 
    int get_geometric(double prob, int length); 
    double get_linear();
    double get_arctg(double y);
    int get_arctg(double y, int max);
    int operator()(int max);
    //void write(ofstream& file);
    //void read(ifstream& file);
    //vector <int> sample_num;   
};
