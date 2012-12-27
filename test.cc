
#include "htest/htest.h"
#include <list>

int main ()
{
  int succeeded = 0;
  int failed = 0;

  std::list<htest::Test*> failed_tests;

  puts("Running tests:");

  for (auto test : htest::TestBucket::tests()) {
    if (test->Run()) {
      ++succeeded;
      printf(".");
    } else {
      ++failed;
      printf("F");
      failed_tests.push_back(test);
    }

    if ((succeeded + failed) % 80 == 0) puts("");
  }

  puts("");
  for (auto test : failed_tests) {
      puts(test->Description());
  }

  puts("");
  printf("%d tests, %d failures\n", succeeded + failed, failed);

  return 0;
}

