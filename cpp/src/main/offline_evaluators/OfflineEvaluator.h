#ifndef OFFLINE_EVALUATOR_H
#define OFFLINE_EVALUATOR_H

//SIP_AUTOCONVERT

using namespace std;
class OfflineEvaluator{ //SIP_ABSTRACT
  public:
    virtual ~OfflineEvaluator(){}
    virtual void evaluate()=0;
    bool self_test(){
      bool OK = true;
      return OK;
    }
};


#endif /* OFFLINE_EVALUATOR_H */
