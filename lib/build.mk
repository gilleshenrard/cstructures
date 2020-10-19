#list of all src and object files
csrc := $(wildcard ../src/*.c)
cobj := $(csrc:.c=.o)

#directories containing the headers, libraries and executables
chead:= ../include

#flags necessary to the compilation
CC := gcc
CFLAGS:= -fPIC -Wall -Werror -g -I$(chead)
lib_b:= libalgo.so libdataset_test.so

#objects compilation from the source files
%.o: %.c
	@ echo "Building $@"
	@ $(CC) $(CFLAGS) -o $@ -c $<


#libraries compilation and linking (version number -> *.so file)
libalgo.so : ../src/algo.o
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -Wl,-soname,$@.2 -o $@.2.1 $<
	@ ldconfig -n . -l $@.2.1
	@ ln -sf $@.2 $@

libdataset_test.so : ../src/dataset_test.o
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -Wl,-soname,$@.1 -o $@.1.0 $<
	@ ldconfig -n . -l $@.1.0
	@ ln -sf $@.1 $@


#overall functions
all: $(lib_b)

.PHONY= clean
clean:
	@ echo "cleaning libraries *.so files"
	@ rm -rf *.so* ../src/*.o
