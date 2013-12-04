#
# Created by gmakemake (Ubuntu Sep  7 2011) on Wed Oct 23 23:54:47 2013
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Default flags (redefine these with a header.mak file if desired)
CXXFLAGS =	-ggdb
CFLAGS =	-ggdb -std=c99
CLIBFLAGS =	-lm
CCLIBFLAGS =	
########## End of default flags


CPP_FILES =	
C_FILES =	lcs_dyn_prog.c lcs_rec_naive.c
PS_FILES =	
S_FILES =	
H_FILES =	lcs.h timeit.h
PY_FILES =  runner.py
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES) $(PY_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	

#
# Main targets
#

all:	lcs_dyn_prog lcs_rec_naive
	chmod +x runner.py

lcs_dyn_prog:	lcs_dyn_prog.o $(OBJFILES)
	$(CC) $(CFLAGS) -o lcs3 lcs_dyn_prog.o $(OBJFILES) $(CLIBFLAGS)

lcs_rec_naive:	lcs_rec_naive.o $(OBJFILES)
	$(CC) $(CFLAGS) -o lcs1 lcs_rec_naive.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

lcs_dyn_prog.o:	lcs.h timeit.h
lcs_rec_naive.o:	lcs.h timeit.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) lcs_rec_naive.o core 2> /dev/null
	-/bin/rm $(OBJFILES) lcs_dyn_prog.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf lcs1
	-/bin/rm -rf lcs3
