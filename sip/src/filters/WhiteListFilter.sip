
class WhiteListFilter  /Abstract/{
  %TypeHeaderCode
  #include "filters/WhiteListFilter.h"
  %End
  %Docstring
  Filter interface for classes that implement white list type filtering.
  %End
public:
  virtual bool active(RecDat* rec_dat) = 0;
  %Docstring
    Parameters
    ----------
    rec_dat : RecDat*
       The sample containing the user and the item.
    Returns
    -------
    bool
       Whether the item is available for the user.
  %End
  virtual std::vector<int> get_white_list(int user) = 0;
  %Docstring
    Parameters
    ----------
    user : int
       The whitelist will be computed for the given user.
    Returns
    -------
    std::vector<int,double>
       The list of allowed items for the given user. The second element of the pair is an upper bound for the score of the item for the given user (or the score itself).
  %End
  virtual ~WhiteListFilter();
};

