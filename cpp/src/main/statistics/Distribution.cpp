#include "Distribution.h"

int Distribution::get_bin_key(double value){
  if(binning_type=="linear"){
    return (int)(value/(double)resolution+0.5);
  }
  else if(binning_type=="exponential"){
    return (int)(log(value)/(double)log(resolution));
  }
  else return -1;
}


double Distribution::get_bin_length(DistIterator dist_it){
  if(binning_type=="linear"){
    return resolution;
  }
  else if(binning_type=="exponential"){
    return pow(resolution,dist_it->first)-pow(resolution,dist_it->first-1);
  }
  else return -1;
    
}

double Distribution::get_bin_value(DistIterator dist_it){
  double value=dist_it->first;
  if(binning_type=="linear"){
    return value*resolution;
  }
  else if(binning_type=="exponential"){
    return pow(resolution,value);
  }
  else return 0;
}

void Distribution::read_from_file(string file_name, int col_num, int act_col){
  ifstream ifs;
  ifs.open(file_name.c_str());
  string value;
  int ii=0;
  while(ifs>>value){ 
    ii++;
    if(ii==act_col){
      int bin_key=get_bin_key(atof(value.c_str()));
      increase_bin(bin_key);
    }
    if(ii==col_num) ii=0; 
  }
}

void Distribution::increase_bin(int bin_key){
  sum_counts++;
  DistIterator dist_it=binned_data.find(bin_key);
  if(dist_it==binned_data.end()){
    Bin bin; 
    bin.counts=1;
    bin.num=1;
    binned_data.insert(make_pair(bin_key,bin));
  }
  else{
    dist_it->second.counts++;
    dist_it->second.num++;
  }
}

void Distribution::increase_bin(int bin_key, double value){
  sum_counts+=value;
  DistIterator dist_it=binned_data.find(bin_key);
  if(dist_it==binned_data.end()){
    Bin bin; 
    bin.counts= value;
    bin.num = 1;
    binned_data.insert(make_pair(bin_key,bin));
  }
  else{
    dist_it->second.counts+=value;
    dist_it->second.num++;
  }
}

void Distribution::set_resolution(double _resolution){
  resolution=_resolution;
  cerr << "resolution: " << resolution << endl;    
}

void Distribution:: set_binning_type(string _binning_type){
  binning_type=_binning_type;
  cerr << "type: " << binning_type << endl;
}

void Distribution::write_into_file(string file_name){
  ofstream ofs(file_name.c_str());
  double cdf=0;
  for(DistIterator dist_it = binned_data.begin(); dist_it!=binned_data.end(); dist_it++){
    double prob=get_probability(dist_it);
    double value=get_bin_value(dist_it);
    double counts=get_counts(dist_it);
    cdf+=prob;
    ofs << value << " " << prob << " " << cdf <<  " " << counts << endl;
  }
}

double Distribution::get_probability(DistIterator dist_it){
  return dist_it->second.counts/(double)sum_counts;
}

double Distribution::get_probability(double value){
  int bin_key=get_bin_key(value);
  DistIterator dist_it=binned_data.find(bin_key);
  if(dist_it!=binned_data.end()) {
    return get_probability(dist_it);
  }
  else{
    return 0;
  }
}

double Averaging::get_average(double value){
  int bin_key=get_bin_key(value);
  AvgIterator avg_it=binned_data.find(bin_key);
  if(avg_it!=binned_data.end()) {
    return get_average(avg_it);
  }
  else{
    return 0;
  }
}

double Averaging::get_average(AvgIterator avg_it){
  return avg_it->second.counts/(double)avg_it->second.num;
}

void Averaging::write_into_file(string out_file){
  ofstream ofs(out_file.c_str());
  double sum=0, num=0, s=0;
  for(DistIterator dist_it = binned_data.begin(); dist_it!=binned_data.end(); dist_it++){
    double avg=get_average(dist_it);
    double value=get_bin_value(dist_it);
    num+=dist_it->second.num;
    sum+=dist_it->second.counts;
    s+=avg;
    if(avg>0)
      ofs << value << " " << avg << " " << sum/num << " " << s << " " << dist_it->second.num << " " << dist_it->second.counts << endl;
  }
    
}


void Averaging::read_from_file(string file_name){
  ifstream ifs;
  ifs.open(file_name.c_str());
  double x,y;
  while(ifs>>x>>y){ 
      int bin_key=get_bin_key(x);
      increase_bin(bin_key,y);
  }
}

void Averaging::read_from_file(string file_name, int col_num, int act_col_x, int act_col_y){
  ifstream ifs(file_name.c_str());
  string value;
  int ii=0;
  int bin_key;
  while(ifs>>value){ 
    ii++;
    if(ii==act_col_x){
      bin_key=get_bin_key(atof(value.c_str()));
    }
    if(ii==act_col_y){
      increase_bin(bin_key,atof(value.c_str())); 
    }
    if(ii==col_num) ii=0; 
  }
}

