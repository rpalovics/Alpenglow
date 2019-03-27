#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include "../../main/filters/FactorModelFilter.h"
#include "../../main/utils/Factors.h"
#include "../../main/utils/Recency.h"
#define DIMENSION 3

namespace {
class DummyRecency : public Recency{
  public:
    DummyRecency(int number){number_ = number;}
    double get(int entity, double){ return 1.0/(1.0+entity+number_); }
    void update(int,double){};
    int number_;
};
class TestFactorModelFilter : public ::testing::Test { 
  public:
    TestFactorModelFilter(){}
    virtual ~TestFactorModelFilter(){}
    virtual void SetUp(){
      users.push_back(0); users.push_back(1); users.push_back(2);
      items.push_back(2); items.push_back(3); items.push_back(4);
    } 
    virtual void TearDown(){}
    vector<int> users; 
    vector<int> items; 
};
}

TEST_F(TestFactorModelFilter, test_basic){
  FactorModelParameters factorModelParameters;
  factorModelParameters.dimension = DIMENSION;
  factorModelParameters.initialize_all = false;
  factorModelParameters.use_sigmoid = false;
  //nobias
  //norecency
  FactorModel model(&factorModelParameters);
  vector<double> factor(DIMENSION,0);
  factor[0]=1;factor[1]=2;factor[2]=3;model.user_factors_.set(0,&factor);
  factor[0]=-1;factor[1]=0;factor[2]=2;model.user_factors_.set(1,&factor);
  factor[0]=-5;factor[1]=1;factor[2]=6;model.user_factors_.set(2,&factor);

  factor[0]=1;factor[1]=1;factor[2]=1;model.item_factors_.set(2,&factor);
  factor[0]=0;factor[1]=1;factor[2]=0;model.item_factors_.set(3,&factor);
  factor[0]=-2;factor[1]=0;factor[2]=3;model.item_factors_.set(4,&factor);

  FactorModelFilter filter;
  filter.set_model(&model);
  filter.set_items(&items);
  filter.set_users(&users);
  ASSERT_TRUE(filter.self_test());
  filter.run(0.0);
  vector<pair<int,double>>* userToplist = filter.get_global_users();
  ASSERT_EQ(3, userToplist->size());
  EXPECT_EQ(2, userToplist->at(0).first);
  EXPECT_EQ(29, userToplist->at(0).second);
  EXPECT_EQ(0, userToplist->at(1).first);
  EXPECT_EQ(12, userToplist->at(1).second);
  EXPECT_EQ(1, userToplist->at(2).first);
  EXPECT_EQ(8, userToplist->at(2).second);
  vector<pair<int,double>>* itemToplist = filter.get_global_items();
  ASSERT_EQ(3, itemToplist->size());
  EXPECT_EQ(4, itemToplist->at(0).first);
  EXPECT_EQ(28, itemToplist->at(0).second);
  EXPECT_EQ(2, itemToplist->at(1).first);
  EXPECT_EQ(9, itemToplist->at(1).second);
  EXPECT_EQ(3, itemToplist->at(2).first);
  EXPECT_EQ(2, itemToplist->at(2).second);
}

TEST_F(TestFactorModelFilter, test_recency){
  FactorModelParameters factorModelParameters;
  factorModelParameters.dimension = DIMENSION;
  factorModelParameters.initialize_all = false;
  factorModelParameters.use_sigmoid = false;
  //nobias
  FactorModel model(&factorModelParameters);
  DummyRecency userRecency(5);
  model.set_user_recency(&userRecency);
  DummyRecency itemRecency(1);
  model.set_item_recency(&itemRecency);
  vector<double> factor(DIMENSION,0);
  factor[0]=1;factor[1]=2;factor[2]=3;model.user_factors_.set(0,&factor);
  factor[0]=-1;factor[1]=0;factor[2]=2;model.user_factors_.set(1,&factor);
  factor[0]=-5;factor[1]=1;factor[2]=6;model.user_factors_.set(2,&factor);

  factor[0]=1;factor[1]=1;factor[2]=1;model.item_factors_.set(2,&factor);
  factor[0]=0;factor[1]=1;factor[2]=0;model.item_factors_.set(3,&factor);
  factor[0]=-2;factor[1]=0;factor[2]=3;model.item_factors_.set(4,&factor);

  FactorModelFilter filter;
  filter.set_model(&model);
  filter.set_items(&items);
  filter.set_users(&users);
  ASSERT_TRUE(filter.self_test());
  filter.run(0.0);
  vector<pair<int,double>>* userToplist = filter.get_global_users();
  ASSERT_EQ(3, userToplist->size());
  EXPECT_DOUBLE_EQ(2, userToplist->at(0).first);
  EXPECT_DOUBLE_EQ(29/8., userToplist->at(0).second);
  EXPECT_DOUBLE_EQ(0, userToplist->at(1).first);
  EXPECT_DOUBLE_EQ(12/6., userToplist->at(1).second);
  EXPECT_DOUBLE_EQ(1, userToplist->at(2).first);
  EXPECT_DOUBLE_EQ(8/7., userToplist->at(2).second);
  vector<pair<int,double>>* itemToplist = filter.get_global_items();
  ASSERT_EQ(3, itemToplist->size());
  EXPECT_DOUBLE_EQ(4, itemToplist->at(0).first);
  EXPECT_DOUBLE_EQ(28/6., itemToplist->at(0).second);
  EXPECT_DOUBLE_EQ(2, itemToplist->at(1).first);
  EXPECT_DOUBLE_EQ(9/4., itemToplist->at(1).second);
  EXPECT_DOUBLE_EQ(3, itemToplist->at(2).first);
  EXPECT_DOUBLE_EQ(2/5., itemToplist->at(2).second);
}

