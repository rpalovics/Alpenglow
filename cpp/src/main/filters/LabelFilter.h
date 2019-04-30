#ifndef LABEL_FILTER_H
#define LABEL_FILTER_H

#include "ModelFilter.h"
#include "../utils/LabelContainer.h"
#include "../models/ModelUpdater.h"
#include <gtest/gtest_prod.h>

struct LabelFilterParameters {
  string label_file_name;
};

class LabelFilter : public ModelFilter, public Updater {
  public:
    LabelFilter(LabelFilterParameters* params){
      label_file_name_ = params->label_file_name;
      ifstream file(label_file_name_);
      label_container_.read_from_file(file);
      temp_ = NULL;
    }
    void update(RecDat* rec_dat) override;
    void run(RecDat* rec_dat) override { update(rec_dat); }
    vector<pair<int,double>>* get_personalized_items(int user) override;
    virtual bool active(RecDat*) override;
    bool self_test(){
      bool OK = ModelFilter::self_test();
      return OK;
    }
  private:
    
    LabelContainer label_container_;
    string label_file_name_;

    //state
    vector<LABEL> last_labels_; //indexed by the user
    RecDat* temp_;

    //xxx
    vector<pair<int,double>> item_filter_;
    FRIEND_TEST(TestLabelFilter, test);

};


#endif /* LABEL_FILTER_H */
