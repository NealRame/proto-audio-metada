export TARGET        = audio-metada
export CC            = clang		
export CXX           = clang++
export COMMON_FLAGS  = -Wall -Werror -I$(CURDIR)/vendors/utf8
export CFLAGS        = $(COMMON_FLAGS)
export CXXFLAGS      = -std=c++11 $(COMMON_FLAGS)

export SOURCES      := $(wildcard $(CURDIR)/sources/*.cpp)
export SOURCES      += $(wildcard $(CURDIR)/sources/id3/*.cpp)
export SOURCES      += $(wildcard $(CURDIR)/sources/metadata/*.cpp)

export OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))
export VPATH        := $(CURDIR)/sources:$(CURDIR)/sources/id3:$(CURDIR)/sources/metadata

export DEPS         := $(CURDIR)/Makefile.depends

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
