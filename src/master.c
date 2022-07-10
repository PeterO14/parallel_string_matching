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


int main(int argc, char** argv) {
	// Check for correct number of arguments
	if(argc != 3) {
		fprintf(stderr,"Usage ./a.out [Path to Directory] [Pattern to search \n");
		exit(1);
	}

	// Declare necessary variables 
	char* path = argv[1];
	char* pattern = argv[2];

	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	ssize_t reading;

	DIR* rootDirectory;				 
	struct dirent* directoryEntry;	

	pid_t parentProcess;

	char* newPath[1000];
	int pipefds[10][2];
	int currentPipe = 0;

	long* inodes = (long*)malloc(MAX_ROOT_SUBDIRS*sizeof(long));
    int inodeIterator = 0;
	
	// Open root directory
	rootDirectory = opendir(path);
	if (rootDirectory == NULL) {
		fprintf(stderr, "ERROR: Failed to open directory\n");
		exit(1);
	}

	// Read all files and sub-directories in a directory and spawn child        processes as necessary
	while ((directoryEntry = readdir(rootDirectory)) != NULL) {
		// No need to print out (., ..,  )
		if (!strcmp(directoryEntry->d_name, ".") || !strcmp(directoryEntry->d_name, "..") || !strcmp(directoryEntry->d_name," ")) {

		} else {
			// Directory entry is a sub-directory
			if (directoryEntry->d_type == DT_DIR) {
				currentPipe++;

				// Create pipe between parent and child
				if (pipe(pipefds[currentPipe]) < 0) {
					fprintf(stderr, "ERROR: Failed to execute pipe\n");
					exit(1);
				}

				// Spawn child process
				parentProcess = fork();
				if (parentProcess > 0) {				

				} else if (parentProcess == 0) {
					// Redirect STDOUT to write end of pipe; Close write end of pipe; Close read end of pipe 
					if (dup2(pipefds[currentPipe][1], STDOUT_FILENO) < 0) {
						fprintf(stderr, "ERROR: Failed to execute dup2\n");
						exit(1);
					}
					close(pipefds[currentPipe][0]);		

					// Calling execvp()
					char* newPath = stringConcatenation(path, "/");	
					newPath = stringConcatenation(newPath, directoryEntry->d_name);	
					char* progName = "./child";
					char* args[] = {progName, newPath, pattern};
					execvp(args[0], args);
					fprintf(stderr, "ERROR: Failed toe execute execvp\n");
					exit(0);
					
				} else {
					fprintf(stderr, "ERROR: Failed to spawn child process\n");
				}
			}
			// Directory entry is a file
			if (directoryEntry->d_type == DT_REG) {
				char* newFilePath = stringConcatenation(path, "/");
				newFilePath = stringConcatenation(newFilePath, directoryEntry->d_name);
				searchPatternInFile(newFilePath, pattern);
			}
		}
	}	

	// Wait for all children to complete 
	pid_t terminated_pid;
	while((terminated_pid = wait(NULL)) > 0) {

	}

	// Read data written on pipe by child and write to stdout
	for (int i = 1; i <= currentPipe; i++) {
		char* buf = malloc(MAX_READ_LENGTH);
		memset(buf, '\0', MAX_READ_LENGTH);
		close(pipefds[i][1]);
		while (read(pipefds[i][0], buf, MAX_READ_LENGTH) > 0) {
			printf("%s", buf);
		}
		close(pipefds[i][0]);
		free(buf);
	}
	
	closedir(rootDirectory);
	return 0;
}