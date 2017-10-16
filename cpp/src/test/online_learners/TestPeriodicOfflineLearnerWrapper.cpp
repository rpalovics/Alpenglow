#include <gtest/gtest.h>
#include "../../main/online_learners/PeriodicOfflineLearnerWrapper.h"
#include "../../main/recommender_data/SimpleIterator.h"
#include <vector>

namespace {
class DummyModel: public Model{
  public:
  void add(RecDat* recDat){ add_counter++;}
  double prediction(RecDat* recDat){ return 0; }
  void write(ostream& file){write_counter++;}
  void read(istream& file){read_counter++;}
  void clear(){clear_counter++;}
  int write_counter = 0;
  int read_counter = 0;
  int clear_counter = 0;
  int add_counter = 0;
};
class DummyOfflineLearner : public OfflineLearner {
  void iterate(){ EXPECT_TRUE(false); }
  void fit(RecommenderData* recommender_data) override {
    if(no_more_fit) EXPECT_TRUE(false);
    EXPECT_TRUE(recommender_data == expected_recommender_data);
    fit_counter++;
  }
  public:
    int fit_counter = 0;
    bool no_more_fit = false;
    RecommenderData* expected_recommender_data = NULL;
};
class DummyDataGenerator : public DataGenerator {
  public:
    RecommenderData* generate_recommender_data(RecDat* rec_dat) override { return &recommender_data_; }
    RecommenderData recommender_data_;
};
class TestPeriodicOfflineLearnerWrapper : public ::testing::Test  {
public:
  PeriodComputerParameters period_computer_parameters;
  PeriodComputer period_computer;
  vector<RecDat> rec_data;
  RecommenderData recommender_data;
  SimpleIterator recommender_data_iterator;
  ExperimentEnvironment experiment_environment;
  DummyOfflineLearner offline_learner1, offline_learner2, offline_learner3;
  DummyModel model;
  DummyDataGenerator data_generator;
  PeriodicOfflineLearnerWrapperParameters params;
  TestPeriodicOfflineLearnerWrapper() {
  }
  virtual ~TestPeriodicOfflineLearnerWrapper() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    rec_data.resize(20);
    for(uint i=0;i<rec_data.size();i++){
      rec_data[i].time = i;
    }
    recommender_data.set_rec_data(rec_data);
    EXPECT_TRUE(recommender_data.initialize());
    recommender_data_iterator.set_recommender_data(&recommender_data);
    EXPECT_TRUE(recommender_data_iterator.initialize());
    experiment_environment.set_recommender_data_iterator(&recommender_data_iterator);
    period_computer_parameters.period_mode = "samplenum";
    period_computer_parameters.period_length = 5;
    period_computer_parameters.start_time = 8;
    period_computer.set_parameters(&period_computer_parameters);
    period_computer.set_experiment_environment(&experiment_environment);
    EXPECT_TRUE(period_computer.initialize());
    EXPECT_TRUE(period_computer.self_test());
    offline_learner1.expected_recommender_data=&data_generator.recommender_data_;
    offline_learner2.expected_recommender_data=&data_generator.recommender_data_;
    offline_learner3.expected_recommender_data=&data_generator.recommender_data_;
  }
  virtual void TearDown(){}
};

}

TEST_F(TestPeriodicOfflineLearnerWrapper, learn) {
  PeriodicOfflineLearnerWrapper learner(&params);
  learner.add_offline_learner(&offline_learner1);
  learner.add_offline_learner(&offline_learner2);
  learner.add_offline_learner(&offline_learner3);
  learner.set_period_computer(&period_computer);
  learner.set_data_generator(&data_generator);
  EXPECT_TRUE(learner.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    learner.update(actual);
    int counter = recommender_data_iterator.get_counter();
    int expected_fits = (counter<8?0:(counter+1)/5-1);
    EXPECT_EQ(expected_fits,offline_learner1.fit_counter);
    EXPECT_EQ(expected_fits,offline_learner2.fit_counter);
    EXPECT_EQ(expected_fits,offline_learner3.fit_counter);
  }
  offline_learner1.no_more_fit=true;
  offline_learner2.no_more_fit=true;
  offline_learner3.no_more_fit=true;
  RecDat extra;
  learner.update(&extra);
  learner.update(&extra);
  learner.update(&extra);
}
TEST_F(TestPeriodicOfflineLearnerWrapper, minimal) {
  PeriodicOfflineLearnerWrapper learner(&params);
  learner.set_period_computer(&period_computer);
  EXPECT_TRUE(learner.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    learner.update(actual);
  }
  RecDat extra;
  learner.update(&extra);
  learner.update(&extra);
  learner.update(&extra);
}

TEST_F(TestPeriodicOfflineLearnerWrapper, clear) {
  params.clear_model=true;
  PeriodicOfflineLearnerWrapper learner(&params);
  learner.set_period_computer(&period_computer);
  learner.set_model(&model);
  EXPECT_TRUE(learner.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    learner.update(actual);
  }
  RecDat extra;
  learner.update(&extra);
  learner.update(&extra);
  learner.update(&extra);
  EXPECT_EQ(3,model.clear_counter);
  EXPECT_EQ(0,model.read_counter);
  EXPECT_EQ(0,model.write_counter);
  EXPECT_EQ(0,model.add_counter);
}
TEST_F(TestPeriodicOfflineLearnerWrapper, write) {
  params.write_model=true;
  params.base_out_file_name = "periodic_learner_test_file";
  PeriodicOfflineLearnerWrapper learner(&params);
  learner.set_period_computer(&period_computer);
  learner.set_model(&model);
  EXPECT_TRUE(learner.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    learner.update(actual);
  }
  RecDat extra;
  learner.update(&extra);
  learner.update(&extra);
  learner.update(&extra);
  EXPECT_EQ(0,model.clear_counter);
  EXPECT_EQ(0,model.read_counter);
  EXPECT_EQ(3,model.write_counter);
  EXPECT_EQ(0,model.add_counter);
}
TEST_F(TestPeriodicOfflineLearnerWrapper, read) {
  params.read_model=true;
  params.base_in_file_name = "periodic_learner_test_file";
  PeriodicOfflineLearnerWrapper learner(&params);
  learner.set_period_computer(&period_computer);
  learner.set_model(&model);
  EXPECT_TRUE(learner.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    learner.update(actual);
  }
  RecDat extra;
  learner.update(&extra);
  learner.update(&extra);
  learner.update(&extra);
  remove("periodic_learner_test_file_2");
  remove("periodic_learner_test_file_3");
  remove("periodic_learner_test_file_4");
  EXPECT_EQ(0,model.clear_counter);
  EXPECT_EQ(3,model.read_counter);
  EXPECT_EQ(0,model.write_counter);
  EXPECT_EQ(0,model.add_counter);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

