
struct LabelFilterParameters {
  %TypeHeaderCode
  #include "filters/LabelFilter.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.LabelFilter`. See documentation there.
  %End
  std::string label_file_name;
};

class LabelFilter
: public WhitelistFilter
, public Updater
{
  %TypeHeaderCode
  #include "filters/LabelFilter.h"
  %End
  %Docstring
  White list filter class that allows items having the same label (e.g. artist)
  as the item that was previously interacted by the user.  Requires updates.

  See python/test_alpenglow/utils/test_label_filter.py for a usage example.
  %End
  public:
    LabelFilter(LabelFilterParameters* params);
    bool active(RecDat*);
    std::vector<int> get_whitelist(int user);
    void update(RecDat* rec_dat);
    bool self_test();
};

