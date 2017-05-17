#include "Distribution.h"

int Distribution::getBinKey(double value){
  if(binningType=="linear"){
    return (int)(value/(double)resolution+0.5);
  }
  else if(binningType=="exponential"){
    return (int)(log(value)/(double)log(resolution));
  }
  else return -1;
}


double Distribution::getBinLength(DistIterator distIt){
  if(binningType=="linear"){
    return resolution;
  }
  else if(binningType=="exponential"){
    return pow(resolution,distIt->first)-pow(resolution,distIt->first-1);
  }
  else return -1;
    
}

double Distribution::getBinValue(DistIterator distIt){
  double value=distIt->first;
  if(binningType=="linear"){
    return value*resolution;
  }
  else if(binningType=="exponential"){
    return pow(resolution,value);
  }
  else return 0;
}

void Distribution::readFromFile(string fileName, int colNum, int actCol){
  ifstream ifs;
  ifs.open(fileName.c_str());
  string value;
  int ii=0;
  while(ifs>>value){ 
    ii++;
    if(ii==actCol){
      int binKey=getBinKey(atof(value.c_str()));
      increaseBin(binKey);
    }
    if(ii==colNum) ii=0; 
  }
}

void Distribution::increaseBin(int binKey){
  sumCounts++;
  DistIterator distIt=binnedData.find(binKey);
  if(distIt==binnedData.end()){
    Bin bin; 
    bin.counts=1;
    bin.num=1;
    binnedData.insert(make_pair(binKey,bin));
  }
  else{
    distIt->second.counts++;
    distIt->second.num++;
  }
}

void Distribution::increaseBin(int binKey, double value){
  sumCounts+=value;
  DistIterator distIt=binnedData.find(binKey);
  if(distIt==binnedData.end()){
    Bin bin; 
    bin.counts= value;
    bin.num = 1;
    binnedData.insert(make_pair(binKey,bin));
  }
  else{
    distIt->second.counts+=value;
    distIt->second.num++;
  }
}

void Distribution::setResolution(double _resolution){
  resolution=_resolution;
  cerr << "resolution: " << resolution << endl;    
}

void Distribution:: setBinningType(string _binningType){
  binningType=_binningType;
  cerr << "type: " << binningType << endl;
}

void Distribution::writeIntoFile(string fileName){
  ofstream ofs(fileName.c_str());
  double cdf=0;
  for(DistIterator distIt = binnedData.begin(); distIt!=binnedData.end(); distIt++){
    double prob=getProbability(distIt);
    double value=getBinValue(distIt);
    double counts=getCounts(distIt);
    cdf+=prob;
    ofs << value << " " << prob << " " << cdf <<  " " << counts << endl;
  }
}

double Distribution::getProbability(DistIterator distIt){
  return distIt->second.counts/(double)sumCounts;
}

double Distribution::getProbability(double value){
  int binKey=getBinKey(value);
  DistIterator distIt=binnedData.find(binKey);
  if(distIt!=binnedData.end()) {
    return getProbability(distIt);
  }
  else{
    return 0;
  }
}

double Averaging::getAverage(double value){
  int binKey=getBinKey(value);
  AvgIterator avgIt=binnedData.find(binKey);
  if(avgIt!=binnedData.end()) {
    return getAverage(avgIt);
  }
  else{
    return 0;
  }
}

double Averaging::getAverage(AvgIterator avgIt){
  return avgIt->second.counts/(double)avgIt->second.num;
}

void Averaging::writeIntoFile(string out_file){
  ofstream ofs(out_file.c_str());
  double sum=0, num=0, s=0;
  for(DistIterator distIt = binnedData.begin(); distIt!=binnedData.end(); distIt++){
    double avg=getAverage(distIt);
    double value=getBinValue(distIt);
    num+=distIt->second.num;
    sum+=distIt->second.counts;
    s+=avg;
    if(avg>0)
      ofs << value << " " << avg << " " << sum/num << " " << s << " " << distIt->second.num << " " << distIt->second.counts << endl;
  }
    
}


void Averaging::readFromFile(string fileName){
  ifstream ifs;
  ifs.open(fileName.c_str());
  double x,y;
  while(ifs>>x>>y){ 
      int binKey=getBinKey(x);
      increaseBin(binKey,y);
  }
}

void Averaging::readFromFile(string fileName, int colNum, int actColX, int actColY){
  ifstream ifs(fileName.c_str());
  string value;
  int ii=0;
  int binKey;
  while(ifs>>value){ 
    ii++;
    if(ii==actColX){
      binKey=getBinKey(atof(value.c_str()));
    }
    if(ii==actColY){
      increaseBin(binKey,atof(value.c_str())); 
    }
    if(ii==colNum) ii=0; 
  }
}

