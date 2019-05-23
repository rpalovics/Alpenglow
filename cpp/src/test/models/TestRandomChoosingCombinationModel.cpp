#include <gtest/gtest.h>
#include <sstream>
#include "../../main/models/combination/RandomChoosingCombinedModel.h"
#include "../../main/models/baseline/TransitionProbabilityModel.h"
#include "../../main/models/combination/RandomChoosingCombinedModelExpertUpdater.h"

namespace {
class DummyModel : public Model {
  public:
    void add(RecDat*){ add_counter_++; }
    int add_counter_ = 0;
    double prediction(RecDat*){ return my_prediction_; }
    double my_prediction_ = 0.5;
    void read(istream&){ read_counter_++; }
    int read_counter_ = 0;
    void write(ostream&){ write_counter_++; }
    int write_counter_ = 0;
};
class TestRandomChoosingCombinedModel : public ::testing::Test { 
  public:
    DummyModel model1, model2, model3;
    vector<RecDat*> rec_dats;
    ExperimentEnvironment experiment_environment;
    OnlineExperimentParameters experiment_parameters;
    TestRandomChoosingCombinedModel(){}
    virtual ~TestRandomChoosingCombinedModel(){}
    virtual void SetUp(){
      model1.my_prediction_ = 1;
      model2.my_prediction_ = 2;
      model3.my_prediction_ = 3;
      experiment_parameters.random_seed = 231243;
      experiment_environment.set_parameters(&experiment_parameters);
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat -> user = user;
      rec_dat -> item = item;
      rec_dat -> time = time;
      rec_dat -> id = rec_dats.size();
      rec_dat -> category = 0;
      rec_dat -> score = score;
      rec_dats.push_back(rec_dat);
      return rec_dat;
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
    }
};
class DummyEvaluator : public Evaluator {
  public:
    double get_loss(RecDat*){ return my_loss_; }
    double my_loss_ = 0.5;
};
class TestRandomChoosingCombinedModelExpertUpdater : public ::testing::Test { 
  public:
    DummyModel model1, model2, model3;
    vector<RecDat*> rec_dats;
    ExperimentEnvironment experiment_environment;
    OnlineExperimentParameters experiment_parameters;
    TestRandomChoosingCombinedModelExpertUpdater(){}
    virtual ~TestRandomChoosingCombinedModelExpertUpdater(){}
    virtual void SetUp(){
      model1.my_prediction_ = 1;
      model2.my_prediction_ = 2;
      model3.my_prediction_ = 3;
      experiment_parameters.random_seed = 231243;
      experiment_environment.set_parameters(&experiment_parameters);
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat -> user = user;
      rec_dat -> item = item;
      rec_dat -> time = time;
      rec_dat -> id = rec_dats.size();
      rec_dat -> category = 0;
      rec_dat -> score = score;
      rec_dats.push_back(rec_dat);
      return rec_dat;
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
    }
};

} //namespace

