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
    void resize(int rowID);
    void insert(int rowID, int colID, double value);
    void update(int rowID, int colID , double value);
    void increase(int rowID, int colID, double value);
    void erase(int rowID, int colID);
    double get(int rowID, int colID);
    MatrixRow * get(int rowID);
    void clear();
    void read_from_file(string fileName);
    void write_into_file(string fileName);
    int row_size(int rowID);
    bool has_value(int rowID,int colID);

    //DEPRECATED
    void readFromFile(string fileName){ read_from_file(fileName); }
    void writeIntoFile(string fileName){ write_into_file(fileName); }
    int rowSize(int rowID){ return row_size(rowID); }
    bool hasValue(int rowID,int colID){ return has_value(rowID,colID); }
  private:    
    vector < MatrixRow * >  matrix;
};

#endif
