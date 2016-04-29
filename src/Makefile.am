CLEANFILES = *.d *.so *_rdict.* *_rdict_* *.rootmap
MAINTAINERCLEANFILES = Makefile.in

# EXTRA_DIST = some extra files, e.g. test-data

lib_LTLIBRARIES = libmeasure.la

libmeasure_la_SOURCES = \
	HelloWorld.cxx

libmeasure_la_headers = \
	HelloWorld.h

pkginclude_HEADERS = $(libmeasure_la_headers)

bin_PROGRAMS = measure

noinst_PROGRAMS =

measure_SOURCES = measure.cxx
measure_LDADD = libmeasure.la
# measure_LDFLAGS = -static


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
