#include "OnlinePredictor.h"

void OnlinePredictor::set_parameters(OnlinePredictorParameters* params){
  min_time_ = params->min_time;
  time_frame_ = params->time_frame;
  ofs_.open(params->file_name.c_str());
  past_time_frame_ = 0;
  actual_time_frame_ = 0;
  prediction_creator_=NULL;
}

void OnlinePredictor::run(RecDat* rec_dat){
  if(do_predict(rec_dat)){
    cerr << "OnlinePredictor::predict computes prediction." << endl;
    vector<RecDat>* top_predictions = prediction_creator_->run(rec_dat);
    for(uint ii=0; ii<top_predictions->size(); ii++){
      ofs_ << actual_time_frame_ << " " << top_predictions->at(ii).user << " " << top_predictions->at(ii).item << " " << top_predictions->at(ii).score << endl; 
    } 
    cerr << "OnlinePredictor::predict done" << endl;
  }
}

bool OnlinePredictor::do_predict(RecDat* rec_dat){
  double actual_time = rec_dat->time;
  actual_time_frame_ = (int)(actual_time - min_time_)/(time_frame_);
  if(actual_time > min_time_ && actual_time_frame_ !=past_time_frame_){
    past_time_frame_ = actual_time_frame_;
    return true;
  }
  else return false;
}
