struct RankComputerParameters{
  %TypeHeaderCode
  #include "ranking/Ranking.h"
  %End
  int topK;
  int random_seed;
};
class RankComputer{
  %TypeHeaderCode
  #include "ranking/Ranking.h"
  %End
  public:
    RankComputer(RankComputerParameters * parameters);
    ~RankComputer();
    void set_recommender(OnlineRecommender* recommender /Transfer/);
    void set_train_matrix(SpMatrix* train_matrix /Transfer/);
    void set_top_pop_container(TopPopContainer* popularity_sorted_container /Transfer/);
    void set_model_filter(ModelFilter* model_filter /Transfer/);
    bool selfTest();
    int get_rank(RecDat*);
};


// struct TopListCreatorParameters{
//   %TypeHeaderCode
//   #include "ranking/Ranking.h"
//   %End
//   OnlineRecommender * recommender;
//   std::vector<int> * items;
//   SpMatrix * trainMatrix;
//   int topK;
// };

// class TopListCreator{
//   %TypeHeaderCode
//   #include "ranking/Ranking.h"
//   %End
//   public:
//    TopListCreator(TopListCreatorParameters * parameters);
//     ~TopListCreator();
//     void setParameters(TopListCreatorParameters * parameters);
//     // std::map<int,double > *  getTopRecommendation(RecDat * recDat);
// };
