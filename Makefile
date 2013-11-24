TARGET        = audio-metada
CC            = clang		
CXX           = clang++
COMMON_FLAGS  = -Wall -Werror -I$(CURDIR)/vendors/utf8
CFLAGS        = $(COMMON_FLAGS)
CXXFLAGS      = -std=c++11 $(COMMON_FLAGS)

SOURCES      := $(wildcard $(CURDIR)/sources/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/id3/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/metadata/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cpp)

OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources
VPATH        := $(VPATH):$(CURDIR)/sources/id3
VPATH        := $(VPATH):$(CURDIR)/sources/metadata
VPATH        := $(VPATH):$(CURDIR)/sources/utils
DEPS         := $(CURDIR)/Makefile.depends

export

.PHONY: all clean depends realclean Debug Release

all: Debug Release

Debug Release:
	@mkdir -p $@
	$(MAKE) --no-print-directory -C $@ -f ../$@.mk $(TARGET)

depends: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)

realclean: clean
	rm -fr log
	rm -fr tags
	rm -fr Debug
	rm -fr Release

clean:
	rm -fr *~
	rm -fr $(DEPS)
