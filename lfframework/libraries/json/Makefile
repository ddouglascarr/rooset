include ../../Makefile.options

json.so: json.o
	$(LD) $(LDFLAGS) -o json.$(SLIB_EXT) json.o

json.o: json.c
	$(CC) -c $(CFLAGS) -o json.o json.c

clean::
	rm -f json.so json.o
