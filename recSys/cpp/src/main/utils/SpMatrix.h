#ifndef SPMATRIX
#define SPMATRIX

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;



typedef map <int,double> MatrixRow;
typedef map <int,double> :: iterator RowIterator;

class SpMatrix{
  public:
    SpMatrix(){clear();};
    ~SpMatrix(){}; 
    int size();
    void resize(int row_id);
    void insert(int row_id, int col_id, double value);
    void update(int row_id, int col_id , double value);
    void increase(int row_id, int col_id, double value);
    void erase(int row_id, int col_id);
    double get(int row_id, int col_id);
    MatrixRow * get(int row_id);
    void clear();
    void read_from_file(string file_name);
    void write_into_file(string file_name);
    int row_size(int row_id);
    bool has_value(int row_id,int col_id);
  private:    
    vector < MatrixRow * >  matrix;
};

#endif
