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
  int user,item,eval;
  //int location;
  int id;
  //double x, y, z;
  int category;
  std::string country;
  if (type == "online") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval) {
      if (t>=max_time && max_time>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.id = id;
      rec_dat.score = score;
      //rec_dat.location.location_id = 0;
      rec_dat.eval=eval;
      rec_data.push_back(rec_dat); 
    }
  }
  if (type == "online_id") {
    while (ifs >> t >> user >> item >> id >> score >> eval) {
      if (t>=max_time && max_time>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.id = id;
      rec_dat.score = score;
      rec_dat.eval=eval;
      //rec_dat.location.location_id = 0;
      rec_data.push_back(rec_dat); 
    }
  }
  if (type == "online_attribute") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval) {
      if (t>=max_time && max_time>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.score = score;
      rec_dat.eval=eval;
      rec_dat.id = id;
      //rec_dat.location.location_id = 0;
      ifs.ignore(1, ' ');
      if(ifs.eof()) break;
      string attribute_string;
      getline(ifs, attribute_string);
      rec_data.push_back(rec_dat); 
      attribute_container_->read_attribute(id, attribute_string);
      id++;
    }
  }
  else if (type == "offline") {
    id = 0;
    while (ifs >> user >> item >> score) {
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = 0;
      rec_dat.score = score;
      rec_dat.eval=1;
      rec_dat.id = id;
      //rec_dat.location.location_id = 0;
      rec_data.push_back(rec_dat);
    }
  }
  else if (type == "offlineTimestamp") {
    id = 0;
    while (ifs >> t >> user >> item >> score) {
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.score = score;
      rec_dat.eval=1;
      rec_dat.id = id;
      //rec_dat.location.location_id = 0;
      rec_data.push_back(rec_dat);
    }
  }
  //else if ( type  == "location") {
  //  id = 0;
  //  std::cout << "read location" << std::endl;
  //  std::string buff;
  //  getline(ifs, buff);
  //  while( ifs >> t >> user >> item >> score >> eval >>  location) {
  //    if (t>=max_time && max_time>0) break;
  //    RecDat rec_dat;
  //    rec_dat.user = user;
  //    rec_dat.time = t;
  //    rec_dat.item = item;
  //    //rec_dat.location.location_id = location;
  //    rec_dat.eval = eval;
  //    rec_dat.id = id;
  //    rec_dat.score = score;
  //    rec_data.push_back(rec_dat);
  //  }
  //}
  //else if ( type == "location_xyz") {
  //  id = 0;
  //  std::cout << "reading, type location_xyz" << std::endl;
  //  while (ifs >> t >> user >> item >> score >> eval >> location >> x >> y >> z) {
  //    if (t>=max_time && max_time>0) break;
  //    RecDat rec_dat;
  //    rec_dat.user = user;
  //    rec_dat.item = item;
  //    rec_dat.time = t;
  //    rec_dat.eval = eval;
  //    rec_dat.id = id;
  //    rec_dat.score = score;
  //    rec_dat.location.location_id = location;
  //    rec_dat.location.x = x;
  //    rec_dat.location.y = y;
  //    rec_dat.location.z = z;
  //    rec_data.push_back(rec_dat);
  //  }
  //}
  if (type == "category") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval >> category ) {
      if (t>=max_time && max_time>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.score = score;
      rec_dat.eval=eval;
      rec_dat.id = id;
      rec_dat.category = category;
      //rec_dat.location.location_id = 0;
      rec_data.push_back(rec_dat); 
    }
  }
  // cerr << "read OK, size: " << rec_data.size() << endl;
}

SpMatrix* RecommenderData::matrix() {
  if(rec_matrix.size()==0){
    rec_matrix.clear();
    for (uint jj=0; jj<rec_data.size(); jj++) {
      RecDat rec_dat = rec_data[jj];
      rec_matrix.update(rec_dat.user,rec_dat.item,rec_dat.score);
    }
  }
  return &rec_matrix;
}

vector<int>* RecommenderData::items(){
  if(items_.size()==0){
    vector<bool> item_map;
    for(uint ii=0; ii<rec_data.size(); ii++){
      int item = rec_data[ii].item;
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
    for(uint ii=0; ii<rec_data.size(); ii++){
      int user = rec_data[ii].user;
      if((int)user_map.size()<=user) user_map.resize(user+1,false);
      if(!user_map[user]){
        user_map[user]=true;
        users_.push_back(user);
      }
    }
  }
  return &users_;
}
