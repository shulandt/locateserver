
PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/local/bin

all: locate_server

rtl-scan: *.cpp 
	g++ locate_server.cpp nmea.cpp utils.cpp -lpthread -o locate_server
clean:
	rm -f locate_server

install: locate_server
	@install -v -d "$(DESTDIR)$(BINDIR)" && install -s -m 0755 -v locate_server "$(DESTDIR)$(BINDIR)/locate_server"
