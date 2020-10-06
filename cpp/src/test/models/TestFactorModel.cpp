#include <gtest/gtest.h>
#include <sstream>
#include "../../main/models/factor/FactorModel.h"
#include "../../main/models/factor/FactorModelGradientUpdater.h"
#include "../../main/models/factor/FactorModelUpdater.h"

#define MAXITEM 3
#define MAXUSER 4

namespace {
class DummyRecency : public Recency {
  public:
    double get(int entity, double actual_time) override {
      return 0.3;
    }
    void update(int entity, double actual_time) override {}
};
class TestFactorModel : public ::testing::Test { 
  public:
    vector<RecDat*> rec_dats;
    TestFactorModel(){}
    virtual ~TestFactorModel(){}
    void SetUp() override {
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat -> user = user;
      rec_dat -> item = item;
      rec_dat -> time = time;
      rec_dat -> category = 0;
      rec_dat -> score = score;
      rec_dat -> id = rec_dats.size();
      rec_dats.push_back(rec_dat);
      return rec_dat;
    }
    void TearDown() override {
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
      rec_dats.resize(0);
    }
};
}

TEST_F(TestFactorModel, base){
  //(1) setup, init, self_test
  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = false;
  FactorModel model(&model_params);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());

  FactorModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.2;
  updater_params.regularization_rate = 0.0001;
  updater_params.learning_rate_bias = 0.2;
  updater_params.regularization_rate_bias = 0.0001;
  FactorModelGradientUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());

  //(2) initial prediction
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0, model.prediction(rec_dat));
  }

  //(3) add, update
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,(i+2)%MAXITEM,i,1); 
    model.add(rec_dat);
    double original_pred = model.prediction(rec_dat);
    updater.update(rec_dat,-1); //negative gradient -> prediction should increase
    double new_pred = model.prediction(rec_dat);
    EXPECT_GT(new_pred,original_pred);
  }

  //(6) ranking_score_iterator
  auto* rsi = model.get_ranking_score_iterator(2);
  EXPECT_TRUE(rsi!=NULL);

  //(5) clear
  model.clear();
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat((i+2)%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0, model.prediction(rec_dat));
  }
}

TEST_F(TestFactorModel, bias){ //TODO dim=0 eseten lehet bias letet tesztelni?
  //(1) setup, init, self_test
  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = true; //bias turned on
  model_params.use_user_bias = true; //bias turned on
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = false;
  FactorModel model(&model_params);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());

  FactorModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.2;
  updater_params.regularization_rate = 0.0001;
  updater_params.learning_rate_bias = 0.2;
  updater_params.regularization_rate_bias = 0.0001;
  FactorModelGradientUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());

  //(2) initial prediction
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0, model.prediction(rec_dat));
  }

  //(3) add, update
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,(i+2)%MAXITEM,i,1); 
    model.add(rec_dat);
    double original_pred = model.prediction(rec_dat);
    updater.update(rec_dat,-1); //negative gradient -> prediction should increase
    double new_pred = model.prediction(rec_dat);
    EXPECT_GT(new_pred,original_pred);
  }

  //(6) ranking_score_iterator
  auto* rsi = model.get_ranking_score_iterator(2);
  EXPECT_TRUE(rsi==NULL);

  //(5) clear
  model.clear();
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat((i+2)%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0, model.prediction(rec_dat));
  }
}

TEST_F(TestFactorModel, initall){
  //(1) setup, init, self_test
  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = true; //test bias+initall
  model_params.use_user_bias = true; //test bias+initall
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = true; //initall on
  model_params.max_item = MAXITEM; //maxitem set
  model_params.max_user = MAXUSER; //maxuser set
  FactorModel model(&model_params);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());

  FactorModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.2;
  updater_params.regularization_rate = 0.0001;
  updater_params.learning_rate_bias = 0.2;
  updater_params.regularization_rate_bias = 0.0001;
  FactorModelGradientUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());

  //(2) initial prediction
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_NE(0, model.prediction(rec_dat)); //not zero, already initialized
  }

  //(3) add, update
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,(i+2)%MAXITEM,i,1); 
    model.add(rec_dat);
    double original_pred = model.prediction(rec_dat);
    updater.update(rec_dat,-1); //negative gradient -> prediction should increase
    double new_pred = model.prediction(rec_dat);
    EXPECT_GT(new_pred,original_pred);
  }

  //(6) ranking_score_iterator
  auto* rsi = model.get_ranking_score_iterator(2);
  EXPECT_TRUE(rsi==NULL); //bias is on

  //(5) clear
  model.clear();
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat((i+2)%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_NE(0, model.prediction(rec_dat)); //not zero, already initialized
  }
}

TEST_F(TestFactorModel, initall_expenv){
  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;

  //model1
  model_params.initialize_all = -1;
  model_params.max_item = -1;
  model_params.max_user = -1;
  FactorModel model1(&model_params);
  EXPECT_FALSE(model1.initialize()); //initall is not set, no expenv

  ExperimentEnvironment exp_env;
  model1.set_experiment_environment(&exp_env);
  EXPECT_TRUE(model1.initialize()); //initall not set, but expenv is set

  //model2
  model_params.initialize_all = true;
  model_params.max_item = -1;
  model_params.max_user = -1;
  FactorModel model2(&model_params);
  model2.set_experiment_environment(&exp_env);
  EXPECT_TRUE(model2.initialize()); //initall true, but expenv is set

  //model3
  model_params.initialize_all = false;
  model_params.max_item = -1;
  model_params.max_user = -1;
  FactorModel model3(&model_params);
  EXPECT_TRUE(model3.initialize()); //initall false, no need for expenv
}

