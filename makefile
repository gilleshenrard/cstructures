#directories containing the headers, libraries and executables
chead:= include
clib := lib
cbin := bin

#flags necessary to the compilation
CC := gcc
CFLAGS:= -fPIC -Wall -Werror -g -I$(chead)
LFLAGS:= -ldataset_test -lalgo
LDFLAGS:= -Wl,--disable-new-dtags -Wl,-rpath,\$$ORIGIN/../lib


#executables compilation
test_algo: balgo bdata
	@ echo "Builing $@"
	@ $(CC) $(CFLAGS) -L$(clib) $(LDFLAGS) -o $(cbin)/$@ $@.c $(LFLAGS)


#phony rules to build needed libraries and to clean builds
.PHONY: balgo
balgo:
	@ $(MAKE) -f build.mk -C$(clib) libalgo.so

.PHONY: bdata
bdata:
	@ $(MAKE) -f build.mk -C$(clib) libdataset_test.so

.PHONY: clean
clean:
	@ echo "cleaning binaries"
	@ rm -rf $(cbin)/*
	@ $(MAKE) -f build.mk -C$(clib) clean

