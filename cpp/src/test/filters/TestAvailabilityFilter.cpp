#include <gtest/gtest.h>
#include "../../main/filters/AvailabilityFilter.h"


namespace {
  class TestAvailabilityFilter : public ::testing::Test { 
    public:
      ExperimentEnvironment expenv;
      TestAvailabilityFilter(){}
      virtual ~TestAvailabilityFilter(){}
      void SetUp() override {} 
      void TearDown() override {}
  };
}

TEST_F(TestAvailabilityFilter, test){
  int user = 0;
  AvailabilityFilter filter;
  filter.set_experiment_environment(&expenv);
  EXPECT_EQ(0,filter.availabilites_.size());
  filter.add_availability(10, 1, 100);
  EXPECT_EQ(1,filter.availabilites_.size());

  filter.add_availability(20, 2, 100);
  filter.add_availability(30, 3, 90);
  filter.add_availability(20, 4, 100);
  filter.add_availability(50, 5, 100);
  filter.add_availability(100, 5, 100);

  RecDat d;
  d.time = 10;
  expenv.update(&d);
  filter.active(&d); //to initiate filter.update()

  EXPECT_EQ(1,filter.available_items_set_.size());
  EXPECT_EQ(5,filter.availabilites_.size());
  ASSERT_EQ(1,filter.availability_ends_.size());
  EXPECT_EQ(110,get<0>(filter.availability_ends_.top()));
  EXPECT_EQ(1,get<1>(filter.availability_ends_.top()));
  ASSERT_EQ(1, filter.get_whitelist(user).size());
  EXPECT_EQ(1, filter.get_whitelist(user)[0]);
  d.item = 1;
  EXPECT_TRUE(filter.active(&d));
  d.item = 2;
  EXPECT_FALSE(filter.active(&d));
  d.item = 3;
  EXPECT_FALSE(filter.active(&d));

  d.time = 20;
  expenv.update(&d);
  EXPECT_EQ(3, filter.get_whitelist(user).size()); //get_whitelist initiates filter.update() if necessary
  /*auto items =*/ filter.get_whitelist(user);
  auto expected_set = set<int>({1,2,4});
  EXPECT_TRUE(expected_set == filter.available_items_set_);
  d.item = 1;
  EXPECT_TRUE(filter.active(&d));
  d.item = 2;
  EXPECT_TRUE(filter.active(&d));
  d.item = 3;
  EXPECT_FALSE(filter.active(&d));
  d.item = 4;
  EXPECT_TRUE(filter.active(&d));

  d.time = 120;
  expenv.update(&d);
  filter.active(&d); //to initiate filter.update()
  expected_set = set<int>({3,5});
  EXPECT_TRUE(expected_set == filter.available_items_set_);

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
