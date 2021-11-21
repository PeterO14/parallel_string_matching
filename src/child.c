/*test machine: csel-kh1260-15.cselabs.umn.edu
* group number: G[55]
* name: Isaac Blaine-Sauer , [Peter Ortiz]
* x500: BLAIN075 , [ORTIZ363] */ 

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include<string.h>
#include<stdlib.h>
#include "utils.h"
#include <errno.h>
#include <sys/stat.h>


/*
	Traverse the directory recursively and search for pattern in files.
	@params:
		name - path to the directory
		pattern - pattern to be recusrively searched in the directory
		
	Note: Feel free to modify the function header if neccessary
	
*/
void dirTraverse(const char* name, char* pattern) {
	// Necessary Variables
	DIR* dir;
	struct stat metaData;
	int linkChecker;
	struct dirent* entry;
	char* newPath[256];
	
	// Recursively traverse the directory and call SearchForPattern when neccessary
	dir = opendir(name);
	if (dir == NULL) {
		printf("Can't open directory: %s\n", name);
		exit(0);
	}
	while((entry = readdir(dir)) != NULL) {
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
