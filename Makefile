LIBRARY=libkiwi.a

SRCS=helpers/core.cc recko.cc

OBJS=$(patsubst %.cc, %.o,$(SRCS))

all: $(LIBRARY)

depend: .depend
.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend;
include .depend

$(LIBRARY): $(OBJS)
	ar r $(LIBRARY) $(OBJS)

