#include <vector>
#include <gtest/gtest.h>
#include "../../main/utils/RankComputer.h"

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
    vector<pair<int,double>> get_top_list(int user, int k, const map<int,double> *exclude) override {
      if(user == 1){
        return {{1,0},{2,0},{3,0},{4,0},{5,0}};
      } else if(user == 2){
        return {{5,0},{4,0},{3,0},{2,0},{1,0}};
      } else if(user == 3){
        return {{7,0},{6,0},{1,0}};
      }
      throw exception(); //never happens
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

} //namespace
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
    rankComputer->set_parameters(&parameters);
    rankComputer->set_train_matrix(&trainMatrix);
    rankComputer->set_model(&toplist_model);
    rankComputer->set_top_pop_container(&pop);
    EXPECT_TRUE(rankComputer->initialize());
    EXPECT_TRUE(rankComputer->self_test());
    
    EXPECT_EQ(0,rankComputer->get_rank(createRecDat(1,1,0.0)));
    EXPECT_EQ(5,rankComputer->get_rank(createRecDat(1,10,0.0)));
    EXPECT_EQ(5,rankComputer->get_rank(createRecDat(3,2,0.0)));
    EXPECT_EQ(1,rankComputer->get_rank(createRecDat(3,6,0.0)));
    EXPECT_EQ(2,rankComputer->get_rank(createRecDat(2,3,0.0)));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

