#directories containing the headers, libraries and executables
chead:= include
clib := lib
cbin := bin

#flags necessary to the compilation
CC := gcc
CFLAGS:= -fPIC -Wall -Werror -Wextra -g -I$(chead)
LFLAGS:= -ldataset_test -lcarrays -lclists -lcstructures
LDFLAGS:= -Wl,--disable-new-dtags -Wl,-rpath,\$$ORIGIN/../lib


#executables compilation
test_cstructures: bcstructures bdata
	@ echo "Builing $@"
	@ mkdir -p $(cbin)
	@ $(CC) $(CFLAGS) -L$(clib) $(LDFLAGS) -o $(cbin)/$@ $@.c $(LFLAGS)


#phony rules to build needed libraries and to clean builds
.PHONY: bcstructures
bcstructures:
	@ $(MAKE) -f build.mk -C$(clib) libcstructures.so

.PHONY: bdata
bdata:
	@ $(MAKE) -f build.mk -C$(clib) libdataset_test.so

.PHONY: clean
clean:
	@ echo "cleaning binaries"
	@ rm -rf $(cbin)/*
	@ $(MAKE) -f build.mk -C$(clib) clean

