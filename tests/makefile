
CC  = g++

EXE = SimplexTest TestCommonTest SMapTest CCMTest
OBJ = $(EXE:=.o)

CFLAGS = -std=c++11 -D PRINT_DIFFERENCE_IN_RESULTS
LFLAGS = -lstdc++ -L../lib/ -I../src/ -lEDM -lpthread

all:	$(EXE)

SimplexTest: SimplexTest.cc TestCommon.h
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS)

CCMTest: CCMTest.cc TestCommon.h
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS)

TestCommonTest: TestCommonTest.cc TestCommon.h
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS)

SMapTest: SMapTest.cc TestCommon.h
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(OBJ) $(EXE)

distclean:
	rm -f $(OBJ) *~ *.bak *.csv

SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE
