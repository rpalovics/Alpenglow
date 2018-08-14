
#include <vector>
#include <gtest/gtest.h>
#include "../../main/recommender_data/RandomIterator.h"


namespace {

class TestRandomIterator : public ::testing::Test  {
public:
  RecommenderData rd;
  TestRandomIterator() {
  }
  virtual ~TestRandomIterator() {
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
TEST_F(TestRandomIterator, iterator) {
    RecDats recData;
    recData.resize(5);
    recData[0].user=0;
    recData[1].user=1;
    recData[2].user=2;
    recData[3].user=3;
    recData[4].user=4;
    rd.set_rec_data(recData);
    rd.initialize();
    RandomIterator it(&rd, 123124, "manual_shuffle");
    EXPECT_TRUE(it.has_next());
    it.next();
    it.next();
    it.next();
    it.next();
    EXPECT_TRUE(it.has_next());
    it.next();
    EXPECT_FALSE(it.has_next());
    it.shuffle();
    it.restart();
    EXPECT_TRUE(it.has_next());
    it.next();
    it.next();
    it.next();
    it.next();
    EXPECT_TRUE(it.has_next());
    it.next();
    EXPECT_FALSE(it.has_next());
    it.restart();
    EXPECT_TRUE(it.has_next());
    it.next();
    it.next();
    it.next();
    it.next();
    EXPECT_TRUE(it.has_next());
    it.next();
    EXPECT_FALSE(it.has_next());
}
TEST_F(TestRandomIterator, shuffle) {
    RecDats recData;
    recData.resize(3);
    recData[0].user=0;
    recData[1].user=1;
    recData[2].user=2;
    rd.set_rec_data(recData);
    rd.initialize();
    RandomIterator it(&rd, 123124, "manual_shuffle");
    int counter[3][3];
    for(int ii=0;ii<3;ii++){
      for(int jj=0;jj<3;jj++){
        counter[ii][jj]=0;
      }
    }
    for(int ii=0;ii<3000;ii++){
      it.shuffle();
      it.restart();
      for(int jj=0;jj<3;jj++){
        RecDat* rec_dat = it.next();
        counter[jj][rec_dat->user]++;
      }
    }
    //EXPECT_LT(900,1100);
    for(int ii=0;ii<3;ii++){
      for(int jj=0;jj<3;jj++){
        EXPECT_LT(900,counter[ii][jj]);
        EXPECT_GT(1100,counter[ii][jj]);
      }
    }
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

