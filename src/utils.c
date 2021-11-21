/*test machine: csel-kh1260-15.cselabs.umn.edu
* group number: G[55]
* name: Isaac Blaine-Sauer , [Peter Ortiz]
* x500: BLAIN075 , [ORTIZ363] */ 

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include <sys/stat.h>

/*
	Searches for pattern in file and writes the entire line containing the pattern to STDOUT
	Format for print: [path to file] : Line in the file
	@Params:
		path - path to the file
		pattern : pattern to be searched
*/
void searchPatternInFile(char* path, char* pattern) {
	FILE* fp;
	int bufferLength = 255;
	char buffer[bufferLength];

	fp = fopen(path, "r");
	if (fp == NULL){
		fprintf(stderr,"Error opening file: %s \n",path);
		exit(1);
	}
	// Read file line by line and check if pattern exists in the line
	while (fgets(buffer, bufferLength, fp)) {
		if (strstr(buffer, pattern) != NULL) {
			char* newPath = stringConcatenation(path, ": ");
			newPath = stringConcatenation(newPath, buffer);
			// Write to STDOUT
			int bytesToBeWritten = write(STDOUT_FILENO, newPath, strlen(newPath));
			if (bytesToBeWritten < 0) {
				fprintf(stderr, "ERROR: Failed to write to STDOUT\n");
				exit(1);
			}
		}
	}
	fclose(fp);
}

char* stringConcatenation(const char* string1, const char* string2) {
	char* finalString = malloc(strlen(string1) + strlen(string2) + 1);
	strcpy(finalString, string1);
	strcat(finalString, string2);
	return finalString;
}
 