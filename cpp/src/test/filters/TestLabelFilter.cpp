#include <gtest/gtest.h>
#include "../../main/filters/LabelFilter.h"

namespace {

class TestLabelFilter : public ::testing::Test { 
  public:
    TestLabelFilter(){}
    virtual ~TestLabelFilter(){}
    void SetUp() override {
      users.push_back(0); users.push_back(1); users.push_back(2);
      users.push_back(5); users.push_back(4);

      items.push_back(2); items.push_back(3); items.push_back(4);
    } 
    void TearDown() override {}
    bool contains_element(vector<pair<int,double>>* container, pair<int,double> element){
      return (std::find(container->begin(), container->end(), element) != container->end());
    }
    vector<int> users; 
    vector<int> items; 
};
}

TEST_F(TestLabelFilter, test){
  LabelFilterParameters params;
  params.label_file_name = ""; //will cause a file reading error in container, wont read trash
  LabelFilter filter(&params);
  stringstream ss;
  ss << "1 3" << endl;
  ss << "2 4" << endl;
  ss << "3 2" << endl;
  ss << "4 3" << endl;
  ss << "6 3" << endl;
  ss << "5 3" << endl;
  ss << "9 4" << endl;
  ss << "7 3" << endl;
  filter.label_container_.read_from_file(ss);
  ASSERT_TRUE(filter.self_test());

  RecDat rec_dat;
  rec_dat.user = 3;
  rec_dat.item = 2; //artist: 4

  //at the beginning, no rec_dat is active  
  filter.run(&rec_dat);
  EXPECT_FALSE(filter.active(&rec_dat));
  vector<pair<int,double>>* user_filter = filter.get_global_users();
  EXPECT_TRUE(user_filter == NULL);
  vector<pair<int,double>>* item_filter = filter.get_personalized_items(rec_dat.user);
  ASSERT_TRUE(item_filter!=NULL);
  EXPECT_EQ(0, item_filter->size());

  //now the same rec_dat is active for the same user
  RecDat rec_dat2 = rec_dat;
  filter.run(&rec_dat2);
  EXPECT_TRUE(filter.active(&rec_dat2));
  RecDat rec_dat3 = rec_dat2;
  rec_dat3.item = 9; //another item with the same label
  filter.run(&rec_dat3);
  EXPECT_TRUE(filter.active(&rec_dat3));
  //the items with the same label should be active
  filter.run(&rec_dat);
  item_filter = filter.get_personalized_items(rec_dat.user);
  ASSERT_TRUE(item_filter!=NULL);
  EXPECT_EQ(2, item_filter->size());
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(2,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(9,-1)));

  RecDat rec_dat4 = rec_dat3;
  rec_dat4.user = 2; //new user, no item should be active
  filter.run(&rec_dat4);
  EXPECT_FALSE(filter.active(&rec_dat4));
  RecDat rec_dat5 = rec_dat4;
  rec_dat5.item = 2; //another item with the same label
  filter.run(&rec_dat5);
  EXPECT_TRUE(filter.active(&rec_dat5));
  RecDat rec_dat6 = rec_dat5;
  rec_dat6.item = 4; //new item, new label=3
  filter.run(&rec_dat6);
  EXPECT_FALSE(filter.active(&rec_dat6));
  item_filter = filter.get_personalized_items(rec_dat6.user);
  ASSERT_TRUE(item_filter!=NULL);
  EXPECT_EQ(2, item_filter->size());
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(2,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(9,-1)));

  RecDat rec_dat7 = rec_dat6;
  rec_dat7.item = 1; //item with the same label
  filter.run(&rec_dat7);
  EXPECT_TRUE(filter.active(&rec_dat7));
  item_filter = filter.get_personalized_items(rec_dat7.user);
  EXPECT_EQ(5, item_filter->size());
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(1,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(4,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(6,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(5,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(7,-1)));

  RecDat rec_dat8 = rec_dat7;
  rec_dat8.user = 3; //the first user again
  rec_dat8.item = 6; //artist: 4
  filter.run(&rec_dat8);
  EXPECT_FALSE(filter.active(&rec_dat8));
  item_filter = filter.get_personalized_items(rec_dat8.user);
  ASSERT_TRUE(item_filter!=NULL);
  EXPECT_EQ(2, item_filter->size());
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(2,-1)));
  EXPECT_TRUE(contains_element(item_filter,std::make_pair(9,-1)));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
