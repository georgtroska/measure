CLEANFILES = *.d *.so *_rdict.* *_rdict_* *.rootmap
MAINTAINERCLEANFILES = Makefile.in

# EXTRA_DIST = some extra files, e.g. test-data

lib_LTLIBRARIES = libmeasure.la

#OPTS = -DUSEDAQCORE

libmeasure_la_SOURCES = \
	DCELink.cpp \
	RS232Link.cpp \
	DCEResource.cpp\
	Channel.cpp \
	Param.cpp \
	Limit.cpp \
	Signal.cpp \
	Device.cpp \
	K2000.cpp \
	HP33120A.cpp \
	MeasureChannel.cpp \
	Scan1D.cpp \
	Window1DGraph.cpp \
	ScanSave.cpp \
	TCPLink.cpp \
	PosixSocket.cpp \
	PosixFile.cpp
	

libmeasure_la_headers = \
	DCEResource.h \
	DCELink.h \
	RS232Link.h \
	Channel.h \
	Param.h \
	Limit.h \
	Signal.h \
	Device.h \
	K2000.h \
	ExcLimit.h \
	HP33120A.h \
	MeasEvent.h \
	MeasureChannel.h \
	Scan1D.h \
	Window1DGraph.h \
	ScanSave.h \
	TCPLink.h \
	PosixSocket.h \
	PosixFile.h

pkginclude_HEADERS = $(libmeasure_la_headers)

bin_PROGRAMS = 

noinst_PROGRAMS = \
	RS232LinkTest

RS232LinkTest_SOURCES = RS232LinkTest.cxx
RS232LinkTest_LDADD = libmeasure.la
# RS232LinkTest_LDFLAGS = -static


rootmapdir = $(libdir)
rootmap_DATA =


libmeasure_la_SOURCES += libmeasure_rdict.cxx
rootmap_DATA += .libs/libmeasure.rootmap

rootmap_DATA += .libs/libmeasure_rdict.pcm

libmeasure_rdict.cxx: $(libmeasure_la_headers) measure_LinkDef.h
	$(ROOTCLING) -f $@.tmp -s libmeasure@SHLIBEXT@ -rml libmeasure@SHLIBEXT@ -rmf libmeasure.rootmap.tmp -c $(CPPFLAGS) $(CXXFLAGS) -I$(includedir) $+
	@# Some magic to prefix header names with "$(PACKAGE)/", and only that, in dictionary and rootmap:
	$(GREP) -F -v '"'"`pwd`"'/",' $@.tmp | $(SED) 's|"\([^"]*/\)\?\([^/"]*[.]h\)",|"'$(PACKAGE)/'\2",| ; s|\\"\([^"]*/\)\?\([^/"]*[.]h\)\\"\\n"|\\"'$(PACKAGE)/'\2\\"\\n"|' > $@.tmp2
	$(SED) 's|\$$clingAutoload\$$\([^/""]\+/\)*|$$clingAutoload$$'$(PACKAGE)'/|; /.*DICTPAYLOAD(.*/,/.*)DICTPAYLOAD.*/ s|#include "\([^/"]\+/\)*\(.*\)"|#include <'$(PACKAGE)'/\2>|' $@.tmp2 > $@ && $(RM) $@.tmp $@.tmp2
	$(SED) 's|\(header \+\)\([^ ].*/\)\?\([^ ].*[.]h\)|\1'$(PACKAGE)/'\3|' libmeasure.rootmap.tmp > libmeasure.rootmap && $(RM) libmeasure.rootmap.tmp

.libs/libmeasure.rootmap: libmeasure.la
	cp libmeasure.rootmap libmeasure_rdict.pcm .libs/

.libs/libmeasure_rdict.pcm: .libs/libmeasure.rootmap
