#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include "../../main/filters/FactorModelFilter.h"
#include "../../main/utils/Factors.h"
#define DIMENSION 3

namespace {

template<typename T>
inline bool sortPairDescendingBySecond(pair<T,double> a, pair<T,double> b){
        return a.second > b.second;
}


class TestFactorFilter : public ::testing::Test { 
  public:
    TestFactorFilter(){}
    virtual ~TestFactorFilter(){}
    virtual void SetUp(){} 
    virtual void TearDown(){}
};
}

TEST_F(TestFactorFilter, test1){
  FactorsParameters params;
  params.begin_min = -0.01;
  params.begin_max = 0.01;
  params.dimension = 3;
  Factors userFactors(params), itemFactors(params);
  vector <double> userFactor1(3,0), userFactor2(3,0), userFactor3(3,0);
  vector <double> itemFactor1(3,0), itemFactor3(3,0), itemFactor4(3,0);
  userFactor1[0] = 0.2;
  userFactor1[1] = 0.3;
  userFactor1[2] = -0.1;
  userFactor2[0] = 0.1;
  userFactor2[1] = 0.2;
  userFactor2[2] = 0.3;
  userFactor3[0] = 0.3;
  userFactor3[1] = 0.2;
  userFactor3[2] = 0.2;
  itemFactor1[0] = 0.1;
  itemFactor1[1] = 0.1;
  itemFactor1[2] = -0.1;
  itemFactor3[0] = 0.1;
  itemFactor3[1] = 0.1;
  itemFactor3[2] = 0.1;
  itemFactor4[0] = 0.2;
  itemFactor4[1] = 0.2;
  itemFactor4[2] = 0.2;
  userFactors.set(1,&userFactor1);
  userFactors.set(2,&userFactor2);
  userFactors.set(3,&userFactor3);
  itemFactors.set(1,&itemFactor1);
  itemFactors.set(3,&itemFactor3);
  itemFactors.set(4,&itemFactor4);
  vector <int> users (3,0), items (3,0);
  users[0] = 1;
  users[1] = 3;
  users[2] = 2;
  items[0] = 3;
  items[1] = 1;
  items[2] = 4;
  FactorFilter userFilter;
  userFilter.set_factors(&userFactors,&itemFactors);
  userFilter.set_entities(&users,&items);
  vector<pair<int,double> > upperBoundVector;
  userFilter.set_upper_vector(&upperBoundVector);
  EXPECT_TRUE(userFilter.self_test());
  userFilter.run();
  vector <pair<int,double> >* upperBounds = &upperBoundVector; 
  sort(upperBounds->begin(),upperBounds->end(),sortPairDescendingBySecond<int>);
  EXPECT_EQ(3,upperBounds->at(0).first);
  EXPECT_EQ(2,upperBounds->at(1).first);
  EXPECT_EQ(1,upperBounds->at(2).first);
  EXPECT_DOUBLE_EQ(0.14,upperBounds->at(0).second);
  EXPECT_DOUBLE_EQ(0.12,upperBounds->at(1).second);
  EXPECT_DOUBLE_EQ(0.11,upperBounds->at(2).second);
  FactorFilter itemFilter;
  itemFilter.set_factors(&itemFactors,&userFactors);
  itemFilter.set_entities(&items,&users);
  vector<pair<int,double> > upperBoundVector2;
  itemFilter.set_upper_vector(&upperBoundVector2);
  EXPECT_TRUE(itemFilter.self_test());
  itemFilter.run();
  upperBounds = &upperBoundVector2; 
  sort(upperBounds->begin(),upperBounds->end(),sortPairDescendingBySecond<int>);
  EXPECT_EQ(4,upperBounds->at(0).first);
  EXPECT_EQ(3,upperBounds->at(1).first);
  EXPECT_EQ(1,upperBounds->at(2).first);
  EXPECT_DOUBLE_EQ(0.18,upperBounds->at(0).second);
  EXPECT_DOUBLE_EQ(0.09,upperBounds->at(1).second);
  EXPECT_DOUBLE_EQ(0.07,upperBounds->at(2).second); 

  //itemUpper, itemLower
  vector<double> upper = *(userFilter.get_upper());
  ASSERT_EQ(3,upper.size());
  EXPECT_DOUBLE_EQ(0.2,upper[0]);
  EXPECT_DOUBLE_EQ(0.2,upper[1]);
  EXPECT_DOUBLE_EQ(0.2,upper[2]);
  vector<double> lower = *(userFilter.get_lower());
  ASSERT_EQ(3,lower.size());
  EXPECT_DOUBLE_EQ(0.1,lower[0]);
  EXPECT_DOUBLE_EQ(0.1,lower[1]);
  EXPECT_DOUBLE_EQ(-0.1,lower[2]);
  
  //userUpper, userLower
  upper = *(itemFilter.get_upper());
  ASSERT_EQ(3,upper.size());
  EXPECT_DOUBLE_EQ(0.3,upper[0]);
  EXPECT_DOUBLE_EQ(0.3,upper[1]);
  EXPECT_DOUBLE_EQ(0.3,upper[2]);
  lower = *(itemFilter.get_lower());
  ASSERT_EQ(3,lower.size());
  EXPECT_DOUBLE_EQ(0.1,lower[0]);
  EXPECT_DOUBLE_EQ(0.2,lower[1]);
  EXPECT_DOUBLE_EQ(-0.1,lower[2]);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
