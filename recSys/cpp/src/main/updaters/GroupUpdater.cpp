#include "GroupUpdater.h"

//void GroupUpdater::update(GradientComputer* gradient_computer){
//}

int GroupUpdater::update_gradient_updaters(RecDat* rec_dat, GradientComputer* gradient_computer, vector<ModelGradientUpdater*>* gradient_updaters){
  int cost = 0;
  for(uint i=0; i<gradient_updaters->size(); i++){
    ModelGradientUpdater* updater = gradient_updaters->at(i);
    updater->beginning_of_updating_cycle(rec_dat);
  }
  while(gradient_computer->hasNext()){
    vector<pair<RecDat*,double> >* gradients = gradient_computer->getNextGradient();
    cost+=gradients->size();
    //update gradient updaters
    for(uint i=0; i<gradient_updaters->size(); i++){
      ModelGradientUpdater* updater = gradient_updaters->at(i);
      for(uint ii=0; ii<gradients->size(); ii++){
        RecDat * rec_dat =  gradients->at(ii).first;
        double gradient = gradients->at(ii).second;
        updater->update(rec_dat, gradient);
      }
    }   
  } 
  for(uint i=0; i<gradient_updaters->size(); i++){
    ModelGradientUpdater* updater = gradient_updaters->at(i);
    updater->end_of_updating_cycle(rec_dat);
  }
  return cost;
}
