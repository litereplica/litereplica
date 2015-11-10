LIBRARY = libsqlite3.so.1
LIBNICK = libsqlite3.so
SHORT   = sqlite3

# the item below cannot be called SHELL because it's a reserved name
SSHELL  = sqlite3

all: $(LIBRARY) $(SSHELL)

$(LIBRARY): $(SHORT).o
	#gcc -shared -Wl,-soname,$(LIBNICK) -lbinn -lnanomsg $^ -o $@
	gcc -shared -Wl,-soname,$(LIBRARY) -lbinn -lnanomsg $^ -o $@
	strip $(LIBRARY)
	ln -sf $(LIBRARY) $(LIBNICK)

$(SHORT).o: $(SHORT).c litereplica.h litereplica-int.h
	gcc -fPIC -DSQLITE_THREADSAFE=2 -DHAVE_USLEEP -DSQLITE_ENABLE_COLUMN_METADATA -c $<

$(SSHELL): shell.o $(LIBRARY)
	gcc -L. -lsqlite3 -ldl $< -o $@
	strip $(SSHELL)

shell.o: shell/shell.c
	gcc -c $< -o $@

install:
	cp $(LIBRARY) /usr/lib
	cd /usr/lib && ln -sf $(LIBRARY) $(LIBNICK)
	cp $(SSHELL) /usr/bin

clean:
	rm -f *.o $(LIBRARY) $(SSHELL) tests

tests: test.o seatest.o sqlite3dbg.o
	gcc -g -lbinn -lnanomsg -ldl $^ -o $@
	cp tests test/tests
	cd test && ./tests

test.o: test/test.c
	gcc -c $< -o $@

seatest.o: ../common/seatest.c
	gcc -c $< -o $@

sqlite3dbg.o: sqlite3.c
	gcc -g -DSQLITE_DEBUG=1 -DDEBUGPRINT -DSQLITE_THREADSAFE=2 -DHAVE_USLEEP -DSQLITE_ENABLE_COLUMN_METADATA -c $< -o $@

# variables:
#   $@  output
#   $^  all the requirements
#   $<  first requirement
