#include <gtest/gtest.h>
#include "../../main/models/baseline/NearestNeighborModel.h"
#include "../../main/models/baseline/NearestNeighborModelUpdater.h"

namespace{

class TestNearestNeighborModel : public ::testing::Test {
  public:
    TestNearestNeighborModel(){}
    virtual ~TestNearestNeighborModel(){}
    virtual void SetUp(){
    }
    virtual void TearDown(){
      for(uint i=0;i<rec_dat_ps.size();i++){
        delete rec_dat_ps[i];
      }
    }
    vector<RecDat*> rec_dat_ps;
    set<int> users;
    set<int> items;
    RecDat* create_recdat_p(int time,int user,int item,int score){
      RecDat* rec_dat = new RecDat();
      rec_dat->time = time;
      users.insert(user);
      rec_dat->user = user;
      items.insert(item);
      rec_dat->item = item;
      rec_dat->score = score;
      rec_dat_ps.push_back(rec_dat);
      return rec_dat;
    }
};
}

TEST_F(TestNearestNeighborModel, test){
  NearestNeighborModelParameters params;
  params.gamma = 0; //csak a kozvetlen koveto szamit
  params.norm = "off"; //nincs
  params.direction = "forward"; //csak a kovetkezok, a megelozok nem
  params.gamma_threshold = 0; //ha <= ennel a lecsengetes, akkor levagjuk
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 10; //10 sec utan szamolja ujra a similarityt
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));
  updater.update(create_recdat_p(15,21,30,1));
  updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,32,1));
  //updater.update(create_recdat_p(18,21,33,1));
  //updater.update(create_recdat_p(19,21,34,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.update(create_recdat_p(22,20,35,1));

  EXPECT_DOUBLE_EQ(1.0,model.prediction(create_recdat_p(24,21,33,1)));
  EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(24,21,30,1)));
  EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(24,21,31,1)));
  EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(24,21,32,1)));
  for(auto item : items){
    EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,20,item,1)));
  }
}

TEST_F(TestNearestNeighborModel, weights){
  NearestNeighborModelParameters params;
  params.norm = "off"; //nincs
  params.direction = "forward"; //csak a kovetkezok, a megelozok nem
  params.gamma_threshold = 0; //ha <= ennel a lecsengetes, akkor levagjuk
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 100; //10 sec utan szamolja ujra a similarityt

  vector<double> gamma_values = {0.0, 0.1, 1.0};
  vector<NearestNeighborModel*> models;
  vector<NearestNeighborModelUpdater*> updaters;

  for(auto gamma_value : gamma_values){
    params.gamma = gamma_value;
    NearestNeighborModel* model = new NearestNeighborModel(&params);
    NearestNeighborModelUpdater* updater = new NearestNeighborModelUpdater(&updater_params);
    updater->set_model(model);
    EXPECT_TRUE(model->self_test());
    EXPECT_TRUE(updater->self_test());
    models.push_back(model);
    updaters.push_back(updater);
  }

  for(auto updater:updaters){
    updater->update(create_recdat_p(10,20,30,1));
    updater->update(create_recdat_p(11,20,31,1));
    updater->update(create_recdat_p(12,20,32,1));
    updater->update(create_recdat_p(13,20,33,1));
    updater->update(create_recdat_p(14,20,34,1));
    updater->update(create_recdat_p(15,21,30,1));
    updater->update(create_recdat_p(16,21,31,1));
    updater->update(create_recdat_p(17,21,32,1));
    //updater->update(create_recdat_p(18,21,33,1));
    //updater->update(create_recdat_p(19,21,34,1));
  }

  for(uint i=0;i<gamma_values.size();i++){
    double gamma=gamma_values[i];
    EXPECT_EQ(5,models[i]->weights_.size());
    double expected_weight = 1.0;
    for(auto weight : models[i]->weights_){
      EXPECT_DOUBLE_EQ(expected_weight,weight);
      expected_weight*=gamma;
    }
  }

  for(auto updater:updaters){
    updater->update(create_recdat_p(22,20,35,1));
  }

  for(uint i=0;i<gamma_values.size();i++){
    double gamma=gamma_values[i];
    EXPECT_EQ(6,models[i]->weights_.size());
    double expected_weight = 1.0;
    for(auto weight : models[i]->weights_){
      EXPECT_DOUBLE_EQ(expected_weight,weight);
      expected_weight*=gamma;
    }
  }

}

