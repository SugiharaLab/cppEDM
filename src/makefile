
CC  = g++
OBJ = Common.o DataIO.o Parameter.o Embed.o Interface.o Neighbors.o\
	Simplex.o Eval.o CCM.o Multiview.o SMap.o DataEmbedNN.o

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
	rm -f $(OBJ) $(LIB) Test  CameronTesting *~ *.bak *.csv

$(LIB): $(OBJ)

DataIO.o: DataIO.cc
	$(CC) -c DataIO.cc $(CFLAGS)

DataEmbedNN.o: DataEmbedNN.cc
	$(CC) -c DataEmbedNN.cc $(CFLAGS)

Common.o: Common.cc
	$(CC) -c Common.cc $(CFLAGS)

Parameter.o: Parameter.cc
	$(CC) -c Parameter.cc $(CFLAGS)

Embed.o: Embed.cc
	$(CC) -c Embed.cc $(CFLAGS)

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

Common.o: Common.h DataFrame.h DataEmbedNN.h DataIO.h Neighbors.h Parameter.h
Common.o: Embed.h
DataIO.o: DataIO.h Common.h DataFrame.h
Parameter.o: Parameter.h Common.h DataFrame.h
Embed.o: Embed.h Common.h DataFrame.h Parameter.h DataIO.h
Interface.o: Common.h DataFrame.h
Neighbors.o: Neighbors.h Common.h DataFrame.h Parameter.h
Simplex.o: Common.h DataFrame.h Parameter.h DataIO.h Neighbors.h Embed.h
Simplex.o: DataEmbedNN.h
Eval.o: Common.h DataFrame.h
CCM.o: Common.h DataFrame.h
Multiview.o: Common.h DataFrame.h
SMap.o: Common.h DataFrame.h Parameter.h DataIO.h Embed.h Neighbors.h
SMap.o: DataEmbedNN.h
