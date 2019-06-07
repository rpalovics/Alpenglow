#include <vector>
#include <gtest/gtest.h>
#include "../../main/utils/ToplistCreator.h"

namespace {

  class DummyModel : public Model {
    public:
      DummyModel(){}
      void add(RecDat* recDat){}
      virtual double prediction(RecDat * recDat){
        //cerr << "prediction user " << recDat->user << " item " << recDat->item << endl;
        predCount[recDat->user][recDat->item]++;
        preds.push_back(*recDat);
        return data[recDat->user][recDat->item];
      }
      vector<vector<int> > predCount;
      vector<RecDat> preds;
      vector<vector<double> > data;
  };

  class DummyFilter : public ModelFilter {
    public:
      void run(double time){}
      vector<pair<int,double> >* get_global_users() override {
        return &userBounds; 
      }
      vector<pair<int,double> >* get_global_items() override {
        return &itemBounds;
      }
      vector<pair<int,double> > userBounds;
      vector<pair<int,double> > itemBounds;
  };

  class TestToplistCreatorGlobal : public ::testing::Test  {
    public:
      ToplistCreatorGlobal* predictionCreator;
      DummyModel model;
      DummyFilter filter;

      TestToplistCreatorGlobal() {}
      virtual ~TestToplistCreatorGlobal(){}
      virtual void SetUp(){
        ToplistCreatorGlobalParameters params;
        params.top_k = 3;
        params.initial_threshold = 0;
        params.exclude_known = 0;
        predictionCreator = new ToplistCreatorGlobal(&params);
        predictionCreator->set_model(&model);
        predictionCreator->set_filter(&filter);
        filter.userBounds.push_back(make_pair(2,9.0));
        filter.userBounds.push_back(make_pair(1,4.0));
        filter.userBounds.push_back(make_pair(0,1.0));
        filter.itemBounds.push_back(make_pair(3,9.0));
        filter.itemBounds.push_back(make_pair(1,8.0));
        filter.itemBounds.push_back(make_pair(2,3.0));
        filter.itemBounds.push_back(make_pair(0,1.0));
        model.data.resize(3);
        model.data[0].push_back(0.99);
        model.data[0].push_back(0.99);
        model.data[0].push_back(0.99);
        model.data[0].push_back(1);
        model.data[1].push_back(0.99);
        model.data[1].push_back(4);
        model.data[1].push_back(2.99);
        model.data[1].push_back(3.1);
        model.data[2].push_back(0.9);
        model.data[2].push_back(8);
        model.data[2].push_back(3);
        model.data[2].push_back(0.5);
        model.predCount.resize(3);
        for(int i=0;i<3;i++){
          model.predCount[i].resize(4,0);
        }
      }
      virtual void TearDown(){
        remove("predictionCreatorTestFile");
      }
  };
} //namespace

