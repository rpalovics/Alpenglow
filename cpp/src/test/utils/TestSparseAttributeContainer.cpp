
#include <gtest/gtest.h>
#include "../../main/utils/SparseAttributeContainer.h"
#include <math.h>
#include <map>

namespace {
  class TestSparseAttributeContainer : public ::testing::Test{
  public:
    bool map_eq(unordered_map<int, double> a, unordered_map<int, double> b){
      return a == b;
    }
  };
}

TEST_F(TestSparseAttributeContainer, file_load){
  FileSparseAttributeContainer c;
  c.load_from_file("../../test_data/sparse_attributes_1.dat");
  EXPECT_EQ(12, c.get_max_attribute_index());
  EXPECT_TRUE(map_eq({{1,1}, {12,3}}, c.get_attributes(0)));
  EXPECT_TRUE(map_eq({{0,0.5}, {7,1}}, c.get_attributes(1)));
  EXPECT_TRUE(map_eq({{3,2}, {2,0.01}}, c.get_attributes(2)));
  EXPECT_TRUE(map_eq({{4,1}, {5,1}, {6,1}}, c.get_attributes(3)));
  EXPECT_TRUE(map_eq({{3,2}, {2,0.01}}, {{3,2}, {2,0.01}}));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
