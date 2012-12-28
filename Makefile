LIBRARY=bin/libkiwi.a
ECC=bin/ecc

CXXFLAGS=-std=c++0x -I . -I ../http-parser -g

HTTP_SRCS=http/server.cc http/request.cc http/response.cc http/parser.cc ../http-parser/http_parser.c
ROUTING_SRCS=routing/base.cc routing/rule.cc
CONTROLLER_SRCS=controller/base.cc
KIWI_SRCS=application/base.cc

AR_GEN_SRCS=activerecord/base.cc activerecord/iterator.cc activerecord/connection.cc
AR_PG_SRCS=activerecord/postgresql/connection.cc activerecord/postgresql/iterator.cc
AR_SRCS=$(AR_GEN_SRCS) $(AR_PG_SRCS)

OTHER_SRCS=helpers/core.cc view/base.cc view/parameters.cc model/attribute.cc $(AR_SRCS)


SRCS=$(HTTP_SRCS) $(ROUTING_SRCS) $(CONTROLLER_SRCS) $(KIWI_SRCS)
CC=c++

TEST_SRCS=$(shell find . -path "*/test/*.cc")

OBJS=$(patsubst %.cc, %.o,$(patsubst %.c, %.o,$(SRCS)))

all: $(LIBRARY) $(ECC)

depend: .depend
.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CXXFLAGS) -MM $^ >> ./.depend;
include .depend

$(LIBRARY): $(OBJS)
	ar r $(LIBRARY) $(OBJS)


ecc/ecc.c: ecc/ecc.yy
	flex -o $@ $<

bin/ecc: ecc/ecc.o
	g++ -o $@ $< -lfl

bin/test: $(LIBRARY) $(TEST_SRCS)
	c++ -g -o bin/test test.cc -I ~/work -I ~/work/kiwi -std=c++0x -lboost_regex-mt ~/work/htest/htest.cc -Lbin $(TEST_SRCS) -lkiwi

test: bin/test
	bin/test

