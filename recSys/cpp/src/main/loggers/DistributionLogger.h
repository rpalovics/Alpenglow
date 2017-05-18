#ifndef DISTRIBUTION_LOGGER
#define DISTRIBUTION_LOGGER

#include <iostream>
#include <string>
#include <set>
#include <exception>
#include "Logger.h"
#include "../models/Model.h"
using namespace std;

struct DistributionLoggerParameters{
  string file_name;
  int user_number;
};

class DistributionLogger : public Logger{
  public:
    DistributionLogger(){}
    DistributionLogger(DistributionLoggerParameters *parameters){
      set_parameters(parameters);
    }
    virtual ~DistributionLogger(){
      set<int> comp_users;

      std::random_device rd;
      std::minstd_rand gen(rd());
      std::uniform_int_distribution<> dis(0, seen_users.size()-1);

      while((int)comp_users.size() < user_number_){
        auto it = this->seen_users.begin();
        int r_offset = dis(gen);
        advance(it,r_offset);
        comp_users.insert(*it);
      }
      ofstream myfile;
      myfile.open (file_name_);
      for(auto u : comp_users){
        float sum=0;
        RecDat query;
        query.user=u;
        for(auto i : this->seen_items){
          query.item=i;
          sum+=this->model->prediction(&query);
        }
        float avg = sum/(this->seen_items.size());

        float varsum=0;
        for(auto i : this->seen_items){
          query.item=i;
          varsum+=pow(this->model->prediction(&query)-avg,2);
        }
        float var = pow(varsum/(this->seen_items.size()),1.0/2.0);
        for(auto i : this->seen_items){
          query.item=i;
          float gpred = (this->model->prediction(&query)-avg)/var;
          myfile << gpred << endl;
        }
      }
      myfile.close();
    }
    virtual void run(RecDat* rec_dat){
    	this->seen_items.insert(rec_dat->item);
    	this->seen_users.insert(rec_dat->user);
    }
    virtual void set_model(Model* model_){
      this->model=model_;
    }
  protected:
    void set_parameters(DistributionLoggerParameters * parameters){
      file_name_=parameters->file_name;
      user_number_=parameters->user_number;
      if(user_number_==-1){
        user_number_=1;
      }
    }
    Model* model;
    string file_name_;
    int user_number_;
    set<int> seen_items;
    set<int> seen_users;
};

#endif
