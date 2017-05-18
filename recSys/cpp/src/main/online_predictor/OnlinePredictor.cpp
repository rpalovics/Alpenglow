#include "OnlinePredictor.h"

void OnlinePredictor::set_parameters(OnlinePredictorParameters * params){
  min_time = params->min_time;
  time_frame = params->time_frame;
  ofs.open(params->file_name.c_str());
  past_time_frame = 0;
  actual_time_frame = 0;
  prediction_creator=NULL;
}

void OnlinePredictor::run(RecDat * rec_dat){
  if(do_predict(rec_dat)){
    cerr << "OnlinePredictor::predict computes prediction." << endl;
    vector<RecDat>* top_predictions = prediction_creator->run(rec_dat);
    for(uint ii=0; ii<top_predictions->size(); ii++){
      ofs << actual_time_frame << " " << top_predictions->at(ii).user << " " << top_predictions->at(ii).item << " " << top_predictions->at(ii).score << endl; 
    } 
    cerr << "OnlinePredictor::predict done" << endl;
  }
}

bool OnlinePredictor::do_predict(RecDat * rec_dat){
  double actual_time = rec_dat->time;
  actual_time_frame = (int)(actual_time - min_time)/(time_frame);
  if(actual_time > min_time && actual_time_frame !=past_time_frame){
    past_time_frame = actual_time_frame;
    return true;
  }
  else return false;
}
