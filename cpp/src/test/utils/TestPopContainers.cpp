#include <vector>
#include <gtest/gtest.h>
#include "../../main/utils/PopContainers.h"

namespace {

class TestPopContainer : public ::testing::Test  {
public:
  PopContainer container;
  TestPopContainer() {
  }
  virtual ~TestPopContainer() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
};

class TestTopPopContainer : public ::testing::Test  {
public:
  TopPopContainer container;
  TestTopPopContainer() {}
  virtual ~TestTopPopContainer() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
};

}

TEST_F(TestPopContainer, increase) {
  EXPECT_EQ(0, container.get(10));
  EXPECT_EQ(0, container.get(5));
  EXPECT_EQ(0, container.get(1));

  container.increase(5);

  EXPECT_EQ(1, container.get(5));
  EXPECT_EQ(0, container.get(1));
  EXPECT_EQ(0, container.get(10));

  container.increase(1);
  container.increase(1);
  container.increase(3);
  container.increase(2);
  container.increase(10);
  container.increase(5);
  container.increase(11);

  EXPECT_EQ(0, container.get(0));
  EXPECT_EQ(2, container.get(1));
  EXPECT_EQ(1, container.get(2));
  EXPECT_EQ(0, container.get(4));
  EXPECT_EQ(2, container.get(5));
  EXPECT_EQ(0, container.get(6));
  EXPECT_EQ(0, container.get(7));
  EXPECT_EQ(1, container.get(10));
  EXPECT_EQ(1, container.get(11));
  EXPECT_EQ(0, container.get(12));
  EXPECT_EQ(0, container.get(100));
}

TEST_F(TestPopContainer, reduce) {
  container.increase(5);
  container.increase(1);
  container.increase(1);
  container.increase(3);
  container.increase(2);
  container.increase(10);
  container.increase(5);
  container.increase(11);

  container.reduce(3);
  container.reduce(1);
  container.reduce(10);

  EXPECT_EQ(0, container.get(3));
  EXPECT_EQ(1, container.get(1));
  EXPECT_EQ(0, container.get(10));

}

TEST_F(TestPopContainer, clear) {
  container.increase(5);
  container.increase(1);
  container.increase(1);
  container.reduce(5);
  container.increase(3);
  container.reduce(3);
  container.reduce(1);
  container.increase(2);
  container.increase(6);
  container.increase(10);
  container.reduce(10);
  container.increase(5);
  container.reduce(1);
  container.increase(11);
  
  container.clear();

  EXPECT_EQ(0, container.get(0));
  EXPECT_EQ(0, container.get(1));
  EXPECT_EQ(0, container.get(4));
  EXPECT_EQ(0, container.get(5));
  EXPECT_EQ(0, container.get(6));
  EXPECT_EQ(0, container.get(10));
  EXPECT_EQ(0, container.get(11));
}

TEST_F(TestTopPopContainer, increaseAndGet) {
  EXPECT_EQ(0, container.get(10).first);
  EXPECT_EQ(0, container.get(10).second);
  EXPECT_EQ(0, container.get(5).first);
  EXPECT_EQ(0, container.get(5).second);
  EXPECT_EQ(0, container.get(1).first);
  EXPECT_EQ(0, container.get(1).second);

  container.increase(5);

  EXPECT_EQ(5, container.get(0).first);
  EXPECT_EQ(1, container.get(0).second);
  EXPECT_EQ(0, container.get(10).first);
  EXPECT_EQ(0, container.get(10).second);
  EXPECT_EQ(0, container.get(5).first);
  EXPECT_EQ(0, container.get(5).second);
  EXPECT_EQ(0, container.get(1).first);
  EXPECT_EQ(0, container.get(1).second);

  container.increase(1);
  container.increase(1);
  container.increase(3);
  container.increase(2);
  container.increase(10);
  container.increase(5);
  container.increase(11);

  EXPECT_EQ(1, container.get(0).first);
  EXPECT_EQ(2, container.get(0).second);
  EXPECT_EQ(5, container.get(1).first);
  EXPECT_EQ(2, container.get(1).second);
  EXPECT_EQ(3, container.get(2).first);
  EXPECT_EQ(1, container.get(2).second);
  EXPECT_EQ(11, container.get(5).first);
  EXPECT_EQ(1, container.get(5).second);
  EXPECT_EQ(0, container.get(6).first);
  EXPECT_EQ(0, container.get(6).second);

  container.increase(2);
  container.increase(2);
  EXPECT_EQ(2, container.get(0).first);
  EXPECT_EQ(3, container.get(0).second);
}

