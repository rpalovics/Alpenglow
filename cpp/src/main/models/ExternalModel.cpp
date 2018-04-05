#include "ExternalModel.h"
#include <map>

void ExternalModel::read_predictions(string file_name){
    ifstream file(file_name);
    unordered_map<int, map<int, int>> lists;
    int user, item, pos;
    while (file >> user >> item >> pos) {
      lists[user][pos]=item;
    }
    file.close();
    toplists_.clear();
    for(const auto &list : lists){
      vector<int> user_toplist;
      user_toplist.reserve(list.second.size());
      for(const auto &item : list.second){
        user_toplist.push_back(item.second);
      }
      toplists_[list.first] = user_toplist;
    }
}

void ExternalModel::add(RecDat* rec_dat){}

double ExternalModel::prediction(RecDat* rec_dat){return 0;}

vector<pair<int,double>> ExternalModel::get_top_list(int user, int k, SpMatrix *exclude){
  if(mode_ == "write"){
    requested_users_.insert(user);
    return vector<pair<int,double>>();
  } else {
    const auto &listfind = toplists_.find(user);
    if(listfind != toplists_.end()){
      vector<pair<int,double>> return_list;
      return_list.reserve(listfind->second.size());
      for(int item : listfind->second){
        return_list.push_back({item, 1});
      }
      return return_list;
    } else {
      return vector<pair<int,double>>();
    }
  }
}

void ExternalModel::write_requested_users(string file_name){
    ofstream file(file_name);
    file << "user" << endl;
    for(auto user : requested_users_){
      file << user << endl;
    }
    file.close();
    requested_users_.clear();
}