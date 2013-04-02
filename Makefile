# -*- Makefile -*-
# encoding: utf-8

.PHONY: all debug release test
CURR_DIR := $(shell pwd)
BUILDDIR := $(CURR_DIR)/build
DEBUG_DIR := $(BUILDDIR)/Debug
RELEASE_DIR := $(BUILDDIR)/Release
ifndef WithRuby
 WithRuby := False
endif

all:
	$(MAKE) debug
	$(MAKE) release

debug:
	$(MAKE) $(DEBUG_DIR)
	cd $(DEBUG_DIR) && $(MAKE)

release:
	$(MAKE) $(RELEASE_DIR)
	cd $(RELEASE_DIR) && $(MAKE)

$(DEBUG_DIR):
	@mkdir -p $@
	cd $@ && cmake -DWithRuby=False -DCMAKE_BUILD_TYPE=Debug ../..

$(RELEASE_DIR):
	@mkdir -p $@
	cd $@ && cmake -DWithRuby=$(WithRuby) -DCMAKE_BUILD_TYPE=Release ../..
