#include "RecommenderData.h"
#include <cstdlib>

std::vector<std::string> Split(const std::string& input, const char sep) {
  size_t temp_pos1 = 0;
  size_t temp_pos2 =  input.find(sep);
  std::vector<std::string> return_vector;
  while (temp_pos2 != std::string::npos) {
    return_vector.push_back(input.substr(temp_pos1, temp_pos2 - temp_pos1));
    temp_pos1 = temp_pos2 + 1;
    temp_pos2 = input.find(sep, temp_pos2 + 1);
  }
  return_vector.push_back(input.substr(temp_pos1, temp_pos2 - temp_pos1));
  return return_vector;
}

std::map<int, std::vector<std::vector<int> >*> track_attribute_map;

void RecommenderData::read_from_file(string file_name, string type){
  ifstream ifs(file_name.c_str());
  read_from_file_core(ifs,type);
}
void RecommenderData::read_from_file_core(istream& ifs, string type){
  double score;
  double t;
  int user,item,eval, location;
  int id;
  double x, y, z;
  int category;
  std::string country;
  if (type == "online") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval) {
      if (t>=maxTime && maxTime>0) break;
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = t;
      recDat.id = id;
      recDat.score = score;
      recDat.location.location_id = 0;
      recDat.eval=eval;
      recData.push_back(recDat); 
    }
  }
  if (type == "online_id") {
    while (ifs >> t >> user >> item >> id >> score >> eval) {
      if (t>=maxTime && maxTime>0) break;
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = t;
      recDat.id = id;
      recDat.score = score;
      recDat.eval=eval;
      recDat.location.location_id = 0;
      recData.push_back(recDat); 
    }
  }
  if (type == "online_attribute") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval) {
      if (t>=maxTime && maxTime>0) break;
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = t;
      recDat.score = score;
      recDat.eval=eval;
      recDat.id = id;
      recDat.location.location_id = 0;
      ifs.ignore(1, ' ');
      if(ifs.eof()) break;
      string attribute_string;
      getline(ifs, attribute_string);
      recData.push_back(recDat); 
      attribute_container_->read_attribute(id, attribute_string);
      id++;
    }
  }
  else if (type == "offline") {
    id = 0;
    while (ifs >> user >> item >> score) {
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = 0;
      recDat.score = score;
      recDat.eval=1;
      recDat.id = id;
      recDat.location.location_id = 0;
      recData.push_back(recDat);
    }
  }
  else if (type == "offlineTimestamp") {
    id = 0;
    while (ifs >> t >> user >> item >> score) {
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = t;
      recDat.score = score;
      recDat.eval=1;
      recDat.id = id;
      recDat.location.location_id = 0;
      recData.push_back(recDat);
    }
  }
  else if ( type  == "location") {
    id = 0;
    std::cout << "read location" << std::endl;
    std::string buff;
    getline(ifs, buff);
    while( ifs >> t >> user >> item >> score >> eval >>  location) {
      if (t>=maxTime && maxTime>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.time = t;
      rec_dat.item = item;
      rec_dat.location.location_id = location;
      rec_dat.eval = eval;
      rec_dat.id = id;
      rec_dat.score = score;
      recData.push_back(rec_dat);
    }
  }
  else if ( type == "location_xyz") {
    id = 0;
    std::cout << "reading, type location_xyz" << std::endl;
    while (ifs >> t >> user >> item >> score >> eval >> location >> x >> y >> z) {
      if (t>=maxTime && maxTime>0) break;
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = t;
      recDat.eval = eval;
      recDat.id = id;
      recDat.score = score;
      recDat.location.location_id = location;
      recDat.location.x = x;
      recDat.location.y = y;
      recDat.location.z = z;
      recData.push_back(recDat);
    }
  }
  if (type == "category") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval >> category ) {
      if (t>=maxTime && maxTime>0) break;
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = t;
      recDat.score = score;
      recDat.eval=eval;
      recDat.id = id;
      recDat.category = category;
      recDat.location.location_id = 0;
      recData.push_back(recDat); 
    }
  }
  cerr << "read OK, size: " << recData.size() << endl;
}

SpMatrix* RecommenderData::matrix() {
  if(recMatrix.size()==0){
    recMatrix.clear();
    for (uint jj=0; jj<recData.size(); jj++) {
      RecDat recDat = recData[jj];
      recMatrix.update(recDat.user,recDat.item,recDat.score);
    }
  }
  return &recMatrix;
}

vector<int>* RecommenderData::items(){
  if(items_.size()==0){
    vector<bool> item_map;
    for(uint ii=0; ii<recData.size(); ii++){
      int item = recData[ii].item;
      if((int)item_map.size()<=item) item_map.resize(item+1,false);
      if(!item_map[item]){
        item_map[item]=true;
        items_.push_back(item);
      }
    }
  }
  return &items_;
}

vector<int>* RecommenderData::users(){
  if(users_.size()==0){
    vector<bool> user_map;
    for(uint ii=0; ii<recData.size(); ii++){
      int user = recData[ii].user;
      if((int)user_map.size()<=user) user_map.resize(user+1,false);
      if(!user_map[user]){
        user_map[user]=true;
        users_.push_back(user);
      }
    }
  }
  return &users_;
}
