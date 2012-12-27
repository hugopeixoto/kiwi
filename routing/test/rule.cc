
#include "htest/htest.h"
#include "routing/rule.h"

using kiwi::routing::Rule;
using kiwi::routing::MatchResult;

HTEST(Routing, ShouldMatchRootRoute, "It should match root route")
{
  Rule rule("/");
  Assert(rule.matches("/"));
}

HTEST(Routing, ShouldNotMatchIfExpressionDifferent, "It should not match if the text doesn't match the rule expression")
{
  Rule rule("/");
  Assert(!rule.matches("herp"));
}

HTEST(Routing, TestShouldMatchARouteWithNoParameters, "It should match a route with no parameters")
{
  Rule rule("/hello");
  Assert(rule.matches("/hello"));
}

HTEST(Routing, TestShoultNotMatchWhenTheRouteIsASubstringOfTheURI, "It should not match when the route is a substring of the URI")
{
  Rule rule("/hello");
  Assert(!rule.matches("/hello_world"));
}

HTEST(Routing, TestShouldMatchWithSingleParameter, "It should match with a single parameter")
{
  Rule rule("/hello/:target");
  Assert(rule.matches("/hello/world"));
}

HTEST(Routing, TestShouldNotMatchWithSingleParameterIfNoneIsProvided, "It should not match with a single parameter if none is provided")
{
  Rule rule("/hello/:target");
  Assert(!rule.matches("/hello"));
}

HTEST(Routing, SingleToManyParameters, "It should not match with a single parameter if more than one is provided")
{
  Rule rule("/hello/:target");
  Assert(!rule.matches("/hello/world/from/peixoto"));
}

HTEST(Routing, ManyParams, "It should match with multiple parameters")
{
  Rule rule("/hello/:target/from/:source");
  Assert(rule.matches("/hello/world/from/peixoto"));
}

HTEST(Routing, FillParameters, "It should fill the match parameters correctly")
{
  MatchResult result = Rule("/hello/:target/from/:source").matches("/hello/world/from/peixoto");
  Assert(result["source"] == "peixoto");
  Assert(result["target"] == "world");
}

