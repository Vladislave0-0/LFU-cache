#include "../include/google_test.hpp"

int slow_get_page(int key) { return key; }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
