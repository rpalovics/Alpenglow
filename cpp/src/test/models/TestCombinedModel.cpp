#include <gtest/gtest.h>
#include "../../main/models/combination/CombinedModel.h"
#include "../../main/models/combination//CombinedDoubleLayerModelGradientUpdater.h"

#define TAU 1.0
namespace {

class DummyModel : public Model {
  public:
    double prediction_ = 0;
    double prediction(RecDat* rec_dat){
      return prediction_;
    }
};
class TestCombinedModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;
    DummyModel model1;
    DummyModel model2;
    DummyModel model3;
    CombinedModel* model = NULL;
    CombinedDoubleLayerModelGradientUpdater* updater = NULL;

    TestCombinedModel(){}
    virtual ~TestCombinedModel(){}
    void SetUp() override {
    }
    void TearDown() override {
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
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
TEST_F(TestCombinedModel, test_prediction){
  CombinedModelParameters combinedModelParams;
  combinedModelParams.log_file_name = "";
  combinedModelParams.log_frequency = 0;
  combinedModelParams.use_user_weights = false;
  model = new CombinedModel(&combinedModelParams);
  model->add_model(&model1);
  model->add_model(&model2);
  model->add_model(&model3);
  int base_model_num = 3;
  for(auto weight : model->global_weights){ //starting weights equal
    EXPECT_DOUBLE_EQ(weight, 1.0/base_model_num);
  }
  ASSERT_EQ(model->global_weights.size(), base_model_num);
  model->global_weights[0]=1.0; //set arbitrary weights
  model->global_weights[1]=2.0;
  model->global_weights[2]=3.0;

  RecDat* rec_dat;
  for(int i=0; i<5; i++){ //create 5 users and 5 items
    rec_dat = createRecDat(i,i,0);
    model->add(rec_dat);
  }

  rec_dat = createRecDat(5,5,0);
  model1.prediction_=5.0; //set arbitrary predictions
  model2.prediction_=3.0;
  model3.prediction_=1.0;
  double prediction = model->prediction(rec_dat);
  EXPECT_EQ(1.0*5.0+2.0*3.0+3.0*1.0, prediction);

  delete model;
}

TEST_F(TestCombinedModel, test_prediction_user_w){
  CombinedModelParameters combinedModelParams;
  combinedModelParams.log_file_name = "";
  combinedModelParams.log_frequency = 0;
  combinedModelParams.use_user_weights = true; //turn on user weights
  model = new CombinedModel(&combinedModelParams);
  model->add_model(&model1);
  model->add_model(&model2);
  model->add_model(&model3);
  int base_model_num = 3;
  for(auto weight : model->global_weights){ //starting weights equal
    EXPECT_DOUBLE_EQ(weight, 1.0/base_model_num);
  }
  ASSERT_EQ(model->global_weights.size(), 3);
  model->global_weights[0]=1.0; //set arbitrary weights
  model->global_weights[1]=2.0;
  model->global_weights[2]=3.0;

  RecDat* rec_dat;
  for(int i=0; i<5; i++){ //create 5 users and 5 items
    rec_dat = createRecDat(i,i,0); //i=user=item
    model->add(rec_dat);
    EXPECT_EQ(model->user_weights.size(), i+1);
    ASSERT_GE(model->user_weights.size(), i+1);
    vector<double>* weights = model->user_weights[i];
    ASSERT_NE(weights, nullptr);
    ASSERT_EQ(weights->size(), base_model_num);
    weights->at(0)=1.0+i; //set arbitrary user weights
    weights->at(1)=3.0+i;
    weights->at(2)=4.0+i;
  }

  int user = 2;
  rec_dat = createRecDat(user,5,0);
  model1.prediction_=5.0; //set arbitrary predictions
  model2.prediction_=3.0;
  model3.prediction_=1.0;
  double prediction = model->prediction(rec_dat);
  double pred_global = 1.0*5.0+2.0*3.0+3.0*1.0;
  double pred_user = 3.0*5.0+5.0*3.0+6.0*1.0;
  EXPECT_EQ(pred_global+pred_user, prediction);

  user = 3;
  rec_dat = createRecDat(user,5,0);
  prediction = model->prediction(rec_dat);
  pred_user = 4.0*5.0+6.0*3.0+7.0*1.0;
  EXPECT_EQ(pred_global+pred_user, prediction);

  delete model;
}

TEST_F(TestCombinedModel, test_update_no_always){
  CombinedModelParameters combinedModelParams; //set up combined model
  combinedModelParams.log_file_name = "";
  combinedModelParams.log_frequency = 0;
  combinedModelParams.use_user_weights = true;
  model = new CombinedModel(&combinedModelParams);
  model->add_model(&model1);
  model->add_model(&model2);
  model->add_model(&model3);
  int base_model_num = 3;
  ASSERT_EQ(model->global_weights.size(), base_model_num);
  model->global_weights[0]=1.0; //set arbitrary starting weights
  model->global_weights[1]=2.0;
  model->global_weights[2]=3.0;
  
  //set up updater
  CombinedDoubleLayerModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.3;
  updater_params.regularization_rate = 0.001;
  updater_params.global_learning_rate = 0.2;
  updater_params.global_regularization_rate = 0.001;
  updater_params.always_learn = false; //won't learn if a predictions is 0
  updater_params.start_combination_learning_time = 10.0; //start learning later
  updater = new CombinedDoubleLayerModelGradientUpdater(&updater_params);
  ASSERT_FALSE(updater->self_test());
  updater->set_model(model);

  RecDat* rec_dat;
  for(int i=0; i<5; i++){ //create 5 users and 5 items
    rec_dat = createRecDat(i,i,i+1);
    model->add(rec_dat);
  }

  rec_dat = createRecDat(3,5,6.0); //time still under start learning time
  model1.prediction_=5.0; //set arbitrary predictions, no 0
  model2.prediction_=3.0;
  model3.prediction_=1.0;
  model->add(rec_dat);
  double prediction = model->prediction(rec_dat); //initial prediction
  updater->update(rec_dat, -0.5);
  double new_pred = model->prediction(rec_dat);
  EXPECT_DOUBLE_EQ(prediction,new_pred); //no learning yet

  rec_dat = createRecDat(3,4,16.0); //time after start learning time
  model->add(rec_dat);
  prediction = model->prediction(rec_dat); //initial prediction
  updater->update(rec_dat, -0.5); //should increase prediction
  new_pred = model->prediction(rec_dat);
  EXPECT_LT(prediction,new_pred);

  rec_dat = createRecDat(2,3,16.1); //time after start learning time
  model->add(rec_dat);
  model2.prediction_=0.0; //prediction 0 of a base model
  prediction = model->prediction(rec_dat); //initial prediction
  updater->update(rec_dat, -0.5); //should not learn
  new_pred = model->prediction(rec_dat);
  EXPECT_DOUBLE_EQ(prediction,new_pred); //no learning

  delete model;
  delete updater;
}

TEST_F(TestCombinedModel, test_update_always){
  CombinedModelParameters combinedModelParams; //set up combined model
  combinedModelParams.log_file_name = "";
  combinedModelParams.log_frequency = 0;
  combinedModelParams.use_user_weights = false;
  model = new CombinedModel(&combinedModelParams);
  model->add_model(&model1);
  model->add_model(&model2);
  model->add_model(&model3);
  int base_model_num = 3;
  ASSERT_EQ(model->global_weights.size(), base_model_num);
  model->global_weights[0]=1.0; //set arbitrary starting weights
  model->global_weights[1]=2.0;
  model->global_weights[2]=3.0;

  //set up updater
  CombinedDoubleLayerModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.3;
  updater_params.regularization_rate = 0.001;
  updater_params.global_learning_rate = 0.2;
  updater_params.global_regularization_rate = 0.001;
  updater_params.always_learn = true; //still learns if a predictions is 0
  updater_params.start_combination_learning_time = 10.0; //start learning later
  updater = new CombinedDoubleLayerModelGradientUpdater(&updater_params);
  updater->set_model(model);

  RecDat* rec_dat;
  for(int i=0; i<5; i++){ //create 5 users and 5 items
    rec_dat = createRecDat(i,i,i+1);
    model->add(rec_dat);
  }

  rec_dat = createRecDat(4,3,16.1); //time after start learning time
  model->add(rec_dat);
  model1.prediction_=5.0; //set arbitrary predictions
  model2.prediction_=0.0; //prediction 0 of a base model
  model3.prediction_=1.0;
  double prediction = model->prediction(rec_dat); //initial prediction
  updater->update(rec_dat, -0.5); //should increase
  double new_pred = model->prediction(rec_dat);
  EXPECT_LT(prediction,new_pred);

  delete model;
  delete updater;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
