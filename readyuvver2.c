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
  unsigned char *filesize = NULL;


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

  filesize = (unsigned char*)malloc((sizeof(unsigned char)*width*height*nbframes*3));
  if(filesize==NULL){
    printf("malloc error\n");
    free(filesize);
    exit(1);
  }
  
  memset(filesize,0,sizeof(unsigned char)*width*height*nbframes*3);

  read_size = width;
int offset;
//only read one frame
for(int i=0;i<nbframes;i++){
    for(int j=0;j<height;j++){
      fread(filesize,1,read_size,origin);
      //printf("%d\n",*filesize);
      fwrite(filesize,1,read_size,compare);
    }
    for(int k=0;k<height/2;k++){
        fread(filesize,1,read_size/2,origin);
        fwrite(filesize,1,read_size/2,compare);
    }

for(int l=0;l<height/2;l++){
        fread(filesize,1,read_size/2,origin);
        fwrite(filesize,1,read_size/2,compare);
    }
    //printf("file position = %ld\n",ftell(origin));
    /*if(i==171||i==172||i==173||i==174){
        printf("file position = %ld\n",ftell(origin));
    }*/
    //offset = ((width/2*height/2)*2);
    //fseek(origin,offset,SEEK_CUR);
  //origin += (width*height)/2;
}

  fclose(origin);
  fclose(compare);
  return 0;
}