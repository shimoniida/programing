#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  FILE *origin;
  int width = 0;
  int height = 0;
  int read_size = 0;
  int nbframes = 0;
  unsigned int *filesize = NULL;

  width = atoi(argv[2]);
  height = atoi(argv[3]);
  nbframes = atoi(argv[4]);
  printf("%d",width);

  if(NULL ==(origin=fopen(argv[1],"rb"))){
    fprintf(stderr,"Can not open input file\n");
    exit(1);
  }

  filesize = (unsigned int*)malloc((sizeof(unsigned int)*width*height*nbframes));
  if(filesize==NULL){
    printf("malloc error\n");
    free(filesize);
    exit(1);
  }
  
  memset(filesize,0,sizeof(unsigned int)*width*height*nbframes);


  read_size = width;


  for(int i=0; i<nbframes;i++){
    for(int j=0;j<height;j++){
      fread(filesize,1,read_size,origin);
      printf("%d\n",*filesize);
    }
    filesize = filesize + width*height/2;
  }
  return 0;
}