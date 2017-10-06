class OfflineLearner{
 %TypeHeaderCode
 #include "offline_learners/OfflineLearner.h"
 %End
 public:
  OfflineLearner();
  virtual void fit(RecommenderData*)=0;
  bool self_test();
};
