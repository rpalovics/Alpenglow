#include <gtest/gtest.h>
#include "../../main/models/factor/FmModel.h"
#include "../../main/models/factor/FmModelUpdater.h"
#include <math.h>
#include <map>
#include <iomanip>

class TestFmModel : public ::testing::Test{
public:
  FactorsParameters fp;
  Factors f;
  Factors f2;
  FmModelParameters par;
  FmModel *model;
  FileSparseAttributeContainer* fsac;
  TestFmModel(){}
  virtual ~TestFmModel(){
  }
  virtual void SetUp(){
    fp.dimension = 4;
    fp.begin_min = -0.01;
    fp.begin_max = 0.01;
    fp.seed = 1234;
    f = Factors(fp);
    f.resize(50);
    vector<double> vec({0,1,0,0});
    f.set(0, &vec);
    vec = {1,0,0,0};
    f.set(1, &vec);
    vec = {0,0,0,1};
    f.set(2, &vec);
    vec = {0,0,1,0};
    f.set(3, &vec);
    vec = {1,1,1,1};
    f.set(4, &vec);

    f2 = Factors(fp);
    f2.resize(50);
    vec = {0,1,0,0};
    f2.set(0, &vec);
    vec = {1,0,0,0};
    f2.set(1, &vec);
    vec = {0,0,0,1};
    f2.set(2, &vec);
    vec = {0,0,1,0};
    f2.set(3, &vec);
    vec = {1,1,1,1};
    f2.set(4, &vec);

    FmModelParameters par;
    fsac = new FileSparseAttributeContainer();
    fsac->load_from_file("../../test_data/sparse_attributes_1.dat");
    par.item_attributes = fsac;
    par.dimension = 4;
    model = new FmModel(&par);
  }
  virtual void TearDown(){
    // delete model;
    // delete fsac;
  }
};

TEST_F(TestFmModel, test){
  RecDat rd;
  rd.user = 0;
  rd.item = 2; //attributes: 3:2 2:0.01, vecs: 3: {0,0,1,0}, 2: 0,0,0,1
  EXPECT_EQ(0, model->prediction(&rd));
  rd.user = 4; //vec: {1,1,1,1}
  model->add(&rd);
  model->user_factors_ = f;
  model->item_factors_ = f;
  EXPECT_EQ(0.01+2, model->prediction(&rd));
}

TEST_F(TestFmModel, test_initialize){
    FmModelParameters par;
    par.dimension = 2;
    par.seed = 1234543;
    model = new FmModel(&par);

    FmModelUpdaterParameters par2;
    par2.learning_rate = 0.01;
    FmModelUpdater updater(&par2);
    updater.set_model(model);

    RecDat rd;
    rd.item = 2; //attributes: 3:2 2:0.01, vecs: 3: {0,0,1,0}, 2: 0,0,0,1
    rd.user = 4; //vec: {1,1,1,1}
    rd.score = 1;
    double score = model->prediction(&rd);
    EXPECT_EQ(0, score);
    score = -10;
    for(uint i=0; i<=15; i++){
      updater.update(&rd);
      EXPECT_LT(score, model->prediction(&rd));
      score = model->prediction(&rd);
    }
}

TEST_F(TestFmModel, test_protected){
  //prepare
  vector<double> vec;

  //sum_attribute_vector
  auto res = model->sum_attribute_vector(f, {{0,1},{1,1},{3,0.5}});
  vec = {1,1,0.5,0};
  EXPECT_EQ(vec, res);
  vec = {0,0,0,1};

  //update_attributes
  model->update_attributes(f, {{0,1},{3,-1},{4,0.1}}, vec, 0.5);
  vec = {0,1,0,0.5};
  EXPECT_EQ(vec, *f.get(0));
  vec = {1,0,0,0};
  EXPECT_EQ(vec, *f.get(1));
  vec = {0,0,0,1};
  EXPECT_EQ(vec, *f.get(2));
  vec = {0,0,1,-0.5};
  EXPECT_EQ(vec, *f.get(3));
  vec = {1,1,1,1.05};
  EXPECT_EQ(vec, *f.get(4));
}

TEST_F(TestFmModel, test_updater){
  FmModelUpdaterParameters par;
  par.learning_rate = 0.01;
  FmModelUpdater updater(&par);
  updater.set_model(model);


  RecDat rd;
  rd.item = 2; //attributes: 3:2 2:0.01, vecs: 3: {0,0,1,0}, 2: 0,0,0,1
  rd.user = 4; //vec: {1,1,1,1}
  rd.score = 0;
  model->add(&rd);
  model->user_factors_ = f;
  model->item_factors_ = f2;

  EXPECT_EQ(0.01+2, model->prediction(&rd));
  double score = 2.01;
  vector<vector<double>> us;
  for(uint i=0; i<=4; i++){
    us.push_back(model->sum_attribute_vector(model->user_factors_, model->get_attributes(model->user_attributes_, i)));
  }
  vector<vector<double>> is;
  for(uint i=0; i<=4; i++){
    is.push_back(model->sum_attribute_vector(model->item_factors_, model->get_attributes(model->item_attributes_, i)));
  }


  for(uint i=0; i<=15; i++){
    updater.update(&rd);
    EXPECT_GT(score, model->prediction(&rd));
    score = model->prediction(&rd);
  }

  for(uint i=0; i<=4; i++){
    auto ut = model->sum_attribute_vector(model->user_factors_, model->get_attributes(model->user_attributes_, i));
    if(i!=4){
      EXPECT_EQ(us[i], ut);
    } else {
      EXPECT_NE(us[i], ut);
    }
  }
  for(uint i=0; i<=4; i++){
    auto it = model->sum_attribute_vector(model->item_factors_, model->get_attributes(model->item_attributes_, i));
    if(i!=2 && i!=4){
      EXPECT_EQ(is[i], it);
    } else {
      EXPECT_NE(is[i], it);
    }
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
