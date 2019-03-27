#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include "../../main/models/factor/FactorModel.h"
#include "../../main/models/factor/FactorModelGradientUpdater.h"
#include "../../main/utils/Factors.h"
#define DIMENSION 3

namespace {
class TestFactorModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;
    FactorModel* model;
    FactorModelGradientUpdater * updater;

    TestFactorModel(){}
    virtual ~TestFactorModel(){}
    virtual void SetUp(){
      FactorModelParameters params;
      params.dimension = DIMENSION;
      params.begin_min = -0.001;
      params.begin_max = 0.001;
      params.initialize_all = false;
      params.use_sigmoid = false;
      model = new FactorModel(&params);
      ASSERT_TRUE(model->initialize());
      FactorModelGradientUpdaterParameters updaterParams;
      updaterParams.learning_rate = 0.1;
      updaterParams.regularization_rate = 0.1;
      updaterParams.turn_off_user_factor_updates = false;
      updaterParams.turn_off_item_factor_updates = false;
      updater = new FactorModelGradientUpdater(&updaterParams);
      updater->set_model(model);
    }
    void initFactors(Factors* factors){
      for(int i=0;i<10;i++){
        factors->init(i);
        factors->set_zero(i);
        factors->set(i,i%DIMENSION,1);
      }
    }
    void initFactors2(Factors* factors){
      for(int i=0;i<10;i++){
        factors->init(i);
        factors->set_zero(i);
        factors->set(i,i%DIMENSION,1);
        factors->set(i,(i%2+i%5)%DIMENSION,2);
      }
    }
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
      delete model;
      delete updater;
    }
};

class TestFactorModel2 : public ::testing::Test { //test initialize all functionality
  public:
    FactorModel* model;

    TestFactorModel2(){}
    virtual ~TestFactorModel2(){}
    virtual void SetUp(){
      FactorModelParameters params;
      params.dimension = DIMENSION;
      params.begin_min = -0.01;
      params.begin_max = 0.01;
      params.initialize_all=true;
      params.max_item = 10;
      params.max_user = 6;
      model = new FactorModel(&params);
      ASSERT_TRUE(model->initialize());
    }
    virtual void TearDown(){
      delete model;
    }
};


}

TEST_F(TestFactorModel, test){
  initFactors(&model->user_factors_);
  initFactors(&model->item_factors_);
  /*
  for(int u=0;u<10;u++){
    for(int i=0;i<10;i++){
      cerr << model->prediction(createRecDat(u,i,3.0,1.0)) << " ";
    }
    cerr << endl;
  }
*/

  //prediction
  EXPECT_DOUBLE_EQ(1.0,model->prediction(createRecDat(0,0,1.0,1.0)));
  EXPECT_DOUBLE_EQ(1.0,model->prediction(createRecDat(1,4,1.0,1.0)));
  EXPECT_DOUBLE_EQ(1.0,model->prediction(createRecDat(2,8,1.0,1.0)));
  EXPECT_DOUBLE_EQ(1.0,model->prediction(createRecDat(5,5,1.0,1.0)));
  EXPECT_DOUBLE_EQ(0.0,model->prediction(createRecDat(3,4,1.0,1.0)));
  EXPECT_DOUBLE_EQ(0.0,model->prediction(createRecDat(9,2,1.0,1.0)));
  EXPECT_DOUBLE_EQ(0.0,model->prediction(createRecDat(2,0,1.0,1.0)));

  //update
  updater->update(createRecDat(2,3,1.5,1.0),-0.5);
  
  //prediction
  EXPECT_DOUBLE_EQ(1.0,model->prediction(createRecDat(0,0,2.0,1.0)));
  EXPECT_DOUBLE_EQ(0.0,model->prediction(createRecDat(5,6,2.0,1.0)));
  EXPECT_DOUBLE_EQ(0.99,model->prediction(createRecDat(2,2,2.0,1.0))); //??? kicsit teved, miert? Tobbi is ezert van kikommentezve.
  EXPECT_DOUBLE_EQ(0.0,model->prediction(createRecDat(2,4,2.0,1.0)));
  EXPECT_DOUBLE_EQ(0.99,model->prediction(createRecDat(0,3,2.0,1.0)));
  EXPECT_DOUBLE_EQ(0.99,model->prediction(createRecDat(3,3,2.0,1.0)));
  EXPECT_DOUBLE_EQ(0.05,model->prediction(createRecDat(8,3,2.0,1.0)));
  /*
  for(int u=0;u<10;u++){
    for(int i=0;i<10;i++){
      cerr << model->prediction(createRecDat(u,i,3.0,1.0)) << " ";
    }
    cerr << endl;
  }
*/
  //update
  updater->update(createRecDat(7,4,2.5,1.0),-0.5);

  //prediction
  EXPECT_DOUBLE_EQ(1.0,model->prediction(createRecDat(0,0,3.0,1.0)));
  EXPECT_DOUBLE_EQ(0.0,model->prediction(createRecDat(9,8,3.0,1.0)));
  EXPECT_DOUBLE_EQ(0.99,model->prediction(createRecDat(3,3,3.0,1.0)));
  EXPECT_DOUBLE_EQ(0.05,model->prediction(createRecDat(2,9,3.0,1.0)));
  EXPECT_DOUBLE_EQ(1.04,model->prediction(createRecDat(1,4,3.0,1.0)));
  EXPECT_DOUBLE_EQ(1.04,model->prediction(createRecDat(4,4,3.0,1.0)));
  EXPECT_DOUBLE_EQ(1.04,model->prediction(createRecDat(7,1,3.0,1.0)));
  
  /*
  for(int u=0;u<10;u++){
    for(int i=0;i<10;i++){
      cerr << model->prediction(createRecDat(u,i,3.0,1.0)) << " ";
    }
    cerr << endl;
  }
  */ 

}

