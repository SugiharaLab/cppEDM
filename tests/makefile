
CC  = g++
OBJ = SimplexTest.o

CFLAGS = -std=c++11 -g -D PRINT_DIFFERENCE_IN_RESULTS
LFLAGS = -lstdc++ -L../src/ -I../src/ -lEDM

all:	
	#@echo "compiling src lib..."
	#$(MAKE) -C ../src
	@echo "compiling tester files..."
	$(CC) SimplexTest.cc -o SimplexTest $(CFLAGS) $(LFLAGS) 

clean:
	rm -f $(OBJ) $(LIB) SimplexTest

distclean:
	rm -f $(OBJ) $(LIB) SimplexTest  *~ *.bak *.csv

$(LIB): $(OBJ)

SimplexTest.o: SimplexTest.cc
	$(CC) -c SimplexTest.cc $(CFLAGS)

SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE

SimplexTest.o: TestCommon.h
