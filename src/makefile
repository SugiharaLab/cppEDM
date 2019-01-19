
CC  = g++
OBJ = Data.o Neighbors.o Simplex.o Eval.o CCM.o Multiview.o SMap.o\
      Common.o Interface.o Parameter.o
LIB = libEDM.a

CFLAGS = -std=c++11 -g -DDEBUG
LFLAGS = -lstdc++ -L./

all:	$(LIB)
	ar -rcs $(LIB) $(OBJ)
	$(CC) Test.cc -o Test $(CFLAGS) $(LFLAGS) -lEDM

clean:
	rm -f $(OBJ) $(LIB) Test

distclean:
	rm -f $(OBJ) $(LIB) Test *~

$(LIB): $(OBJ)

Data.o: Data.cc
	$(CC) -c Data.cc $(CFLAGS)

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

Common.o: Common.cc
	$(CC) -c Common.cc $(CFLAGS)

Interface.o: Interface.cc
	$(CC) -c Interface.cc $(CFLAGS)

Parameter.o: Parameter.cc
	$(CC) -c Parameter.cc $(CFLAGS)

Test.o: Test.cc
	$(CC) -c Test.cc $(CFLAGS)


SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE
