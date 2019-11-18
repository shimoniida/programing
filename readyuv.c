#include <stdio.h>
#include <stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
  FILE *origin;
  //FILE *compare;
  int width = 0;
  int height = 0;
  int read_size = 0;
  int nbframes = 0;
  int size = 0;
  int data = 0;
  unsigned char *filesize = NULL;
  //unsigned char *ofsize = NULL;

  width = atoi(argv[2]);
  height = atoi(argv[3]);
  nbframes = atoi(argv[4]);
  //printf("%d",width);

  if(NULL ==(origin=fopen(argv[1],"rb"))){
    fprintf(stderr,"Can not open input file\n");
    exit(1);
  }
  /*if(NULL ==(compare=fopen(argv[2],"wb"))){
    fprintf(stderr,"Can not open output file\n");
    exit(1);
  }*/

  filesize = (unsigned char*)malloc((sizeof(unsigned char)*width*height*nbframes));
  
  if((filesize==NULL)){//||(ofsize==NULL)){
    printf("malloc error\n");
    fflush(stdout);
    free(filesize);
    exit(1);
  }
  
  memset(filesize,0,sizeof(unsigned char)*width*height*nbframes);
  //memset(ofsize,0,sizeof(unsigned char)*width*height*nbframes);

  read_size = width;
 int i,j;
 //int ret;
  for(i=0; i<nbframes;i++){
    for(j=0;j<height;j++){
      int fs;
      fs = fread(filesize,1,read_size,origin);
      printf("%d\n",*filesize);
      //fs++;
    }
    origin += width*height/2;
  }
  
  //fwrite(filesize,read_size,1,compare);
  
  return 0;
}