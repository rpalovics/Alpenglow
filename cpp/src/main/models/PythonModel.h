#ifndef PYTHON_MODEL_H
#define PYTHON_MODEL_H

#include "Model.h"
#include "TopListRecommender.h"
#include "RankingScoreIterator.h"

class PythonModel : public Model {
  public:
    virtual void write(ostream& file) override {};
    virtual void read(istream& file) override {};
    virtual void clear() override {};
};


class PythonToplistModel : public PythonModel, public TopListRecommender {};

class PythonRankingIteratorModel;

class PythonModelRankingScoreIterator : public RankingScoreIterator{
public:
  PythonModelRankingScoreIterator(PythonRankingIteratorModel* model, int id, int user){
    model_ = model;
    user_ = user;
    id_ = id;
  }
  ~PythonModelRankingScoreIterator(){};

  bool has_next(double upper_bound) override;
  using RankingScoreIterator::has_next;
  pair<int, double> get_next() override;
  int unique_items_num() override {
    return 0;
  };
protected:
  PythonRankingIteratorModel* model_;
  int user_;
  int id_;
};


class PythonRankingIteratorModel : public PythonModel, public RankingScoreIteratorProvider
 {
public:
  PythonRankingIteratorModel() : iterator_(this, -1, -1), id_(0) {}
  virtual RankingScoreIterator* get_ranking_score_iterator(int user){
    id_++;
    iterator_ = PythonModelRankingScoreIterator(this, id_, user);
    return &iterator_;
  }
  virtual bool iterator_has_next_(int id, int user, double upper_bound) = 0;
  virtual pair<int, double> iterator_get_next_(int id, int user) = 0;
protected:
  int id_;
  PythonModelRankingScoreIterator iterator_;
};

#endif /* PYTHON_MODEL_H */
