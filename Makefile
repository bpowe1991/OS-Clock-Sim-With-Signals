default: Master Worker

master.o: Master.c
	gcc -c Master.c -o master.o

Master: master.o
	gcc master.o -o Master

worker.o: worker.c
	gcc -c worker.c -o worker.o

Worker: worker.o
	gcc worker.o -o Worker

clean:
	-rm -f *.o
	-rm -f Master
    -rm -f Worker
	-rm -f *.out