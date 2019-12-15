mldap.so: mldap.o
	ld -shared -L/usr/lib -o mldap.so mldap.o /usr/lib/libldap.so

mldap.o: mldap.c
	gcc -g -c -fPIC -I/usr/include -I/usr/include/lua5.1 -Wall -o mldap.o mldap.c

clean::
	rm -f mldap.so mldap.o
