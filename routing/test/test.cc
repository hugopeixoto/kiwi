
#include "htest/htest.h"
#include "routing/rule.h"

typedef kiwi::routing::Rule<int> Rule;

HTEST(Routing, ShouldMatchRootRoute, "It should match root route")
{
  Rule rule(kiwi::http::Method::GET, "/");
  Assert(rule.matches(kiwi::http::Method::GET, "/"));
}

HTEST(Routing, ShouldNotMatchIfExpressionDifferent, "It should not match if the text doesn't match the rule expression")
{
  Rule rule(kiwi::http::Method::GET, "/");
  Assert(!rule.matches(kiwi::http::Method::GET, "herp"));
}

HTEST(Routing, TestShouldNotMatchIfTheVerbIsDifferent, "It should not match if the verb is different")
{
  Rule rule(kiwi::http::Method::GET, "/");
  Assert(!rule.matches(kiwi::http::Method::POST, "/"));
}

HTEST(Routing, TestShouldMatchARouteWithNoParameters, "It should match a route with no parameters")
{
  Rule rule(kiwi::http::Method::GET, "/hello");
  Assert(rule.matches(kiwi::http::Method::GET, "/hello"));
}

HTEST(Routing, TestShoultNotMatchWhenTheRouteIsASubstringOfTheURI, "It should not match when the route is a substring of the URI")
{
  Rule rule(kiwi::http::Method::GET, "/hello");
  Assert(!rule.matches(kiwi::http::Method::GET, "/hello_world"));
}


HTEST(Routing, TestShouldMatchWithSingleParameter, "It should match with a single parameter")
{
  Rule rule(kiwi::http::Method::GET, "/hello/:target");
  Assert(rule.matches(kiwi::http::Method::GET, "/hello/world"));
}

/*
bool TestShouldNotMatchWithSingleParameterIfNoneIsProvided ()
{
  kiwi::routing::Base router;
  kiwi::routing::Match match;

  router.map(kiwi::http::Method::GET, "/hello/:target", NULL);
  ASSERT(!router.match(kiwi::http::Method::GET, "/hello", match));
  return true;
}

bool TestShouldNotMatchWithSingleParameterIfMoreThanOneIsProvided ()
{
  kiwi::routing::Base router;
  kiwi::routing::Match match;

  router.map(kiwi::http::Method::GET, "/hello/:target", NULL);
  ASSERT(!router.match(kiwi::http::Method::GET, "/hello/world/from/peixoto", match));
  return true;
}

bool TestShouldMatchWithMultipleParameters ()
{
  kiwi::routing::Base router;
  kiwi::routing::Match match;

  router.map(kiwi::http::Method::GET, "/hello/:target/from/:source", NULL);
  ASSERT(router.match(kiwi::http::Method::GET, "/hello/world/from/peixoto", match));
  return true;
}

bool TestShouldRetrieveTheCorrectMatchWhenThereAreNoParameters ()
{
  kiwi::routing::Base router;
  kiwi::routing::Match match;
  void* expected = (void*)0x123456;

  router.map(kiwi::http::Method::GET, "/hello", expected);
  router.match(kiwi::http::Method::GET, "/hello", match);
  ASSERT(match.action == expected);
  return true;
}

bool TestShouldFillTheMatchParametersCorrectly ()
{
  kiwi::routing::Base router;
  kiwi::routing::Match match;

  router.map(kiwi::http::Method::GET, "/hello/:target/from/:source", NULL);
  router.match(kiwi::http::Method::GET, "/hello/world/from/peixoto", match);

  ASSERT(match.parameters["source"] == "peixoto");
  ASSERT(match.parameters["target"] == "world");
  return true;
}
*/

int main ()
{
  int succeeded = 0;
  int failed = 0;

  for (auto test : htest::TestBucket::tests()) {
    if (test->Run()) {
      ++succeeded;
      printf(".");
    } else {
      ++failed;
      printf("F");
    }

    if ((succeeded + failed) % 80 == 0) puts("");
  }

  puts("");
  printf("%d tests, %d failures\n", succeeded + failed, failed);
  return 0;
}

