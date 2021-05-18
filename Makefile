CPPFLAGS=`pkg-config --cflags-only-I glib-2.0`
CFLAGS=-g #`pkg-config --cflags-only-other glib-2.0`
LDFLAGS=-g `pkg-config --libs-only-other glib-2.0`
LDLIBS=`pkg-config --libs-only-l glib-2.0` -lfl
LOADLIBES=`pkg-config --libs-only-L glib-2.0` -L/usr/lib64/flex-2.5.4a/

all: facile_bison.tab.o facile_lex.o facile

test: clean all Test.class

facile_lex.c: facile_lex.lex
	flex -o $@ < $<

facile_bison.tab.c: facile_bison.y
	bison -d -t -v $<

facile: facile.o facile_lex.o facile_bison.tab.o

%.class: %.j
	java -jar jasmin.jar $<

%.j: %.facile facile
	./facile $<

zip:
	zip -ru facile.zip facile_bison.y facile_lex.lex facile.h facile.c jasmin.jar Makefile *.facile

clean:
	rm -f *.o facile

