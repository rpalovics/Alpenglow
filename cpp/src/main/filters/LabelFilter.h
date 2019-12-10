#ifndef LABEL_FILTER_H
#define LABEL_FILTER_H

#include "WhitelistFilter.h"
#include "../utils/LabelContainer.h"
#include "../models/ModelUpdater.h"
#include <gtest/gtest_prod.h>

//SIP_AUTOCONVERT

struct LabelFilterParameters {
  string label_file_name;
};

class LabelFilter
: public WhitelistFilter
, public Updater
{
/**
  White list filter class that allows items having the same label (e.g. artist)
  as the item that was previously interacted by the user.  Requires updates
  (i.e., add the object to the online experiment as an updater).

  Sample usage:
  
  .. code-block:: python
  
      import alpenglow as ag
  
      model = ag.PopularityModel()
      updater = ag.PopularityModelUpdater()
      updater.set_model(model)
      label_filter = ag.LabelFilter(
          label_file_name = "/path/to/file/"
      )
      adapter = ag.WhitelistFilter2ModelAdapter()
      adapter.set_model(model)
      adapter.set_whitelist_filter(label_filter)

*/
  public:
    LabelFilter(LabelFilterParameters* params){
      label_file_name_ = params->label_file_name;
      ifstream file(label_file_name_);
      label_container_.read_from_file(file);
    }
    bool active(RecDat*) override;
    /**
    active(RecDat*)

    Implements :py:meth:`alpenglow.cpp.WhitelistFilter.active`.
    */
    vector<int> get_whitelist(int user) override;
    /**
    get_whitelist(int user)

    Implements :py:meth:`alpenglow.cpp.WhitelistFilter.get_whitelist`.
    */
    void update(RecDat* rec_dat) override;
    /**
    update(RecDat* rec_dat)

    Implements :py:meth:`alpenglow.cpp.Updater.update`.
    */
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
