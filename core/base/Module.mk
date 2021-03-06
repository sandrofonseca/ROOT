# Module.mk for base module
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

MODNAME      := base
MODDIR       := $(ROOT_SRCDIR)/core/$(MODNAME)
MODDIRS      := $(MODDIR)/src
MODDIRI      := $(MODDIR)/inc

BASEDIR      := $(MODDIR)
BASEDIRS     := $(BASEDIR)/src
BASEDIRI     := $(BASEDIR)/inc

##### libBase (part of libCore) #####
BASEL1       := $(MODDIRI)/LinkDef1.h
BASEL2       := $(MODDIRI)/LinkDef2.h
BASEL3       := $(MODDIRI)/LinkDef3.h
BASEDS1      := $(call stripsrc,$(MODDIRS)/G__Base1.cxx)
BASEDS2      := $(call stripsrc,$(MODDIRS)/G__Base2.cxx)
BASEDS3      := $(call stripsrc,$(MODDIRS)/G__Base3.cxx)
BASEDO1      := $(BASEDS1:.cxx=.o)
BASEDO2      := $(BASEDS2:.cxx=.o)
BASEDO3      := $(BASEDS3:.cxx=.o)

BASEDS       := $(BASEDS1) $(BASEDS2) $(BASEDS3)
BASEDO       := $(BASEDO1) $(BASEDO2) $(BASEDO3)
BASEDH       := $(BASEDS:.cxx=.h)

BASEH1       := $(wildcard $(MODDIRI)/T*.h)
BASEH3       := GuiTypes.h KeySymbols.h Buttons.h TTimeStamp.h TVirtualMutex.h \
                TVirtualPerfStats.h TVirtualX.h TParameter.h \
                TVirtualAuth.h TFileInfo.h TFileCollection.h \
                TRedirectOutputGuard.h TVirtualMonitoring.h TObjectSpy.h \
                TUri.h TUrl.h TInetAddress.h TVirtualTableInterface.h \
                TBase64.h
BASEH3       := $(patsubst %,$(MODDIRI)/%,$(BASEH3))
BASEH1       := $(filter-out $(BASEH3),$(BASEH1))
BASEH        := $(filter-out $(MODDIRI)/LinkDef%,$(wildcard $(MODDIRI)/*.h))
BASES        := $(filter-out $(MODDIRS)/G__%,$(wildcard $(MODDIRS)/*.cxx))
BASEO        := $(call stripsrc,$(BASES:.cxx=.o))

BASEDEP      := $(BASEO:.o=.d) $(BASEDO:.o=.d)

# used in the main Makefile
ALLHDRS     += $(patsubst $(MODDIRI)/%.h,include/%.h,$(BASEH))

# include all dependency files
INCLUDEFILES += $(BASEDEP)

##### local rules #####
.PHONY:         all-$(MODNAME) clean-$(MODNAME) distclean-$(MODNAME)

include/%.h:    $(BASEDIRI)/%.h
		cp $< $@

# Explicitely state this dependency.
# rmkdepend does not pick it up if $(COMPILEDATA) doesn't exist yet.
$(call stripsrc,$(BASEDIRS)/TSystem.d $(BASEDIRS)/TSystem.o): $(COMPILEDATA)
$(call stripsrc,$(BASEDIRS)/TROOT.d $(BASEDIRS)/TROOT.o): $(COMPILEDATA)
 
$(BASEDS1):     $(BASEH1) $(BASEL1) $(ROOTCINTTMPDEP)
		$(MAKEDIR)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(BASEH1) $(BASEL1)
$(BASEDS2):     $(BASEH1) $(BASEL2) $(ROOTCINTTMPDEP)
		$(MAKEDIR)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(BASEH1) $(BASEL2)
$(BASEDS3):     $(BASEH3) $(BASEL3) $(ROOTCINTTMPDEP)
		$(MAKEDIR)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(BASEH3) $(BASEL3)

all-$(MODNAME): $(BASEO) $(BASEDO)

clean-$(MODNAME):
		@rm -f $(BASEO) $(BASEDO)

clean::         clean-$(MODNAME)

distclean-$(MODNAME): clean-$(MODNAME)
		@rm -f $(BASEDEP) $(BASEDS) $(BASEDH)

distclean::     distclean-$(MODNAME)

##### extra rules ######
$(call stripsrc,$(BASEDIRS)/TPRegexp.o): $(PCREDEP)
$(call stripsrc,$(BASEDIRS)/TPRegexp.o): CXXFLAGS += $(PCREINC)

ifeq ($(GCC_MAJOR),4)
ifeq ($(GCC_MINOR),1)
$(call stripsrc,$(BASEDIRS)/TString.o): CXXFLAGS += -Wno-strict-aliasing
$(call stripsrc,$(BASEDIRS)/TContextMenu.o): CXXFLAGS += -Wno-strict-aliasing
endif
endif

$(BASEDO1) $(BASEDO2): $(PCREDEP)
$(BASEDO1) $(BASEDO2): CXXFLAGS += $(PCREINC)
ifeq ($(ARCH),linuxicc)
$(BASEDO3):     CXXFLAGS += -wd191
endif

# Optimize dictionary with stl containers.
$(BASEDO2): NOOPT = $(OPT)
$(BASEDO3): NOOPT = $(OPT)

# rebuild after reconfigure
$(call stripsrc,$(BASEDIRS)/TROOT.o): config/Makefile.config
