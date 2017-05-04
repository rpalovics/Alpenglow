#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include<stdio.h>
#include<map>
#include<stdlib.h>
#include<vector>
#include<utility>

using namespace std;

struct Bin{
  double counts;
  double num;
};

typedef map < int , Bin > :: iterator DistIterator;
typedef map < int , Bin > :: iterator AvgIterator;

class Distribution {
  protected: 
    map <int,Bin> binnedData;
    double resolution;
    string binningType;
    double sumCounts;
  public:
    Distribution(){sumCounts=0;};
    Distribution(double _resolution, string _binningType){
      sumCounts=0;
      resolution=_resolution;
      binningType=_binningType;
      cerr << "type: " << binningType << endl;
      cerr << "resolution: " << resolution << endl;
    }
    ~Distribution(){};
    void readFromFile(string fileName, int colNum, int actCol);
    void writeIntoFile(string fileName);
    int getBinKey(double value);
    double getBinValue(DistIterator distIt);
    double getBinLength(DistIterator distIt);
    double getProbability(DistIterator distIt);
    double getCounts(DistIterator distIt){return distIt->second.counts;};
    void increaseBin(int binKey);
    void increaseBin(int binKey, double num);
    void insert(double value){increaseBin(getBinKey(value));};
    void insert(double value, int num){increaseBin(getBinKey(value),num);};
    void setResolution(double _resolution);
    void setBinningType(string _binningType);
    DistIterator begin(){return binnedData.begin();};
    DistIterator end(){return binnedData.end();};
    int size(){return (int)binnedData.size();};
    double getProbability(double value);
    

};

class Averaging : public Distribution{
  public:
    Averaging():Distribution(){};
    void readFromFile(string fileName);
    void readFromFile(string fileName, int colNum, int actColX, int actColY);
    double getAverage(double value);
    double getAverage(AvgIterator avgIt);
    void writeIntoFile(string outFile); 
};

#endif
