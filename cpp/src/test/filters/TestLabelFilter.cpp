#include <gtest/gtest.h>
#include "../../main/filters/LabelFilter.h"

namespace {

class TestLabelFilter : public ::testing::Test { 
  public:
    TestLabelFilter(){}
    virtual ~TestLabelFilter(){}
    void SetUp() override {
      users = {1,2,3};
      for(int i=0;i<10;i++){ items.push_back(i); }
    } 
    void TearDown() override {}
    bool contains_element(vector<int> container, int element){
      return (std::find(container.begin(), container.end(), element) != container.end());
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
  for(int i=0;i<10;i++){
    ss << i << " " << i%3 << endl; //artist=item mod 3
  }
  filter.label_container_.read_from_file(ss);
  ASSERT_TRUE(filter.self_test());

  //at the beginning, no rec_dat is active  
  RecDat rec_dat;
  for( auto user : users ){
    rec_dat.user = user;
    for( auto item : items ){
      rec_dat.item = item;
      EXPECT_FALSE(filter.active(&rec_dat));
    }
    vector<int> whitelist = filter.get_whitelist(user);
    EXPECT_EQ(0,whitelist.size());
  }

  //after the first update item 2,5,8 are active for user 3 and none to others
  rec_dat.user = 3;
  rec_dat.item = 2; //artist: 2
  filter.update(&rec_dat);

  //user 3
  int user = 3;
  for( auto item : vector<int>{2,5,8} ){
    rec_dat.item = item;
    EXPECT_TRUE(filter.active(&rec_dat));
  }
  for( auto item : vector<int>{1,3,4,6,7,9} ){
    rec_dat.item = item;
    EXPECT_FALSE(filter.active(&rec_dat));
  }
  vector<int> whitelist = filter.get_whitelist(user);
  EXPECT_EQ(3,whitelist.size());
  EXPECT_TRUE(contains_element(whitelist,2));
  EXPECT_TRUE(contains_element(whitelist,5));
  EXPECT_TRUE(contains_element(whitelist,8));

  //user 1,2
  for( auto user : vector<int>{1,2} ){
    rec_dat.user = user;
    for( auto item : items ){
      rec_dat.item = item;
      EXPECT_FALSE(filter.active(&rec_dat));
      vector<int> whitelist = filter.get_whitelist(user);
      EXPECT_EQ(0,whitelist.size());
    }
  }

  //after the second update item 1,4,7 are active for user 3 and none to others
  rec_dat.user = 3;
  rec_dat.item = 7; //artist: 1
  filter.update(&rec_dat);

  //user 3
  user = 3;
  for( auto item : vector<int>{1,4,7} ){
    rec_dat.item = item;
    EXPECT_TRUE(filter.active(&rec_dat));
  }
  for( auto item : vector<int>{2,3,5,6,8,9} ){
    rec_dat.item = item;
    EXPECT_FALSE(filter.active(&rec_dat));
  }
  whitelist = filter.get_whitelist(user);
  EXPECT_EQ(3,whitelist.size());
  EXPECT_TRUE(contains_element(whitelist,1));
  EXPECT_TRUE(contains_element(whitelist,4));
  EXPECT_TRUE(contains_element(whitelist,7));

  //user 1,2
  for( auto user : vector<int>{1,2} ){
    rec_dat.user = user;
    for( auto item : items ){
      rec_dat.item = item;
      EXPECT_FALSE(filter.active(&rec_dat));
      vector<int> whitelist = filter.get_whitelist(user);
      EXPECT_EQ(0,whitelist.size());
    }
  }
}

TEST_F(TestLabelFilter, test2){
  LabelFilterParameters params;
  params.label_file_name = "";
  LabelFilter* pfilter;
  pfilter = new LabelFilter(&params);
  delete pfilter; //this covers _ZN11LabelFilterD0Ev in order to get complete function coverage
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
