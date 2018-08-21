#include <vector>
#include <gtest/gtest.h>
#include "../../main/ranking/Ranking.h"

class DummyModel : public Model {
  public:
    DummyModel(){
      scores.resize(20);
      scores[9]=0.8;
      scores[0]=0.6;
      scores[4]=0.6;
      scores[5]=0.6;
      scores[8]=0.6;
      scores[2]=0.5;
      scores[3]=0.4;
      scores[6]=0.4;
      scores[7]=0.4;
      scores[1]=0.3;
    }
    vector<double> scores;
    double prediction(RecDat* rec_dat) override {
      return scores[rec_dat->item];
    }
    void add(RecDat* recDat){
    }
};

class DummyToplistModel : public Model, public TopListRecommender {
  public:
    DummyToplistModel(){}
    double prediction(RecDat* rec_dat) override {
      return 0;
    }
    void add(RecDat* recDat){}
    vector<pair<int,double>> get_top_list(int user, int k, SpMatrix *exclude) override {
      if(user == 1){
        return {{1,0},{2,0},{3,0},{4,0},{5,0}};
      } else if(user == 2){
        return {{5,0},{4,0},{3,0},{2,0},{1,0}};
      } else if(user == 3){
        return {{7,0},{6,0},{1,0}};
      }
    }
};

namespace {

class TestRankComputer : public ::testing::Test  {
public:
  RankComputer * rankComputer;
  SpMatrix trainMatrix;
  DummyModel model;
  DummyToplistModel toplist_model;
  vector<int> items;
  vector<int> itemMap;
  TopPopContainer pop;
  vector<RecDat*> recDats;
  TestRankComputer() {
  }
  virtual ~TestRankComputer() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    RankComputerParameters parameters;
    parameters.top_k = 6;
    parameters.random_seed = 1231232;
    rankComputer = new RankComputer(&parameters);
    rankComputer->set_train_matrix(&trainMatrix);
    rankComputer->set_model(&model);
    rankComputer->set_top_pop_container(&pop);
    srand(34723892);
    learn(createRecDat(2,0,0.0));
    learn(createRecDat(2,1,0.0));
    learn(createRecDat(2,2,0.0));
    learn(createRecDat(2,3,0.0));
    learn(createRecDat(2,4,0.0));
    learn(createRecDat(2,5,0.0));
    learn(createRecDat(2,6,0.0));
    learn(createRecDat(2,7,0.0));
    learn(createRecDat(2,8,0.0));
    learn(createRecDat(2,9,0.0));
  }
  virtual void TearDown(){
    delete rankComputer;
  }
  RecDat* createRecDat(int user, int item, double score){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> score = score;
      recDats.push_back(recDat);
      return recDat;
  }
  void learn(RecDat* recDat){
    pop.increase(recDat->item);
    if ((int)itemMap.size()<=recDat->item) itemMap.resize(recDat->item+1);
    if (itemMap[recDat->item]==0) items.push_back(recDat->item);
    itemMap[recDat->item]=1;
    trainMatrix.update(recDat->user,recDat->item,recDat->score);
  }
};
// class TestTopListCreator : public ::testing::Test  {
// public:
//  TopListCreator * topListCreator;
//  SpMatrix trainMatrix;
//  OnlineRecommender * recommender;
//  DummyModel model;
//  vector<int> items;
//  vector<int> itemMap;
//  vector<RecDat*> recDats;
//  TestTopListCreator() {
//  }
//  virtual ~TestTopListCreator() {
//    // You can do clean-up work that doesn't throw exceptions here.
//  }
//  virtual void SetUp(){
//    recommender = new OnlineRecommender;
//    recommender->set_model(&model);
//    TopListCreatorParameters parameters;
//    parameters.trainMatrix = &trainMatrix;
//    parameters.recommender = recommender;
//    parameters.items = &items;
//    parameters.topK = 6;
//    topListCreator = new TopListCreator(&parameters);
//    srand(34723892);
//    learn(createRecDat(2,0,0.0));
//    learn(createRecDat(2,1,0.0));
//    learn(createRecDat(2,2,0.0));
//    learn(createRecDat(2,3,0.0));
//    learn(createRecDat(2,4,0.0));
//    learn(createRecDat(2,5,0.0));
//    learn(createRecDat(2,6,0.0));
//    learn(createRecDat(2,7,0.0));
//    learn(createRecDat(2,8,0.0));
//    learn(createRecDat(2,9,0.0));
//  }
//  virtual void TearDown(){
//    delete recommender;
//    delete topListCreator;
//  }
//  RecDat* createRecDat(int user, int item, double score){
//      RecDat* recDat = new RecDat;
//      recDat -> user = user;
//      recDat -> item = item;
//      recDat -> score = score;
//      recDats.push_back(recDat);
//      return recDat;
//  }
//  void learn(RecDat* recDat){
//    if ((int)itemMap.size()<=recDat->item) itemMap.resize(recDat->item+1);
//    if (itemMap[recDat->item]==0) items.push_back(recDat->item);
//    itemMap[recDat->item]=1;
//    trainMatrix.update(recDat->user,recDat->item,recDat->score);
//  }
// };

}
TEST_F(TestRankComputer,rank) {
  /*
      scores[9]=0.8;
      scores[0]=0.6;
      scores[4]=0.6;
      scores[5]=0.6;
      scores[8]=0.6;
      scores[2]=0.5;
      scores[3]=0.4;
      scores[6]=0.4;
      scores[7]=0.4;
      scores[1]=0.3;
      */
  EXPECT_EQ(5,rankComputer->get_rank(createRecDat(2,2,0.0)));
  EXPECT_EQ(0,rankComputer->get_rank(createRecDat(2,9,0.0)));
  //cerr << "9: " << rankComputer->get_rank(createRecDat(2,9,0.0)) << endl;
  //cerr << "0: " << rankComputer->get_rank(createRecDat(2,0,0.0)) << endl;
  //cerr << "4: " << rankComputer->get_rank(createRecDat(2,4,0.0)) << endl;
  //cerr << "5: " << rankComputer->get_rank(createRecDat(2,5,0.0)) << endl;
  //cerr << "8: " << rankComputer->get_rank(createRecDat(2,8,0.0)) << endl;
  //cerr << "2: " << rankComputer->get_rank(createRecDat(2,2,0.0)) << endl;
  //cerr << "3: " << rankComputer->get_rank(createRecDat(2,3,0.0)) << endl;
  //cerr << "6: " << rankComputer->get_rank(createRecDat(2,6,0.0)) << endl;
  //cerr << "7: " << rankComputer->get_rank(createRecDat(2,7,0.0)) << endl;
  //cerr << "1: " << rankComputer->get_rank(createRecDat(2,1,0.0)) << endl;
}

