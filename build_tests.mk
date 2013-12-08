SOURCES      := $(wildcard $(CURDIR)/sources/id3/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/tests/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/metadata/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cpp)

OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources/tests
VPATH        := $(VPATH):$(CURDIR)/sources/id3
VPATH        := $(VPATH):$(CURDIR)/sources/metadata
VPATH        := $(VPATH):$(CURDIR)/sources/utils

INCLUDE_PATH  = -isystem $(CURDIR)/vendors/gtest/include -I$(CURDIR)/vendors/gtest

CFLAGS       += $(INCLUDE_PATH)
CXXFLAGS     += $(INCLUDE_PATH)

export

.PHONY: all

all: Makefile.depends
	@mkdir -p Tests
	$(MAKE) --no-print-directory -C Tests -f ../Tests.mk

Makefile.depends: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)
