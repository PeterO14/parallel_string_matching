#ifndef UTILS_H
#define UTILS_H

#define MAX_INODE_LENGTH 128
#define MAX_PATH_LENGTH 1024
#define MAX_READ_LENGTH 5000
#define MAX_ROOT_SUBDIRS 100
#define MAX_WRITE_LENGTH 5000 


void searchPatternInFile(char* path, char* pattern);

char* stringConcatenation(const char* string1, const char* string2);

void isSoftLink(const char* fileName);

#endif