TEST_F(TestRankComputer,threshold){
  EXPECT_EQ(6,rankComputer->get_rank(createRecDat(2,1,0.0)));
}

TEST_F(TestRankComputer,random){
  int rank = rankComputer->get_rank(createRecDat(2,0,0.0));
  EXPECT_LE(1,rank);
  EXPECT_GE(4,rank);
  rank = rankComputer->get_rank(createRecDat(2,4,0.0));
  EXPECT_LE(1,rank);
  EXPECT_GE(4,rank);
  rank = rankComputer->get_rank(createRecDat(2,5,0.0));
  EXPECT_LE(1,rank);
  EXPECT_GE(4,rank);
  rank = rankComputer->get_rank(createRecDat(2,8,0.0));
  EXPECT_LE(1,rank);
  EXPECT_GE(4,rank);
  rank = rankComputer->get_rank(createRecDat(2,3,0.0));
  EXPECT_LE(6,rank);
  EXPECT_GE(8,rank);
  rank = rankComputer->get_rank(createRecDat(2,6,0.0));
  EXPECT_LE(6,rank);
  EXPECT_GE(8,rank);
  rank = rankComputer->get_rank(createRecDat(2,7,0.0));
  EXPECT_LE(6,rank);
  EXPECT_GE(8,rank);
}

TEST_F(TestRankComputer,testTopListRanks){
    RankComputerParameters parameters;
    parameters.top_k = 5;
    parameters.random_seed = 1231232;
    rankComputer = new RankComputer(&parameters);
    rankComputer->set_train_matrix(&trainMatrix);
    rankComputer->set_model(&toplist_model);
    rankComputer->set_top_pop_container(&pop);
    rankComputer->initialize();
    
    EXPECT_EQ(0,rankComputer->get_rank(createRecDat(1,1,0.0)));
    EXPECT_EQ(5,rankComputer->get_rank(createRecDat(1,10,0.0)));
    EXPECT_EQ(5,rankComputer->get_rank(createRecDat(3,2,0.0)));
    EXPECT_EQ(1,rankComputer->get_rank(createRecDat(3,6,0.0)));
    EXPECT_EQ(2,rankComputer->get_rank(createRecDat(2,3,0.0)));
}

//TEST_F(TestTopListCreator,test) {
//  /*
//      scores[9]=0.8;
//      scores[0]=0.6;
//      scores[4]=0.6;
//      scores[5]=0.6;
//      scores[8]=0.6;
//      scores[2]=0.5;
//      scores[3]=0.4;
//      scores[6]=0.4;
//      scores[7]=0.4;
//      scores[1]=0.3;
//      */
//  RecMap* toplist = topListCreator->getTopRecommendation(createRecDat(2,2,0.0));
//  EXPECT_EQ(6,toplist->size());
//  EXPECT_EQ(1,toplist->at(9));
//  EXPECT_GE(1/2.0,toplist->at(0));
//  EXPECT_GE(1/2.0,toplist->at(4));
//  EXPECT_GE(1/2.0,toplist->at(5));
//  EXPECT_GE(1/2.0,toplist->at(8));
//  EXPECT_LE(1/5.0,toplist->at(0));
//  EXPECT_LE(1/5.0,toplist->at(4));
//  EXPECT_LE(1/5.0,toplist->at(5));
//  EXPECT_LE(1/5.0,toplist->at(8));
//  EXPECT_EQ(1/6.0,toplist->at(2));
//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

