#include <vector>
#include <gtest/gtest.h>
#include "../../main/recommender_data/RecommenderData.h"
#include "../../main/utils/Toplist.h"


namespace {

class TestToplist : public ::testing::Test  {
public:
  TestToplist() {
  }
  virtual ~TestToplist() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
};

}

bool compare_func(int a, int b){
  return a<b;
}
bool compare_rec_dat(RecDat a, RecDat b){
  return a.score > b.score;
}

TEST_F(TestToplist, dummy) {
  Toplist<int,compare_func > heap(3);
}

TEST_F(TestToplist, insert){
  Toplist<RecDat,compare_rec_dat> heap(50);
  RecDat recDat;
  recDat.user = 1;
  recDat.item = 2;
  recDat.score = 3;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  recDat.score = 5;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  recDat.score = 2;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  recDat.score = 87;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  recDat.score = 234;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  recDat.score = 2;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  recDat.score = 4;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  recDat.score = 3;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  recDat.score = -3232;
  heap.insert(recDat);
  EXPECT_EQ(-3232, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(2, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(2, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(3, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(3, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(4, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(5, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(87, heap.get_min().score);
  heap.delete_min();
  EXPECT_EQ(234, heap.get_min().score);
}

TEST_F(TestToplist, topK){
  Toplist<RecDat,compare_rec_dat> heap(3);
  RecDat recDat;
  recDat.user = 1;
  recDat.item = 2;
  recDat.score = 3;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  //heap.show();
  recDat.score = 5;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  EXPECT_EQ(2, heap.size());
  //heap.show();
  recDat.score = 2;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  EXPECT_EQ(3, heap.size());
  //heap.show();
  recDat.score = 87;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  EXPECT_EQ(3, heap.size());
  //heap.show();
  recDat.score = 234;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  //heap.show();
  recDat.score = 2;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  //heap.show();
  recDat.score = 4;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  //heap.show();
  recDat.score = 3;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  //heap.show();
  recDat.score = -3232;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  //heap.show();
  heap.delete_min();
  EXPECT_EQ(87, heap.get_min().score);
  //heap.show();
  heap.delete_min();
  EXPECT_EQ(234, heap.get_min().score);
  //heap.show();
}
#define maxtime 10000000
TEST_F(TestToplist, small){
  Toplist<RecDat,compare_rec_dat> heap(10);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=rand()/RAND_MAX;
    heap.insert(recDat);
  }
}

TEST_F(TestToplist, medium){
  Toplist<RecDat,compare_rec_dat> heap(100);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=rand()/RAND_MAX;
    heap.insert(recDat);
  }
}

TEST_F(TestToplist, medium2){
  Toplist<RecDat,compare_rec_dat> heap(1000);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=rand()/RAND_MAX;
    heap.insert(recDat);
  }
}

TEST_F(TestToplist, large){
  Toplist<RecDat,compare_rec_dat> heap(10000);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=rand()/RAND_MAX;
    heap.insert(recDat);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

