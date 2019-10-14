#ifndef LABEL_FILTER_H
#define LABEL_FILTER_H

#include "WhiteListFilter.h"
#include "../utils/LabelContainer.h"
#include "../models/ModelUpdater.h"
#include <gtest/gtest_prod.h>

//SIP_AUTOCONVERT

struct LabelFilterParameters {
  string label_file_name;
};

class LabelFilter
: public WhiteListFilter
, public Updater
{
/**
  White list filter class that allows items having the same label (e.g. artist)
  as the item that was previously interacted by the user.  Requires updates.

  See python/test_alpenglow/utils/test_label_filter.py for a usage example.
*/
  public:
    LabelFilter(LabelFilterParameters* params){
      label_file_name_ = params->label_file_name;
      ifstream file(label_file_name_);
      label_container_.read_from_file(file);
    }
    bool active(RecDat*) override;
    vector<int> get_white_list(int user) override;
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = true;
      return ok;
    }
  private:
    void resize_last_labels(int user);
    
    LabelContainer label_container_;
    string label_file_name_;

    //state
    vector<LABEL> last_labels_; //indexed by the user

    //xxx
    FRIEND_TEST(TestLabelFilter, test);

};


#endif /* LABEL_FILTER_H */
