
SQUIRREL=.
MAKE=make

sq32:
	cd squirrel; $(MAKE) 
	cd sqstdlib; $(MAKE) 
	cd sq; $(MAKE) 

sqprof:
	cd squirrel; $(MAKE) sqprof
	cd sqstdlib; $(MAKE) sqprof
	cd sq; $(MAKE) sqprof

sq64:
	cd squirrel; $(MAKE) sq64
	cd sqstdlib; $(MAKE) sq64
	cd sq; $(MAKE) sq64
	
sq64d:
	cd squirrel; $(MAKE) sq64d
	cd sqstdlib; $(MAKE) sq64d
	cd sq; $(MAKE) sq64d

.PHONY: clean mrproper

clean:
	cd squirrel; $(MAKE) clean
	cd sqstdlib; $(MAKE) clean
	cd sq; $(MAKE) clean

mrproper:
	cd squirrel; $(MAKE) mrproper
	cd sqstdlib; $(MAKE) mrproper
	cd sq; $(MAKE) mrproper
