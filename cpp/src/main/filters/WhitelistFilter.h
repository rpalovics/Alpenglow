#ifndef WHITE_LIST_FILTER
#define WHITE_LIST_FILTER

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

#endif //WHITE_LIST_FILTER_H
