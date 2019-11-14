#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<string.h>
#include<tiff.h>
#include<tiffio.h>
#include<omp.h> 
#include<time.h>

#define WINDOW 11

double Sd(float **y_before,float **y_after, float ave_before ,float ave_after ,int i ,int j,int H,int W){
    double sd = 0;
    int n=0,m=0;
    int p=0,q=0;

	#pragma omp parallel for
    for(n=i-5;n<=i+5;n++){
        for(m=j-5;m<=j+5;m++){
          p=n;
          q=m;
          //printf("%d,%d\n",n,m) ; 
            if(n<0){
              p = p+10;
            }
            else if(n>=H){
                p = p-10;
            }

            if(m<0){
                q = q+10; 
                //y_before[p][q]=0;
                  //y_after[p][q]=0;
            }

            else if(m>=W){
                q = q-10;
                //y_before[p][q]=0;
                  //y_after[p][q]=0;
            }
            sd += (ave_before-y_before[p][q])* (ave_after - y_after[p][q]);
        }
    }
    
    sd = sd/(WINDOW*WINDOW);
 //   printf("sd:%lf\n",sd);
    return sd;
}

//----------------------------------------------------SSIM計算------------------------------------------------------------------------------------------------------------

double cal_ssim(float **y_before,float **y_after,int H,int W){
    double ssim_sum = 0;
    double ssim_ave = 0;
    float c1 =(0.01 * 255)*(0.01*255);
    float c2 =(0.03*255)*(0.03*255);
    float ave_before=0;
    float ave_after=0;
    float sd_before = 0;
    float sd_after = 0;
    float sd_ab = 0;
    int i,j,n,m;
    int p=0,q=0;


    //11*11window  
	#pragma omp parallel for
    for(i=0;i<=H-1;i++){
        for(j=0;j<=W-1;j++){
            
            //printf("ssim_sum:%lf[%d][%d]\n",ssim_sum,i,j);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            for(n=i-5;n<=i+5;n++){
                for(m=j-5;m<=j+5;m++){
                    p=n;
                    q=m;
          //printf("%d,%d\n",n,m) ; 
                if(n<0){
                 //y_before[p][q]=0;
                //y_after[p][q]=0;
                    p = p+10; 
                }
                else if(n>=H){
                    p = p-10;
               // y_before[p][q]=0;
               // y_after[p][q]=0;
                }

            if(m<0){
                q = q+10; 
               // y_before[p][q]=0;
                //y_after[p][q]=0;
            }

            else if(m>=W){
                q = q-10;
               // y_before[p][q]=0;
                //y_after[p][q]=0;
            }
                ave_before += y_before[p][q];
                ave_after += y_after[p][q];
            }
        }
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        ave_before = ave_before/(11*11);
        ave_after = ave_after/(11*11);

        //printf("[%d][%d],ave_b:%f,ave_a:%f\n",i,j,ave_before,ave_after);
        sd_before = sqrt(Sd(y_before,y_before,ave_before,ave_before,i,j,H,W));
        sd_after = sqrt(Sd(y_after,y_after,ave_after,ave_after,i,j,H,W));
        sd_ab = Sd(y_before,y_after,ave_before,ave_after,i,j,H,W);                            
            
            float a = (2*ave_after*ave_before) + c1;
            float b = 2*sd_ab +c2;
            float c = (ave_before*ave_before) + (ave_after*ave_after) + c1;
            float d = (sd_before*sd_before) + (sd_after*sd_after) + c2;
          //  if(i==251&&j==70)printf("[%d][%d]a:%f,b:%f,c:%f,d:%f\n",i,j,a,b,c,d);
            ssim_sum += (a*b)/(c*d);
            
            //if(i<=1070 && j<=5) {
               //printf("ssim_sum:%lf[%d][%d]\n",ssim_sum,i,j);
            ave_after = 0;
            ave_before = 0;

			
        }
    }
    ssim_ave = ssim_sum/(W*H);
    //printf("ssim:%lf\n",ssim_ave);
    return ssim_ave;
}


double cal_psnr(float **y_before,float **y_after,int height,int width){
	int i,j;
	double sum_numerator= 0;
	double sum_denominator = 0;
	double psnr = 0;

	sum_numerator = height*width;
	sum_numerator = sum_numerator*255*255;

	#pragma omp parallel for
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			sum_denominator += (y_before[i][j]-y_after[i][j])*(y_before[i][j]-y_after[i][j]);
		}
	}

	printf("sum_nemurator:%lf\n",sum_numerator);
	//fflush(stdout);

	psnr = 10* log10(sum_numerator / sum_denominator);
	
	return psnr;
}


