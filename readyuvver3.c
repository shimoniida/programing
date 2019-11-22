#include <stdio.h>
#include <stdlib.h>
#include<string.h>



int main(int argc, char *argv[]){
  FILE *origin;
  FILE *compare;
  unsigned int width = 0;
  unsigned  int height = 0;
  unsigned int read_size = 0;
  unsigned int nbframes = 0;
  unsigned int size = 0;
  unsigned int data = 0;
  unsigned char *filesize = NULL;
  unsigned char *Y = NULL;
  unsigned char *U = NULL;
  unsigned char *V = NULL;


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
  Y = (unsigned char*)malloc(sizeof(unsigned char)*width*height*nbframes);
  U = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2*nbframes);
  V = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2*nbframes);

  
  memset(filesize,0,sizeof(unsigned char)*width*height*nbframes*3);

  read_size = width;
int offset;
//only read one frame
for(int i=0;i<nbframes;i++){
    for(int j=0;j<height;j++){
      fread(filesize,1,read_size,origin);
      //printf("%d\n",*filesize);
      //printf("Y pointer =%p\nfilesize pointer = %p",&Y,&filesize);
      //fwrite(filesize,1,read_size,compare);
      //printf("filesize =%ld\n",*filesize);
      //fwrite(Y,1,read_size,compare);
      //printf("%d\n",*Y);
      memcpy(Y,filesize,read_size);
      //printf("%d\n",*Y);
      fwrite(Y,1,read_size,compare);
    }
    for(int k=0;k<height/2;k++){
        fread(filesize,1,read_size/2,origin);
        //fwrite(filesize,1,read_size/2,compare);
        U = filesize;
        //printf("filesize =%ld\n",*filesize);
    }

for(int l=0;l<height/2;l++){
        fread(filesize,1,read_size/2,origin);
        //fwrite(filesize,1,read_size/2,compare);
        V = filesize;
        //printf("filesize =%ld\n",*filesize);
    }
    //printf("file position = %ld\n",ftell(origin));
    /*if(i==171||i==172||i==173||i==174){
        printf("file position = %ld\n",ftell(origin));
    }*/
    //offset = ((width/2*height/2)*2);
    //fseek(origin,offset,SEEK_CUR);
  //origin += (width*height)/2;

            /*memcpy(Y,filesize,width*height);
            offset = width*height/2;
            filesize += offset;
            printf("%d\n",Y);
            //fwrite(Y,1,read_size,compare);*/

}
  
  
  
  
  
  
  
  fclose(origin);
  fclose(compare);


  return 0;
}