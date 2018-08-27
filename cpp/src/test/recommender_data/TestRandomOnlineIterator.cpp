#include <vector>
#include <gtest/gtest.h>
#include "../../main/recommender_data/RandomOnlineIterator.h"
#include "../../main/recommender_data/RandomIterator.h"


namespace {

class TestRandomOnlineIterator : public ::testing::Test  {
public:
  RecommenderData rd;
  TestRandomOnlineIterator() {
  }
  virtual ~TestRandomOnlineIterator() {
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
    for(uint i=0; i<list.size(); i++){
      if(list[i]==element) return true;
    }
    return false;
  }
};

}
TEST_F(TestRandomOnlineIterator, test) {
    RecDats recData;
    recData.resize(5);
    recData[0].user=0;
    recData[1].user=1;
    recData[2].user=2;
    recData[3].user=3;
    recData[4].user=4;
    recData[0].id=0;
    recData[1].id=1;
    recData[2].id=2;
    recData[3].id=3;
    recData[4].id=4;
    recData[0].time=0;
    recData[1].time=1;
    recData[2].time=2;
    recData[3].time=3;
    recData[4].time=4;
    rd.set_rec_data(recData);
    ASSERT_TRUE(rd.initialize());
    RandomOnlineIteratorParameters params;
    int seed = 123134;
    params.seed = seed;
    RandomOnlineIterator it(&params);
    it.set_recommender_data(&rd);
    ASSERT_TRUE(it.initialize());
    RandomIterator random_it(&rd, seed, "auto_shuffle");
    ASSERT_TRUE(random_it.initialize());

    EXPECT_TRUE(it.has_next());
    it.next();
    it.next();
    it.next();
    it.next();
    EXPECT_TRUE(it.has_next());
    it.next();
    EXPECT_FALSE(it.has_next());

    for(int i=0;i<5;i++){
      EXPECT_EQ(recData[i].time, it.get(i)->time);
      EXPECT_EQ(random_it.get_future(i)->id, it.get(i)->id);
    }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

