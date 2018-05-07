#include"../../hdfs.h"
#include"func.h"
void hdfs_down(const char* filename) {
 
       hdfsFS fs =hdfsConnect("hdfs://127.0.0.1:9000/", 0); 
        if(!fs)
	{
	   perror("hdfsFS:");
	}
	char writePath[500]={0};
	memset(writePath,0,sizeof(writePath));
	strcat(writePath,"hdfs://127.0.0.1:9000/");
	strcat(writePath,filename);
	int fd = open(filename,O_CREAT|O_WRONLY,0666);
        hdfsFile readFile = hdfsOpenFile(fs,writePath, O_RDONLY|O_CREAT, 0, 0, 0);
        if(!readFile) {
              fprintf(stderr, "Failed toopen %s for writing!\n", writePath);
              exit(-1);
        }
	char buffer[1024]={0};
        tSize ret = 0; 
	while(memset(buffer,0,sizeof(buffer)),(ret=hdfsRead(fs,readFile, buffer, sizeof(buffer)))>0){
		write(fd,buffer,ret);
	}
        close(fd);
        hdfsCloseFile(fs, readFile);
}


