#ifndef PREDICTION_CREATOR_H
#define PREDICTION_CREATOR_H
//TODO rename to toplistcreator/globaltoplistcreator/usertoplistcreator
#include <climits>
#include <iostream>
#include <gtest/gtest_prod.h>
#include "../utils/Toplist.h"
#include "../models/Model.h"
#include "../filters/ModelFilter.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"
#include "../models/TopListRecommender.h"

using namespace std;

struct ToplistCreatorParameters {
  int top_k = -1;
  int exclude_known = -1;
};
class ToplistCreator : public NeedsExperimentEnvironment, public Initializable {
 public:
   ToplistCreator(ToplistCreatorParameters* params){
     top_k_ = params->top_k;
     exclude_known_ = params->exclude_known;
   }
   virtual ~ToplistCreator(){}
   virtual vector<RecDat>* run(RecDat* rec_dat)=0; 
   void set_model(Model* model){model_=model;}
   void set_filter(ModelFilter* filter){filter_=filter;} //TODO alternative: items or popsortedcont
   void set_train_matrix(SpMatrix *train_matrix){train_matrix_ = train_matrix; }
   bool self_test(){
     bool OK = true;
     if(model_==NULL){
       OK = false;
       cerr << "Not set: Model of ToplistCreator." << endl;
     }
     if(filter_==NULL){
       OK = false;
       cerr << "Not set: Filter of ToplistCreator." << endl;
     }
     if(top_k_<=0){
       OK = false;
       cerr << "Invalid value top_k_==" << top_k_ << " is set in ToplistCreator." << endl;
     }
     if(exclude_known_==1 and train_matrix_==NULL){
       OK = false;
       cerr << "Not set: train_matrix of ToplistCreator." << endl;
     }
     return OK;
   }
 protected:
   bool autocalled_initialize() override {
     if (train_matrix_ == NULL) {
       if (experiment_environment_ == NULL) return false;
       train_matrix_=experiment_environment_->get_train_matrix();
     }
     if(top_k_ == -1) {
       if (experiment_environment_ == NULL) return false;
       top_k_=experiment_environment_->get_top_k();
     }
     if(exclude_known_ == -1) {
       if (experiment_environment_ == NULL) return false; 
       exclude_known_=experiment_environment_->get_exclude_known();
     }
     return true;
   }
   vector<RecDat> top_predictions_;
   Model* model_ = NULL;
   ModelFilter* filter_ = NULL;
   const SpMatrix* train_matrix_ = NULL;
   SpMatrix dummy_train_matrix_;
   int top_k_;
   int exclude_known_;
};

struct ToplistCreatorGlobalParameters : public ToplistCreatorParameters {
  int initial_threshold; //TODO initial_threshold
};

class ToplistCreatorGlobal: public ToplistCreator{
  public:
    ToplistCreatorGlobal(ToplistCreatorGlobalParameters* params):ToplistCreator(params){
      initial_threshold_ = (uint)params->initial_threshold;
      min_heap_.set_max_length(params->top_k);
    };
    virtual ~ToplistCreatorGlobal(){}
    vector<RecDat>* run(RecDat* rec_dat);
    bool self_test(){
      bool OK = ToplistCreator::self_test() && min_heap_.self_test(); 
      if(initial_threshold_ < 0){
        OK = false;
        cerr << "Invalid value initial_threshold=" << initial_threshold_ << " is set in ToplistCreatorGlobal." << endl;
      }
      return OK;
    }
  
  private:
    inline static bool compare_rec_dat(RecDat a, RecDat b){
      return a.score > b.score;
    }
    Toplist<RecDat,compare_rec_dat> min_heap_;
    uint initial_threshold_;
    //void process_row(vector<pair<int,double> >* sorted_entities_a,uint start_index_a,int index_b,RecDat* rec_dat,uint threshold);
    //void process_column(vector<pair<int,double> >* sorted_entities_a,uint start_index_a,int index_b,RecDat* rec_dat,uint threshold);


    void process_line(vector<pair<int,double> >* sorted_as,uint begin_a_index, uint end_a_index, int* _rec_dat_a, RecDat* _rec_dat);
    void process_rectangle(vector<pair<int,double> >* sorted_users, vector<pair<int,double> >* sorted_items, uint begin_user_index, uint begin_item_index, uint end_user_index, uint end_item_index, RecDat* _rec_dat);
    FRIEND_TEST(TestToplistCreatorGlobal, global);
    FRIEND_TEST(TestToplistCreatorGlobal, global2);
    FRIEND_TEST(TestToplistCreatorGlobal, process_line);
    FRIEND_TEST(TestToplistCreatorGlobal, process_line2);
    FRIEND_TEST(TestToplistCreatorGlobal, process_square);
    FRIEND_TEST(TestToplistCreatorGlobal, exclude_known);
};

struct ToplistCreatorPersonalizedParameters : public ToplistCreatorParameters {
};
class ToplistCreatorPersonalized: public ToplistCreator{
  public:
    ToplistCreatorPersonalized(ToplistCreatorParameters * params):ToplistCreator(params){
      min_heap_.set_max_length(top_k_);
    };
    vector<RecDat>* run(RecDat* rec_dat);
    bool self_test(){ return ToplistCreator::self_test() && min_heap_.self_test(); }
  protected:
    bool autocalled_initialize() override {
      if (!parent_is_initialized_){
        parent_is_initialized_ = ToplistCreator::autocalled_initialize();
        if (!parent_is_initialized_) return false;
      }
      TopListRecommender* ranking_model = dynamic_cast<TopListRecommender*>(model_);
      if(ranking_model){
        ranking_model_ = ranking_model;
      }
      min_heap_.set_max_length(top_k_);
      return true;
    }
  private:
    inline static bool compare_rec_dat(RecDat a, RecDat b){
      return a.score > b.score;
    }
    Toplist<RecDat,compare_rec_dat> min_heap_;
    TopListRecommender *ranking_model_ = NULL;
    vector<RecDat>* run_bruteforce(RecDat* rec_dat);
    vector<RecDat>* run_ranking_model(RecDat* rec_dat);
    bool parent_is_initialized_ = false;
};

#endif /* PREDICTION_CREATOR_H */