TEST_F(TestFactorModel, sigmoid){
  //(1) setup, init, self_test
  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = true; //use sigmoid
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = false;
  FactorModel model(&model_params);
  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());

  FactorModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.2;
  updater_params.regularization_rate = 0.0001;
  updater_params.learning_rate_bias = 0.2;
  updater_params.regularization_rate_bias = 0.0001;
  FactorModelGradientUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());

  //(2) initial prediction
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0.5, model.prediction(rec_dat));
  }

  //(3) add, update
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,(i+2)%MAXITEM,i,1); 
    model.add(rec_dat);
    double original_pred = model.prediction(rec_dat);
    updater.update(rec_dat,-1); //negative gradient -> prediction should increase
    double new_pred = model.prediction(rec_dat);
    EXPECT_GT(new_pred,original_pred);
  }

  //(6) ranking_score_iterator
  auto* rsi = model.get_ranking_score_iterator(2);
  EXPECT_TRUE(rsi!=NULL);

  //(5) clear
  model.clear();
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat((i+2)%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0.5, model.prediction(rec_dat));
  }
}

TEST_F(TestFactorModel, recency){
  //(1) setup, init, self_test
  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = false;
  FactorModel model(&model_params);
  DummyRecency user_recency;
  model.set_user_recency(&user_recency);
  DummyRecency item_recency;
  model.set_item_recency(&item_recency);

  EXPECT_TRUE(model.initialize());
  EXPECT_TRUE(model.self_test());

  FactorModelGradientUpdaterParameters updater_params;
  updater_params.learning_rate = 0.2;
  updater_params.regularization_rate = 0.0001;
  updater_params.learning_rate_bias = 0.2;
  updater_params.regularization_rate_bias = 0.0001;
  FactorModelGradientUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());

  FactorModelUpdater simple_updater;
  simple_updater.set_model(&model);
  EXPECT_TRUE(simple_updater.self_test());

  //(2) initial prediction
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0, model.prediction(rec_dat));
  }

  //(3) add, update
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,(i+2)%MAXITEM,i,1); 
    model.add(rec_dat);
    double original_pred = model.prediction(rec_dat);
    updater.update(rec_dat,-1); //negative gradient -> prediction should increase
    simple_updater.update(rec_dat); //call simple updater
    double new_pred = model.prediction(rec_dat);
    EXPECT_GT(new_pred,original_pred);
  }

  //(6) ranking_score_iterator
  auto* rsi = model.get_ranking_score_iterator(2);
  EXPECT_TRUE(rsi!=NULL);

  //(5) clear
  model.clear();
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat((i+2)%MAXUSER,i%MAXITEM,i,1); 
    EXPECT_EQ(0, model.prediction(rec_dat));
  }
}

TEST_F(TestFactorModel, self_test){
  FactorModelParameters model_params;
  model_params.dimension = -3;
  model_params.begin_min = 3;
  model_params.begin_max = 3;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = true;
  model_params.max_item = -1;
  model_params.max_user = -1;
  FactorModel model(&model_params);
  EXPECT_FALSE(model.self_test());
}

TEST_F(TestFactorModel, read_write){
  stringstream ss;

  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = false;

  //model1
  FactorModel model1(&model_params);
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    model1.add(rec_dat);
  }
  EXPECT_NO_THROW(model1.write(ss));

  FactorModel model2(&model_params);
  EXPECT_NO_THROW(model2.read(ss));
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    double pred1 = model1.prediction(rec_dat);
    double pred2 = model2.prediction(rec_dat);
    EXPECT_EQ(pred1,pred2);
  }

  //recency: write/read not implemented
  FactorModel model3(&model_params);
  DummyRecency recency;
  model3.set_user_recency(&recency);
  EXPECT_ANY_THROW(model3.write(ss));
  EXPECT_ANY_THROW(model3.read(ss));

  //bias: write/read not implemented
  model_params.use_item_bias = true;
  FactorModel model4(&model_params);
  EXPECT_ANY_THROW(model4.write(ss));
  EXPECT_ANY_THROW(model4.read(ss));
}

TEST_F(TestFactorModel, similarity){
  stringstream ss;

  FactorModelParameters model_params;
  model_params.dimension = 10;
  model_params.begin_min = -0.1;
  model_params.begin_max = 0.1;
  model_params.use_sigmoid = false;
  model_params.use_item_bias = false;
  model_params.use_user_bias = false;
  model_params.seed=1234;
  model_params.lemp_bucket_size = 64;
  model_params.initialize_all = true;
  model_params.max_user = MAXUSER;
  model_params.max_item = MAXITEM;

  FactorModel model(&model_params);
  for(int i=0;i<10;i++){
    RecDat* rec_dat = create_rec_dat(i%MAXUSER,i%MAXITEM,i,1); 
    model.add(rec_dat);
  }
  for(int i=0;i<10;i++){
    int item1 = i%MAXITEM;
    int item2 = (7*i+2)%MAXITEM;
    EXPECT_NE(9,model.similarity(item1,item2));
  }
}

TEST_F(TestFactorModel, destructors){
  FactorModelParameters model_params;
  FactorModel* model = new FactorModel(&model_params);
  delete model;

  FactorModelGradientUpdaterParameters updater_params;
  FactorModelGradientUpdater* updater = new FactorModelGradientUpdater(&updater_params);
  delete updater;

  FactorModelUpdater* simple_updater = new FactorModelUpdater;
  delete simple_updater;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