TEST_F(TestToplistCreatorGlobal, global) {
  ASSERT_TRUE(predictionCreator->self_test());
  //tesztek:
  //jo toplistat ad
  RecDat recDat;
  recDat.time = 1234;
  vector<RecDat>* topList = predictionCreator->run(&recDat);
  EXPECT_EQ(3,topList->size());
  ASSERT_GE(topList->size(),3);
  EXPECT_DOUBLE_EQ(8,topList->at(0).score);
  EXPECT_EQ(2,topList->at(0).user);
  EXPECT_EQ(1,topList->at(0).item);
  EXPECT_DOUBLE_EQ(4,topList->at(1).score);
  EXPECT_EQ(1,topList->at(1).user);
  EXPECT_EQ(1,topList->at(1).item);
  EXPECT_DOUBLE_EQ(3.1,topList->at(2).score);
  EXPECT_EQ(1,topList->at(2).user);
  EXPECT_EQ(3,topList->at(2).item);
  //nem hiv folos fuggvenyt
  vector<vector<int>> call_num;
  call_num.resize(3);
  call_num[0].push_back(0);
  call_num[0].push_back(0);
  call_num[0].push_back(0);
  call_num[0].push_back(1);
  call_num[1].push_back(0);
  call_num[1].push_back(1);
  call_num[1].push_back(0);
  call_num[1].push_back(1);
  call_num[2].push_back(0);
  call_num[2].push_back(1);
  call_num[2].push_back(1);
  call_num[2].push_back(1);
  for(int userIndex=0;userIndex<3;userIndex++){
    for(int itemIndex=0;itemIndex<4;itemIndex++){
      EXPECT_EQ(call_num[userIndex][itemIndex],model.predCount[userIndex][itemIndex]);
    }
  }
}
TEST_F(TestToplistCreatorGlobal, global2) {
  predictionCreator->initial_threshold_=2;
  ASSERT_TRUE(predictionCreator->self_test());
  //tesztek:
  //jo toplistat ad
  RecDat recDat;
  recDat.time = 1234;
  vector<RecDat>* topList = predictionCreator->run(&recDat);
  EXPECT_EQ(3,topList->size());
  ASSERT_GE(topList->size(),3);
  EXPECT_DOUBLE_EQ(8,topList->at(0).score);
  EXPECT_EQ(2,topList->at(0).user);
  EXPECT_EQ(1,topList->at(0).item);
  EXPECT_DOUBLE_EQ(4,topList->at(1).score);
  EXPECT_EQ(1,topList->at(1).user);
  EXPECT_EQ(1,topList->at(1).item);
  EXPECT_DOUBLE_EQ(3.1,topList->at(2).score);
  EXPECT_EQ(1,topList->at(2).user);
  EXPECT_EQ(3,topList->at(2).item);
  //nem hiv folos fuggvenyt
  vector<vector<int>> call_num;
  call_num.resize(3);
  call_num[0].push_back(0);
  call_num[0].push_back(0);
  call_num[0].push_back(0);
  call_num[0].push_back(0);
  call_num[1].push_back(0);
  call_num[1].push_back(1);
  call_num[1].push_back(0);
  call_num[1].push_back(1);
  call_num[2].push_back(0);
  call_num[2].push_back(1);
  call_num[2].push_back(0);
  call_num[2].push_back(1);
  for(int userIndex=0;userIndex<3;userIndex++){
    for(int itemIndex=0;itemIndex<4;itemIndex++){
      EXPECT_EQ(call_num[userIndex][itemIndex],model.predCount[userIndex][itemIndex]);
    }
  }
}

TEST_F(TestToplistCreatorGlobal, process_line){
  ASSERT_TRUE(predictionCreator->self_test());
  //tests if process_line processes the proper recDats
  RecDat _recDat;
  _recDat.user = 1;
  predictionCreator->process_line(&filter.itemBounds, 0, UINT_MAX, &(_recDat.item), &_recDat);
  ASSERT_EQ(3,model.preds.size());
  EXPECT_EQ(3,model.preds[0].item);
  EXPECT_EQ(1,model.preds[1].item);
  EXPECT_EQ(2,model.preds[2].item);
  EXPECT_EQ(1,model.preds[0].user);
  EXPECT_EQ(1,model.preds[1].user);
  EXPECT_EQ(1,model.preds[2].user);
}

TEST_F(TestToplistCreatorGlobal, process_line2){
  ASSERT_TRUE(predictionCreator->self_test());
  //tests if process_line processes the proper recDats
  RecDat _recDat;
  _recDat.user = 2;
  predictionCreator->process_line(&filter.itemBounds, 1, 3, &(_recDat.item), &_recDat);
  ASSERT_EQ(2,model.preds.size());
  EXPECT_EQ(1,model.preds[0].item);
  EXPECT_EQ(2,model.preds[1].item);
  EXPECT_EQ(2,model.preds[0].user);
  EXPECT_EQ(2,model.preds[1].user);
}

TEST_F(TestToplistCreatorGlobal, process_square){
  ASSERT_TRUE(predictionCreator->self_test());
  RecDat _recDat;
  predictionCreator->process_rectangle(&filter.userBounds,&filter.itemBounds,2,3,3,4,&_recDat);
  ASSERT_EQ(1,model.preds.size());
  EXPECT_EQ(0,model.preds[0].item);
  EXPECT_EQ(0,model.preds[0].user);
}

TEST_F(TestToplistCreatorGlobal, exclude_known){
  predictionCreator->initial_threshold_=1;
  predictionCreator->exclude_known_ = 1;
  SpMatrix matrix;
  matrix.insert(2,1,1);
  matrix.insert(1,3,1);
  predictionCreator->set_train_matrix(&matrix);
  ASSERT_TRUE(predictionCreator->self_test());
  RecDat recDat;
  recDat.time = 1234;
  vector<RecDat>* topList = predictionCreator->run(&recDat);
  EXPECT_EQ(3,topList->size());
  ASSERT_GE(topList->size(),3);
  EXPECT_DOUBLE_EQ(4,topList->at(0).score);
  EXPECT_DOUBLE_EQ(3,topList->at(1).score);
  EXPECT_DOUBLE_EQ(2.99,topList->at(2).score);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

