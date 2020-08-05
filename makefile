#==============================================================================================
# Makefile:
#==============================================================================================

CC = gcc
RC = windres
# Compiler flags for debug:
CFLAGS = -c -Wall -Wextra -Wsign-conversion -pedantic-errors -g -o
LFLAGS = -o
SDIR = src
ODIR = build/obj
EDIR = build
IDIR = include
LDIR = lib
LIBS = -luser32 -lgdi32
RESDIR = resources


all: Directories Program

#==============================================================================================
# Dependences (for further creating the object *.o files):
#==============================================================================================

# Dependances for win32_platform.o
_DEPS_win32_platform = utils.h simulation.h 
DEPS_win32_platform = $(patsubst %, $(IDIR)/%, $(_DEPS_win32_platform))

# Dependances for software_rendering.o
_DEPS_software_rendering = software_rendering.h utils.h 
DEPS_software_rendering = $(patsubst %, $(IDIR)/%, $(_DEPS_software_rendering))

# Dependances for simulation.o
_DEPS_simulation = simulation.h utils.h software_rendering.h 
DEPS_simulation = $(patsubst %, $(IDIR)/%, $(_DEPS_simulation))

# Dependances for traffic_light.o
_DEPS_traffic_light = traffic_light.h utils.h software_rendering.h 
DEPS_traffic_light = $(patsubst %, $(IDIR)/%, $(_DEPS_traffic_light))

#==============================================================================================
# Rule for make necessary directories
#==============================================================================================
.PHONY: Directories
Directories:
	mkdir -p build
	mkdir -p build/obj

#==============================================================================================
# Rules for compilation of the resources:
#==============================================================================================

$(ODIR)/%.o: $(RESDIR)/%.rc $(RESDIR)/%.h
	$(RC) -o $@ $<   

#==============================================================================================
# Rules for compilation of the object *.o files:
#==============================================================================================

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS_%) 
	$(CC) $(CFLAGS) $@ $< -I$(IDIR) -I$(RESDIR)

#==============================================================================================
# Rules for the linker:
#==============================================================================================

Program: $(ODIR)/win32_platform.o $(ODIR)/software_rendering.o $(ODIR)/simulation.o \
$(ODIR)/traffic_light.o $(ODIR)/resource.o
	$(CC) $(LFLAGS) $(EDIR)/$@ $^ -L$(LDIR) $(LIBS)
	ctags -f ./TAGS -e -R ./include ./src
	ctags -f ./src/TAGS -e -R ./include ./src
# use flag -s for stipping the executable (to reduce the size)

# IMPORTANT: MinGW do not support the unicode version of WinMain. But MinGW-w64 supports it, but
# it is necessary to add -municode flag to linker. Read Information.docx for details.

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
