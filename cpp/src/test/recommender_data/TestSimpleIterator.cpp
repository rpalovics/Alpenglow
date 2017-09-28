#include <vector>
#include <gtest/gtest.h>
#include "../../main/recommender_data/SimpleIterator.h"


namespace {

class TestSimpleIterator : public ::testing::Test  {
public:
  RecommenderData rd;
  TestSimpleIterator() {
  }
  virtual ~TestSimpleIterator() {
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
};

}
TEST_F(TestSimpleIterator, size) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.0,1.0));
    recData.push_back(createRecDat(1,6,10.0,1.0));
    recData.push_back(createRecDat(2,8,10.0,1.0));
    rd.set_rec_data(recData);
    SimpleIterator it(&rd);
    EXPECT_EQ(3,it.size());
}

TEST_F(TestSimpleIterator, iterate) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.0,1.0));
    recData.push_back(createRecDat(1,6,10.0,1.0));
    recData.push_back(createRecDat(2,8,10.0,1.0));
    rd.set_rec_data(recData);
    SimpleIterator it(&rd);
    EXPECT_EQ(3,it.size());
    EXPECT_TRUE(it.has_next());
    RecDat* recDat = it.next();
    EXPECT_EQ(3, recDat->item);
    EXPECT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(6, recDat->item);
    EXPECT_TRUE(it.has_next());
    recDat = it.next();
    EXPECT_EQ(8, recDat->item);
    EXPECT_FALSE(it.has_next());
}

TEST_F(TestSimpleIterator, get) {
    RecDats recData;
    recData.push_back(createRecDat(2,3,10.0,1.0));
    recData.push_back(createRecDat(1,6,10.0,1.0));
    recData.push_back(createRecDat(2,8,10.0,1.0));
    rd.set_rec_data(recData);
    SimpleIterator it(&rd);
    EXPECT_EQ(3,it.size());
    EXPECT_EQ(3, (it.get_future(0))->item);
    EXPECT_EQ(6, (it.get_future(1))->item);
    EXPECT_EQ(8, (it.get_future(2))->item);
    EXPECT_EQ(6, (it.get_future(1))->item);
    EXPECT_EQ(3, (it.get_future(0))->item);
    EXPECT_EQ(8, (it.get_future(2))->item);
}

//disabled function
//TEST_F(TestSimpleIterator, matrix) {
//    RecDats recData;
//    recData.push_back(createRecDat(2,3,10.0,1.0));
//    recData.push_back(createRecDat(1,6,10.0,1.0));
//    recData.push_back(createRecDat(2,8,10.0,1.0));
//    rd.set_rec_data(recData);
//    SimpleIterator it(&rd);
//    EXPECT_EQ(rd.matrix(),it.matrix());
//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

