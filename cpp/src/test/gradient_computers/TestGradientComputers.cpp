#include <vector>
#include <gtest/gtest.h>
#include "../../main/gradient_computers/GradientComputer.h"
#include "../../main/objectives/ObjectiveMSE.h"
#include "../../main/objectives/ObjectiveBPR.h"
#include <typeinfo>

namespace {

  class DummyModel : public Model {
    public:
      DummyModel(){
        for(int i=1; i<=5; i++){predictions.push_back(0.1*i);}
        predictVal = predictions.begin();
      }
      void update(){
        predictVal++;
        if(predictVal==predictions.end()) predictVal=predictions.begin();
      }
      void add(RecDat* rec_dat){
        add_calls.push_back(make_pair(prediction_calls.size(),rec_dat));
      }
      virtual double prediction(RecDat* rec_dat){
        prediction_calls.push_back(rec_dat);
        return (*predictVal)*(rec_dat->item);
      }
      vector<double> predictions;
      vector<double>::iterator predictVal;
      vector<RecDat*> prediction_calls;
      vector<pair<int,RecDat*>> add_calls;
  };

  class DummyModelGradientUpdater : public ModelGradientUpdater {
    public:
      void update(RecDat* recDat, double gradient){
        model->update();
        gradientData.push_back(make_pair(recDat,gradient));
      }
      void message(UpdaterMessage message){
        message_calls.push_back(message);
      }
      vector<pair<RecDat*,double>> gradientData;
      vector<UpdaterMessage> message_calls;
      void set_model(DummyModel* model_){model = model_;}
      DummyModel* model;
  };

  class DummyObjective : public ObjectivePointWise {
    double get_gradient(RecPred *rec_pred){ return rec_pred->score+rec_pred->prediction; }
  };

  class TestGradientComputer : public ::testing::Test  {
    public:
      DummyModel model;
      DummyModelGradientUpdater updater;
      GradientComputerPointWise gradient_computer;
      DummyObjective objective;
  
      vector<RecDat*> recDats;
      TestGradientComputer() {
      }
      virtual ~TestGradientComputer() {
        // You can do clean-up work that doesn't throw exceptions here.
      }
      virtual void SetUp(){
        updater.set_model(&model);
        gradient_computer.add_gradient_updater(&updater);
        gradient_computer.set_objective(&objective);
        gradient_computer.set_model(&model);
        gradient_computer.set_objective(&objective);
      }
      virtual void TearDown(){
        for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
          delete *it;
        }
      }
      RecDat* create_rec_dat(int user, int item, double time, double score){
        RecDat* recDat = new RecDat;
        recDat -> user = user;
        recDat -> item = item;
        recDat -> time = time;
        recDat -> score = score;
        recDats.push_back(recDat);
        return recDat;
      }
  };
}

TEST_F(TestGradientComputer, message) {
  EXPECT_TRUE(gradient_computer.self_test());
  gradient_computer.message(UpdaterMessage::start_of_implicit_update_cycle);
  gradient_computer.message(UpdaterMessage::end_of_implicit_update_cycle);
  gradient_computer.message(UpdaterMessage::start_of_implicit_update_cycle);
  gradient_computer.message(UpdaterMessage::end_of_implicit_update_cycle);
  gradient_computer.message(UpdaterMessage::start_of_implicit_update_cycle);
  gradient_computer.message(UpdaterMessage::end_of_implicit_update_cycle);
  ASSERT_EQ(6,updater.message_calls.size());
  EXPECT_EQ(UpdaterMessage::start_of_implicit_update_cycle,updater.message_calls[0]);
  EXPECT_EQ(UpdaterMessage::end_of_implicit_update_cycle,updater.message_calls[1]);
  EXPECT_EQ(UpdaterMessage::start_of_implicit_update_cycle,updater.message_calls[2]);
  EXPECT_EQ(UpdaterMessage::end_of_implicit_update_cycle,updater.message_calls[3]);
  EXPECT_EQ(UpdaterMessage::start_of_implicit_update_cycle,updater.message_calls[4]);
  EXPECT_EQ(UpdaterMessage::end_of_implicit_update_cycle,updater.message_calls[5]);
}
TEST_F(TestGradientComputer, update) {
  RecDat* rec_dat = create_rec_dat(1,5,11,1.0); //user=1,item=5,time=11,score=1.0
  gradient_computer.update(rec_dat);
  ASSERT_EQ(1,model.add_calls.size());
  EXPECT_EQ(0,model.add_calls[0].first); //the add call should happen before the prediction call
  ASSERT_EQ(1,model.prediction_calls.size());
  EXPECT_EQ(rec_dat,model.prediction_calls[0]);
  ASSERT_EQ(1,updater.gradientData.size());
  EXPECT_DOUBLE_EQ(5*0.1+1.0,updater.gradientData[0].second);
  rec_dat = create_rec_dat(3,2,12,1.0); //user=3,item=2,time=12,score=1.0
  gradient_computer.update(rec_dat);
  ASSERT_EQ(2,model.add_calls.size());
  EXPECT_EQ(1,model.add_calls[1].first); //the add call should happen before the prediction call
  ASSERT_EQ(2,model.prediction_calls.size());
  EXPECT_EQ(rec_dat,model.prediction_calls[1]);
  ASSERT_EQ(2,updater.gradientData.size());
  EXPECT_DOUBLE_EQ(2*0.2+1.0,updater.gradientData[1].second);
}