TEST_F(TestRandomChoosingCombinedModelExpertUpdater, abs_err){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  RandomChoosingCombinedModelExpertUpdaterParameters updater_params;
  updater_params.eta=0.1;
  updater_params.loss_type="abs";
  RandomChoosingCombinedModelExpertUpdater updater(&updater_params);
  //updater.set_model(&model);
  model.inject_wms_into(&updater);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(updater.initialize());
  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(updater.self_test());
  updater.update(create_rec_dat(1,2,10.0,1.7));
  updater.update(create_rec_dat(1,3,10.0,1.7));
  updater.update(create_rec_dat(1,4,10.0,1.7));
  updater.update(create_rec_dat(1,5,10.0,1.7));
  //1______1.7__2___________3 --> err of 2 < err of 1 < err of 3
  vector<int> predictions(4);
  for(int i=0;i<1000;i++){
    double prediction = model.prediction(create_rec_dat(1,5,10.0,1));
    ASSERT_LT(prediction,4.0);
    ASSERT_GE(prediction,0.0);
    predictions[prediction]++;
  }
  EXPECT_GT(predictions[2],predictions[1]);
  EXPECT_GT(predictions[1],predictions[3]);
}
TEST_F(TestRandomChoosingCombinedModelExpertUpdater, dcg_err){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  RandomChoosingCombinedModelExpertUpdaterParameters updater_params;
  updater_params.eta=0.1;
  updater_params.loss_type="dcg";
  RandomChoosingCombinedModelExpertUpdater updater(&updater_params);
  //updater.set_model(&model);
  model.inject_wms_into(&updater);
  updater.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(updater.initialize());
  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(updater.self_test());
  updater.update(create_rec_dat(1,2,10.0,1));
  updater.update(create_rec_dat(1,3,10.0,1));
  updater.update(create_rec_dat(1,4,10.0,1));
  updater.update(create_rec_dat(1,5,10.0,1));
  experiment_environment.update(create_rec_dat(1,2,10.0,1));
  experiment_environment.update(create_rec_dat(1,3,10.0,1));
  experiment_environment.update(create_rec_dat(1,4,10.0,1));
  experiment_environment.update(create_rec_dat(1,5,10.0,1));
  vector<int> predictions(4);
  for(int i=0;i<90000;i++){
    double prediction = model.prediction(create_rec_dat(2,5,10.0,1));
    ASSERT_LT(prediction,4.0);
    ASSERT_GE(prediction,0.0);
    predictions[prediction]++;
  }
  for(int i=1;i<4;i++){
    EXPECT_NEAR(30000,predictions[i],1000);
  }

}
TEST_F(TestRandomChoosingCombinedModelExpertUpdater, weights){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  RandomChoosingCombinedModelExpertUpdaterParameters updater_params;
  updater_params.eta=0.1;
  updater_params.loss_type="other";
  RandomChoosingCombinedModelExpertUpdater updater(&updater_params);
  //updater.set_model(&model);
  model.inject_wms_into(&updater);
  vector<Evaluator*> evaluators;
  for(int i=0;i<3;i++){
    DummyEvaluator* evaluator = new DummyEvaluator;
    evaluator->my_loss_ = i+1;
    evaluators.push_back(evaluator);
  }
  updater.set_evaluators(evaluators);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(updater.initialize());
  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(updater.self_test());
  updater.update(create_rec_dat(1,2,10.0,1));
  updater.update(create_rec_dat(1,3,10.0,1));
  updater.update(create_rec_dat(1,4,10.0,1));
  updater.update(create_rec_dat(1,5,10.0,1));
  vector<int> predictions(4);
  for(int i=0;i<1000;i++){
    double prediction = model.prediction(create_rec_dat(1,5,10.0,1));
    EXPECT_LT(prediction,4.0);
    EXPECT_GE(prediction,0.0);
    predictions[prediction]++;
  }
  EXPECT_GT(predictions[1],predictions[2]);
  EXPECT_GT(predictions[2],predictions[3]);
}

TEST_F(TestRandomChoosingCombinedModel, rsi){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  model.prediction(create_rec_dat(10,10,10.0,1));
  EXPECT_EQ(NULL,model.get_ranking_score_iterator(10));

  RandomChoosingCombinedModel model2(&params);
  TransitionProbabilityModel trans_model;
  model2.add_model(&trans_model);
  EXPECT_TRUE(model2.initialize());
  EXPECT_TRUE(model2.self_test());
  model2.prediction(create_rec_dat(10,10,10.0,1));
  EXPECT_TRUE(NULL!=model2.get_ranking_score_iterator(10));
}

