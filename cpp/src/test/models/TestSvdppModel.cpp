#include <gtest/gtest.h>
#include "../../main/models/factor/SvdppModel.h"
#include "../../main/models/factor/SvdppModelUpdater.h"
#include "../../main/models/factor/SvdppModelGradientUpdater.h"
#define DIMENSION 10
#define MAXUSER 13
#define MAXITEM 9

namespace {
class TestSvdppModel : public ::testing::Test { 
  public:
    vector<RecDat*> rec_dats;
    
    TestSvdppModel(){}
    virtual ~TestSvdppModel(){}
    void SetUp() override {
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat->user = user;
      rec_dat->item = item;
      rec_dat->time = time;
      rec_dat->id = rec_dats.size();
      rec_dat->category = 0;
      rec_dat->score = score;
      rec_dats.push_back(rec_dat);
      return rec_dat;
    }
    void TearDown() override {
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
    }
};
}

TEST_F(TestSvdppModel, test){
  vector<double> user_vector_weights={1.0,0.0,1.0,0.3};
  vector<double> history_weights={0.0,1.0,1.0,0.6};
  for(uint i=0;i<user_vector_weights.size();i++){
    double user_vector_weight=user_vector_weights[i];
    double history_weight=history_weights[i];
    SvdppModelParameters model_params;
    model_params.dimension=10;
    model_params.begin_min=-0.1;
    model_params.begin_max=0.1;
    model_params.initialize_all=false;
    model_params.use_sigmoid=false;
    model_params.user_vector_weight=user_vector_weight;
    model_params.history_weight=history_weight;
    model_params.norm_type="exponential";
    model_params.gamma=0.8;
    SvdppModel model(&model_params);
    SvdppModelGradientUpdaterParameters grad_upd_parameters;
    grad_upd_parameters.learning_rate=0.1;
    grad_upd_parameters.cumulative_item_updates=false;
    SvdppModelGradientUpdater gradient_updater(&grad_upd_parameters);
    SvdppModelUpdater simple_updater;
    gradient_updater.set_model(&model);
    simple_updater.set_model(&model);

    EXPECT_TRUE(model.self_test());
    EXPECT_TRUE(gradient_updater.self_test());
    EXPECT_TRUE(simple_updater.self_test());
     
    for(int i=0;i<100;i++){
      create_rec_dat(i%MAXUSER,i%MAXITEM,i,1);
    }
    for(uint i=0;i<rec_dats.size();i++){
      model.add(rec_dats[i]);
      simple_updater.update(rec_dats[i]);
    }
    for(uint i=0;i<rec_dats.size();i++){
      double orig_pred = model.prediction(rec_dats[i]);
      double gradient = orig_pred - 1;
      gradient_updater.update(rec_dats[i],gradient);
      double new_pred = model.prediction(rec_dats[i]);
      if(gradient<0){
        EXPECT_GT(new_pred,orig_pred);
      }
    }
  }
}

TEST_F(TestSvdppModel, test_norm_types){
  for(int i=0;i<100;i++){
    create_rec_dat(i%MAXUSER,i%MAXITEM,i,1);
  }

  vector<string> norm_types = {"disabled", "constant", "youngest", "recency"};
  for (auto norm_type : norm_types){
    SvdppModelParameters model_params;
    model_params.dimension=DIMENSION;
    model_params.begin_min=-0.1;
    model_params.begin_max=0.1;
    model_params.norm_type=norm_type;
    model_params.gamma=-1;
    model_params.initialize_all=false;
    model_params.user_vector_weight=1;
    model_params.history_weight=1;
    SvdppModel model(&model_params);
    SvdppModelUpdater simple_updater;
    simple_updater.set_model(&model);
    SvdppModelGradientUpdaterParameters grad_upd_params;
    grad_upd_params.learning_rate=0.14;
    grad_upd_params.cumulative_item_updates=false;
    SvdppModelGradientUpdater gradient_updater(&grad_upd_params);
    gradient_updater.set_model(&model);

    EXPECT_TRUE(model.self_test());
    EXPECT_TRUE(simple_updater.self_test());
    EXPECT_TRUE(gradient_updater.self_test());

    for(uint i=0;i<rec_dats.size();i++){
      EXPECT_EQ(0,model.prediction(rec_dats[i]));
    }
    for(uint i=0;i<rec_dats.size();i++){
      simple_updater.update(rec_dats[i]);
      model.add(rec_dats[i]);
      double orig_pred = model.prediction(rec_dats[i]);
      double gradient = orig_pred - 1;
      gradient_updater.update(rec_dats[i],gradient);
      double new_pred = model.prediction(rec_dats[i]);
      if(gradient<0){
        EXPECT_GT(new_pred,orig_pred);
      }
    }
  }
}

