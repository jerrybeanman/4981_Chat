CC=g++
CFLAGS=-std=c++11 -g -W -Wall -pedantic -c
DEBUG=-DDEBUG
LIBS=src/
INCLUDE_PATH=src/
output: libraries
	$(CC) $(wildcard libraries/*) -I$(INCLUDE_PATH) -o faketerm
	mkdir binary/
	mv faketerm binary/

libraries:
	mkdir libraries/
	cd $(LIBS); \
	$(CC) $(CFLAGS) *.cpp -I../; \
	mv *.o ../libraries/; \

clean:
	rm -rf libraries/ binary/