TEST_F(TestFactorModel, similarity){
  initFactors2(&model->item_factors_);
  EXPECT_DOUBLE_EQ(1,model->similarity(0,0)); //2*2/2/2
  EXPECT_DOUBLE_EQ(1,model->similarity(1,1)); //(0*0+1*1+2*2)/5
  EXPECT_DOUBLE_EQ(1,model->similarity(3,6)); //azonos a ket vektor

  EXPECT_DOUBLE_EQ(0,model->similarity(0,1));
  EXPECT_DOUBLE_EQ(0,model->similarity(0,2));
  EXPECT_DOUBLE_EQ(0,model->similarity(0,4));
  EXPECT_DOUBLE_EQ(0,model->similarity(0,5));
  EXPECT_DOUBLE_EQ(0,model->similarity(2,7));
  EXPECT_DOUBLE_EQ(0,model->similarity(4,9));

  EXPECT_DOUBLE_EQ(4.0/5.0,model->similarity(6,7));
  EXPECT_DOUBLE_EQ(4.0/5.0,model->similarity(8,9));
  EXPECT_DOUBLE_EQ(4.0/5.0,model->similarity(1,5));
  EXPECT_DOUBLE_EQ(4.0/5.0,model->similarity(5,6));

  //test symmetry
  for(int i=0;i<10;i++){
    for(int j=0;j<10;j++){
      EXPECT_DOUBLE_EQ(model->similarity(i,j), model->similarity(j,i));
    }
  }

}
TEST_F(TestFactorModel, persistence){
  RecDat* recDat1 = createRecDat(2,3,1.5,1.0);
  RecDat* recDat2 = createRecDat(7,4,2.5,1.0);
  RecDat* recDat3 = createRecDat(3,2,2.5,1.0);
  model->add(createRecDat(2,3,1.5,1.0));
  model->add(createRecDat(7,4,2.5,1.0));
  model->add(createRecDat(3,2,2.5,1.0));
  ofstream ofs("testFile.txt", ios::binary);
  model->write(ofs);
  ofs.close();

  FactorModelParameters params;
  params.dimension = DIMENSION;
  params.begin_min = -0.001;
  params.begin_max = 0.001;
  params.use_sigmoid = false;
  params.initialize_all = false;
  FactorModel * model2 = new FactorModel(&params);
  ifstream ifs("testFile.txt", ios::binary);
  model2->read(ifs);
  ifs.close();

  EXPECT_EQ(model->prediction(recDat1),model2->prediction(recDat1));
  EXPECT_EQ(model->prediction(recDat2),model2->prediction(recDat2));
  EXPECT_EQ(model->prediction(recDat3),model2->prediction(recDat3));

  delete model2;
  remove("testFile.txt");

}

TEST_F(TestFactorModel, clear){
  RecDat* recDat1 = createRecDat(2,3,1.5,1.0);
  RecDat* recDat2 = createRecDat(7,4,2.5,1.0);
  RecDat* recDat3 = createRecDat(3,2,2.5,1.0);
  model->add(recDat1);
  model->add(recDat2);
  model->add(recDat3);
  model->clear();
  EXPECT_EQ(0,model->prediction(recDat1));
  EXPECT_EQ(0,model->prediction(recDat2));
  EXPECT_EQ(0,model->prediction(recDat3));
}

TEST_F(TestFactorModel2, initAll){
  RecDat recDat;
  for(int user=0;user<7;user++){
    recDat.user = user;
    for(int item=0;item<11;item++){
      recDat.item = item;
      EXPECT_NE(0,model->prediction(&recDat));
    }
  } 
}

TEST_F(TestFactorModel2, clear){
  model->clear();
  RecDat recDat;
  for(int user=0;user<7;user++){
    recDat.user = user;
    for(int item=0;item<11;item++){
      recDat.item = item;
      EXPECT_NE(0,model->prediction(&recDat));
    }
  } 
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
