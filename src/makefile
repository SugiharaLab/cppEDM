
CC  = g++
OBJ = EDM.o EDM_Functions.o Simplex.o

LIB = libEDM.a

CFLAGS = -std=c++11 -O3 -Wreorder -M # -g -DDEBUG -DDEBUG_ALL
LFLAGS = -L./ -lstdc++ -lEDM -lpthread 

all:	$(LIB)
	ar -rcs $(LIB) $(OBJ)
	cp $(LIB) ../lib/

clean:
	rm -f $(OBJ) $(LIB)

distclean:
	rm -f $(OBJ) $(LIB) ../lib/$(LIB) *~ *.bak *.csv

$(LIB): $(OBJ)

%.o : %.cc %.h
	$(CC) -c $< -o $@

SRCS = `echo ${OBJ} | sed -e 's/.o /.cc /g'`
depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS)
# DO NOT DELETE

EDM.o: EDM.h Common.h DataFrame.h
