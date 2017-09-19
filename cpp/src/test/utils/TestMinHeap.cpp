#include <vector>
#include <gtest/gtest.h>
#include "../../main/utils/MinHeap.h"
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