int main(int argc, char *argv[]){

    FILE *fp_write;
	unsigned char file_before[512];
	unsigned char file_after[512];
	unsigned char file_to_write[512];

	unsigned char file_name_before[5];
	unsigned char file_name_after[5];

	unsigned char connect[3]= "_";
	unsigned char tiff[5]=".tiff";
	unsigned char csv[5]=".csv";
	unsigned char count_string[512];

	unsigned int before_length = 0;
	unsigned int before_width = 0;
	unsigned int after_length = 0;
	unsigned int after_width = 0;
	unsigned int bitpersample_before = 0;
	unsigned int bitpersample_after = 0;
	unsigned int ch = 0;
	unsigned int *rgbaData_before = NULL;
	unsigned char *rgbData_before = NULL;
	unsigned int *rgbaData_after = NULL;
	unsigned char *rgbData_after = NULL;
	int i,j,c;
	int count;
	double psnr=0;
	double sum_psnr = 0;
	double average_psnr = 0;
	double ssim=0;
	double sum_ssim=0;
	double average_ssim=0;
    float **y_before;
	float **y_after;
	TIFF *image1;
	TIFF *image2;
	uint16_t orientation1;
	uint16_t orientation2;
	int NUMBER_OF_SHEETS=1;
	int t1,t2;
	int dt;

	t1 = clock();

	sprintf(file_to_write,"%s_and_%s%s", argv[1] , argv[2] , csv);
    fp_write = fopen(file_to_write,"a");
	fprintf(fp_write,"count,psnr,ssim,\n");
    //#pragma omp parallel for
	for(count = 1; count<(NUMBER_OF_SHEETS +1); count++){
		sprintf(count_string,"%d",count);
	 	for(i=0;i<11;i++){
			file_before[i] = 0;
			file_after[i] = 0;
	 	}


		if((argv[1]==NULL)||(argv[2]==NULL)){
			printf("ファイル名を入力してください\n");
			exit(1);
		}

		if(argv[3]==NULL){
			printf("count : default=1\n");
		}
		else{
			NUMBER_OF_SHEETS = atoi(argv[3]);
		}

		

		strcat(file_before,argv[1]);	//ex
		strcat(file_before,connect);	//ex_
		strcat(file_before,count_string);	//ex_1
		strcat(file_before,".tiff");	//ex_1.tiff

		strcat(file_after,argv[2]);
		strcat(file_after,connect);
		strcat(file_after,count_string);
		strcat(file_after,".tiff");

		image1 = TIFFOpen(file_before,"r");
		image2 = TIFFOpen(file_after,"r");
		

		if(fp_write==NULL){
			printf("Error:can not open fp\n");
			exit(1);
		}


        if(image1==NULL||image2==NULL){
			printf("death\n");
		  	exit(1);
	 	}
		
		if(count==1){
			//tiff画像の高さが同じか確認
			if((!TIFFGetField(image1,TIFFTAG_IMAGELENGTH,&before_length))||(!TIFFGetField(image2,TIFFTAG_IMAGELENGTH,&after_length))||(before_length!=after_length)){
				printf("TIFFGetField length error\n");
				TIFFClose(image1);
				TIFFClose(image2);
				exit(1);
			}
			if((!TIFFGetField(image1,TIFFTAG_IMAGEWIDTH,&before_width))||(!TIFFGetField(image2,TIFFTAG_IMAGEWIDTH,&after_width))||(before_width!=after_width)){
				printf("TIFFGetField width error\n");
				TIFFClose(image1);
				TIFFClose(image2);
				exit(1);
			}

			if((!TIFFGetField(image1,TIFFTAG_BITSPERSAMPLE,&bitpersample_before))||(!TIFFGetField(image2,TIFFTAG_BITSPERSAMPLE,&bitpersample_after))){
				printf("TIFFGetField bitpersample error\n");
				TIFFClose(image1);
				TIFFClose(image2);
				exit(1);
			}
			 

			rgbaData_before = (unsigned int*)malloc(sizeof(unsigned int)*before_width*before_length);
			rgbaData_after = (unsigned int*)malloc(sizeof(unsigned int)*after_width*after_length);

			if((rgbaData_before == NULL)||(rgbaData_after == NULL)){
				printf("malloc rgbaData error\n");
				free(rgbaData_before);
				free(rgbaData_after);
				TIFFClose(image1);
				TIFFClose(image2);
				exit(1);
			}
			rgbData_before = (unsigned char*)malloc(sizeof(unsigned char)*before_width*before_length*3);
			rgbData_after = (unsigned char*)malloc(sizeof(unsigned char)*after_width*after_length*3);

			if((rgbData_before==NULL)||(rgbData_after==NULL)){
				printf("malloc rgbData error\n");
				free(rgbData_before);
				free(rgbData_after);
				exit(1);
			}

			y_before = (float**)malloc(sizeof(float*)*before_length);
			y_after = (float**)malloc(sizeof(float*)*before_length);
    		for(i=0;i<before_length;i++){
				y_before[i] = (float*)malloc(sizeof(float)*before_width);
				y_after[i] = (float*)malloc(sizeof(float)*before_width);
			}

		}

		//printf("-------------------------------------------------------\n");
		if(!TIFFGetField(image1,TIFFTAG_ORIENTATION,&orientation1)){
			printf("image1 eeeor\n");
		}

		
		if(!TIFFGetField(image2,TIFFTAG_ORIENTATION,&orientation2)){
			//printf("image2 eeeor\n");
		}

		//printf("image1 orientation:%d\n",orientation1);
		//printf("image2 orientation:%d\n",orientation2);


		//init
		memset(rgbaData_after,0,sizeof(unsigned int)*before_length*before_width);
		memset(rgbaData_before,0,sizeof(unsigned int)*before_length*before_width);
		memset(rgbData_after,0,sizeof(unsigned char)*before_length*before_width*3);
		memset(rgbData_before,0,sizeof(unsigned char)*before_length*before_width*3);
		//memset(y_before,0,sizeof(float*)*before_length);
		//memset(y_after,0,sizeof(float*)*before_length);
		/*
		for(i=0;i<before_length;i++){
				memset(y_before[i],0,(sizeof(float)*before_width));
				memset(y_after[i],0,(sizeof(float)*before_width));
		}
*/
		if((!TIFFReadRGBAImage(image1,before_width,before_length,rgbaData_before,0))||(!TIFFReadRGBAImage(image2,after_width,after_length,rgbaData_after,0))){
			printf("TIFFReadRGBAImage error\n");
			free(rgbaData_before);
			free(rgbaData_after);
			TIFFClose(image1);
			TIFFClose(image2);
			exit(1);
		}

		//printf("height:%d,width:%d\n",before_length,before_width);

		for(j=0;j<before_length;j++){
			for(i=0;i<before_width;i++){
				y_before[j][i] = 0.299*(TIFFGetR(rgbaData_before[i + j * before_width]))+
							0.587*(TIFFGetG(rgbaData_before[i + j * before_width]))+
							0.114*(TIFFGetB(rgbaData_before[i + j * before_width]));
				

				y_after[j][i] = 0.299*(TIFFGetR(rgbaData_after[i + j * before_width]))+
							0.587*(TIFFGetG(rgbaData_after[i + j * before_width]))+
							0.114*(TIFFGetB(rgbaData_after[i + j * before_width]));
				
				//printf("i:%d,j:%d,Data_before:%lf\n",i,j,Data_before);
				//printf("i:%d,j:%d,Data_after:%lf\n",i,j,Data_after);
				//printf("sum_numeratot:%f\n",sum_numerator);
				//printf("sum_denominator:%f\n",sum_denominator);
			}
		}
		psnr = cal_psnr(y_before,y_after,before_length,before_width);
		sum_psnr += psnr;
		ssim = cal_ssim(y_before,y_after,before_length,before_width);
		sum_ssim += ssim;
		//printf("psnr = %lf\n",psnr);
		//printf("ssim = %lf\n",ssim);
		fprintf(fp_write,"%d FRAME,%lf,%lf,\n",count,psnr,ssim);
		fflush(stdout);
		TIFFClose(image1);
		TIFFClose(image2);
	}

	average_psnr = (sum_psnr/NUMBER_OF_SHEETS);
	average_ssim = (sum_ssim/NUMBER_OF_SHEETS);
    fflush(stdout);
	fprintf(fp_write,"\n");
	fprintf(fp_write,"SUM(PSNR),%lf,\n",sum_psnr);
	fprintf(fp_write,"AVE(PSNR),%lf,\n",average_psnr);
	fprintf(fp_write,"SUM(SSIM),%lf,\n",sum_ssim);
	fprintf(fp_write,"AVE(SSIM),%lf,\n",average_ssim);


	free(rgbaData_before);
	free(rgbaData_after);
	free(rgbData_before);
	free(rgbData_after);
	fclose(fp_write);

	t2 = clock();
	dt = t2-t1;

	printf("計測時間 %d[ms]\n ",dt);


	return 0;
}
