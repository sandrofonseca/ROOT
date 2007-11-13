#! /bin/bash

# Dictionary Filename
FILENAME=
# Can be used to produce a list of libraries needed by the header files being parsed
PREFIX=
# Options -c and -p
COPTION=
POPTION=
# Arguments
ARGS=$*
# List of object files
OBJS=
# Used Option for passing the object files (--objec-files or -o)
ARGOBJS=
# User ask for help
HELP=
# Mode (-cint or -reflex or -gccxml)
MODE=
# CXXFLAGS (from root-config if unset)
CXXFLAGS=${CXXFLAGS:-`root-config --cflags`}

# Usage Message
USAGE="Usage: rootcint [-v][-v0-4] [-cint|-reflex|-gccxml] [-l] [-f] [out.cxx] [-o] \"file1.o file2.o...\" [-c] file1.h[+][-][!] file2.h[+][-][!]...[LinkDef.h] 
Only one verbose flag is authorized (one of -v, -v0, -v1, -v2, -v3, -v4) 
and must be before the -f flags 
For more extensive help type: utils/src/rootcint -h"

# Getopt call
TEMP=`getopt -a -o vlf:o:c:hI:p: --long cint,reflex,gccxml,v0,v1,v2,v3,v4,object-files:,symbols-file:,lib-list-prefix: \
     -n 'rootcint' -- "$@"`

# Wrong usage
if [ $? != 0 ] ; then echo "$USAGE" >&2 ; exit 1 ; fi

# Note the quotes around `$TEMP': they are essential!
# getopt evaluation
eval set -- "$TEMP"

# Options and parameters iteration
while true ; do
        case "$1" in
                -v)  shift ;;
                --v0|--v1|--v2|--v3|--v4) shift ;;
                --cint) MODE=$1; shift ;;
                --reflex) MODE=$1; shift ;;
                --gccxml) MODE=$1; shift ;;
                -l) shift ;;
                -p) POPTION="-p"; shift ;;
                -f) FILENAME=$2; shift 2 ;;
                -o|--object-files) OBJS=$2; ARGOBJS=$1; shift 2;;
                -c) COPTION="-c"; shift;;
                -I) shift; break;;
                -.) shift 2;;
                -h) HELP=1; shift 1;;
                --symbols-file) shift 2;;
                --lib-list-prefix) PREFIX=$2; shift 2;;
                --) shift ; break ;;
                *) echo "Internal error!" ;; #exit 1 ;;
        esac
done

if [ "x${FILENAME}" = "x" ]; then
    HELP=1;
fi

# User Needs Help. 
if [ $HELP ] ; then rootcint -h >&2 ; exit 1 ; fi

# Removing old dictionaries if there is any
if [ -e $FILENAME -o -e ${FILENAME%.*}.h -o -e ${FILENAME%.*}"Tmp1".cxx -o -e ${FILENAME%.*}"Tmp2".cxx  ]; 
then 
    rm ${FILENAME%.*}*; 
fi

# We make up the new list of arguments for generating temporary dictionaries
ROOTCINTARGS=${ARGS/$OBJS/} 
ROOTCINTARGS=${ROOTCINTARGS/--object-files/} 
ROOTCINTARGS=${ROOTCINTARGS/-o/}
ROOTCINTARGS=${ROOTCINTARGS/-f/}
ROOTCINTARGS=${ROOTCINTARGS/-cint/}
ROOTCINTARGS=${ROOTCINTARGS/-reflex/}
ROOTCINTARGS=${ROOTCINTARGS/-gccxml/}
ROOTCINTARGS=${ROOTCINTARGS/-I./}
ROOTCINTARGS=${ROOTCINTARGS/$FILENAME/}
ROOTCINTARGS=${ROOTCINTARGS/--cxx/}
ROOTCINTARGS=${ROOTCINTARGS/--lib-list-prefix=/}
ROOTCINTARGS=${ROOTCINTARGS/$PREFIX/}
ROOTCINTARGS=${ROOTCINTARGS/-p/}
ROOTCINTARGS=${ROOTCINTARGS/-c/}

# We remove one score from the mode option name
MODE=${MODE/--/-}

if [ -n $PREFIX ]
then
    ARGS="--lib-list-prefix=$PREFIX -f"
fi


# Temporary dictionaries generation
echo rootcint $MODE $ARGS ${FILENAME%.*}"Tmp1".cxx $COPTION $POPTION -. 1 $ROOTCINTARGS
rootcint $MODE $ARGS ${FILENAME%.*}"Tmp1".cxx $COPTION $POPTION -. 1 $ROOTCINTARGS

# Temporary dictionaries compilation
g++ $CXXFLAGS -pthread -Ipcre/src/pcre-6.4 -I$ROOTSYS/include/ -I. -o ${FILENAME%.*}"Tmp1".o -c ${FILENAME%.*}"Tmp1".cxx

# Symbols extraction (Object Files and Dictionary Tmp1)
nm -g -p --defined-only $OBJS | awk '{printf("%s\n", $3)}' >> ${FILENAME%.*}.nm
nm -g -p --defined-only ${FILENAME%.*}"Tmp1".o | awk '{printf("%s\n", $3)}' > ${FILENAME%.*}.nm

echo rootcint $MODE $ARGS ${FILENAME%.*}"Tmp2".cxx $COPTION  -. 2 $ROOTCINTARGS
rootcint $MODE $ARGS ${FILENAME%.*}"Tmp2".cxx $COPTION -. 2 -L${FILENAME%.*}".nm"  $ROOTCINTARGS

# Temporary Dictionar 2 compilation
g++ $CXXFLAGS -Iinclude -pthread -Ipcre/src/pcre-6.4 -I$ROOTSYS/include/ -I. -o ${FILENAME%.*}"Tmp2".o -c ${FILENAME%.*}"Tmp2".cxx

# Symbols extraction from dictionary Tmp2
nm -g -p --defined-only ${FILENAME%.*}"Tmp2".o | awk '{printf("%s\n", $3)}' >> ${FILENAME%.*}.nm

# We don't need the temporaries anymore
#rm ${FILENAME%.*}"Tmp1".*
#rm ${FILENAME%.*}"Tmp2".*

# Final Dictionary Generation
echo rootcint $MODE $ARGS $FILENAME $COPTION $POPTION -. 3 -L${FILENAME%.*}".nm" $ROOTCINTARGS
rootcint $MODE $ARGS $FILENAME $COPTION $POPTION -. 3 -L${FILENAME%.*}".nm" $ROOTCINTARGS
# We don't need the symbols file anymore
#rm ${FILENAME%.*}.nm





 
