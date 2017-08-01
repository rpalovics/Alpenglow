#include <gtest/gtest.h>
#include "../../main/models/baseline/PopularityModel.h"
// #include "../../main/models/baseline/PopularityTimeFrameModel.h"
#include "../../main/models/combination/CombinedModel.h"
#include "../../main/models/baseline/PopularityModelUpdater.h"
#include "../../main/models/baseline/PopularityTimeFrameModelUpdater.h"
#include "../../main/models/combination//CombinedDoubleLayerModelGradientUpdater.h"

#define TAU 1.0
namespace {
class TestCombinedModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;
    CombinedModel* model;
    CombinedDoubleLayerModelGradientUpdater* updater;
    PopularityModel* popModel;
    PopularityModelUpdater* popUpdater;
    PopularityModel* timeFrameModel;
    PopularityTimeFrameModelUpdater* timeFrameUpdater;

    TestCombinedModel(){}
    virtual ~TestCombinedModel(){}
    virtual void SetUp(){
      CombinedModelParameters combinedModelParams;
      combinedModelParams.log_file_name = "";
      combinedModelParams.log_frequency = 0;
      model = new CombinedModel(&combinedModelParams);
      popModel = new PopularityModel;
      // PopularityTimeFrameModelParameters popularityTimeFrameModelParams;
      // popularityTimeFrameModelParams.tau = TAU;
      timeFrameModel = new PopularityModel();
      CombinedDoubleLayerModelGradientUpdaterParameters combinedDoubleLayerModelGradientUpdaterParams;
      combinedDoubleLayerModelGradientUpdaterParams.learning_rate = 0.1;
      combinedDoubleLayerModelGradientUpdaterParams.regularization_rate = 0.1;
      combinedDoubleLayerModelGradientUpdaterParams.global_learning_rate = 0.1;
      combinedDoubleLayerModelGradientUpdaterParams.global_regularization_rate = 0.1;
      combinedDoubleLayerModelGradientUpdaterParams.always_learn = false;
      combinedDoubleLayerModelGradientUpdaterParams.start_combination_learning_time = 3.0;
      updater = new CombinedDoubleLayerModelGradientUpdater(&combinedDoubleLayerModelGradientUpdaterParams);
      popUpdater = new PopularityModelUpdater;
      PopularityTimeFrameModelUpdaterParameters popularityTimeFrameModelUpdaterParameters;
      popularityTimeFrameModelUpdaterParameters.tau=TAU;
      timeFrameUpdater = new PopularityTimeFrameModelUpdater(&popularityTimeFrameModelUpdaterParameters);
      model->add_model(popModel);
      model->add_model(timeFrameModel);
      updater->set_model(model);
      popUpdater->set_model(popModel);
      timeFrameUpdater->set_model(timeFrameModel);
    }
    void learn(RecDat* recDat){
      model->add(recDat);
      updater->update(recDat, -0.5);
      popModel->add(recDat);
      popUpdater->update(recDat);
      timeFrameModel->add(recDat);
      timeFrameUpdater->update(recDat);
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
      delete model;
      delete updater;
      delete popModel;
      delete popUpdater;
      delete timeFrameModel;
      delete timeFrameUpdater;
    }
    RecDat* createRecDat(int user, int item, double time){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDats.push_back(recDat);
      return recDat;
    }
};
}
TEST_F(TestCombinedModel, test){
  learn(createRecDat(1,1,0.1));
  learn(createRecDat(1,2,0.2));
  learn(createRecDat(1,2,0.3));
  learn(createRecDat(2,1,0.4));
  learn(createRecDat(3,1,0.5));
  EXPECT_EQ(1.0,model->prediction(createRecDat(1,1,0.6)));
  learn(createRecDat(2,2,1.6));
  learn(createRecDat(3,2,1.6));
  learn(createRecDat(1,2,1.6));
  learn(createRecDat(1,1,1.6));
  EXPECT_EQ(1.0,model->prediction(createRecDat(1,2,1.7)));
  learn(createRecDat(3,1,2.7));
  EXPECT_EQ(0.75,model->prediction(createRecDat(1,1,2.8)));
  learn(createRecDat(1,1,3.1));
  RecDat* recDat = createRecDat(2,1,3.1);
  EXPECT_EQ((0.5-(-0.5*1*0.1+0.1*0.1*0.5))*popModel->prediction(recDat) + (0.5-(-0.5*0.5*0.1+0.1*0.1*0.5))*timeFrameModel->prediction(recDat),model->prediction(recDat));
  //cerr << model->prediction(recDat) << " " << popModel->prediction(recDat) << " " << timeFrameModel->prediction(recDat) <<  " " << (1-(-0.5*1*0.1+0.1*0.1*1))*popModel->prediction(recDat) + (0.5-(-0.5*0.5*0.1+0.1*0.1*0.5))*timeFrameModel->prediction(recDat) << endl;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
