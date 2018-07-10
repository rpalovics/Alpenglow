#ifndef TRANSITION_PROBABILITY_MODEL_UPDATER
#define TRANSITION_PROBABILITY_MODEL_UPDATER

//SIP_AUTOCONVERT

#include "../ModelUpdater.h"
#include "TransitionProbabilityModel.h"
#include "../../utils/LabelContainer.h"

struct TransitionProbabilityModelUpdaterParameters{
  bool filter_freq_updates = false;
  string mode = ""; //old codes: -1: normal; 0: inverted; 1: symmetric
  bool label_transition_mode = false;
  string label_file_name = "";
};

class TransitionProbabilityModelUpdater: public Updater{
  public:
    TransitionProbabilityModelUpdater(TransitionProbabilityModelUpdaterParameters* params):
      label_transition_mode_(params->label_transition_mode),
      filter_freq_updates_(params->filter_freq_updates),
      mode_((params->mode=="")?("normal"):(params->mode))
    {
      if(label_transition_mode_){
        label_file_name_ = params->label_file_name;
        ifstream file(label_file_name_);
        label_container_.read_from_file(file);
      }
    }
    void update(RecDat* rec_dat) override;
    void set_model(TransitionProbabilityModel* model){
      model_ = model;
    }
    bool self_test(){
      bool ok = Updater::self_test();
      if(model_==NULL){ ok=false; cerr << "TransitionProbabilityModelUpdater::model_ is not set." << endl; }
      if(mode_!="symmetric" and mode_!="normal" and mode_!="inverted"){ ok=false; cerr << "TransitionProbabilityModelUpdater::mode should be normal, symmetric or inverted." << endl; }
      return ok;
    }
  private:
    //params
    const bool filter_freq_updates_;
    const string mode_;
    const bool label_transition_mode_;
    string label_file_name_;
    LabelContainer label_container_;
    //state
    vector<int> last_items_; //used only in label transition, inverted and symmetric mode

    TransitionProbabilityModel* model_ = NULL;
    bool init_user(RecDat* rec_dat);
    void update_frequencies(RecDat* rec_dat);
    void increase_transition(int from, int to);
    inline int item2entity(int item);
    inline bool recording_last_item_is_necessary() const;
};

#endif
