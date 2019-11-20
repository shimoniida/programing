#include <stdio.h>
#include <stdlib.h>
#include<string.h>



int main(int argc, char *argv[]){
  FILE *origin;
  FILE *compare;
  int width = 0;
  int height = 0;
  int read_size = 0;
  int nbframes = 0;
  int size = 0;
  int data = 0;
  long filesize = 0; 



  width = atoi(argv[2]);
  height = atoi(argv[3]);
  nbframes = atoi(argv[4]);
  //printf("%d",width);

  if(NULL ==(origin=fopen(argv[1],"rb"))){
    fprintf(stderr,"Can not open input file\n");
    exit(1);
  }
  if(NULL ==(compare=fopen(argv[5],"wb"))){
    fprintf(stderr,"Can not open output file\n");
    exit(1);
  }
    fseek(origin,0,2);
    filesize = ftell(origin);
    fseek(origin,0,0);

  read_size = width;

//only read one frame
for(int i=0;i<nbframes;i++){
    for(int j=0;j<height;j++){
      fread(filesize,1,read_size,origin);
      //printf("%d\n",*filesize);
      fwrite(filesize,1,read_size,compare);
    }
    //printf("file position = %ld\n",ftell(origin));
    fseek(origin,(width/2*height/2)*2,SEEK_CUR);
  //origin += (width*height)/2;
}

  fclose(origin);
  fclose(compare);
  return 0;
}