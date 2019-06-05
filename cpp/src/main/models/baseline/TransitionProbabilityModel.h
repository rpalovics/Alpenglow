#ifndef TRANSITION_PROBABILITY_MODEL_RANKING_SCORE_ITERATOR_H
#define TRANSITION_PROBABILITY_MODEL_RANKING_SCORE_ITERATOR_H

//SIP_AUTOCONVERT

#include <exception>
#include "../Model.h"
#include "../../utils/SpMatrix.h"
#include "../RankingScoreIterator.h"
#include <gtest/gtest_prod.h>

class TransitionProbabilityModelRankingScoreIterator : public RankingScoreIterator{
public:
  bool has_next(double bound) override;
  pair<int, double> get_next() override;
  void set_up(map<int,int>* actual_frequency_map);
  void reinit(){counter_=0;}
  int unique_items_num() override { throw runtime_error("Not implemented. Should not be called by RankComputer as all scores are nonnegatives."); }
private:
  void clear(){counter_=0;current_scores_.clear();}
  vector<pair<int,double>> current_scores_;
  int counter_;
};

class TransitionProbabilityModel
  : public Model
  , virtual public RankingScoreIteratorProvider
{
  public:
    double prediction(RecDat* rec_dat) override;
    RankingScoreIterator* get_ranking_score_iterator(int user) override;
    void clear() override { transition_frequencies_.clear(); lastly_visited_entities_.clear();}
    bool self_test(){
      bool ok = Model::self_test();
      return ok;
    }
  private:
    map<int,int>* get_frequency_map(int user);
    vector<map<int,int>> transition_frequencies_;
    vector<int> lastly_visited_entities_;
    TransitionProbabilityModelRankingScoreIterator ranking_score_iterator_;
    int active_user_=-1;
    //int get_frequency_sum(int user);
    //vector<int> frequency_sums_; //TransitionModelLogger szamara
    friend class TransitionModelEndLogger;
    friend class TransitionModelLogger;
    friend class TransitionProbabilityModelUpdater;
    FRIEND_TEST(TestTransitionEndLogger, test);
};


#endif /* TRANSITION_PROBABILITY_MODEL_RANKING_SCORE_ITERATOR_H */
