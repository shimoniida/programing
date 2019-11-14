#include <tiffio.h>
#include <windows.h>

BOOL WriteTIFF( LPCTSTR lpszFName, int nImageWidth, int nImageLength, LPSTR pImg )
{
  int nRowsPerStrip;
  int nStripSize;
  int nRow;
  int nStrip = 0;
  int i, j;
  LPSTR pData = pImg;
  
  TIFF *pOut = TIFFOpen( lpszFName, "w" );
  if ( !pOut )
  {
    return FALSE;
  }
  
  if ( TIFFSetField( pOut, TIFFTAG_IMAGEWIDTH, nImageWidth ) != 1 )
    return false;      //横
  if ( TIFFSetField( pOut, TIFFTAG_IMAGELENGTH, nImageLength ) != 1 )
    return false;      //縦
  if ( TIFFSetField( pOut, TIFFTAG_COMPRESSION, COMPRESSION_NONE ) != 1 )
    return false;      //圧縮形式
  if ( TIFFSetField( pOut, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB ) != 1 )
    return false;      //種別
  if ( TIFFSetField( pOut, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(pOut, 0) ) != 1 )
    return false;      //ストリップあたり行数
  if ( TIFFSetField( pOut, TIFFTAG_SAMPLESPERPIXEL, 3 ) != 1 )
    return false;      //色数
  if ( TIFFSetField( pOut, TIFFTAG_BITSPERSAMPLE, 8 ) != 1 )
    return false;      //色深度
  if ( TIFFSetField( pOut, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG ) != 1 )
    return false;      //PLANARCONFIG
  if ( TIFFSetField( pOut, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT ) != 1 )
    return false;    //Orientation

  TIFFGetFieldDefaulted( pOut, TIFFTAG_ROWSPERSTRIP, &nRowsPerStrip );
  for ( i = 0; i < nImageLength; i += nRowsPerStrip )
  {
    nRow = ( i+nRowsPerStrip > nImageLength ) ? nImageLength - i : nRowsPerStrip;
    nStripSize = TIFFVStripSize( pOut, nRow );
    if ( TIFFWriteEncodedStrip( pOut, nStrip++, pData, nStripSize ) < 0 )
    {
      TIFFClose( pOut );
      return FALSE;
    }
    pData += nStripSize;
  }
  
  TIFFClose( pOut );
}

int main( int argc, char *argv[] )
{
  LPSTR pImg = (LPSTR)malloc( 500*500*3 );
  
  for ( int i = 0; i < 500; i++ )
  {
    for ( int j = 0; j < 500; j++ )
    {
      pImg[i*1500+j*3+0] = (BYTE)((double)(i+j)/1000*255);
      pImg[i*1500+j*3+1] = (BYTE)((double)j/500*255);
      pImg[i*1500+j*3+2] = (BYTE)((double)i/500*255);
    }
  }
  WriteTIFF( "test_write.tif", 500, 500, pImg );
  
  free( pImg );
  
  return 0;
}