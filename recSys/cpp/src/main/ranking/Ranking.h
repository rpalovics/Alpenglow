#ifndef RANKING
#define RANKING


#include "../utils/SpMatrix.h"
#include "../utils/PopContainers.h"
#include "../filters/ModelFilter.h"
#include "../online_recommender/OnlineRecommender.h"
#include "../recommender_data/RecommenderData.h"
#include <numeric> 
#include "../utils/Random.h"
struct RankComputerParameters{
  int top_k;
  int random_seed;
  RankComputerParameters(){
    top_k = -1;
    random_seed = -1;
  }
};
class RankComputer{
  public:
    RankComputer(RankComputerParameters* parameters){
      top_k_=(parameters->top_k==-1?parameters->top_k:parameters->top_k);
      random_.set(parameters->random_seed);
      recommender_=NULL;
      train_matrix_=NULL;
      popularity_sorted_container_=NULL;
      model_filter_=NULL;
      itemlist_=NULL;
    }
    //RankComputer(){}
    //virtual ~RankComputer(){}
    void set_recommender(OnlineRecommender* recommender){ recommender_ = recommender; } 
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_ = train_matrix; }
    void set_top_pop_container(TopPopContainer* popularity_sorted_container){ popularity_sorted_container_ = popularity_sorted_container; }
    void set_model_filter(ModelFilter* model_filter){ model_filter_ = model_filter; }
    bool self_test(){
      bool ok=true;
      if(top_k_<0){ok=false;cerr<<"RankComputer::top_k is not set properly." << endl;}
      if(recommender_==NULL){ok=false;cerr<<"RankComputer::recommender_ is not set." << endl;}
      if(popularity_sorted_container_==NULL and model_filter_==NULL){ok=false;cerr<<"RankComputer::popularity_sorted_container_ is not set, RankComputer::model_filter is not set." << endl;}
      if(train_matrix_==NULL){ok=false;cerr<<"RankComputer::train_matrix_ is not set." << endl;}
      return ok;
    }
    int get_rank(RecDat*);
  protected:
    void itemlist_init(RecDat* rec_dat);
    bool itemlist_next(RecDat* rec_dat);
    int itemlist_index_;
    int itemlist_max_index_;
    int itemlist_positive_item_;
    vector<pair<int,double>>* itemlist_;
    OnlineRecommender* recommender_;
    SpMatrix* train_matrix_;
    TopPopContainer* popularity_sorted_container_;
    ModelFilter* model_filter_;
    Random random_;
    int top_k_;
};


//struct TopListCreatorParameters{
//  OnlineRecommender * recommender;
//  vector <int> * items;
//  SpMatrix * train_matrix;
//  int top_k;
//};
//
//class TopListCreator{
//  public:
//   TopListCreator(TopListCreatorParameters * parameters){
//      set_parameters(parameters);
//    };
//    ~TopListCreator(){};
//    void set_parameters(TopListCreatorParameters * parameters);
//    RecMap *  getTopRecommendation(RecDat * recDat);
//  protected:
//    void sortTopK();
//    void normTopK();
//    void createTopKMap();
//    void recommend(RecDat * recDat);
//    OnlineRecommender * recommender;
//    vector <int> * items;
//    SpMatrix * train_matrix;
//    RecVector  rec;
//    RecMap recMap;
//    int top_k;
//};

#endif
