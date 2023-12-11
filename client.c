#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mfs.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1);
    }

    char *hostname = argv[1];
    int port = atoi(argv[2]);

    // Initialize connection to the server
    MFS_Init(hostname, port);

    // Create a directory "TestDir"
    MFS_Creat(0, MFS_DIRECTORY, "TestDir");

    // Look up the inode of the created directory
    int dirInode = MFS_Lookup(0, "TestDir");
    printf("Created directory inode: %d\n", dirInode);

    // Create a file "TestFile" inside the directory
    for(int i=0; i<10; i++){
    	char filename[20];  // Assuming a reasonable maximum length for the filename
    sprintf(filename, "TestFile_%d", i);
    MFS_Creat(dirInode, MFS_REGULAR_FILE, filename);
    // Look up the inode of the created file
    int cfileInode = MFS_Lookup(dirInode, filename);
    printf("Created file %d inode: %d\n", i, cfileInode);
	}
    // Look up the inode of the created file
    int fileInode = MFS_Lookup(dirInode, "TestFile_5");
    printf("Created file inode: %d\n", fileInode);

    // Write some data to the file
    char writeBuffer[] = "Hello, MFS!";
    MFS_Write(fileInode, writeBuffer, 0);

    // Read the data from the file
    char readBuffer[MFS_BLOCK_SIZE];
    if (MFS_Read(fileInode, readBuffer, 0) > -1) {
        printf("Read from file: %s\n", readBuffer);
    } else {
        printf("Failed to read from file.\n");
    }

    // Stat information for the file
    MFS_Stat_t fileStat;
    if (MFS_Stat(fileInode, &fileStat) == 0) {
        printf("File Stat - Type: %d, Size: %d\n", fileStat.type, fileStat.size);
    } else {
        printf("Failed to get file stat.\n");
    }

    // Unlink the file
    if (MFS_Unlink(dirInode, "TestFile_3") == 0) {
        printf("File unlinked successfully.\n");
    } else {
        printf("Failed to unlink file.\n");
    }

    // Shutdown the server
    MFS_Shutdown();

    return 0;
}

