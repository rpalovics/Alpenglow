#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

struct Bin{
  double counts;
  double num;
};

typedef map < int , Bin > :: iterator DistIterator;
typedef map < int , Bin > :: iterator AvgIterator;

class Distribution {
  protected: 
    map <int,Bin> binned_data;
    double resolution;
    string binning_type;
    double sum_counts;
  public:
    Distribution(){sum_counts=0;};
    Distribution(double _resolution, string _binning_type){
      sum_counts=0;
      resolution=_resolution;
      binning_type=_binning_type;
      cerr << "type: " << binning_type << endl;
      cerr << "resolution: " << resolution << endl;
    }
    ~Distribution(){};
    void read_from_file(string file_name, int col_num, int act_col);
    void write_into_file(string file_name);
    int get_bin_key(double value);
    double get_bin_value(DistIterator dist_it);
    double get_bin_length(DistIterator dist_it);
    double get_probability(DistIterator dist_it);
    double get_counts(DistIterator dist_it){return dist_it->second.counts;};
    void increase_bin(int bin_key);
    void increase_bin(int bin_key, double num);
    void insert(double value){increase_bin(get_bin_key(value));};
    void insert(double value, int num){increase_bin(get_bin_key(value),num);};
    void set_resolution(double _resolution);
    void set_binning_type(string _binning_type);
    DistIterator begin(){return binned_data.begin();};
    DistIterator end(){return binned_data.end();};
    int size(){return (int)binned_data.size();};
    double get_probability(double value);
    

};

class Averaging : public Distribution{
  public:
    Averaging():Distribution(){};
    void read_from_file(string file_name);
    void read_from_file(string file_name, int col_num, int act_col_x, int act_col_y);
    double get_average(double value);
    double get_average(AvgIterator avg_it);
    void write_into_file(string out_file); 
};

#endif /* DISTRIBUTION_H */
