LIBRARY=bin/libkiwi.a
ECC=bin/ecc

CXXFLAGS=-std=c++0x -I . -I ../http-parser -g

HTTP_SRCS=http/server.cc http/request.cc http/response.cc http/parser.cc ../http-parser/http_parser.c
SRCS=helpers/core.cc routing/base.cc application/base.cc controller/base.cc controller/engine.cc view/parameters.cc $(HTTP_SRCS)

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
