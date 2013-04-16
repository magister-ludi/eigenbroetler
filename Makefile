# -*- Makefile -*-
# encoding: utf-8

.PHONY: all debug release test realclean
CURR_DIR := $(shell pwd)
BUILDDIR := $(CURR_DIR)/build
DEBUG_DIR := $(BUILDDIR)/Debug
RELEASE_DIR := $(BUILDDIR)/Release
WithRuby := True
ifndef WithRuby
 WithRuby := False
endif

SUBDIR_CALL := $(MAKE)
GENERATOR := "Unix Makefiles"
#SUBDIR_CALL := ninja
#GENERATOR := "Ninja"

all:
	$(MAKE) debug
	$(MAKE) release

debug:
	$(MAKE) $(DEBUG_DIR)
	cd $(DEBUG_DIR) && $(SUBDIR_CALL)

release:
	$(MAKE) $(RELEASE_DIR)
	cd $(RELEASE_DIR) && $(SUBDIR_CALL)

$(DEBUG_DIR):
	@mkdir -p $@
	cd $@ && cmake -G $(GENERATOR) -DWithRuby=False -DCMAKE_BUILD_TYPE=Debug ../..

$(RELEASE_DIR):
	@mkdir -p $@
	cd $@ && cmake -G $(GENERATOR) -DWithRuby=$(WithRuby) -DCMAKE_BUILD_TYPE=Release ../..

clean:
	rm -rf eigenbrötler eigenbrötlerD

realclean: clean
	rm -rf build external_dependencies/*/src external_dependencies/*/tmp
