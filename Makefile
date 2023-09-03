build: libscheduler.so

libscheduler.so: so_scheduler.o list.o
	gcc -Wall -shared so_scheduler.o list.o -o libscheduler.so

so_scheduler.o: so_scheduler.c
	gcc -Wall -fPIC -o so_scheduler.o -c so_scheduler.c

list.o: list.c
	gcc -Wall -fPIC -o list.o -c list.c	

.PHONY: clean

clean:
	rm -f *.o libscheduler.so