all:
	gcc *.c -o maze

clean:
	rm -f *.o maze

run-all: all
	./run-all.sh

.PHONY : clean all
