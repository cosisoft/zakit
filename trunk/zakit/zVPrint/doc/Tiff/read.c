#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  TIFF *image;
  uint16 photo, bps, spp, fillorder;
  uint32 width;
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  char *buffer, tempbyte;
  unsigned long bufferSize, count;

  // Open the TIFF image
  if((image = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  // Check that it is of a type that we support
  if((TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &bps) == 0) || (bps != 1)){
    fprintf(stderr, "Either undefined or unsupported number of bits per sample\n");
    exit(42);
  }

  if((TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, &spp) == 0) || (spp != 1)){
    fprintf(stderr, "Either undefined or unsupported number of samples per pixel\n");
    exit(42);
  }

  // Read in the possibly multiple strips
  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  imageOffset = 0;
  
  bufferSize = TIFFNumberOfStrips (image) * stripSize;
  if((buffer = (char *) malloc(bufferSize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory for the uncompressed image\n");
    exit(42);
  }
  
  for (stripCount = 0; stripCount < stripMax; stripCount++){
    if((result = TIFFReadEncodedStrip (image, stripCount,
				      buffer + imageOffset,
				      stripSize)) == -1){
      fprintf(stderr, "Read error on input strip number %d\n", stripCount);
      exit(42);
    }

    imageOffset += result;
  }

  // Deal with photometric interpretations
  if(TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo) == 0){
    fprintf(stderr, "Image has an undefined photometric interpretation\n");
    exit(42);
  }
  
  if(photo != PHOTOMETRIC_MINISWHITE){
    // Flip bits
    printf("Fixing the photometric interpretation\n");

    for(count = 0; count < bufferSize; count++)
      buffer[count] = ~buffer[count];
  }

  // Deal with fillorder
  if(TIFFGetField(image, TIFFTAG_FILLORDER, &fillorder) == 0){
    fprintf(stderr, "Image has an undefined fillorder\n");
    exit(42);
  }
  
  if(fillorder != FILLORDER_MSB2LSB){
    // We need to swap bits -- ABCDEFGH becomes HGFEDCBA
    printf("Fixing the fillorder\n");

    for(count = 0; count < bufferSize; count++){
      tempbyte = 0;
      if(buffer[count] & 128) tempbyte += 1;
      if(buffer[count] & 64) tempbyte += 2;
      if(buffer[count] & 32) tempbyte += 4;
      if(buffer[count] & 16) tempbyte += 8;
      if(buffer[count] & 8) tempbyte += 16;
      if(buffer[count] & 4) tempbyte += 32;
      if(buffer[count] & 2) tempbyte += 64;
      if(buffer[count] & 1) tempbyte += 128;
      buffer[count] = tempbyte;
    }
  }
     
  // Do whatever it is we do with the buffer -- we dump it in hex
  if(TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width) == 0){
    fprintf(stderr, "Image does not define its width\n");
    exit(42);
  }
  
  for(count = 0; count < bufferSize; count++){
    printf("%02x", (unsigned char) buffer[count]);
    if((count + 1) % (width / 8) == 0) printf("\n");
    else printf(" ");
  }

  TIFFClose(image);
}
