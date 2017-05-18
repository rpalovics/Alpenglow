struct RankComputerParameters{
  %TypeHeaderCode
  #include "ranking/Ranking.h"
  %End
  int top_k;
  int random_seed;
};
class RankComputer{
  %TypeHeaderCode
  #include "ranking/Ranking.h"
  %End
  public:
    RankComputer(RankComputerParameters * parameters);
    ~RankComputer();
    void set_recommender(OnlineRecommender* recommender);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_top_pop_container(TopPopContainer* popularity_sorted_container);
    void set_model_filter(ModelFilter* model_filter);
    bool self_test();
    int get_rank(RecDat*);
};


// struct TopListCreatorParameters{
//   %TypeHeaderCode
//   #include "ranking/Ranking.h"
//   %End
//   OnlineRecommender * recommender;
//   std::vector<int> * items;
//   SpMatrix * train_matrix;
//   int top_k;
// };

// class TopListCreator{
//   %TypeHeaderCode
//   #include "ranking/Ranking.h"
//   %End
//   public:
//    TopListCreator(TopListCreatorParameters * parameters);
//     ~TopListCreator();
//     void set_parameters(TopListCreatorParameters * parameters);
//     // std::map<int,double > *  getTopRecommendation(RecDat * recDat);
// };
