
CC  = g++

EXE =  SimplexTest TestCommonTest SMapTest CCMTest MultiviewTest DateTimeTest CETest 
OBJ = $(EXE:=.o) TestCommon.o

CFLAGS = -D PRINT_DIFFERENCE_IN_RESULTS
CFLAGS += -g
LFLAGS = -lstdc++ -L../lib/ -I../src/ -lEDM -lpthread -llapack

all:	$(EXE)

# Need to figure out how to build TestCommon.o automatically
SimplexTest: SimplexTest.cc
	$(CC) TestCommon.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

DateTimeTest: DateTimeTest.cc ../src/DateTime.cc
	$(CC) DateTimeTest.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

CCMTest: CCMTest.cc
	$(CC) TestCommon.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

TestCommonTest: TestCommonTest.cc
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

SMapTest: SMapTest.cc
	$(CC) TestCommon.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

MultiviewTest: MultiviewTest.cc
	$(CC) TestCommon.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

CETest: CETest.cc
	$(CC) TestCommon.cc -c $(CFLAGS) $(LFLAGS)
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS) TestCommon.o

clean:
	rm -f TestCommon.o $(OBJ) $(EXE)

distclean:
	rm -f TestCommon.o $(OBJ) $(EXE) *~ *.bak
	rm -f *.csv ./data/*_cppEDM.csv
	rm -f ./data/Smap_circle_coef.csv ./data/Smap_circle_SV.csv
	rm -f ./data/Smap_nan_circle_*.csv
	rm -f ./data/CELib_Smap_circle_coef.csv

SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE

SimplexTest.o: TestCommon.h
TestCommonTest.o: TestCommon.h
SMapTest.o: TestCommon.h
CCMTest.o: TestCommon.h
MultiviewTest.o: TestCommon.h
DateTimeTest.o: TestCommon.h
CETest.o: TestCommon.h
