#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main (int arg1, char *arg2[])
{
    pid_t pid;
    pid=fork();
    int pipeFd[2];
    int fileBytesLength;
    char buffer[100];
    char childBuffer[100];
    char* srcFile = arg2[1];
    char* dstFile = arg2[2];

    if (pipe(pipeFd) < 0) {
      printf("Something went wrong creating the pipe!");
      exit(1);
    }
    switch(pid)
    {
      case -1:
        printf("Error forking child process.");
        exit(1);
      case 0:
        close(pipeFd[1]);
        ssize_t num_bytes_child = read(pipeFd[0], childBuffer, sizeof(childBuffer));
        close(pipeFd[0]);
        int targetDesc = open(dstFile, O_CREAT | O_WRONLY);
        write(targetDesc, childBuffer, num_bytes_child);
      default:
        close(pipeFd[0]);
        int fileInDesc = open(srcFile, O_RDONLY);
        ssize_t num_bytes = read(fileInDesc, buffer, sizeof(buffer));
        write(pipeFd[1], buffer, num_bytes);
        close(pipeFd[1]);
    }
    return 0;
}
