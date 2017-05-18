#ifndef UNIFORM_NEGATIVE_SAMPLE_GENERATOR
#define UNIFORM_NEGATIVE_SAMPLE_GENERATOR

#include "NegativeSampleGenerator.h"

struct UniformNegativeSampleGeneratorParameters{
    double negative_rate; 
    bool initialize_all;
    int max_item;
    bool filter_repeats;
    UniformNegativeSampleGeneratorParameters(){
      negative_rate=-1;
      initialize_all=false;
      max_item=-1;
      filter_repeats=false;
    }
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator{
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* parameters):
      negative_rate_(parameters->negative_rate),
      filter_repeats_(parameters->filter_repeats),
      initialize_all_(parameters->initialize_all),
      max_item_(parameters->max_item)
    {
      items_ = NULL;
      train_matrix_ = NULL;
      if(initialize_all_){
        items_=new vector<int>(max_item_+1);
        for(int i=0;i<items_->size();i++){items_->at(i)=i;}
      }
    };
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_=train_matrix; }
    void set_items(vector<int>* items){ if(!initialize_all_) items_=items; }
    vector<int>* generate(RecDat* rec_dat);
    bool self_test(){
      bool ok = NegativeSampleGenerator::self_test();
      if(negative_rate_ < 0){
        ok=false;
        cerr << "UniformNegativeSampleGeneratorParameters::negative_rate is negative." << endl;
      }
      if(initialize_all_ && max_item_<0){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::max_item is negative but initialize_all is set." << endl;
      }
      if(items_==NULL){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::items is not set." << endl;
      }
      if(train_matrix_ == NULL){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::train_matrix is not set." << endl;
      }
      return ok;
    }
  protected:
    vector<int>* items_;
    SpMatrix* train_matrix_; 
    const double negative_rate_;
    const bool filter_repeats_;
    const bool initialize_all_;
    const int max_item_;
};

#endif
