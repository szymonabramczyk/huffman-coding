huff: main.o tree.o heap.o codes.o
	$(CC) -o huff main.o tree.o heap.o codes.o

main.o: heap.h tree.h codes.h main.c
	$(CC) -c main.c

heap.o: heap.h heap.c
	$(CC) -c heap.c

tree.o: tree.h heap.h tree.c
	$(CC) -c tree.c

codes.o: codes.h heap.h codes.c
	$(CC) -c codes.c

.PHONY: clean

clean:
	-rm *.o huff