TEST_F(TestRandomChoosingCombinedModel, add){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  model.add(create_rec_dat(1,2,10.0,1));
  EXPECT_EQ(1,model1.add_counter_);
  EXPECT_EQ(1,model2.add_counter_);
  EXPECT_EQ(1,model3.add_counter_);
  model.add(create_rec_dat(2,2,10.0,1));
  EXPECT_EQ(2,model1.add_counter_);
  EXPECT_EQ(2,model2.add_counter_);
  EXPECT_EQ(2,model3.add_counter_);
  model.add(create_rec_dat(2,3,10.0,1));
  EXPECT_EQ(3,model1.add_counter_);
  EXPECT_EQ(3,model2.add_counter_);
  EXPECT_EQ(3,model3.add_counter_);
}
TEST_F(TestRandomChoosingCombinedModel, prediction_id_change){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  //initial distribution: 0.33 0.33 0.33
  //id change -> generate new model, p of model eq = 0.33
  int last_model = 0;
  int eq = 0;
  int all = 1000;
  for(int i=0;i<all;i++){
    int pred = model.prediction(create_rec_dat(2,3,10.0,1));
    if(pred==last_model) eq++;
    last_model = pred;
  }
  EXPECT_NEAR(all/3,eq,25);
}

TEST_F(TestRandomChoosingCombinedModel, prediction_user_change){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  //initial distribution: 0.33 0.33 0.33
  //user change -> generate new model, p of model eq = 0.33
  int last_model = 0;
  int eq = 0;
  int all = 1000;
  RecDat* rec_dat = create_rec_dat(2,3,10.0,1);
  for(int i=0;i<all;i++){
    rec_dat->user = i;
    int pred = model.prediction(rec_dat);
    if(pred==last_model) eq++;
    last_model = pred;
  }
  EXPECT_NEAR(all/3,eq,25);
}

TEST_F(TestRandomChoosingCombinedModel, prediction_item_change){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  //initial distribution: 0.33 0.33 0.33
  //item change -> do not generate new model
  int last_model = 0;
  int eq = 0;
  int all = 10;
  for(int j=0;j<10;j++){
    RecDat* rec_dat = create_rec_dat(2,3,10.0,1);
    for(int i=0;i<all;i++){
      rec_dat->item = i;
      int pred = model.prediction(rec_dat);
      if(pred==last_model) eq++;
      last_model = pred;
    }
    EXPECT_EQ(all-1,eq);
    eq = 0;
    last_model = 0;
  }
}

TEST_F(TestRandomChoosingCombinedModel, prediction_distribution){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  int all = 10000;
  vector<int> experienced_distribution(3);
  //distribution: 0.1 0.7 0.2
  vector<double> expected_distribution = {0.1,0.7,0.2};
  model.wms_.distribution_ = expected_distribution;
  //id change -> generate new model
  for(int i=0;i<all;i++){
    uint pred = model.prediction(create_rec_dat(2,3,10.0,1))-1;
    ASSERT_LT(pred,3);
    experienced_distribution[pred]++; 
  }
  for(uint i=0;i<expected_distribution.size();i++){
    EXPECT_NEAR(expected_distribution[i], (double)experienced_distribution[i]/all,0.05);
  }

  experienced_distribution.clear();
  experienced_distribution.resize(3,0);
  //distribution: 0.3 0.3 0.1 (->0.3 0.3 0.4)
  model.wms_.distribution_ = {0.3,0.3,0.1};
  expected_distribution = {0.3,0.3,0.4};
  //id change -> generate new model
 
  for(int i=0;i<all;i++){
    uint pred = model.prediction(create_rec_dat(2,3,10.0,1))-1;
    ASSERT_LT(pred,3);
    experienced_distribution[pred]++; 
  }
  for(uint i=0;i<expected_distribution.size();i++){
    EXPECT_NEAR(expected_distribution[i], (double)experienced_distribution[i]/all,0.05);
  }
  
}
TEST_F(TestRandomChoosingCombinedModel, read_write){
  RandomChoosingCombinedModelParameters params;
  params.seed=231234;
  RandomChoosingCombinedModel model(&params);
  model.add_model(&model1);
  model.add_model(&model2);
  model.add_model(&model3);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());
  stringstream ss;
  model.write(ss);
  EXPECT_EQ(1,model1.write_counter_);
  EXPECT_EQ(1,model2.write_counter_);
  EXPECT_EQ(1,model3.write_counter_);
  model.read(ss);
  EXPECT_EQ(1,model1.read_counter_);
  EXPECT_EQ(1,model2.read_counter_);
  EXPECT_EQ(1,model3.read_counter_);
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