TEST_F(TestSvdppModel, cumulative){
  vector<string> norm_types = {"disabled", "constant", "youngest", "recency"};
  for (auto norm_type : norm_types){
    SvdppModelParameters model_params;
    model_params.dimension=DIMENSION;
    model_params.begin_min=-0.1;
    model_params.begin_max=0.1;
    model_params.norm_type=norm_type;
    model_params.gamma=-1;
    model_params.initialize_all=false;
    SvdppModel model(&model_params);
    SvdppModelUpdater simple_updater;
    simple_updater.set_model(&model);
    SvdppModelGradientUpdaterParameters grad_upd_params;
    grad_upd_params.learning_rate=0.14;
    grad_upd_params.cumulative_item_updates=true;
    SvdppModelGradientUpdater gradient_updater(&grad_upd_params);
    gradient_updater.set_model(&model);

    EXPECT_TRUE(model.self_test());
    EXPECT_TRUE(simple_updater.self_test());
    EXPECT_TRUE(gradient_updater.self_test());

    for(int i=0;i<10;i++){
      RecDat* rec_dat = create_rec_dat(1,i,1,1);
      simple_updater.update(rec_dat);
      model.add(rec_dat);
    }
    vector<RecDat*> negative_samples;
    vector<RecDat*> other_user_samples;
    for(int j=20;j<25;j++){
      RecDat* negative_sample = create_rec_dat(1,j,1,0);
      model.add(negative_sample);
      negative_sample->score = model.prediction(negative_sample); //the model does not use the score field, we store there the original prediction
      negative_samples.push_back(negative_sample);
    }
    for(int i=10;i<20;i++){
      gradient_updater.message(UpdaterMessage::start_of_implicit_update_cycle);
      RecDat* positive_sample = create_rec_dat(1,i,1,1);
      model.add(positive_sample);
      double orig_prediction = model.prediction(positive_sample);
      gradient_updater.update(positive_sample, -0.1);
      for(auto negative_sample : negative_samples){
        gradient_updater.update(negative_sample, 0.1);
      }

      gradient_updater.message(UpdaterMessage::end_of_implicit_update_cycle);
      EXPECT_LT(orig_prediction, model.prediction(positive_sample));
      for(auto negative_sample : negative_samples){
        EXPECT_GT(negative_sample->score,model.prediction(negative_sample));
      }
    }
  }
}

TEST_F(TestSvdppModel, sigmoid){
  for(int i=0;i<100;i++){
    create_rec_dat(i%MAXUSER,i%MAXITEM,i,1);
  }

  SvdppModelParameters model_params;
  model_params.use_sigmoid=true;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="constant";
  model_params.gamma=-1;
  model_params.initialize_all=false;
  model_params.user_vector_weight=1;
  model_params.history_weight=1;
  SvdppModel model(&model_params);
  SvdppModelUpdater simple_updater;
  simple_updater.set_model(&model);
  SvdppModelGradientUpdaterParameters grad_upd_params;
  grad_upd_params.learning_rate=0.14;
  grad_upd_params.cumulative_item_updates=false;
  SvdppModelGradientUpdater gradient_updater(&grad_upd_params);
  gradient_updater.set_model(&model);

  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(simple_updater.self_test());
  EXPECT_TRUE(gradient_updater.self_test());

  for(uint i=0;i<rec_dats.size();i++){
    EXPECT_DOUBLE_EQ(0.5,model.prediction(rec_dats[i]));
  }
  for(uint i=0;i<rec_dats.size();i++){
    simple_updater.update(rec_dats[i]);
    model.add(rec_dats[i]);
    double orig_pred = model.prediction(rec_dats[i]);
    double gradient = orig_pred - 1;
    gradient_updater.update(rec_dats[i],gradient);
    double new_pred = model.prediction(rec_dats[i]);
    if(gradient<0){
      EXPECT_GT(new_pred,orig_pred);
    }
  }
}

