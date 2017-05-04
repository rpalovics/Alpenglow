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
    void readFromFile(string fileName);
    void writeIntoFile(string fileName);
    int size();
    int rowSize(int rowID);
    void resize(int rowID);
    void insert(int rowID, int colID, double value);
    void update(int rowID, int colID , double value);
    void increase(int rowID, int colID, double value);
    void erase(int rowID, int colID);
    double get(int rowID, int colID);
    MatrixRow * get(int rowID);
    bool hasValue(int rowID,int colID);
    void clear();
  private:    
    vector < MatrixRow * >  matrix;
};

#endif
