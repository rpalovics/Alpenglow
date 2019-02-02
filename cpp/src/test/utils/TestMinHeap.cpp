#include <vector>
#include <gtest/gtest.h>
#include "../../main/utils/MinHeap.h"
#include "../../main/utils/Toplist.h"
#include "../../main/utils/Random.h"


namespace {

class TestMinHeap : public ::testing::Test  {
public:
  Random random_;
  TestMinHeap() {
  }
  virtual ~TestMinHeap() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    random_.set(3324451);
  }
  virtual void TearDown(){}
};

}

bool compare_rec_dat(RecDat a, RecDat b){
  return a.score > b.score;
}

//TEST_F(TestMinHeap, compare) {
//  MinHeap heap(10);
//  Toplist<RecDat,compare_rec_dat> toplist(10);
//  RecDat rec_dat;
//  rec_dat.user = 1;
//  rec_dat.item = 0;
//  rec_dat.time = 10;
//  rec_dat.score = 2.3;
//  for (int i=0;i<15;i++) {
//    rec_dat.item = i;
//    heap.insert(rec_dat);
//    toplist.insert(rec_dat);
//  }
//  for (int i=0;i<10;i++) {
//    EXPECT_EQ(heap.get_min().item,toplist.get_min().item);
//    heap.delete_min();
//    toplist.delete_min();
//  }
//}

TEST_F(TestMinHeap, dummy) {
  MinHeap heap(3);
}

TEST_F(TestMinHeap, insert){
  MinHeap heap(50);
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

TEST_F(TestMinHeap, topK){
  MinHeap heap(3);
  RecDat recDat;
  recDat.user = 1;
  recDat.item = 2;
  recDat.score = 3;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  heap.show();
  recDat.score = 5;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  heap.show();
  recDat.score = 2;
  heap.insert(recDat);
  EXPECT_EQ(2, heap.get_min().score);
  heap.show();
  recDat.score = 87;
  heap.insert(recDat);
  EXPECT_EQ(3, heap.get_min().score);
  heap.show();
  recDat.score = 234;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  heap.show();
  recDat.score = 2;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  heap.show();
  recDat.score = 4;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  heap.show();
  recDat.score = 3;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  heap.show();
  recDat.score = -3232;
  heap.insert(recDat);
  EXPECT_EQ(5, heap.get_min().score);
  heap.show();
  heap.delete_min();
  EXPECT_EQ(87, heap.get_min().score);
  heap.show();
  heap.delete_min();
  EXPECT_EQ(234, heap.get_min().score);
  heap.show();
}

#define maxtime 10000000
TEST_F(TestMinHeap, small){
  MinHeap heap(10);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=random_.get();
    heap.insert(recDat);
  }
}

TEST_F(TestMinHeap, medium){
  MinHeap heap(100);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=random_.get();
    heap.insert(recDat);
  }
}

TEST_F(TestMinHeap, medium2){
  MinHeap heap(1000);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=random_.get();
    heap.insert(recDat);
  }
}

TEST_F(TestMinHeap, large){
  MinHeap heap(10000);
  RecDat recDat;
  recDat.user =1;
  recDat.item=2;
  recDat.time=10;
  for(int i=0;i<maxtime;i++){
    recDat.score=random_.get();
    heap.insert(recDat);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

