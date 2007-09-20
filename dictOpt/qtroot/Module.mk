# Module.mk for qtroot module
# Copyright (c) 2003 Valeri Fine
#
# Author: Valeri Fine, 20/5/2003

MODDIR       := qtroot
MODDIRS      := $(MODDIR)/src
MODDIRI      := $(MODDIR)/inc

QTROOTDIR    := $(MODDIR)
QTROOTDIRS   := $(QTROOTDIR)/src
QTROOTDIRI   := $(QTROOTDIR)/inc

##### libQtRoot #####
QTROOTL      := $(MODDIRI)/LinkDef.h
QTROOTDS     := $(MODDIRS)/G__QtRoot.cxx
QTROOTDO     := $(QTROOTDS:.cxx=.o)
QTROOTDH     := $(QTROOTDS:.cxx=.h)

QTROOTH      := $(filter-out $(MODDIRI)/LinkDef%,$(wildcard $(MODDIRI)/*.h))
QTROOTS      := $(filter-out $(MODDIRS)/G__%,$(wildcard $(MODDIRS)/*.cxx))
QTROOTO      := $(QTROOTS:.cxx=.o)

QTROOTDEP    := $(QTROOTO:.o=.d) $(QTROOTDO:.o=.d)

QTROOTLIB    := $(LPATH)/libQtRoot.$(SOEXT)
QTROOTMAP    := $(QTROOTLIB:.$(SOEXT)=.rootmap)

# used in the main Makefile
ALLHDRS     += $(patsubst $(MODDIRI)/%.h,include/%.h,$(QTROOTH))
ALLLIBS     += $(QTROOTLIB)
ALLMAPS     += $(QTROOTMAP)

# include all dependency files
INCLUDEFILES += $(QTROOTDEP)

##### local rules #####
include/%.h:    $(QTROOTDIRI)/%.h
		cp $< $@

$(QTROOTLIB):   $(QTROOTO) $(QTROOTDO) $(ORDER_) $(MAINLIBS) $(QTROOTLIBDEP)
		@$(MAKELIB) $(PLATFORM) $(LD) "$(LDFLAGS)" \
		   "$(SOFLAGS)" libQtRoot.$(SOEXT) $@ "$(QTROOTO) $(QTROOTDO)" \
		   "$(QTROOTLIBEXTRA) $(QTLIBDIR) $(QTLIB)"

$(QTROOTDS):    $(QTROOTH) $(QTROOTL) $(ROOTCINTTMPEXE)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(QTROOTH) $(QTROOTL)

$(QTROOTMAP):   $(RLIBMAP) $(MAKEFILEDEP) $(QTROOTL)
		$(RLIBMAP) -o $(QTROOTMAP) -l $(QTROOTLIB) \
		   -d $(QTROOTLIBDEPM) -c $(QTROOTL)

all-qtroot:     $(QTROOTLIB) $(QTROOTMAP)

clean-qtroot:
		@rm -f $(QTROOTO) $(QTROOTDO)

clean::         clean-qtroot

distclean-qtroot:  clean-qtroot
		@rm -f $(QTROOTDEP) $(QTROOTDS) $(QTROOTDH) $(QTROOTLIB) $(QTROOTMAP)

distclean::     distclean-qtroot


##### extra rules ######
$(sort $(QTROOTO)) $(QTROOTDO): CXXFLAGS += $(GQTCXXFLAGS)
