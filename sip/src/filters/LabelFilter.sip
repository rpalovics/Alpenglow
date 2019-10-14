
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
: public WhiteListFilter
, public Updater
{
  %TypeHeaderCode
  #include "filters/LabelFilter.h"
  %End
  %Docstring
  White list filter class that allows items having the same label (e.g. artist)
  as the item that was previously interacted by the user.  Requires updates.
  %End
  public:
    LabelFilter(LabelFilterParameters* params);
    bool active(RecDat*);
    std::vector<int> get_white_list(int user);
    void update(RecDat* rec_dat);
    bool self_test();
};

