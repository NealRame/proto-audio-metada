SOURCES      := $(wildcard $(CURDIR)/sources/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/id3/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/metadata/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cpp)

OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources
VPATH        := $(VPATH):$(CURDIR)/sources/id3
VPATH        := $(VPATH):$(CURDIR)/sources/metadata
VPATH        := $(VPATH):$(CURDIR)/sources/utils

export

.PHONY: all Debug Release

all: Debug Release

Debug Release: Makefile.depends
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $@ -f ../$@.mk $(TARGET)

Makefile.depends: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)
