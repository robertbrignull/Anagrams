all: anagrams

anagrams: anagrams.c
	gcc -Wall -std=c99 anagrams.c -o anagrams

clean:
	rm -f anagrams