TEST_F(TestSvdppModel, clear){
  for(int i=0;i<100;i++){
    create_rec_dat(i%MAXUSER,i%MAXITEM,i,1);
  }

  SvdppModelParameters model_params;
  model_params.use_sigmoid=false;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="constant";
  model_params.gamma=-1;
  model_params.initialize_all=true;
  model_params.max_item=MAXITEM;
  model_params.max_user=MAXUSER;
  model_params.user_vector_weight=0;
  model_params.history_weight=1;

  SvdppModel model(&model_params);
  SvdppModelUpdater simple_updater;
  simple_updater.set_model(&model);

  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(simple_updater.self_test());

  for(uint i=0;i<rec_dats.size();i++){
    EXPECT_DOUBLE_EQ(0,model.prediction(rec_dats[i])); //user_vector_weight=0-->no history, 0 pred
  }
  for(uint i=0;i<rec_dats.size();i++){
    simple_updater.update(rec_dats[i]); //adding history
  }
  for(uint i=0;i<rec_dats.size();i++){
    double pred = model.prediction(rec_dats[i]);
    EXPECT_NE(0,pred); //tests initall
  }
  model.clear();
  for(uint i=0;i<rec_dats.size();i++){
    EXPECT_DOUBLE_EQ(0,model.prediction(rec_dats[i])); //tests clear
  }
  for(uint i=0;i<rec_dats.size();i++){
    simple_updater.update(rec_dats[i]);
  }
  for(uint i=0;i<rec_dats.size();i++){
    double pred = model.prediction(rec_dats[i]);
    EXPECT_NE(0,pred); //tests if factors are initialized again after clear
  }
}

TEST_F(TestSvdppModel, self_test){
  SvdppModelParameters model_params;
  model_params.dimension=0;
  model_params.begin_min=3;
  model_params.begin_max=3;
  model_params.norm_type="asdf";
  model_params.gamma=-1;
  model_params.initialize_all=true;
  model_params.max_item=-1;
  model_params.max_user=-1;
  model_params.user_vector_weight=1;
  model_params.history_weight=1;
  SvdppModel model(&model_params);
  EXPECT_FALSE(model.self_test());

  //TODO self_test of gradient updater
}

TEST_F(TestSvdppModel, expenv){
  SvdppModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=0.8;
  model_params.initialize_all=-1;
  model_params.user_vector_weight=1;
  model_params.history_weight=1;
  SvdppModel model(&model_params);
  EXPECT_FALSE(model.initialize());

  ExperimentEnvironment exp_env;
  model.set_experiment_environment(&exp_env);
  EXPECT_TRUE(model.initialize());
}

TEST_F(TestSvdppModel, expenv2){
  SvdppModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=0.8;
  model_params.initialize_all=true;
  model_params.max_item=-1;
  model_params.max_user=-1;
  model_params.user_vector_weight=1;
  model_params.history_weight=1;
  SvdppModel model(&model_params);
  EXPECT_FALSE(model.initialize());

  ExperimentEnvironment exp_env;
  model.set_experiment_environment(&exp_env);
  EXPECT_TRUE(model.initialize());
}

TEST_F(TestSvdppModel, readwrite){
  SvdppModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=0.8;
  model_params.initialize_all=-1;
  SvdppModel model(&model_params);

  stringstream ss;
  EXPECT_ANY_THROW(model.write(ss)); //UserHistoryContainer::write is not implemented
  EXPECT_ANY_THROW(model.read(ss)); //UserHistoryContainer::read is not implemented
}

TEST_F(TestSvdppModel, destructors){
  SvdppModelParameters model_params;
  SvdppModel* model = new SvdppModel(&model_params);
  delete model;

  SvdppModelUpdater* simple_updater = new SvdppModelUpdater;
  delete simple_updater;

  SvdppModelGradientUpdaterParameters grad_upd_params;
  SvdppModelGradientUpdater* gradient_updater = new SvdppModelGradientUpdater(&grad_upd_params);
  delete gradient_updater;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

