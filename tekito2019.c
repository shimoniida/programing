#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "tiffio.h"
#include"tiff.h"


unsigned long getFilesize(FILE *fp){
 unsigned long size;
 fseek(fp,0,SEEK_END);
 size = ftell(fp);
 fseek(fp,0,SEEK_SET);
 
 return size;
}

int Yuv420toTiff(unsigned char *yuvfile,unsigned long filesize){
    
    //unsigned int filesize;
    int frame_number;
    int i;
    unsigned char *frame;
    unsigned int width=3840;
    unsigned int height=2160;
    unsigned int offset=2;
    frame_number = filesize/(3840*2160*2);
    printf("frame_number:%d\n",frame_number);
    frame_number=1;
    frame = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
    crframe = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2);
    cbframe = (unsigned char*)malloc(sizeof(unsigned char)*width/2*height/2);
    
    TIFF *pOut;
    TIFF *cbOut;
    TIFF *crOut;
    
    if((pOut = TIFFOpen("output.tiff", "w")) == NULL){
     printf("Could not open output.tif for writing\n");
     exit(1);
    }
    
    if((cbOut = TIFFOpen("output.tiff", "w")) == NULL){
     printf("Could not open output.tif for writing\n");
     exit(1);
    }
    if((crOut = TIFFOpen("output.tiff", "w")) == NULL){
     printf("Could not open output.tif for writing\n");
     exit(1);
    }

    for(i=0;i<frame_number;i++){
        offset = i*width*height*(2/3);
        memcpy(frame,yuvfile+(offset),sizeof(unsigned char)*width*height);
       
        if(TIFFSetField(pOut,TIFFTAG_IMAGEWIDTH,width) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_IMAGELENGTH,height) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_BITSPERSAMPLE,8) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_SAMPLESPERPIXEL,1) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_COMPRESSION,COMPRESSION_NONE) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_MINISBLACK) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_FILLORDER,FILLORDER_MSB2LSB) !=1)return -1;
        //if(TIFFSetField(pOut,TIFFTAG_ROWSPERSTRIP,TIFFDefaultStripSize(pOut, 0)) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_ORIENTATION,ORIENTATION_TOPLEFT) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_XRESOLUTION,96)!=1) return -1; //横方向解像度
        if(TIFFSetField(pOut,TIFFTAG_YRESOLUTION,96)!=1) return -1; //高さ方向解像度
        if(TIFFSetField(pOut,TIFFTAG_RESOLUTIONUNIT,RESUNIT_CENTIMETER)!=1) return -1;//解像度の単位はセンチメートル
        if(TIFFSetField(pOut,TIFFTAG_SOFTWARE,"LibTiffTest")!=1) return -1;
        TIFFWriteEncodedStrip(pOut,0,frame,sizeof(unsigned char)*3840*2160);

        memcpy(frame,yuvfile+(offset),sizeof(unsigned char)*width/2*height/2);
       
        if(TIFFSetField(pOut,TIFFTAG_IMAGEWIDTH,width) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_IMAGELENGTH,height) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_BITSPERSAMPLE,8) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_SAMPLESPERPIXEL,1) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_COMPRESSION,COMPRESSION_NONE) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_MINISBLACK) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_FILLORDER,FILLORDER_MSB2LSB) !=1)return -1;
        //if(TIFFSetField(pOut,TIFFTAG_ROWSPERSTRIP,TIFFDefaultStripSize(pOut, 0)) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_ORIENTATION,ORIENTATION_TOPLEFT) !=1) return -1;
        if(TIFFSetField(pOut,TIFFTAG_XRESOLUTION,96)!=1) return -1; //横方向解像度
        if(TIFFSetField(pOut,TIFFTAG_YRESOLUTION,96)!=1) return -1; //高さ方向解像度
        if(TIFFSetField(pOut,TIFFTAG_RESOLUTIONUNIT,RESUNIT_CENTIMETER)!=1) return -1;//解像度の単位はセンチメートル
        if(TIFFSetField(pOut,TIFFTAG_SOFTWARE,"LibTiffTest")!=1) return -1;
        TIFFWriteEncodedStrip(pOut,0,frame,sizeof(unsigned char)*3840*2160);
        
    }
    TIFFClose(pOut);
    TIFFClose(cbOut);
    TIFFClose(crOut);
    printf("END\n");
    fflush(stdout);
    return 0;
}

int main(int argc,char **argv){
 FILE *fp;
 unsigned char *data;
 unsigned long size=0;

 if((fp=fopen(argv[1],"rb"))==NULL){
     printf("sine");
     fflush(stdout);
 }

 size = getFilesize(fp);
 printf("frame_number:%d\n",size);
 data = (unsigned char*)malloc(sizeof(unsigned char)*size);
 fread(data,size,1,fp);
 if(Yuv420toTiff(data,size)==-1){
     printf("ERROR\n");
 }
fflush(stdout);
 return 0;
}