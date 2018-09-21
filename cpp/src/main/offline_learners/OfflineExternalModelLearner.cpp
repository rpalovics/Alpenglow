#include "OfflineExternalModelLearner.h"

void OfflineExternalModelLearner::fit(RecommenderData* recommender_data){
  if(mode_=="write"){
    vector<RecDat> *rec_data = recommender_data->get_rec_data();
    model_->write_requested_users(out_name_base_ + "_" + to_string(call_number_-1) + "_test.dat");
    ofstream file(out_name_base_ + "_" + to_string(call_number_) + "_train.dat");

    file << "time,user,item,id,score,category" << endl;
    for(RecDat &r : *rec_data){
      file 
        << r.time << ","
        << r.user << ","
        << r.item << ","
        << r.id << ","
        << r.score << ","
        << r.category
        << endl;
    }

    file.close();
  } else if(mode_=="read"){
    model_->read_predictions(in_name_base_ + "_" + to_string(call_number_) + "_predictions.dat");
  } else {
     throw exception();
  }

  call_number_++;
}