TEST_F(TestFactorModelFilter, test_bias){
  FactorModelParameters factorModelParameters;
  factorModelParameters.dimension = DIMENSION;
  factorModelParameters.initialize_all = false;
  factorModelParameters.use_sigmoid = false;
  factorModelParameters.use_user_bias = true;
  factorModelParameters.use_item_bias = true;
  //norecency
  FactorModel model(&factorModelParameters);
  model.user_bias_.init(0); model.user_bias_.update(0,0);
  model.user_bias_.init(1); model.user_bias_.update(1,3);
  model.user_bias_.init(2); model.user_bias_.update(2,-1);
  model.item_bias_.init(2); model.item_bias_.update(2,5);
  model.item_bias_.init(3); model.item_bias_.update(3,-4);
  model.item_bias_.init(4); model.item_bias_.update(4,12);
  vector<double> factor(DIMENSION,0);
  factor[0]=1;factor[1]=2;factor[2]=3;model.user_factors_.set(0,&factor);
  factor[0]=-1;factor[1]=0;factor[2]=2;model.user_factors_.set(1,&factor);
  factor[0]=-5;factor[1]=1;factor[2]=6;model.user_factors_.set(2,&factor);

  factor[0]=1;factor[1]=1;factor[2]=1;model.item_factors_.set(2,&factor);
  factor[0]=0;factor[1]=1;factor[2]=0;model.item_factors_.set(3,&factor);
  factor[0]=-2;factor[1]=0;factor[2]=3;model.item_factors_.set(4,&factor);

  FactorModelFilter filter;
  filter.set_model(&model);
  filter.set_items(&items);
  filter.set_users(&users);
  ASSERT_TRUE(filter.self_test());
  filter.run(0.0);
  vector<pair<int,double>>* userToplist = filter.get_global_users();
  ASSERT_EQ(3, userToplist->size());
  EXPECT_EQ(2, userToplist->at(0).first);
  EXPECT_EQ(29+12-1, userToplist->at(0).second);
  EXPECT_EQ(0, userToplist->at(1).first);
  EXPECT_EQ(12+12+0, userToplist->at(1).second);
  EXPECT_EQ(1, userToplist->at(2).first);
  EXPECT_EQ(8+12+3, userToplist->at(2).second);
  vector<pair<int,double>>* itemToplist = filter.get_global_items();
  ASSERT_EQ(3, itemToplist->size());
  EXPECT_EQ(4, itemToplist->at(0).first);
  EXPECT_EQ(28+3+12, itemToplist->at(0).second);
  EXPECT_EQ(2, itemToplist->at(1).first);
  EXPECT_EQ(9+3+5, itemToplist->at(1).second);
  EXPECT_EQ(3, itemToplist->at(2).first);
  EXPECT_EQ(2+3-4, itemToplist->at(2).second);
}
TEST_F(TestFactorModelFilter, test_sigmoid){
  FactorModelParameters factorModelParameters;
  factorModelParameters.dimension = DIMENSION;
  factorModelParameters.initialize_all = false;
  factorModelParameters.use_sigmoid = true;
  //nobias
  //norecency
  FactorModel model(&factorModelParameters);
  vector<double> factor(DIMENSION,0);
  factor[0]=1;factor[1]=2;factor[2]=3;model.user_factors_.set(0,&factor);
  factor[0]=-1;factor[1]=0;factor[2]=2;model.user_factors_.set(1,&factor);
  factor[0]=-5;factor[1]=1;factor[2]=6;model.user_factors_.set(2,&factor);

  factor[0]=1;factor[1]=1;factor[2]=1;model.item_factors_.set(2,&factor);
  factor[0]=0;factor[1]=1;factor[2]=0;model.item_factors_.set(3,&factor);
  factor[0]=-2;factor[1]=0;factor[2]=3;model.item_factors_.set(4,&factor);

  FactorModelFilter filter;
  filter.set_model(&model);
  filter.set_items(&items);
  filter.set_users(&users);
  ASSERT_TRUE(filter.self_test());
  filter.run(0.0);
  vector<pair<int,double>>* userToplist = filter.get_global_users();
  ASSERT_EQ(3, userToplist->size());
  EXPECT_DOUBLE_EQ(2, userToplist->at(0).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function(29), userToplist->at(0).second);
  EXPECT_DOUBLE_EQ(0, userToplist->at(1).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function(12), userToplist->at(1).second);
  EXPECT_DOUBLE_EQ(1, userToplist->at(2).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function(8), userToplist->at(2).second);
  vector<pair<int,double>>* itemToplist = filter.get_global_items();
  ASSERT_EQ(3, itemToplist->size());
  EXPECT_DOUBLE_EQ(4, itemToplist->at(0).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function(28), itemToplist->at(0).second);
  EXPECT_DOUBLE_EQ(2, itemToplist->at(1).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function(9), itemToplist->at(1).second);
  EXPECT_DOUBLE_EQ(3, itemToplist->at(2).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function(2), itemToplist->at(2).second);
}
TEST_F(TestFactorModelFilter, test_all){
  FactorModelParameters factorModelParameters;
  factorModelParameters.dimension = DIMENSION;
  factorModelParameters.initialize_all = false;
  factorModelParameters.use_sigmoid = true;
  factorModelParameters.use_user_bias = true;
  factorModelParameters.use_item_bias = true;
  FactorModel model(&factorModelParameters);
  DummyRecency userRecency(5);
  model.set_user_recency(&userRecency);
  DummyRecency itemRecency(1);
  model.set_item_recency(&itemRecency);
  //norecency
  model.user_bias_.init(0); model.user_bias_.update(0,0);
  model.user_bias_.init(1); model.user_bias_.update(1,3);
  model.user_bias_.init(2); model.user_bias_.update(2,-1);
  model.item_bias_.init(2); model.item_bias_.update(2,5);
  model.item_bias_.init(3); model.item_bias_.update(3,-4);
  model.item_bias_.init(4); model.item_bias_.update(4,12);
  vector<double> factor(DIMENSION,0);
  factor[0]=1;factor[1]=2;factor[2]=3;model.user_factors_.set(0,&factor);
  factor[0]=-1;factor[1]=0;factor[2]=2;model.user_factors_.set(1,&factor);
  factor[0]=-5;factor[1]=1;factor[2]=6;model.user_factors_.set(2,&factor);

  factor[0]=1;factor[1]=1;factor[2]=1;model.item_factors_.set(2,&factor);
  factor[0]=0;factor[1]=1;factor[2]=0;model.item_factors_.set(3,&factor);
  factor[0]=-2;factor[1]=0;factor[2]=3;model.item_factors_.set(4,&factor);

  FactorModelFilter filter;
  filter.set_model(&model);
  filter.set_items(&items);
  filter.set_users(&users);
  ASSERT_TRUE(filter.self_test());
  filter.run(0.0);
  vector<pair<int,double>>* userToplist = filter.get_global_users();
  ASSERT_EQ(3, userToplist->size());
  EXPECT_DOUBLE_EQ(2, userToplist->at(0).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function((29+12-1)/8.), userToplist->at(0).second);
  EXPECT_DOUBLE_EQ(0, userToplist->at(1).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function((12+12+0)/6.), userToplist->at(1).second);
  EXPECT_DOUBLE_EQ(1, userToplist->at(2).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function((8+12+3)/7.), userToplist->at(2).second);
  vector<pair<int,double>>* itemToplist = filter.get_global_items();
  ASSERT_EQ(3, itemToplist->size());
  EXPECT_DOUBLE_EQ(4, itemToplist->at(0).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function((28+3+12)/6.), itemToplist->at(0).second);
  EXPECT_DOUBLE_EQ(2, itemToplist->at(1).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function((9+3+5)/4.), itemToplist->at(1).second);
  EXPECT_DOUBLE_EQ(3, itemToplist->at(2).first);
  EXPECT_DOUBLE_EQ(Util::sigmoid_function((2+3-4)/5.), itemToplist->at(2).second);
}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
