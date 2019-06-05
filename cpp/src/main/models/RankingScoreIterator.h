#ifndef RANKING_SCORE_ITERATOR_H
#define RANKING_SCORE_ITERATOR_H

#include <utility>
#include <limits>
#include <cmath>

/**
   Iterator-like interface for the purpose of making toplist and rank computation faster.

   Example usage is in :py:class:`alpenglow.cpp.RankComputer`.
*/
class RankingScoreIterator {
public:
  /**
     Returns false if the iterator can guarantee that there is no item for the user having a score higher than or equal to lower_bound.

     @param lower_bound the scores will be compared to the bound
     @return false if guaranteed that there is no item for the user that has a higher or equal score 
  */
  virtual bool has_next(double lower_bound)=0;
  /**
     Returns false if the iterator can guarantee that there is no item for the user having a score higher than or equal to lower_bound, depending on the strict parameter.

     @param lower_bound the scores will be compared to the bound
     @param strict true means excluding equal scored items
     @return false if guaranteed that there is no item for the user that has a higher (or equal) score
  */
  virtual bool has_next(double lower_bound, bool strict){
    if(!strict){
      return has_next(lower_bound);
    } else {
      return has_next(std::nextafter(lower_bound, std::numeric_limits<double>::max()));
    }
  }
  /**
     Returns true if at least one item rests, regardless of the scores

     @return true if there is at least one more item
  */
  virtual bool has_next(){
    return has_next(std::numeric_limits<double>::lowest());
  };
  /**
     The next item-score pair.

     @return The next item-score pair.
  */
  virtual std::pair<int, double> get_next()=0;
  /**
     Number of items that are known by the model 

     @return number of items
  */
  virtual int unique_items_num()=0;
};

/**
   Interface to be implemented by a model if it provides RankingScoreIterator.
   
   The toplist or rank computer gets the iterator through this interface.
*/
class RankingScoreIteratorProvider {
public:
  /**
     Returns the actual RankingScoreIterator for a given user. The RankingScoreIterator is valid for that user until the first change of the model or the change in the time.

     @param user The RankingScoreIterator is valid for the given user.
     @return the iterator object, if NULL, the model can't provide iterator for that user at that time.
  */
  virtual RankingScoreIterator* get_ranking_score_iterator(int user)=0;
};

#endif /* RANKING_SCORE_ITERATOR_H */
