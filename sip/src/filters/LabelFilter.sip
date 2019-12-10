
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

  %End
  public:
    LabelFilter(LabelFilterParameters* params);
    bool active(RecDat*);
  %Docstring
    active(RecDat*)

    Implements :py:meth:`alpenglow.cpp.WhitelistFilter.active`.
  %End
    std::vector<int> get_whitelist(int user);
  %Docstring
    get_whitelist(int user)

    Implements :py:meth:`alpenglow.cpp.WhitelistFilter.get_whitelist`.
  %End
    void update(RecDat* rec_dat);
  %Docstring
    update(RecDat* rec_dat)

    Implements :py:meth:`alpenglow.cpp.Updater.update`.
  %End
    bool self_test();
};

