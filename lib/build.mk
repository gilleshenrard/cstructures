#list of all src and object files
csrc := $(wildcard ../src/*.c)
cobj := $(csrc:.c=.o)

#directories containing the headers, libraries and executables
chead:= ../include

#flags necessary to the compilation
CC := gcc
CFLAGS:= -fPIC -Wall -Werror -Wextra -g -I$(chead)
lib_b:= libcarrays.so libclists.so libcavl.so libcstructurescommon.so libcstructures.so libcqueues.so libcstacks.so libdataset_test.so libscreen.so

#objects compilation from the source files
%.o: %.c
	@ echo "Building $@"
	@ $(CC) $(CFLAGS) -o $@ -c $<

#libraries compilation and linking (version number -> *.so file)
libcstructurescommon.so : ../src/cstructurescommon.o
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -Wl,-soname,$@.1 -o $@.1.0 $<
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libcavl.so : ../src/cavl.o libcstructurescommon.so
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -L. -Wl,-soname,$@.1 -o $@.1.0 $< -lcstructurescommon
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libclists.so : ../src/clists.o libcstructurescommon.so
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -L. -Wl,-soname,$@.1 -o $@.1.0 $< -lcstructurescommon
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libcarrays.so : ../src/carrays.o libcstructurescommon.so
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -L. -Wl,-soname,$@.1 -o $@.1.0 $< -lcstructurescommon
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libcqueues.so : ../src/cqueues.o libcstructurescommon.so
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -L. -Wl,-soname,$@.1 -o $@.1.0 $< -lcstructurescommon
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libcstacks.so : ../src/cstacks.o libcstructurescommon.so
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -L. -Wl,-soname,$@.1 -o $@.1.0 $< -lcstructurescommon
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libcstructures.so : ../src/cstructures.o libcarrays.so libclists.so libcavl.so libcqueues.so libcstacks.so
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -L. -Wl,-soname,$@.2 -o $@.2.1 $<
	@ ldconfig -ln $@.2.1
	@ ln -sf $@.2 $@

libdataset_test.so : ../src/dataset_test.o
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -Wl,-soname,$@.1 -o $@.1.0 $<
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@

libscreen.so : ../src/screen.o
	@ echo "Building $@"
	@ $(CC) -shared -fPIC -lc -Wl,-soname,$@.1 -o $@.1.0 $<
	@ ldconfig -ln $@.1.0
	@ ln -sf $@.1 $@


#overall functions
all: $(lib_b)

.PHONY= clean
clean:
	@ echo "cleaning libraries *.so files"
	@ rm -rf *.so* ../src/*.o
