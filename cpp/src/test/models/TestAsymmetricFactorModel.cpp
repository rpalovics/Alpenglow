#include <gtest/gtest.h>
#include "../../main/models/factor/AsymmetricFactorModel.h"
#include "../../main/models/factor/AsymmetricFactorModelGradientUpdater.h"
#include "../../main/models/factor/AsymmetricFactorModelUpdater.h"
#define DIMENSION 10
#define MAXUSER 13
#define MAXITEM 9

namespace {
class TestAsymmetricFactorModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;

    TestAsymmetricFactorModel(){}
    virtual ~TestAsymmetricFactorModel(){}
    virtual void SetUp(){
    }
    //void initFactors(Factors* factors){
    //  for(int i=0;i<10;i++){
    //    factors->init(i);
    //    factors->setZero(i);
    //    factors->set(i,i%DIMENSION,1);
    //  }
    //}
    //void initFactors2(Factors* factors){
    //  for(int i=0;i<10;i++){
    //    factors->init(i);
    //    factors->setZero(i);
    //    factors->set(i,i%DIMENSION,1);
    //    factors->set(i,(i%2+i%5)%DIMENSION,2);
    //  }
    //}
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

TEST_F(TestAsymmetricFactorModel, test){
  AsymmetricFactorModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=0.8;
  model_params.initialize_all=false;
  AsymmetricFactorModel model(&model_params);
  AsymmetricFactorModelUpdater simple_updater;
  AsymmetricFactorModelGradientUpdaterParameters grad_upd_params;
  grad_upd_params.learning_rate=0.14;
  grad_upd_params.cumulative_item_updates=false;
  AsymmetricFactorModelGradientUpdater gradient_updater(&grad_upd_params);
  simple_updater.set_model(&model);
  gradient_updater.set_model(&model);

  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(simple_updater.self_test());
  EXPECT_TRUE(gradient_updater.self_test());

  for(int i=0;i<100;i++){
    createRecDat(i%MAXUSER,i%MAXITEM,i,1);
  }
  for(uint i=0;i<recDats.size();i++){
    model.add(recDats[i]);
    simple_updater.update(recDats[i]);
  }
  for(uint i=0;i<recDats.size();i++){
  //for(uint i=0;i<5;i++){
    double orig_pred = model.prediction(recDats[i]);
    double gradient = orig_pred - 1;
    gradient_updater.update(recDats[i],gradient);
    double new_pred = model.prediction(recDats[i]);
    if(orig_pred<1){
      EXPECT_GT(new_pred,orig_pred);
    }
    //cerr << "orig pred=" << orig_pred << " new pred=" << new_pred << endl;
  }

}

TEST_F(TestAsymmetricFactorModel, test2){
  AsymmetricFactorModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=0;
  model_params.initialize_all=false;
  AsymmetricFactorModel model(&model_params);
  AsymmetricFactorModelUpdater simple_updater;
  AsymmetricFactorModelGradientUpdaterParameters grad_upd_params;
  grad_upd_params.learning_rate=0.14;
  grad_upd_params.cumulative_item_updates=false;
  AsymmetricFactorModelGradientUpdater gradient_updater(&grad_upd_params);
  simple_updater.set_model(&model);
  gradient_updater.set_model(&model);

  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(simple_updater.self_test());
  EXPECT_TRUE(gradient_updater.self_test());

  for(int i=0;i<100;i++){
    createRecDat(i%MAXUSER,i%MAXITEM,i,1);
  }
  for(uint i=0;i<recDats.size();i++){
    model.add(recDats[i]);
    simple_updater.update(recDats[i]);
  }
  for(uint i=0;i<recDats.size();i++){
  //for(uint i=0;i<5;i++){
    double orig_pred = model.prediction(recDats[i]);
    double gradient = orig_pred - 1;
    gradient_updater.update(recDats[i],gradient);
    double new_pred = model.prediction(recDats[i]);
    if(orig_pred<1){
      EXPECT_GT(new_pred,orig_pred);
    }
    //cerr << "orig pred=" << orig_pred << " new pred=" << new_pred << endl;
  }

}

TEST_F(TestAsymmetricFactorModel, test3){
  AsymmetricFactorModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=-0.8;
  model_params.initialize_all=false;
  AsymmetricFactorModel model(&model_params);
  AsymmetricFactorModelUpdater simple_updater;
  AsymmetricFactorModelGradientUpdaterParameters grad_upd_params;
  grad_upd_params.learning_rate=0.14;
  grad_upd_params.cumulative_item_updates=false;
  AsymmetricFactorModelGradientUpdater gradient_updater(&grad_upd_params);
  simple_updater.set_model(&model);
  gradient_updater.set_model(&model);

  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(simple_updater.self_test());
  EXPECT_TRUE(gradient_updater.self_test());

  for(int i=0;i<100;i++){
    createRecDat(i%MAXUSER,i%MAXITEM,i,1);
  }
  for(uint i=0;i<recDats.size();i++){
    model.add(recDats[i]);
    simple_updater.update(recDats[i]);
  }
  for(uint i=0;i<recDats.size();i++){
  //for(uint i=0;i<5;i++){
    double orig_pred = model.prediction(recDats[i]);
    double gradient = orig_pred - 1;
    gradient_updater.update(recDats[i],gradient);
    double new_pred = model.prediction(recDats[i]);
    if(orig_pred<1){
      EXPECT_GT(new_pred,orig_pred);
    }
    //cerr << "orig pred=" << orig_pred << " new pred=" << new_pred << endl;
  }

}

TEST_F(TestAsymmetricFactorModel, test_weights){
  AsymmetricFactorModelParameters model_params;
  model_params.dimension=DIMENSION;
  model_params.begin_min=-0.1;
  model_params.begin_max=0.1;
  model_params.norm_type="exponential";
  model_params.gamma=0.8;
  model_params.initialize_all=false;
  AsymmetricFactorModel model(&model_params);
  AsymmetricFactorModelUpdater simple_updater;
  AsymmetricFactorModelGradientUpdaterParameters grad_upd_params;
  grad_upd_params.learning_rate=0.14;
  grad_upd_params.cumulative_item_updates=false;
  AsymmetricFactorModelGradientUpdater gradient_updater(&grad_upd_params);
  simple_updater.set_model(&model);
  gradient_updater.set_model(&model);

  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(simple_updater.self_test());
  EXPECT_TRUE(gradient_updater.self_test());

  for(int i=0;i<100;i++){
    createRecDat(i%MAXUSER,i%MAXITEM,i,1);
  }
  for(uint i=0;i<recDats.size();i++){
    model.add(recDats[i]);
    simple_updater.update(recDats[i]);
  }
  for(int user=0;user<MAXUSER;user++){
    RecDat* rec_dat=createRecDat(user,0,101,1);
    model.prediction(rec_dat);
    //cerr << "user: " << user;
    double sum=0;
    for(uint i=0;i<model.cached_weights_.size();i++){
    //  cerr << " " << model.cached_weights_[i];
      sum+=model.cached_weights_[i];
    }
    //pow((pow(gamma_,user_activity_size)-1)/(gamma_-1),-0.5)
    double cached_norm=model.cached_norm_;
    EXPECT_DOUBLE_EQ(sum,1/(cached_norm*cached_norm));
    //cerr << "user: " << user << " sum: " << sum << " cached: " << cached_norm << "->" << 1/(cached_norm*cached_norm) << endl;
    //cerr << endl;
    //auto user_history = model.user_history_container_.get_user_history(user);
    //for(uint i=0;i<user_history->size();i++){
    //  RecDat rec_dat=*((*user_history)[i]);
    //  cerr << rec_dat << endl;;
    //}
  }


}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
