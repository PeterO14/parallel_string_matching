#include "utils.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


// Traverse the directory recursively and search for pattern in files.
void dirTraverse(const char* name, char* pattern) {
	// Necessary Variables
	DIR* directory;
	struct stat metaData;
	int linkChecker;
	struct dirent* entry;
	char* newPath[256];
	
	// Recursively traverse the directory and call SearchForPattern when neccessary
	directory = opendir(name);
	if (directory == NULL) {
		printf("Can't open directory: %s\n", name);
		exit(0);
	}

	while((entry = readdir(directory)) != NULL) {
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name," ")) {

		} else {
			// Entry is a directory
			if (entry->d_type == DT_DIR) {
				char* newPath = stringConcatenation(name, "/");
				newPath = stringConcatenation(newPath, entry->d_name);
				dirTraverse(newPath, pattern);
			// Entry is a file
			} else if (entry->d_type == DT_REG) {
				char* newPath = stringConcatenation(name, "/");
				newPath = stringConcatenation(newPath, entry->d_name);
				searchPatternInFile(newPath, pattern);
			}
		}
	}
}

int main(int argc, char** argv){
	
	// Check for correct number of arguments
	if(argc !=3){
		fprintf(stderr,"Child process : %d recieved %d arguments, expected 3 \n",getpid(), argc);
		fprintf(stderr,"Usage child.o [Directory Path] [Pattern] \n");
		exit(EXIT_FAILURE);
	}
	
	char* path = argv[1];
	char* pattern = argv[2];

	// printf("Path: %s\n", path);
	
	close(STDIN_FILENO);
	
	dirTraverse(path,pattern);
	
	close(STDOUT_FILENO);
	
	exit(EXIT_SUCCESS);
}