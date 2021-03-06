
class WhitelistFilter  /Abstract/{
  %TypeHeaderCode
  #include "filters/WhitelistFilter.h"
  %End
  %Docstring
  Filter interface for classes that implement white list type filtering.
  %End
public:
  virtual bool active(RecDat* rec_dat) = 0;
  %Docstring
    active(RecDat* rec_dat)

    Returns whether the item is active for the user.

    Parameters
    ----------
    rec_dat : RecDat*
       The sample containing the user and the item.
    Returns
    -------
    bool
       Whether the item is available for the user.
  %End
  virtual std::vector<int> get_whitelist(int user) = 0;
  %Docstring
    get_whitelist(int user)

    Returns the set of active items for the user.

    Parameters
    ----------
    user : int
       The whitelist will be computed for the given user.
    Returns
    -------
    std::vector<int,double>
       The list of allowed items for the given user. The second element of the pair is an upper bound for the score of the item for the given user (or the score itself).
  %End
  virtual ~WhitelistFilter();
};

