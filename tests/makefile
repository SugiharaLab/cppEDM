
CC  = g++

EXE = SimplexTest TestCommonTest
OBJ = $(EXE:=.o)

CFLAGS = -std=c++11 -D PRINT_DIFFERENCE_IN_RESULTS
LFLAGS = -lstdc++ -L../src/ -I../src/ -lEDM

all:
	echo "use make (Test File Name w/o extension)"

SimplexTest: SimplexTest.cc TestCommon.h
	$(CC) $@.cc -o $@ $(CFLAGS) $(LFLAGS)

TestCommonTest: TestCommonTest.cc TestCommon.h
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