TEST_F(TestNearestNeighborModel, norm_sum){
  NearestNeighborModelParameters params;
  params.gamma = 1;
  //params.norm = "num"; //default: num
  params.direction = "forward";
  params.gamma_threshold = 0;
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 10; //10 sec utan szamolja ujra a similarityt
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));
  updater.update(create_recdat_p(15,21,30,1));
  updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,32,1));
  //updater.update(create_recdat_p(18,21,33,1));
  //updater.update(create_recdat_p(19,21,34,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.update(create_recdat_p(22,20,35,1));

  EXPECT_DOUBLE_EQ(1.5,model.prediction(create_recdat_p(24,21,33,1)));
  EXPECT_DOUBLE_EQ(1.5,model.prediction(create_recdat_p(24,21,34,1)));
  EXPECT_DOUBLE_EQ(1.5,model.prediction(create_recdat_p(24,21,35,1)));

  //for (auto x:model.similarities_){
  //  cerr << "xxx" << endl;
  //  for (auto y:x){
  //    cerr << y.first << " " << y.second << endl;
  //  }
  //}

}
TEST_F(TestNearestNeighborModel, test2){
  NearestNeighborModelParameters params;
  params.gamma = 1;
  params.norm = "off"; //nincs
  params.direction = "forward";
  params.gamma_threshold = 0;
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 10; //10 sec utan szamolja ujra a similarityt
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));
  updater.update(create_recdat_p(15,21,30,1));
  updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,32,1));
  //updater.update(create_recdat_p(18,21,33,1));
  //updater.update(create_recdat_p(19,21,34,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.update(create_recdat_p(22,20,35,1));

  EXPECT_DOUBLE_EQ(3.0,model.prediction(create_recdat_p(24,21,33,1)));
  EXPECT_DOUBLE_EQ(3.0,model.prediction(create_recdat_p(24,21,34,1)));
  EXPECT_DOUBLE_EQ(3.0,model.prediction(create_recdat_p(24,21,35,1)));

  //for (auto x:model.similarities_){
  //  cerr << "xxx" << endl;
  //  for (auto y:x){
  //    cerr << y.first << " " << y.second << endl;
  //  }
  //}

}

TEST_F(TestNearestNeighborModel, test3){
  NearestNeighborModelParameters params;
  params.gamma = 0.1;
  params.norm = "off"; //nincs
  params.direction = "forward";
  params.gamma_threshold = 0;
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 10; //10 sec utan szamolja ujra a similarityt
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));
  updater.update(create_recdat_p(15,21,30,1));
  updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,32,1));
  //updater.update(create_recdat_p(18,21,33,1));
  //updater.update(create_recdat_p(19,21,34,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.update(create_recdat_p(22,20,35,1));

  EXPECT_DOUBLE_EQ(1.0101,model.prediction(create_recdat_p(24,21,33,1)));
  EXPECT_DOUBLE_EQ(0.10101,model.prediction(create_recdat_p(24,21,34,1)));
  EXPECT_DOUBLE_EQ(0.010101,model.prediction(create_recdat_p(24,21,35,1)));

  //for (auto x:model.similarities_){
  //  cerr << "xxx" << endl;
  //  for (auto y:x){
  //    cerr << y.first << " " << y.second << endl;
  //  }
  //}

}

TEST_F(TestNearestNeighborModel, direction){
  NearestNeighborModelParameters params;
  params.gamma = 1;
  params.norm = "off"; //nincs
  params.direction = "both";
  params.gamma_threshold = 0;
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 10; //10 sec utan szamolja ujra a similarityt
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));

  updater.update(create_recdat_p(15,21,30,1));
  //updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,36,1));
  updater.update(create_recdat_p(18,21,33,1));
  updater.update(create_recdat_p(19,21,37,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.update(create_recdat_p(22,20,35,1));

  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,31,1)));
  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,32,1)));
  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,34,1)));
  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,35,1)));

  //for (auto x:model.similarities_){
  //  cerr << "xxx" << endl;
  //  for (auto y:x){
  //    cerr << y.first << " " << y.second << endl;
  //  }
  //}

}

