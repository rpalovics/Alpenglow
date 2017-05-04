#ifndef OFFLINE_EVALUATOR
#define OFFLINE_EVALUATOR

using namespace std;
class OfflineEvaluator{
  public:
    OfflineEvaluator(){}
    virtual ~OfflineEvaluator(){}
    virtual void evaluate()=0;
    bool self_test(){
      bool OK = true;
      return OK;
    }
};


#endif
