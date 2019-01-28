
CC  = g++
OBJ = DataFrame.o Common.o Parameter.o Interface.o Neighbors.o\
	Simplex.o Eval.o CCM.o Multiview.o SMap.o EmbedData.o

LIB = libEDM.a

CFLAGS = -std=c++11 -g -DDEBUG # -DDEBUG_ALL
LFLAGS = -lstdc++ -L./

all:	$(LIB)
	ar -rcs $(LIB) $(OBJ)
	$(CC) Test.cc -o Test $(CFLAGS) $(LFLAGS) -lEDM
	$(CC) CameronTesting.cc -o CameronTesting $(CFLAGS) $(LFLAGS) -lEDM

clean:
	rm -f $(OBJ) $(LIB) Test CameronTesting

distclean:
	rm -f $(OBJ) $(LIB) Test  CameronTesting *~ *.bak

$(LIB): $(OBJ)

DataFrame.o: DataFrame.cc
	$(CC) -c DataFrame.cc $(CFLAGS)

Common.o: Common.cc
	$(CC) -c Common.cc $(CFLAGS)

EmbedData.o: EmbedData.cc
	$(CC) -c EmbedData.cc $(CFLAGS)

Parameter.o: Parameter.cc
	$(CC) -c Parameter.cc $(CFLAGS)

Interface.o: Interface.cc
	$(CC) -c Interface.cc $(CFLAGS)

Neighbors.o: Neighbors.cc
	$(CC) -c Neighbors.cc $(CFLAGS)

Simplex.o: Simplex.cc
	$(CC) -c Simplex.cc $(CFLAGS)

Eval.o: Eval.cc
	$(CC) -c Eval.cc $(CFLAGS)

CCM.o: CCM.cc
	$(CC) -c CCM.cc $(CFLAGS)

Multiview.o: Multiview.cc
	$(CC) -c Multiview.cc $(CFLAGS)

SMap.o: SMap.cc
	$(CC) -c SMap.cc $(CFLAGS)

Test.o: Test.cc
	$(CC) -c Test.cc $(CFLAGS)

CameronTesting.o: CameronTesting.cc
	$(CC) -c CameronTesting.cc $(CFLAGS)

SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE

DataFrame.o: DataFrame.h Common.h Matrix.h
Common.o: Common.h Matrix.h
Parameter.o: Parameter.h Common.h Matrix.h
Interface.o: Common.h Matrix.h
Neighbors.o: Neighbors.h Common.h Matrix.h Parameter.h
Simplex.o: Common.h Matrix.h Parameter.h DataFrame.h Neighbors.h
Eval.o: Common.h Matrix.h
CCM.o: Common.h Matrix.h
Multiview.o: Common.h Matrix.h
SMap.o: Common.h Matrix.h
