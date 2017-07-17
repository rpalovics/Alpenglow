#include <gtest/gtest.h>
#include <stdio.h>
#include <fstream>
#include "../../main/models/factor/EigenFactorModel.h"
#include "../../main/models/factor/EigenFactorModelALSUpdater.h"
#include "../../main/utils/EigenFactors.h"

namespace {
class TestEigenFactorModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;

    TestEigenFactorModel(){}
    virtual ~TestEigenFactorModel(){}
    virtual void SetUp(){}
    RecDat* createRecDat(int user, int item, double time, double score){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDat -> category = 0;
      recDat -> score = score;
      recDats.push_back(recDat);
      return recDat;
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
    }
};

}

TEST_F(TestEigenFactorModel, testEigenFactors){
  EXPECT_DOUBLE_EQ(1,1);
  EigenFactorsParameters ep;
  ep.seed = 1345;
  EigenFactors ef(&ep);
  ef.reinitialize_factors(10, 50, -0.01, 0.01);
  for(int i=0; i<10; i++){
    for(int j=0; j<=50; j++){
      EXPECT_GE(0.01, ef.factors(j,i));
      EXPECT_LE(-0.01, ef.factors(j,i));
    }
  }
  ef.reinitialize_factors(10, 50, -0.05, 0.01);
  for(int i=0; i<10; i++){
    for(int j=0; j<=50; j++){
      EXPECT_GE(0.01, ef.factors(j,i));
      EXPECT_LE(-0.05, ef.factors(j,i));
    }
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
