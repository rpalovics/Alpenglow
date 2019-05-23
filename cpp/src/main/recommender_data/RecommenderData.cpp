#include "RecommenderData.h"

SpMatrix* RecommenderData::get_full_matrix() {
  if(full_matrix_.size()==0){
    for (uint jj=0; jj<rec_data_.size(); jj++) {
      RecDat& rec_dat = rec_data_[jj];
      full_matrix_.update(rec_dat.user,rec_dat.item,rec_dat.score);
    }
  }
  return &full_matrix_;
}

vector<int>* RecommenderData::get_all_items(){
  if(items_.size()==0){
    vector<int> item_map;
    for(uint ii=0; ii<rec_data_.size(); ii++){
      int item = rec_data_[ii].item;
      if (!GET_VECTORMAP(item_map,item,false)){
        PUT_VECTORMAP(item_map,item,true);
        items_.push_back(item);
      }
    }
  }
  return &items_;
}

vector<int>* RecommenderData::get_all_users(){
  if(users_.size()==0){
    vector<int> user_map;
    for(uint ii=0; ii<rec_data_.size(); ii++){
      int user = rec_data_[ii].user;
      if (!GET_VECTORMAP(user_map,user,false)){
        PUT_VECTORMAP(user_map,user,true);
        users_.push_back(user);
      }
    }
  }
  return &users_;
}

void RecommenderData::clear(){
  rec_data_.clear();
  full_matrix_.clear();
  items_.clear();
  users_.clear();
}

void LegacyRecommenderData::read_from_file(string file_name, string type){
  ifstream ifs(file_name.c_str());
  if (ifs.fail()){
    cerr << "WARNING: (LegacyRecommenderData) Could not open the input file." << endl;
    cerr << "Input file name is '" << file_name << "'" << endl;
  }
  read_from_file_core(ifs,type);
}
void LegacyRecommenderData::read_from_file_core(istream& ifs, string type){
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
      if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.id = id;
      rec_dat.score = score;
      //rec_dat.location.location_id = 0;
      rec_dat.eval=eval;
      rec_data_.push_back(rec_dat); 
      id++;
    }
  }
  if (type == "online_id") {
    while (ifs >> t >> user >> item >> id >> score >> eval) {
      if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.id = id;
      rec_dat.score = score;
      rec_dat.eval=eval;
      //rec_dat.location.location_id = 0;
      rec_data_.push_back(rec_dat); 
    }
  }
  if (type == "online_id_noeval") {
    while (ifs >> t >> user >> item >> id >> score) {
      if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.id = id;
      rec_dat.score = score;
      rec_dat.eval=1;
      //rec_dat.location.location_id = 0;
      rec_data_.push_back(rec_dat); 
    }
  }
  if (type == "online_attribute") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval) {
      if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
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
      rec_data_.push_back(rec_dat); 
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
      rec_data_.push_back(rec_dat);
      id++;
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
      rec_data_.push_back(rec_dat);
      id++;
    }
  }
  //else if ( type  == "location") {
  //  id = 0;
  //  std::cout << "read location" << std::endl;
  //  std::string buff;
  //  getline(ifs, buff);
  //  while( ifs >> t >> user >> item >> score >> eval >>  location) {
  //    if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
  //    RecDat rec_dat;
  //    rec_dat.user = user;
  //    rec_dat.time = t;
  //    rec_dat.item = item;
  //    //rec_dat.location.location_id = location;
  //    rec_dat.eval = eval;
  //    rec_dat.id = id;
  //    rec_dat.score = score;
  //    rec_data_.push_back(rec_dat);
  //  }
  //}
  //else if ( type == "location_xyz") {
  //  id = 0;
  //  std::cout << "reading, type location_xyz" << std::endl;
  //  while (ifs >> t >> user >> item >> score >> eval >> location >> x >> y >> z) {
  //    if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
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
  //    rec_data_.push_back(rec_dat);
  //  }
  //}
  if (type == "category") {
    id = 0;
    while (ifs >> t >> user >> item >> score >> eval >> category ) {
      if (t>=experiment_termination_time_ && experiment_termination_time_>0) break;
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = t;
      rec_dat.score = score;
      rec_dat.eval=eval;
      rec_dat.id = id;
      rec_dat.category = category;
      //rec_dat.location.location_id = 0;
      rec_data_.push_back(rec_dat); 
      id++;
    }
  }
  // cerr << "read OK, size: " << rec_data_.size() << endl;
  if (rec_data_.size() == 0) {
    cerr << "WARNING: (LegacyRecommenderData) Size of rec_data_ is 0." << endl;
  }
}

