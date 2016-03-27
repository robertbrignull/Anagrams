#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

int min(int x, int y) { return (x < y) ? x : y; }
int max(int x, int y) { return (x > y) ? x : y; }

// The purpose of this is to solve anagrams, you give it some characters and
// it finds as many words as possible that can be made from them.

// Usage is:
//     anagrams word                       - to find all anagrams that can be made
//     anagrams --generateWordList file    - to preprocess a word list file, saved to words.txt

int wordLength = 0;

int compareStrings(const void *a, const void *b) {
	return strncmp((char*) a, (char*) b, wordLength);
}

int compareWords(const void* a, const void* b) {
	return strncmp(((char*) a) + wordLength, ((char*) b) + wordLength, wordLength);
}

int main(int argc, char **argv) {
	if (argc >= 3 && strcmp(argv[1], "--generateWordList") == 0) {
		FILE* wordListIn = fopen(argv[2], "r");

		if (wordListIn == NULL) {
			printf("Could not open word list input file.\n");
			exit(1);
		}

		////////////////////
		// Get the number of words in the file
		// and the length of the longest one

		int lineCount = 0;
		int maxLength = 0;

		const int bufferSize = 1000;
		char buffer[bufferSize];

	    while (fgets(buffer, bufferSize, wordListIn)) {
	    	lineCount += 1;
	    	maxLength = max(maxLength, strlen(buffer));
	    }

	    ///////////////////
	    // Now allocate an array for them all, for each word storing
	    // a normal version and one where the letters are sorted.
	    // Also, sort the whole lot based on their sorted versions.

	    wordLength = maxLength + 1;
	    char *wordList = (char*) malloc(lineCount * wordLength * 2);

	    if (wordList == NULL) {
	    	printf("Could not allocate data for wordlist\n");
	    	exit(1);
	    }

	    fseek(wordListIn, 0, SEEK_SET);

	    for (int i = 0; i < lineCount; i++) {
	        fgets(buffer, bufferSize, wordListIn);
	    	int len = strlen(buffer);

	    	strncpy(&wordList[wordLength * i * 2], buffer, len - 1);
	    	strncpy(&wordList[wordLength * (i * 2 + 1)], buffer, len - 1);
	    	qsort(&wordList[wordLength * (i * 2 + 1)], len - 1, 1, compareStrings);
	    }

	    fclose(wordListIn);

	    qsort(wordList, lineCount, wordLength * 2, compareWords);

	    /////////////////
	    // Output the newly generated wordList

	    FILE *wordListOut = fopen("words.txt", "w");

		if (wordListOut == NULL) {
			printf("Could not open word list output file.\n");
			exit(1);
		}

		fprintf(wordListOut, "%d %d\n", lineCount, wordLength);

	    for (int i = 0; i < lineCount; i++) {
	    	fprintf(wordListOut, "%s %s\n", &wordList[wordLength * i * 2], &wordList[wordLength * (i * 2 + 1)]);
	    }

	    fclose(wordListOut);
	    free(wordList);
	}
	else {
		//////////////
		// Read the ready formatted word list

		FILE *wordListIn = fopen("words.txt", "r");

		int lineCount;
		fscanf(wordListIn, "%d %d ", &lineCount, &wordLength);

		char *wordList = (char*) malloc(lineCount * wordLength * 2);

	    if (wordList == NULL) {
	    	printf("Could not allocate data for wordlist\n");
	    	exit(1);
	    }

		for (int i = 0; i < lineCount; i++) {
			fscanf(wordListIn, "%s %s ", &wordList[wordLength * i * 2], &wordList[wordLength * (i * 2 + 1)]);
		}

		for (int n = 1; n < argc; n++) {
			///////////////
			// Use binary search to find our given word

			char *targetWord = argv[n];
			int targetWordLen = strlen(targetWord);

			char *sortedWord = (char*) malloc(targetWordLen + 1);
			strcpy(sortedWord, targetWord);
			qsort(sortedWord, targetWordLen, 1, compareStrings);

			int i = 0;
			int j = lineCount - 1;

			// Invariant: targetWord <- wordList[i..j]

			while (i < j) {
				int m = (i + j) / 2;

				if(strcmp(sortedWord, &wordList[wordLength * (m * 2 + 1)]) > 0) {
					i = m + 1;
				}
				else {
					j = m;
				}
			}

			//////////////
			// Now read off the anagrams

			if (strcmp(sortedWord, &wordList[wordLength * (i * 2 + 1)]) == 0) {
				printf("Anagrams for '%s' found:\n", targetWord);
				do
				{
				 	printf("    %s\n", &wordList[wordLength * i * 2]);
				 	i++;
				} while (strcmp(sortedWord, &wordList[wordLength * (i * 2 + 1)]) == 0);
			}
			else {
				printf("No anagrams found for '%s'.\n", targetWord);
			}

			printf("\n");
		}
	}

	return 0;
}
