#ifndef SPMATRIX
#define SPMATRIX

//SIP_AUTOCONVERT

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

typedef map<int,double> MatrixRow;
typedef map<int,double>::iterator RowIterator;

class SpMatrix{
  public:
    ~SpMatrix(){clear();} 
    int size();
    void resize(int row_id);
    void insert(int row_id, int col_id, double value);
    void update(int row_id, int col_id , double value);
    void increase(int row_id, int col_id, double value);
    void erase(int row_id, int col_id);
    double get(int row_id, int col_id);
    map<int,double>* get(int row_id);
    void clear();
    void read_from_file(string file_name);
    void write_into_file(string file_name);
    int row_size(int row_id);
    bool has_value(int row_id,int col_id);
  private:    
    vector<MatrixRow*>  matrix_;
};

#endif
