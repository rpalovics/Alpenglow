#include "RandomChoosingCombinedModelExpertUpdater.h"

void RandomChoosingCombinedModelExpertUpdater::update(RecDat* rec_dat){
  for(uint i=0;i<model_->distribution_.size();i++){
    double loss = evaluators_[i]->get_loss(rec_dat);
    double exp_loss = exp(-eta_*loss);
    model_->distribution_[i]*=exp_loss;
  }
  double sum=0;
  for(auto& weight:model_->distribution_){
    sum+=weight;
  }
  for(uint i=0;i<model_->distribution_.size();i++){
    model_->distribution_[i]/=sum;
  }
}
