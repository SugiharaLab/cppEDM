.PHONY: all clean distclean depend 

CC  = g++
HEADERS = Common.h DataFrame.h EDM.h Embed.h Parameter.h Simplex.h Version.h

SRCS = Common.cc EDM.cc EDM_Functions.cc EDM_Helpers.cc Embed.cc\
       Parameter.cc  Simplex.cc

OBJ = $(SRCS:%.cc=%.o)

LIB = libEDM.a

CFLAGS += -std=c++11 -DMULTIVIEW_VALUES_OVERLOAD -O3\
          -Wpedantic -Wall -Wextra -Wreorder # -g -DDEBUG -DDEBUG_ALL

LFLAGS = -L./ -lstdc++ -lEDM -lpthread # -llapacke -llapack -lblas 

all:	$(LIB)
	cp $(LIB) ../lib/

clean:
	rm -f $(OBJ) $(LIB)

distclean:
	rm -f $(OBJ) $(LIB) ../lib/$(LIB) *~ *.bak *.csv

$(LIB): $(OBJ)
	ar -rcs $(LIB) $(OBJ)

%.o : %.cc 
	$(CC) $(CFLAGS) -c $<

depend:
	@echo ${SRCS}
	makedepend -Y $(SRCS) -w160
# DO NOT DELETE

Common.o: Common.h DataFrame.h
EDM.o: EDM.h Common.h DataFrame.h Embed.h Parameter.h Version.h EDM_Helpers.cc
EDM_Functions.o: Common.h DataFrame.h Simplex.h EDM.h Embed.h Parameter.h Version.h
EDM_Helpers.o: Common.h DataFrame.h
Embed.o: Embed.h Common.h DataFrame.h Parameter.h Version.h
Parameter.o: Parameter.h Common.h DataFrame.h Version.h
Simplex.o: Simplex.h Common.h DataFrame.h EDM.h Embed.h Parameter.h Version.h
