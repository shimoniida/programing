#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>



int main(int argc, char *argv[]){
  FILE *Origin;
  FILE *Compare;
  unsigned int width = 0;
  unsigned  int height = 0;
  unsigned int read_size = 0;
  unsigned int nbframes = 0;
  unsigned int size = 0;
  unsigned int data = 0;
  unsigned char *originsize = NULL;
  unsigned char *comparesize = NULL;
  unsigned char *Y_origin = NULL;
  unsigned char *U_origin = NULL;
  unsigned char *V_origin = NULL;
  unsigned char *Y_compare = NULL;
  unsigned char *U_compare = NULL;
  unsigned char *V_compare = NULL;
  double sum_numerator = 0;
  double sum_denominator = 0;
  double psnr = 0;


  width = atoi(argv[2]);
  height = atoi(argv[3]);
  nbframes = atoi(argv[4]);
  //printf("%d",width);

  
//file open
  if(NULL ==(Origin=fopen(argv[1],"rb"))){
    fprintf(stderr,"Can not open input file\n");
    exit(1);
  }
  if(NULL ==(Compare=fopen(argv[5],"wb"))){
    fprintf(stderr,"Can not open output file\n");
    exit(1);
  }
//file region secure
  originsize = (unsigned char*)malloc((sizeof(unsigned char)*width*height*nbframes*3));
  comparesize = (unsigned char*)malloc((sizeof(unsigned char)*width*height*nbframes*3));
  if((originsize==NULL)||(comparesize==NULL)){
    printf("malloc error\n");
    free(originsize);
    free(comparesize);
    exit(1);
  }
  
  //Y,U,V region secure
  Y_origin = (unsigned char*)malloc(sizeof(unsigned char)*width*height*nbframes);
  U_origin = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2*nbframes);
  V_origin = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2*nbframes);
  Y_compare = (unsigned char*)malloc(sizeof(unsigned char)*width*height*nbframes);
  U_compare = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2*nbframes);
  V_compare = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2*nbframes);

  //each region initialization
  memset(originsize,0,sizeof(unsigned char)*width*height*nbframes*3);
  memset(Y_origin,0,sizeof(unsigned char)*width*height*nbframes);
  memset(U_origin,0,sizeof(unsigned char)*width/2*height/2*nbframes);
  memset(V_origin,0,sizeof(unsigned char)*width/2*height/2*nbframes);
  memset(comparesize,0,sizeof(unsigned char)*width*height*nbframes*3);
  memset(Y_compare,0,sizeof(unsigned char)*width*height*nbframes);
  memset(U_compare,0,sizeof(unsigned char)*width/2*height/2*nbframes);
  memset(V_compare,0,sizeof(unsigned char)*width/2*height/2*nbframes);

  read_size = width;
  //printf("%d\n",read_size);

    for(int i=0;i<nbframes;i++){
        for(int j=0;j<height;j++){
            fread(originsize,1,read_size,Origin);
            fread(comparesize,1,read_size,Compare);
            memcpy(Y_origin,originsize,read_size);   //read only Y
            memcpy(Y_compare,comparesize,read_size);
            //printf("%d\n",fread(originsize,1,read_size,Origin));
            //fwrite(Y,1,read_size,compare);  //output only Y
        }
        for(int k=0;k<height/2;k++){
            fread(originsize,1,read_size/2,Origin);
            fread(comparesize,1,read_size,Compare);   
            memcpy(U_origin,originsize,read_size/2);     //read only U
            memcpy(U_compare,comparesize,read_size/2);
        }

        for(int l=0;l<height/2;l++){
            fread(originsize,1,read_size/2,Origin);
            fread(comparesize,1,read_size/2,Compare);
            memcpy(V_origin,originsize,read_size/2);     //read only V
            memcpy(V_compare,comparesize,read_size/2);
        }
    
    sum_numerator = width*height;
    sum_numerator = sum_numerator*255*255;

    for(int m=0; m<height; m++){
        for(int n=0; n<width; n++){
            sum_denominator += (Y_origin[m*width +n]-Y_compare[m*width +n])*(Y_origin[m*width +n]-Y_compare[m*width +n]);
        }
    }
        //printf("%ld\n",sum_numerator);

        psnr = 10*log10(sum_numerator/sum_denominator);
        //printf("%ld\n",psnr);

    }
  fclose(Origin);
  fclose(Compare);
  return 0;
}