CXX=g++
GCC=gcc
LLVMGCC=../../local/bin/llvm-gcc
CFLAGS+= -D__STDC_LIMIT_MACROS -D_GNU_SOURCE
SRCDIR=./src
OBJDIR=./src
BINDIR=./bin
GAMEDIR=.

all: capman_client capman_client_cheat capman_server capman_server_log capman_client.bc capman_stats
	
capman_client:  ktest_client.c
	$(GCC) -o $(BINDIR)/$@ $< -lncurses
	
capman_stats:  ktest_stats.c KTest.o
	$(GCC) -o $(BINDIR)/$@ $< KTest.o

capman_client_cheat:  ktest_client.c 
	$(GCC) -o $(BINDIR)/$@ $< -lncurses -DCHEATS_ENABLED

capman_server: ktest_server.c KTest.o
	$(GCC) -o $(BINDIR)/$@ $< KTest.o -g -lncurses
	
capman_server_log: ktest_server.c KTest.o
	$(GCC) -o $(BINDIR)/$@ $< KTest.o -lncurses -DKLEE_LOG

capman_client.bc: ktest_client.c
	$(LLVMGCC) -o $(BINDIR)/$@ $< -c --emit-llvm -DNUKLEAR

#client.bc: ktest_client.c
#	$(LLVMGCC) -o $(BINDIR)/$@ $< -c --emit-llvm -DKLEE

KTest.o: KTest.c

.c.o:
	$(GCC) -o $@ -c $<

clean:
	-rm *.o 
	-rm $(BINDIR)/*