// TEST_F(TestGradientComputer, pairwise) {
//   gradient_computer = new GradientComputerImplicitPairWise(&objectiveBPR);
//   gradient_computer->set_model(&model);
//   learner.set_gradient_computer(gradient_computer);

//   learner.learn(create_rec_dat(1,5,1.1,1.0));
//   EXPECT_EQ(1,model.counter);
//   EXPECT_EQ(4,updater.counter);
//   ASSERT_EQ(4,model.gradientData.size());
//   double pred1 = 5*0.1/1;
//   double pred2 = 2*0.1/1;
//   double gradient = -1.0/(1.0+exp(pred1-pred2));
//   EXPECT_EQ(5,model.gradientData[0].first->item);
//   EXPECT_EQ(1,model.gradientData[0].first->score);
//   EXPECT_EQ(1,model.gradientData[0].first->user);
//   EXPECT_DOUBLE_EQ(gradient,model.gradientData[0].second);
//   EXPECT_EQ(2,model.gradientData[1].first->item);
//   EXPECT_EQ(0,model.gradientData[1].first->score);
//   EXPECT_EQ(1,model.gradientData[1].first->user);
//   EXPECT_DOUBLE_EQ(-gradient,model.gradientData[1].second);
//   pred1 = 5*0.1/3;
//   pred2 = 4*0.1/3;
//   gradient = -1.0/(1.0+exp(pred1-pred2));
//   EXPECT_EQ(5,model.gradientData[2].first->item);
//   EXPECT_EQ(1,model.gradientData[2].first->score);
//   EXPECT_EQ(1,model.gradientData[2].first->user);
//   EXPECT_DOUBLE_EQ(gradient,model.gradientData[2].second);
//   EXPECT_EQ(4,model.gradientData[3].first->item);
//   EXPECT_EQ(0,model.gradientData[3].first->score);
//   EXPECT_EQ(1,model.gradientData[3].first->user);
//   EXPECT_DOUBLE_EQ(-gradient,model.gradientData[3].second);
//   EXPECT_DOUBLE_EQ(1*0.1/5,model.prediction(create_rec_dat(1,1,1.1,1.0)));
//   learner.learn(create_rec_dat(2,1,1.1,1.0));
//   EXPECT_EQ(2,model.counter);
//   EXPECT_EQ(8,updater.counter);
//   ASSERT_EQ(8,model.gradientData.size());
//   pred1 = 1*0.1/5;
//   pred2 = 2*0.1/5;
//   gradient = -1.0/(1.0+exp(pred1-pred2));
//   EXPECT_EQ(1,model.gradientData[4].first->item);
//   EXPECT_EQ(1,model.gradientData[4].first->score);
//   EXPECT_EQ(2,model.gradientData[4].first->user);
//   EXPECT_DOUBLE_EQ(gradient,model.gradientData[4].second);
//   EXPECT_EQ(2,model.gradientData[5].first->item);
//   EXPECT_EQ(0,model.gradientData[5].first->score);
//   EXPECT_EQ(2,model.gradientData[5].first->user);
//   EXPECT_DOUBLE_EQ(-gradient,model.gradientData[5].second);
//   pred1 = 1*0.1/2;
//   pred2 = 4*0.1/2;
//   gradient = -1.0/(1.0+exp(pred1-pred2));
//   EXPECT_EQ(1,model.gradientData[6].first->item);
//   EXPECT_EQ(1,model.gradientData[6].first->score);
//   EXPECT_EQ(2,model.gradientData[6].first->user);
//   EXPECT_DOUBLE_EQ(gradient,model.gradientData[6].second);
//   EXPECT_EQ(4,model.gradientData[7].first->item);
//   EXPECT_EQ(0,model.gradientData[7].first->score);
//   EXPECT_EQ(2,model.gradientData[7].first->user);
//   EXPECT_DOUBLE_EQ(-gradient,model.gradientData[7].second);
//   EXPECT_DOUBLE_EQ(3*0.1/4,model.prediction(create_rec_dat(1,3,1.1,1.0)));
// }

