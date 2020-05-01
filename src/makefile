
CC  = g++
OBJ = Common.o Embed.o EDM.o Simplex.o EDM_Functions.o Parameter.o EDM_Helpers.o

LIB = libEDM.a

CFLAGS = -std=c++11 -O3 -Wreorder -M  -g -DDEBUG -DDEBUG_ALL
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

Common.o: Common.h DataFrame.h
Parameter.o: Common.o
Embed.o: Parameter.o
EDM_Helpers.o: Common.o
EDM.o: Embed.o EDM_Helpers.o
Simplex.o: EDM.o
EDM_Functions.o: Simplex.o