TEST_F(TestNearestNeighborModel, both2){
  NearestNeighborModelParameters params;
  params.gamma = 0.1;
  params.norm = "off"; //nincs
  params.direction = "both";
  params.gamma_threshold = 0;
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.compute_similarity_period = 10; //10 sec utan szamolja ujra a similarityt
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));

  updater.update(create_recdat_p(15,21,30,1));
  //updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,36,1));
  updater.update(create_recdat_p(18,21,33,1));
  updater.update(create_recdat_p(19,21,37,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.update(create_recdat_p(22,20,35,1));

  EXPECT_DOUBLE_EQ(0.01+0.001,model.prediction(create_recdat_p(24,21,31,1)));
  EXPECT_DOUBLE_EQ(0.0001+0.1,model.prediction(create_recdat_p(24,21,32,1)));
  EXPECT_DOUBLE_EQ(0.1+0.000001,model.prediction(create_recdat_p(24,21,34,1)));
  EXPECT_DOUBLE_EQ(0.01+0.0000001,model.prediction(create_recdat_p(24,21,35,1)));

  //for (auto x:model.similarities_){
  //  cerr << "xxx" << endl;
  //  for (auto y:x){
  //    cerr << y.first << " " << y.second << endl;
  //  }
  //}

  vector<int> users = {20, 21, 21};
  for (auto user : users){
    auto rsi = model.get_ranking_score_iterator(user);
    map<int,int> rsi_items;
    while(rsi->has_next()){
      int item;
      double score;
      tie(item,score) = rsi->get_next();
      EXPECT_EQ(0,rsi_items[item]); //each items should occur only once in the iterator
      rsi_items[item]=1;
    }
    EXPECT_ANY_THROW(rsi->unique_items_num());
  }
}

TEST_F(TestNearestNeighborModel, offline){
  NearestNeighborModelParameters params;
  params.gamma = 1;
  params.norm = "off"; //nincs
  params.direction = "both";
  params.gamma_threshold = 0;
  params.num_of_neighbors = 10; //a 10 legkozelebbi (leghasonlobb itemet vesszuk figyelembe)
  NearestNeighborModel model(&params);
  EXPECT_TRUE(model.self_test());
  NearestNeighborModelUpdaterParameters updater_params;
  updater_params.period_mode="off"; //offline usage
  NearestNeighborModelUpdater updater(&updater_params);
  updater.set_model(&model);
  EXPECT_TRUE(updater.self_test());
  updater.message(UpdaterMessage::start_of_offline_update);

  updater.update(create_recdat_p(10,20,30,1));
  updater.update(create_recdat_p(11,20,31,1));
  updater.update(create_recdat_p(12,20,32,1));
  updater.update(create_recdat_p(13,20,33,1));
  updater.update(create_recdat_p(14,20,34,1));

  updater.update(create_recdat_p(15,21,30,1));
  //updater.update(create_recdat_p(16,21,31,1));
  updater.update(create_recdat_p(17,21,36,1));
  updater.update(create_recdat_p(18,21,33,1));
  updater.update(create_recdat_p(19,21,37,1));
  updater.update(create_recdat_p(22,20,35,1));

  for(auto user : users){
    for(auto item : items){
      EXPECT_DOUBLE_EQ(0.0,model.prediction(create_recdat_p(20,user,item,1)));
    }
  }
  updater.message(UpdaterMessage::end_of_offline_update);

  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,31,1)));
  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,32,1)));
  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,34,1)));
  EXPECT_DOUBLE_EQ(2,model.prediction(create_recdat_p(24,21,35,1)));

  //for (auto x:model.similarities_){
  //  cerr << "xxx" << endl;
  //  for (auto y:x){
  //    cerr << y.first << " " << y.second << endl;
  //  }
  //}

}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
