CXX=g++
GCC=gcc
LLVMGCC=../../local/llvm-gcc4.2-2.7/bin/llvm-gcc
CFLAGS+= -D__STDC_LIMIT_MACROS -D_GNU_SOURCE
SRCDIR=./src
OBJDIR=./src
BINDIR=./bin
GAMEDIR=.

all: ktest_client ktest_client_cheat ktest_server ktest_server_log ktest_client.bc ktest_stats
	
ktest_client:  ktest_client.c
	$(GCC) -o $(BINDIR)/$@ $< -lncurses
	
ktest_stats:  ktest_stats.c KTest.o
	$(GCC) -o $(BINDIR)/$@ $< KTest.o

ktest_client_cheat:  ktest_client.c 
	$(GCC) -o $(BINDIR)/$@ $< -lncurses -DCHEATS_ENABLED

ktest_server: ktest_server.c KTest.o
	$(GCC) -o $(BINDIR)/$@ $< KTest.o -g -lncurses
	
ktest_server_log: ktest_server.c KTest.o
	$(GCC) -o $(BINDIR)/$@ $< KTest.o -lncurses -DKLEE_LOG

ktest_client.bc: ktest_client.c
	$(LLVMGCC) -o $(BINDIR)/$@ $< -c --emit-llvm -DNUKLEAR

client.bc: ktest_client.c
	$(LLVMGCC) -o $(BINDIR)/$@ $< -c --emit-llvm -DKLEE

KTest.o: KTest.c

.c.o:
	$(GCC) -o $@ -c $<

clean:
	-rm *.o 
	-rm $(BINDIR)/*

