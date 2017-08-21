#include <vector>
#include <sstream>
#include <algorithm>
#include <gtest/gtest.h>
#include "../../main/utils/LabelContainer.h"

namespace {

class TestLabelContainer : public ::testing::Test  {
public:
  TestLabelContainer() {
  }
  virtual ~TestLabelContainer() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
  bool contains_element(vector<int> container, int element){
    return (std::find(container.begin(), container.end(), element) != container.end());
  }
};

}

TEST_F(TestLabelContainer, dummy) {
  stringstream ss;
  ss << "1 3" << endl;
  ss << "2 4" << endl;
  ss << "3 2" << endl;
  ss << "4 3" << endl;
  ss << "6 3" << endl;
  ss << "5 3" << endl;
  ss << "9 4" << endl;
  ss << "7 3" << endl;
  LabelContainer container;
  container.read_from_file(ss);

  vector<int> entities = container.get_entities(2);
  EXPECT_EQ(1,entities.size());
  EXPECT_TRUE(contains_element(entities,3));

  entities = container.get_entities(4);
  EXPECT_EQ(2,entities.size());
  EXPECT_TRUE(contains_element(entities,2));
  EXPECT_TRUE(contains_element(entities,9));

  entities = container.get_entities(3);
  EXPECT_EQ(5,entities.size());
  EXPECT_TRUE(contains_element(entities,1));
  EXPECT_TRUE(contains_element(entities,4));
  EXPECT_TRUE(contains_element(entities,6));
  EXPECT_TRUE(contains_element(entities,5));
  EXPECT_TRUE(contains_element(entities,7));

  entities = container.get_entities(1); //non existing label
  EXPECT_EQ(0,entities.size());

  //test repeatability
  entities = container.get_entities(4);
  EXPECT_EQ(2,entities.size());
  EXPECT_TRUE(contains_element(entities,2));
  EXPECT_TRUE(contains_element(entities,9));

  EXPECT_EQ(3,container.get_label(1));
  EXPECT_EQ(4,container.get_label(2));
  EXPECT_EQ(2,container.get_label(3));
  EXPECT_EQ(3,container.get_label(4));
  EXPECT_EQ(3,container.get_label(6));
  EXPECT_EQ(3,container.get_label(5));
  EXPECT_EQ(4,container.get_label(9));
  EXPECT_EQ(3,container.get_label(7));
  EXPECT_EQ(-1,container.get_label(333)); //non existing entity

  //repeatability
  EXPECT_EQ(3,container.get_label(1));
  EXPECT_EQ(4,container.get_label(2));
  EXPECT_EQ(2,container.get_label(3));
  EXPECT_EQ(3,container.get_label(4));
  EXPECT_EQ(3,container.get_label(6));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

