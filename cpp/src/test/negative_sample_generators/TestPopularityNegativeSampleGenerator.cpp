#include <vector>
#include <gtest/gtest.h>
#include "../../main/negative_sample_generators/PopularityNegativeSampleGenerator.h"
#include "../../main/recommender_data/SimpleIterator.h"


namespace {

class TestPopularityNegativeSampleGenerator : public ::testing::Test  {
public:
  PopContainer pop;
  SpMatrix trainMatrix;
  vector<int> items;
  vector<bool> itemMap;
  TestPopularityNegativeSampleGenerator() {
  }
  virtual ~TestPopularityNegativeSampleGenerator() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
  }
  virtual void TearDown(){
  }
  vector<RecDat*> recDats;
  RecDat createRecDat(int user, int item, double score){
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.score = score;
      return recDat;
  }
  void stepTime(int steps, RecommenderDataIterator* iterator){
    for(int i=0;i<steps && iterator->has_next();i++){
      RecDat* recDat = iterator->next();
      trainMatrix.insert(recDat->user,recDat->item,recDat->score);
      pop.increase(recDat->item);
      if(itemMap.size()<recDat->item+1) itemMap.resize(recDat->item+1,false);
      if(itemMap[recDat->item]==false) items.push_back(recDat->item);
      itemMap[recDat->item]=true;
    }
  }
  bool subset(vector<int> set, vector<int> subset){
    //True if subset is subset of set.
    for(int subindex=0; subindex<subset.size(); subindex++){
      int index;
      for(index=0; index<set.size();index++){
        if(subset[subindex]==set[index]){break;}
      }
      if(index==set.size()) return false;
    }
    return true;
  }
};
}

TEST_F(TestPopularityNegativeSampleGenerator,general) {
  RecommenderData recommenderData;
  vector<RecDat> recData;
  recData.push_back(createRecDat(1,1,1.0));
  recData.push_back(createRecDat(2,1,1.0));
  recData.push_back(createRecDat(3,2,1.0));
  recData.push_back(createRecDat(1,3,1.0));
  recData.push_back(createRecDat(1,5,1.0));
  recData.push_back(createRecDat(1,6,1.0));
  recData.push_back(createRecDat(1,7,1.0));
  recData.push_back(createRecDat(1,8,1.0));
  recData.push_back(createRecDat(1,2,1.0));
  recData.push_back(createRecDat(4,1,1.0));
  recData.push_back(createRecDat(5,4,1.0));
  recData.push_back(createRecDat(6,1,1.0));
  recData.push_back(createRecDat(7,1,1.0));
  recData.push_back(createRecDat(8,1,1.0));
  recData.push_back(createRecDat(9,1,1.0));
  recData.push_back(createRecDat(10,1,1.0));
  recData.push_back(createRecDat(11,1,1.0));
  recData.push_back(createRecDat(12,1,1.0));
  recData.push_back(createRecDat(13,1,1.0));
  recData.push_back(createRecDat(14,1,1.0));
  recData.push_back(createRecDat(15,1,1.0));
  recommenderData.set_rec_data(recData);
  SimpleIterator dataIterator(&recommenderData);
  PopularityNegativeSampleGeneratorParameters params;
  params.negative_rate = 3;
  params.seed = 89243;
  PopularityNegativeSampleGenerator generator(&params);
  generator.set_pop_container(&pop);
  generator.set_train_matrix(&trainMatrix);
  generator.set_recommender_data_iterator(&dataIterator);
  generator.set_items(&items);
  stepTime(18, &dataIterator);
  
  int counter = 0;

  for(int i=0;i<400;i++){
    RecDat recDat = createRecDat(3,2,1.0);
    vector<int>* samples = generator.generate(&recDat);
    EXPECT_EQ(3,samples->size());
    EXPECT_TRUE(subset(items,*samples));
    for(int j=0;j<samples->size();j++){
      if (samples->at(j)==1) counter++;
    }
  }    
  EXPECT_LT(300,counter);
  EXPECT_GT(400,counter);
}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

