#include <vector>
#include <gtest/gtest.h>
#include "../../main/recommender_data/ShuffleIterator.h"


namespace {

class TestShuffleIterator : public ::testing::Test  {
public:
  RecommenderData rd;
  TestShuffleIterator() {
    EXPECT_TRUE(rd.initialize());
  }
  virtual ~TestShuffleIterator() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
  }
  virtual void TearDown(){
  }
  RecDat createRecDat(int user, int item, double time, double score){
    RecDat recDat;
    recDat.user=user;
    recDat.item=item;
    recDat.time=time;
    recDat.score=score;
    return recDat;
  }
  bool in(int element, vector<int> list){
    //element is in list
    for(int i=0; i<list.size(); i++){
      if(list[i]==element) return true;
    }
    return false;
  }
};

}
TEST_F(TestShuffleIterator, size) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.0,1.0));
    recData.push_back(createRecDat(1,6,10.0,1.0));
    recData.push_back(createRecDat(2,8,10.0,1.0));
    rd.set_rec_data(recData);
    ShuffleIterator it(&rd, 123124);
    ASSERT_TRUE(it.initialize());
    EXPECT_EQ(3,it.size());
}

TEST_F(TestShuffleIterator, hasNext) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.1,1.0));
    recData.push_back(createRecDat(1,6,10.2,1.0));
    recData.push_back(createRecDat(2,8,10.3,1.0));
    rd.set_rec_data(recData);
    ShuffleIterator it(&rd, 1231212);
    ASSERT_TRUE(it.initialize());
    EXPECT_EQ(3,it.size());
    ASSERT_TRUE(it.has_next());
    it.next();
    ASSERT_TRUE(it.has_next());
    it.next();
    ASSERT_TRUE(it.has_next());
    it.next();
    EXPECT_FALSE(it.has_next());
}

TEST_F(TestShuffleIterator, noshuffle_it) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.1,1.0));
    recData.push_back(createRecDat(1,6,10.2,1.0));
    recData.push_back(createRecDat(2,8,10.3,1.0));
    rd.set_rec_data(recData);
    ShuffleIterator it(&rd, 1231212);
    ASSERT_TRUE(it.initialize());
    EXPECT_EQ(3,it.size());
    ASSERT_TRUE(it.has_next());
    RecDat* recDat = it.next();
    EXPECT_EQ(3, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(6, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(8, recDat->item);
    EXPECT_FALSE(it.has_next());
}

TEST_F(TestShuffleIterator, shuffle_it) {
    RecDats recData;
    recData.push_back(createRecDat(2,1,10.0,1.0));
    recData.push_back(createRecDat(1,2,10.0,1.0));
    recData.push_back(createRecDat(2,3,10.0,1.0));
    recData.push_back(createRecDat(4,4,10.1,1.0));
    recData.push_back(createRecDat(2,5,10.2,1.0));
    recData.push_back(createRecDat(3,6,10.5,1.0));
    recData.push_back(createRecDat(1,7,10.5,1.0));
    recData.push_back(createRecDat(2,8,10.5,1.0));
    recData.push_back(createRecDat(3,9,10.5,1.0));
    rd.set_rec_data(recData);
    ShuffleIterator it(&rd, 1239);
    ASSERT_TRUE(it.initialize());
    EXPECT_EQ(9,it.size());
    vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
    ASSERT_TRUE(it.has_next());
    RecDat* recDat = it.next();
    EXPECT_EQ(10.0, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(10.0, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    EXPECT_NE(2, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(10.0, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    EXPECT_NE(3, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(4, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(5, recDat->item);
    items.clear();
    items.push_back(6);items.push_back(7);items.push_back(8);items.push_back(9);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(10.5, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    EXPECT_NE(6, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(10.5, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    EXPECT_NE(7, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(10.5, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    EXPECT_NE(8, recDat->item);
    ASSERT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(10.5, recDat->time);
    EXPECT_TRUE(in(recDat->item, items));
    EXPECT_NE(9, recDat->item);
}

TEST_F(TestShuffleIterator, noshuffle_get) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.1,1.0));
    recData.push_back(createRecDat(1,6,10.2,1.0));
    recData.push_back(createRecDat(2,8,10.3,1.0));
    rd.set_rec_data(recData);
    ShuffleIterator it(&rd, 12361887);
    ASSERT_TRUE(it.initialize());
    EXPECT_EQ(3,it.size());
    EXPECT_EQ(3, (it.get_future(0))->item);
    EXPECT_EQ(6, (it.get_future(1))->item);
    EXPECT_EQ(8, (it.get_future(2))->item);
    EXPECT_EQ(6, (it.get_future(1))->item);
    EXPECT_EQ(3, (it.get_future(0))->item);
    EXPECT_EQ(8, (it.get_future(2))->item);
}

TEST_F(TestShuffleIterator, reproducable) {
    RecDats recData;
    recData.push_back(createRecDat(2,1,10.0,1.0));
    recData.push_back(createRecDat(1,2,10.0,1.0));
    recData.push_back(createRecDat(2,3,10.0,1.0));
    recData.push_back(createRecDat(4,4,10.0,1.0));
    recData.push_back(createRecDat(2,5,10.0,1.0));
    recData.push_back(createRecDat(3,6,10.0,1.0));
    recData.push_back(createRecDat(1,7,10.5,1.0));
    recData.push_back(createRecDat(2,8,10.5,1.0));
    recData.push_back(createRecDat(3,9,10.5,1.0));
    rd.set_rec_data(recData);
    ShuffleIterator it(&rd, 123124);
    ASSERT_TRUE(it.initialize());
    ShuffleIterator it2(&rd, 123124);
    ASSERT_TRUE(it2.initialize());
    EXPECT_EQ(it.get_future(0)->item,it2.get_future(0)->item);
    EXPECT_EQ(it.get_future(1)->item,it2.get_future(1)->item);
    EXPECT_EQ(it.get_future(2)->item,it2.get_future(2)->item);
    EXPECT_EQ(it.get_future(3)->item,it2.get_future(3)->item);
    EXPECT_EQ(it.get_future(4)->item,it2.get_future(4)->item);
    EXPECT_EQ(it.get_future(5)->item,it2.get_future(5)->item);
    EXPECT_EQ(it.get_future(6)->item,it2.get_future(6)->item);
    EXPECT_EQ(it.get_future(7)->item,it2.get_future(7)->item);
    EXPECT_EQ(it.get_future(8)->item,it2.get_future(8)->item);
}

//matrix function disabled, online recommender should not use it
//TEST_F(TestShuffleIterator, matrix) {
//    RecDats recData;
//    recData.push_back(createRecDat(2,3,10.0,1.0));
//    recData.push_back(createRecDat(1,6,10.0,1.0));
//    recData.push_back(createRecDat(2,8,10.0,1.0));
//    rd.set_rec_data(recData);
//    ShuffleIterator it(&rd, 1283761287);
//    EXPECT_EQ(rd.matrix(),it.matrix());
//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

