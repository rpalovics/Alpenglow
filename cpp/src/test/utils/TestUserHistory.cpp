#include <gtest/gtest.h>
#include "../../main/utils/UserHistory.h"

namespace {

class TestUserHistory : public ::testing::Test  {
public:
  TestUserHistory() {
  }
  virtual ~TestUserHistory() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
};

}

TEST_F(TestUserHistory, dummy) {
  UserHistory user_history;
  //update
  vector<RecDat> rec_dats;
  rec_dats.resize(10);
  rec_dats[0].user=3;
  rec_dats[0].item=5;
  user_history.update(&rec_dats[0]);
  rec_dats[1].user=3;
  rec_dats[1].item=2;
  user_history.update(&rec_dats[1]);
  rec_dats[2].user=1;
  rec_dats[2].item=4;
  user_history.update(&rec_dats[2]);
  rec_dats[3].user=4;
  rec_dats[3].item=5;
  user_history.update(&rec_dats[3]);
  rec_dats[4].user=3;
  rec_dats[4].item=8;
  user_history.update(&rec_dats[4]);
  rec_dats[5].user=1;
  rec_dats[5].item=7;
  user_history.update(&rec_dats[5]);

  //get
  const vector<const RecDat*>* history = user_history.get_user_history(1);
  ASSERT_FALSE(history==NULL);
  ASSERT_EQ(2,history->size());
  EXPECT_EQ(4,history->at(0)->item);
  EXPECT_EQ(7,history->at(1)->item);
  history = user_history.get_user_history(2);
  EXPECT_TRUE(history==NULL);
  history = user_history.get_user_history(3);
  ASSERT_FALSE(history==NULL);
  ASSERT_EQ(3,history->size());
  EXPECT_EQ(5,history->at(0)->item);
  EXPECT_EQ(2,history->at(1)->item);
  EXPECT_EQ(8,history->at(2)->item);
  history = user_history.get_user_history(4);
  ASSERT_FALSE(history==NULL);
  ASSERT_EQ(1,history->size());
  EXPECT_EQ(5,history->at(0)->item);
  for(int user=5;user<20;user++){
    history = user_history.get_user_history(user);
    EXPECT_TRUE(history==NULL);
  }
  
  //clear
  user_history.clear_user_history(4);
  history = user_history.get_user_history(4);
  ASSERT_FALSE(history==NULL);
  EXPECT_EQ(0,history->size());
  user_history.clear_user_history(2);
  history = user_history.get_user_history(2);
  EXPECT_TRUE(history==NULL);

  //delete
  user_history.delete_user_history(3);
  history = user_history.get_user_history(3);
  EXPECT_TRUE(history==NULL);
  user_history.delete_user_history(2);
  history = user_history.get_user_history(2);
  EXPECT_TRUE(history==NULL);

  //clear all
  user_history.clear_all();
  history = user_history.get_user_history(1);
  ASSERT_FALSE(history==NULL);
  EXPECT_EQ(0,history->size());
  history = user_history.get_user_history(2);
  EXPECT_TRUE(history==NULL);
  history = user_history.get_user_history(3);
  EXPECT_TRUE(history==NULL);
  history = user_history.get_user_history(4);
  ASSERT_FALSE(history==NULL);
  EXPECT_EQ(0,history->size());
  for(int user=5;user<20;user++){
    history = user_history.get_user_history(user);
    EXPECT_TRUE(history==NULL);
  }

  //delete all
  user_history.delete_all();
  for(int user=1;user<20;user++){
    history = user_history.get_user_history(user);
    EXPECT_TRUE(history==NULL);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