// TEST_F(TestGradientComputer, listwise) {
//   gradient_computer = new GradientComputerListWise(&objectiveDummyListWise);
//   gradient_computer->set_model(&model);
//   learner.set_gradient_computer(gradient_computer);

//   learner.learn(create_rec_dat(1,5,1.1,1.0));
//   EXPECT_EQ(1,model.counter);
//   EXPECT_EQ(3,updater.counter);
//   ASSERT_EQ(3,model.gradientData.size());
//   EXPECT_EQ(5,model.gradientData[0].first->item);
//   EXPECT_EQ(1,model.gradientData[0].first->score);
//   EXPECT_EQ(1,model.gradientData[0].first->user);
//   EXPECT_DOUBLE_EQ((5*0.1/1-1.0)*1,model.gradientData[0].second);
//   EXPECT_EQ(2,model.gradientData[1].first->item);
//   EXPECT_EQ(0,model.gradientData[1].first->score);
//   EXPECT_EQ(1,model.gradientData[1].first->user);
//   EXPECT_DOUBLE_EQ((2*0.1/1-0.0)*2,model.gradientData[1].second);
//   EXPECT_EQ(4,model.gradientData[2].first->item);
//   EXPECT_EQ(0,model.gradientData[2].first->score);
//   EXPECT_EQ(1,model.gradientData[2].first->user);
//   EXPECT_DOUBLE_EQ((4*0.1/1-0.0)*3,model.gradientData[2].second);
//   EXPECT_DOUBLE_EQ(1*0.1/4,model.prediction(create_rec_dat(1,1,1.1,1.0)));
//   learner.learn(create_rec_dat(2,1,1.1,1.0));
//   EXPECT_EQ(2,model.counter);
//   EXPECT_EQ(6,updater.counter);
//   ASSERT_EQ(6,model.gradientData.size());
//   EXPECT_EQ(1,model.gradientData[3].first->item);
//   EXPECT_EQ(1,model.gradientData[3].first->score);
//   EXPECT_EQ(2,model.gradientData[3].first->user);
//   EXPECT_DOUBLE_EQ((1*0.1/4-1.0)*1,model.gradientData[3].second);
//   EXPECT_EQ(2,model.gradientData[4].first->item);
//   EXPECT_EQ(0,model.gradientData[4].first->score);
//   EXPECT_EQ(2,model.gradientData[4].first->user);
//   EXPECT_DOUBLE_EQ((2*0.1/4-0.0)*2,model.gradientData[4].second);
//   EXPECT_EQ(4,model.gradientData[5].first->item);
//   EXPECT_EQ(0,model.gradientData[5].first->score);
//   EXPECT_EQ(2,model.gradientData[5].first->user);
//   EXPECT_DOUBLE_EQ((4*0.1/4-0.0)*3,model.gradientData[5].second);
//   EXPECT_DOUBLE_EQ(3*0.1/2,model.prediction(create_rec_dat(1,3,1.1,1.0)));
// }
// TEST_F(TestOfflineGradientComputerPointWise, pointwise) {
//   //EXPECT_FALSE(gradient_computer_.selfTest());
//   gradient_computer_.set_model(&model_);
//   gradient_computer_.set_objective(&objective_);
//   //EXPECT_TRUE(gradient_computer_.selfTest());
//   double gradient = gradient_computer_.get_gradient(create_rec_dat(1,2,3.0,5.0));
//   EXPECT_DOUBLE_EQ(1+2+5.0, gradient);
//   EXPECT_EQ(0,model_.counter_);
// }

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