TEST_F(TestTopPopContainer, get_item) {
  container.increase(5);

  EXPECT_EQ(5, container.get_item(0));

  container.increase(1);
  container.increase(1);
  container.increase(3);
  container.increase(2);
  container.increase(5);

  EXPECT_EQ(1, container.get_item(0));
  EXPECT_EQ(5, container.get_item(1));
  EXPECT_EQ(3, container.get_item(2));
  EXPECT_EQ(2, container.get_item(3));
}

//TEST_F(TestTopPopContainer, reduceAndSize) {
//  container.increase(5);
//  container.increase(1);
//  container.increase(5);
//  container.increase(1);
//  container.increase(3);
//  container.increase(3);
//  container.increase(10);
//  container.increase(3);
//  container.increase(11);
//  container.increase(1);
//
//  EXPECT_EQ(5, container.size());
//
//  container.reduce(3);
//
//  EXPECT_EQ(5, container.size());
//  EXPECT_EQ(1, container.get(0).first);
//  EXPECT_EQ(3, container.get(0).second);
//
//  container.reduce(3);
//  container.reduce(1);
//  
//  EXPECT_EQ(5, container.size());
//  EXPECT_EQ(1, container.get(0).first);
//  EXPECT_EQ(2, container.get(0).second);
//  EXPECT_EQ(5, container.get(1).first);
//  EXPECT_EQ(2, container.get(1).second);
//  EXPECT_EQ(3, container.get(2).first);
//  EXPECT_EQ(1, container.get(2).second);
//  EXPECT_EQ(10, container.get(3).first);
//  EXPECT_EQ(1, container.get(3).second);
//  EXPECT_EQ(11, container.get(4).first);
//  EXPECT_EQ(1, container.get(4).second);
//
//  container.reduce(10);
//
//  EXPECT_EQ(4, container.size());
//  EXPECT_EQ(11, container.get(3).first);
//  EXPECT_EQ(1, container.get(3).second);
//  EXPECT_EQ(0, container.get(4).first);
//  EXPECT_EQ(0, container.get(4).second);
//}
//
//TEST_F(TestTopPopContainer, get_normed) {
//  container.increase(5);
//  container.increase(1);
//  container.increase(5);
//  container.increase(1);
//  container.increase(3);
//  container.increase(3);
//  container.increase(8);
//  container.increase(3);
//  container.increase(9);
//  container.increase(1);
//
//  EXPECT_EQ(3, container.get_normed(0).first);
//  EXPECT_EQ(1, container.get_normed(0).second);
//  EXPECT_EQ(1, container.get_normed(1).first);
//  EXPECT_EQ(1, container.get_normed(1).second);
//  EXPECT_EQ(5, container.get_normed(2).first);
//  EXPECT_EQ(1/3.0, container.get_normed(2).second);
//  EXPECT_EQ(8, container.get_normed(3).first);
//  EXPECT_EQ(1/4.0, container.get_normed(3).second);
//  EXPECT_EQ(9, container.get_normed(4).first);
//  EXPECT_EQ(1/4.0, container.get_normed(4).second);
//  EXPECT_EQ(0, container.get_normed(5).first);
//  EXPECT_EQ(0, container.get_normed(5).second);
//}
//
//TEST_F(TestTopPopContainer, get_recommendation) {
//  container.increase(1);
//  container.increase(3);
//  container.increase(4);
//  container.increase(3);
//  container.increase(3);
//  container.increase(7);
//  container.increase(7);
//  container.increase(1);
// 
//  map<int, double>* recommendation = container.get_recommendation(3);
//  EXPECT_EQ(3, recommendation->size());
//  map<int,double>::iterator it=recommendation->find(3);
//  EXPECT_EQ(3, it->second);
//  it=recommendation->find(1);
//  EXPECT_EQ(2, it->second);
//  it=recommendation->find(7);
//  EXPECT_EQ(2, it->second);
//}
//
//TEST_F(TestTopPopContainer, treshold) {
//  //Threshold is N. If order of the first N element has changed after last call of get, get_normed or get_recommendation, has_changed returns true. 
//  container.set_threshold(3);
//  container.get(0);
//  EXPECT_FALSE(container.has_changed());
//
//  container.increase(1);
//  container.increase(2);
//  container.increase(2);
//  EXPECT_TRUE(container.has_changed());
//
//  container.get(0);
//  EXPECT_FALSE(container.has_changed());
//
//  container.reduce(2);
//  container.reduce(2);
//  EXPECT_TRUE(container.has_changed());
//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

