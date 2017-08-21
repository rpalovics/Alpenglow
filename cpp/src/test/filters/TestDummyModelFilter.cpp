#include <gtest/gtest.h>
#include "../../main/filters/DummyModelFilter.h"

namespace {
class TestDummyModelFilter : public ::testing::Test { 
  public:
    TestDummyModelFilter(){}
    virtual ~TestDummyModelFilter(){}
    virtual void SetUp(){
      users.push_back(0); users.push_back(1); users.push_back(2);
      users.push_back(5); users.push_back(4);

      items.push_back(2); items.push_back(3); items.push_back(4);
    } 
    virtual void TearDown(){}
    vector<int> users; 
    vector<int> items; 
};
}

TEST_F(TestDummyModelFilter, test){
  DummyModelFilter filter;
  filter.set_items(&items);
  filter.set_users(&users);
  ASSERT_TRUE(filter.self_test());
  filter.run((RecDat*)NULL);

  vector<pair<int,double>>* item_filter = filter.get_global_items();
  EXPECT_EQ(items.size(),item_filter->size());
  vector<pair<int,double>>* user_filter = filter.get_global_users();
  EXPECT_EQ(users.size(),user_filter->size());
  item_filter = filter.get_personalized_items(1);
  EXPECT_EQ(items.size(),item_filter->size());
  user_filter = filter.get_personalized_users(1);
  EXPECT_EQ(users.size(),user_filter->size());

  filter.run(NULL);
  filter.run(NULL);
  filter.run(NULL);
  filter.run(NULL);

  item_filter = filter.get_global_items();
  EXPECT_EQ(items.size(),item_filter->size());
  user_filter = filter.get_global_users();
  EXPECT_EQ(users.size(),user_filter->size());
  item_filter = filter.get_personalized_items(1);
  EXPECT_EQ(items.size(),item_filter->size());
  user_filter = filter.get_personalized_users(1);
  EXPECT_EQ(users.size(),user_filter->size());

  ModelFilter* filter_pointer = &filter;
  filter_pointer->run((RecDat*)NULL);

  item_filter = filter_pointer->get_global_items();
  EXPECT_EQ(items.size(),item_filter->size());
  user_filter = filter_pointer->get_global_users();
  EXPECT_EQ(users.size(),user_filter->size());
  item_filter = filter_pointer->get_personalized_items(1);
  EXPECT_EQ(items.size(),item_filter->size());
  user_filter = filter_pointer->get_personalized_users(1);
  EXPECT_EQ(users.size(),user_filter->size());
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
