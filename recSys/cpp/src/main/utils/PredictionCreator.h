#ifndef PREDICTION_CREATOR
#define PREDICTION_CREATOR
//TODO rename to toplistcreator/globaltoplistcreator/usertoplistcreator
#include <climits>
#include <iostream>
#include <gtest/gtest_prod.h>
#include "../utils/MinHeap.h"
#include "../models/Model.h"
#include "../filters/ModelFilter.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"

using namespace std;

struct PredictionCreatorParameters{
  int top_k;
  int lookback;
};
class PredictionCreator : public INeedExperimentEnvironment{
 public:
   PredictionCreator(PredictionCreatorParameters* params){
     top_k_ = params->top_k;
     lookback_ = params->lookback;
     train_matrix_ = NULL;
     model_ = NULL;
     filter_ = NULL;
   }
   virtual ~PredictionCreator(){}
   virtual vector<RecDat>* run(RecDat* rec_dat)=0; 
   void set_model(Model* model){model_=model;}
   void set_filter(ModelFilter* filter){filter_=filter;} //TODO alternative: items or popsortedcont
   void set_train_matrix(SpMatrix *train_matrix){train_matrix_ = train_matrix; }
   void init(){
     if(train_matrix_ == NULL) train_matrix_=experiment_environment_->get_train_matrix();
     if(top_k_ == -1) top_k_=experiment_environment_->get_top_k();
     if(lookback_ == -1) lookback_=experiment_environment_->is_lookback();
   }
   bool self_test(){
     bool OK = true;
     if(model_==NULL){
       OK = false;
       cerr << "Not set: Model of PredictionCreator." << endl;
     }
     if(filter_==NULL){
       OK = false;
       cerr << "Not set: Filter of PredictionCreator." << endl;
     }
     if(top_k_<=0){
       OK = false;
       cerr << "Invalid value top_k_==" << top_k_ << " is set in PredictionCreator." << endl;
     }
     if(lookback_==1 and train_matrix_==NULL){
       OK = false;
       cerr << "Not set: train_matrix of PredictionCreator." << endl;
     }
     return OK;
   }
 protected:
   ExperimentEnvironment* experiment_environment_;
   vector<RecDat> top_predictions_;
   Model* model_;
   ModelFilter* filter_;
   SpMatrix* train_matrix_;
   int top_k_; //TODO const
   int lookback_;
};

struct PredictionCreatorGlobalParameters : public PredictionCreatorParameters{
  int initial_threshold; //TODO initial_threshold
};

class PredictionCreatorGlobal: public PredictionCreator{
  public:
    PredictionCreatorGlobal(PredictionCreatorGlobalParameters* params):PredictionCreator(params){
      min_heap_ = new MinHeap(params->top_k); //TODO use utils/Toplist
      initial_threshold_ = (uint)params->initial_threshold;
    };
    virtual ~PredictionCreatorGlobal(){ delete min_heap_; }
    vector<RecDat>* run(RecDat* rec_dat);
    bool self_test(){
      bool OK = PredictionCreator::self_test(); 
      if(initial_threshold_ < 0){
        OK = false;
        cerr << "Invalid value initial_threshold=" << initial_threshold_ << " is set in PredictionCreatorGlobal." << endl;
      }
      return OK;
    }
  
  private:
    MinHeap* min_heap_;
    uint initial_threshold_;
    //void process_row(vector<pair<int,double> >* sorted_entities_a,uint start_index_a,int index_b,RecDat* rec_dat,uint threshold);
    //void process_column(vector<pair<int,double> >* sorted_entities_a,uint start_index_a,int index_b,RecDat* rec_dat,uint threshold);


    void process_line(vector<pair<int,double> >* sorted_as,uint begin_a_index, uint end_a_index, int* _rec_dat_a, RecDat* _rec_dat);
    void process_rectangle(vector<pair<int,double> >* sorted_users, vector<pair<int,double> >* sorted_items, uint begin_user_index, uint begin_item_index, uint end_user_index, uint end_item_index, RecDat* _rec_dat);
    FRIEND_TEST(TestPredictionCreatorGlobal, global);
    FRIEND_TEST(TestPredictionCreatorGlobal, global2);
    FRIEND_TEST(TestPredictionCreatorGlobal, process_line);
    FRIEND_TEST(TestPredictionCreatorGlobal, process_line2);
    FRIEND_TEST(TestPredictionCreatorGlobal, process_square);
    FRIEND_TEST(TestPredictionCreatorGlobal, lookback);
};

struct PredictionCreatorPersonalizedParameters : public PredictionCreatorParameters{
};
class PredictionCreatorPersonalized: public PredictionCreator{
  public:
    PredictionCreatorPersonalized(PredictionCreatorParameters * params):PredictionCreator(params){
      min_heap_ = new MinHeap(params->top_k); //TODO use utils/Toplist
    };
    vector<RecDat>* run(RecDat* rec_dat);
    bool self_test(){ return PredictionCreator::self_test(); }
  private:
    MinHeap* min_heap_;
};

#endif

