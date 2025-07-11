#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = C:/GBDK/
endif

LCC = $(GBDK_HOME)bin/lcc 

GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

# You can set the name of the .gb ROM file here
PROJECTNAME := SuperSpacePrincess3

SRCDIR      := src
DIR1        := src/headers
DIR2		:= src/res
INCLUDEDIR  := -I$(DIR1) -I$(DIR2)
OBJDIR      := obj
BINS	    := $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    := $(wildcard src/*.c) $(wildcard src/res/graphics/*.c) $(wildcard src/res/levels/*.c)
# ASMSOURCES  := $(wildcard src/*.s) $(wildcard src/graphics/*.s) $(wildcard src/levels/*.s)

all: $(BINS)

$(BINS): $(CSOURCES) # $(ASMSOURCES)
	$(LCC) $(LCCFLAGS) $(INCLUDEDIR) -o $@ $^

clean:
	del /s *.gb *.ihx *.cdb *.adb *.noi *.map

