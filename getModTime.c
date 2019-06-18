#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<stdlib.h>
#include <stdio.h>

time_t getModTime(char* name) {


struct stat sb;

if(stat(name,&sb)== -1) {
    fprintf(stderr,"stat failed");
    exit(EXIT_FAILURE);
}

return sb.st_mtime;

}