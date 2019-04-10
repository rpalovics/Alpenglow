#include <cstdio>
#include <iostream>
#include <gtest/gtest.h>
#include "../../main/models/Model.h"

namespace {
class DummyModel : public Model {
  public:
    double prediction(RecDat* rec_dat) override { return 0; }
};
class DummyModel2 : public Model {
  public:
    double prediction(RecDat* rec_dat) override { return 0; }
    using Model::write;
    void write(ostream& file) override {
      file << "26 Test message" << std::endl;
    }
    using Model::read;
    void read(istream& file) override {
      file >> data;
    }
    int data = 0;
};
class TestModel : public ::testing::Test { 
  public:
    DummyModel model;
    DummyModel2 model2;

    TestModel(){}
    virtual ~TestModel(){}
    virtual void SetUp(){}
    virtual void TearDown(){}
    RecDat create_rec_dat(int user, int item, double time){
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = time;
      rec_dat.score = 1;
      return rec_dat;
    }
};
} //namespace

TEST_F(TestModel, test){
  EXPECT_TRUE(model.self_test());
  RecDat rec_dat = create_rec_dat(10,10,10);
  model.add(&rec_dat);
  EXPECT_EQ(0.0, model.prediction(&rec_dat));
  stringstream ss;
  EXPECT_ANY_THROW(model.write(ss));
  EXPECT_ANY_THROW(model.read(ss));
  EXPECT_ANY_THROW(model.clear());
}

TEST_F(TestModel, test2){
  string file_name = "temporal_test_file_name_26";
  model2.write(file_name);
  model2.read(file_name);
  EXPECT_EQ(26, model2.data);
  std::remove(file_name.c_str());
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
