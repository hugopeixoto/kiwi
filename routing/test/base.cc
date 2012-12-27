
#include "htest/htest.h"
#include "routing/base.h"
#include "routing/method.h"

using kiwi::routing::Method;
using kiwi::routing::Base;

class Controller {
    public:
    Controller (void*) {}
    void index () {}
};

class RestfulController {
    public:
    RestfulController (void*) {}

    void index () {}
    void show () {}
    void update () {}
    void create () {}
    void destroy () {}
};

HTEST(Routing, ShouldMapGETRoot, "It should map a GET request to root")
{
    Base base;
    base.map(Method::GET, "/", &Controller::index);
}

HTEST(Routing, MapShouldBeChainable, "Map should be chainable")
{
    Base base;
    Assert(
      &base.
         map(Method::GET, "/", &Controller::index).
         map(Method::POST, "/", &Controller::index)
      == &base);
}

HTEST(Routing, ShouldRespondToRecognizePath, "It should respond to recognize_path")
{
    Base().recognize_path(Method::GET, "/");
}

HTEST(Routing, ShouldRecognizePath, "It should recognize a path")
{
    Assert(Base().
        map(Method::GET, "/", &Controller::index).
        recognize_path(Method::GET, "/"));
}

HTEST(Routing, ItShouldntRecognizeWithoutMap, "It shouldn't recognize any paths before mapping")
{
    Assert(!Base().
        recognize_path(Method::GET, "/"));
}

HTEST(Routing, ItShouldntRecognizeIfRuleDoesntMatch,
    "It shouldn't recognize any paths if the added rule does not match")
{
    Assert(!Base().
        map(Method::GET, "/", &Controller::index).
        recognize_path(Method::GET, "/no-match"));
}

HTEST(Routing, ItShouldntRecognizeWhenDifferentMethods,
    "It shouldn't recognize a path if the method does not match")
{
    Assert(!Base().
        map(Method::GET, "/", &Controller::index).
        recognize_path(Method::POST, "/"));
}

HTEST(Routing, ItShouldReturnParameters,
    "It should return the matching route parameters")
{
    Assert("world" == Base().
        map(Method::POST, "/hello/:from", &Controller::index).
        recognize_path(Method::POST, "/hello/world")["from"]);
}

HTEST(Routing, ItShouldReturnRouteAction,
    "recognize_path should have an accessor that returns the route action")
{
    (void)Base().
        map(Method::GET, "/", &Controller::index).
        recognize_path(Method::GET, "/").action;
}

HTEST(Routing, ItShouldReturnCorrectRouteAction,
    "The routing action should be the same as added")
{
    struct C { C (void*) {} void index () { throw 42; } };
    bool executed = false;

    try {
        Base().map(Method::GET, "/", &C::index).
            recognize_path(Method::GET, "/").action(NULL);
    } catch (int k) {
        executed = (k == 42);
    }

    Assert(executed);
}

HTEST(Routing, ItShouldHaveRootAlias,
    "It should respond to the root alias")
{
    (void)Base().root(&Controller::index);
}

HTEST(Routing, RootShouldBeChainable, "It should be root chainable")
{
    Base base;
    Assert(&base.root(&Controller::index) == &base);
}

HTEST(Routing, ShouldRecognizeRoot,
    "It should recognize a root path with GET")
{
    Assert(Base().
        root(&Controller::index).
        recognize_path(Method::GET, "/"));
}

HTEST(Routing, ShouldRespondToResource,
    "It should respond to the resources alias")
{
    (void)Base().resources<RestfulController>("posts");
}

HTEST(Routing, ResourcesShouldBeChainable, "It should be resources chainable")
{
    Base base;
    Assert(&base.resources<RestfulController>("posts") == &base);
}

HTEST(Routing, ShouldRecognizeResourcesIndex,
    "It should recognize the resources index")
{
    Assert(Base().
        resources<RestfulController>("posts").
        recognize_path(Method::GET, "/posts"));
}

HTEST(Routing, ShouldRecognizeResourcesShow,
    "It should recognize the resources show")
{
    Assert(Base().
        resources<RestfulController>("posts").
        recognize_path(Method::GET, "/posts/42"));
}

HTEST(Routing, ShouldRecognizeResourcesUpdate,
    "It should recognize the resources update")
{
    Assert(Base().
        resources<RestfulController>("posts").
        recognize_path(Method::PUT, "/posts/42"));
}

HTEST(Routing, ShouldRecognizeResourcesCreate,
    "It should recognize the resources create")
{
    Assert(Base().
        resources<RestfulController>("posts").
        recognize_path(Method::POST, "/posts"));
}

HTEST(Routing, ShouldRecognizeResourcesDestroy,
    "It should recognize the resources destroy")
{
    Assert(Base().
        resources<RestfulController>("posts").
        recognize_path(Method::DELETE, "/posts/42"));
}

