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
      set<int> compUsers;

      std::random_device rd;
      std::minstd_rand gen(rd());
      std::uniform_int_distribution<> dis(0, seenUsers.size()-1);

      while((int)compUsers.size() < user_number_){
        auto it = this->seenUsers.begin();
        int rOffset = dis(gen);
        advance(it,rOffset);
        compUsers.insert(*it);
      }
      ofstream myfile;
      myfile.open (file_name_);
      for(auto u : compUsers){
        float sum=0;
        RecDat query;
        query.user=u;
        for(auto i : this->seenItems){
          query.item=i;
          sum+=this->model->prediction(&query);
        }
        float avg = sum/(this->seenItems.size());

        float varsum=0;
        for(auto i : this->seenItems){
          query.item=i;
          varsum+=pow(this->model->prediction(&query)-avg,2);
        }
        float var = pow(varsum/(this->seenItems.size()),1.0/2.0);
        for(auto i : this->seenItems){
          query.item=i;
          float gpred = (this->model->prediction(&query)-avg)/var;
          myfile << gpred << endl;
        }
      }
      myfile.close();
    }
    virtual void run(RecDat* recDat){
    	this->seenItems.insert(recDat->item);
    	this->seenUsers.insert(recDat->user);
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
    set<int> seenItems;
    set<int> seenUsers;
};

#endif
