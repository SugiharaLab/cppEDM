
CC  = g++

EXE =  SimplexTest TestCommonTest SMapTest CCMTest
OBJ = $(EXE:=.o) TestCommon.o

CFLAGS = -std=c++11 -D PRINT_DIFFERENCE_IN_RESULTS
LFLAGS = -lstdc++ -L../lib/ -I../src/ -lEDM -lpthread

all:	$(EXE)

# Need to figure out how to build TestCommon.o automatically
SimplexTest: SimplexTest.cc
	$(CC) TestCommon.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

CCMTest: CCMTest.cc
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

TestCommonTest: TestCommonTest.cc
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

SMapTest: SMapTest.cc
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

clean:
	rm -f TestCommon.o $(OBJ) $(EXE)

distclean:
	rm -f TestCommon.o $(OBJ) $(EXE) *~ *.bak *.csv ./data/*_cppEDM.csv

SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE
