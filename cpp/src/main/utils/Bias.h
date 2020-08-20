#ifndef BIAS_H
#define BIAS_H

//SIP_AUTOCONVERT

#include <gtest/gtest_prod.h>
#include <vector>

/**
  Implements simple bias.  For sample usage, see the source code of
  :py:class:`alpenglow.cpp.models.FactorModel`.

  Note that this class does not check the parameters.  If you call the functions
  with inappropriate parameters, you will get hard-to-debug memory errors.
  Examples: negative entity or calling update for an entity that hasn't been
  initialized.
*/
class Bias {
  public:
    double get(int entity){
      if ( entity < (int) biases_.size() ) return biases_[entity];
      else return 0;
    }
    /**
      get(int entity)

      Returns the bias value for the entity (user or item).  If the entity is
      not initialized, returns 0.

      Parameters
      ----------
      entity: int
         The user or item.
      Returns
      -------
      double
         The bias value.
    */
    void update(int entity, double val){
      biases_[entity]+=val;
    }
    /**
      update(int entity, double val)

      Sets the bias value for the entity (user or item).  For each item, call
      `init(entity)` before the first call of `update`.

      Parameters
      ----------
      entity: int
         The user or item.
    */
    void init(int entity){
      if (entity >= (int) biases_.size() ) biases_.resize(entity+1,0);
    }
    /**
      init(int entity)

      Initializes the bias value for the entity (user or item).  For each item, call
      `init(entity)` before the first call of `update`.

      Parameters
      ----------
      entity: int
         The user or item.
    */
    void clear(){
      biases_.clear();
    }
    /**
      Removes the bias value for all entities.
    */
  private:
    std::vector<double> biases_;
  FRIEND_TEST(TestBias, test);
};

#endif /* BIAS_H */
