##===- cling/lib/UserInterface/Makefile---------------------*- Makefile -*-===##
# 
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
# 
##===----------------------------------------------------------------------===##
#
#  This implements the a C++ interpreter user frontend.
#
##===----------------------------------------------------------------------===##

CLING_LEVEL = ../..
DIRS = textinput

LIBRARYNAME := clingUserInterface
BUILD_ARCHIVE = 1
CXXFLAGS = -fno-rtti

CPPFLAGS += -I$(PROJ_SRC_DIR)/../../include -I$(PROJ_SRC_DIR)/../../../clang/include \
-I$(PROJ_SRC_DIR)/../../../clang/lib -I$(PROJ_OBJ_DIR)/../../include -I$(PROJ_OBJ_DIR)/../../../clang/include 

include $(CLING_LEVEL)/Makefile

$(PROJ_SRC_DIR)/textinput/Makefile.d:
	svn co $(shell svn info $(PROJ_SRC_DIR) | grep ^URL: | sed 's,^URL: \(.*\)interpreter/cling/lib/UserInterface,\1/core/textinput/src/textinput,') $(PROJ_SRC_DIR)/textinput
	touch $@

# trigger checkput before building
include $(PROJ_SRC_DIR)/textinput/Makefile.d
