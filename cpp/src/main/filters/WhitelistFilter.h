#ifndef WHITELIST_FILTER_H
#define WHITELIST_FILTER_H

#include <vector>
#include "../recommender_data/RecommenderData.h"

//SIP_AUTOCONVERT

class WhitelistFilter { //SIP_ABSTRACT
/**
  Filter interface for classes that implement white list type filtering.
*/
public:
  virtual bool active(RecDat* rec_dat) = 0;
  /**
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
  */
  virtual vector<int> get_whitelist(int user) = 0;
  /**
    get_whitelist(int user)

    Returns the set of active items for the user.

    Parameters
    ----------
    user : int
       The whitelist will be computed for the given user.
    Returns
    -------
    vector<int,double>
       The list of allowed items for the given user. The second element of the pair is an upper bound for the score of the item for the given user (or the score itself).
  */
  virtual ~WhitelistFilter(){}
};

#endif /* WHITELIST_FILTER_H */
