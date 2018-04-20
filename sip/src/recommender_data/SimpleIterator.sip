/*
 * SimpleIterator.h
 * Order of rec_dats is the same as order of them in the scrobble file (no shuffling).
 */
class SimpleIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/SimpleIterator.h"
  %End
  public:
    SimpleIterator(RecommenderData *rd);
    SimpleIterator();
    RecDat* next() ;
    RecDat* get(int index) const ;
    RecDat* get_actual() ;
    RecDat* get_future(int index) const ;
    double get_following_timestamp() const ;
};
