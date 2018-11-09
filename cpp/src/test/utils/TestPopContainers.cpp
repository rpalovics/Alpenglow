#include <vector>
#include <gtest/gtest.h>
#include <iostream>
#include "../../main/utils/PopContainers.h"
#include "../../main/utils/Random.h"

using namespace std;

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

TEST_F(TestTopPopContainer, get_item_huge) {
  int max_item = 50000;
  for(int i=0;i<max_item;i++){
    container.increase(i);
  }
  cerr << "first part ok" << endl;
  Random random(2238823);
  for(int i=0;i<1000000;i++){
    container.increase(random.get(max_item*3));
    if (!(i%1000)) cerr << i/10000 << "% ";
  }
  cerr << endl;
  container.get_item(0);
  container.get_item(1);
  container.get_item(2);
  container.get_item(3);
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

