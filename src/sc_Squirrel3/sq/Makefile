SQUIRREL= ..


OUT= $(SQUIRREL)/bin/sq
INCZ= -I$(SQUIRREL)/include -I. -I$(SQUIRREL)/sqlibs
LIBZ= -L$(SQUIRREL)/lib 
LIB= -lsquirrel -lsqstdlib

OBJS= sq.o
	
SRCS= sq.c
	
	
sq32:
	$(CXX) -O2 -s -fno-exceptions -fno-rtti -o $(OUT) $(SRCS) $(INCZ) $(LIBZ) $(LIB)

sqprof:
	$(CXX) -O2 -pg -fno-exceptions -fno-rtti -pie -gstabs -g3 -o $(OUT) $(SRCS) $(INCZ) $(LIBZ) $(LIB)
	
sq64:
	$(CXX) -O2 -s -m64 -fno-exceptions -fno-rtti -D_SQ64 -o $(OUT) $(SRCS) $(INCZ) $(LIBZ) $(LIB)
	
sq64d:
	$(CXX) -O0 -g3 -m64 -fno-exceptions -fno-rtti -D_SQ64 -o $(OUT) $(SRCS) $(INCZ) $(LIBZ) $(LIB)
	
.PHONY: clean mrproper

clean:
	rm -f *.o
	rm -rf $(OUT)

mrproper: clean

