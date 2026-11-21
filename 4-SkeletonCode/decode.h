
#ifndef DECODE_H       // Header to prevent multiple inclusions
#define DECODE_H      // Define the header macro

#include "types.h"     // Contains user defined types

// Comment for the structure definition
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1           // Define maximum secret buffer size

#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)       // Define maximum image buffer size based on secret buffer

#define MAX_FILE_SUFFIX 4              // Define maximum file suffix length

typedef struct _DecodeInfo           // Define the DecodeInfo structure
{
    /* Stego Image info */
    char *stego_image_fname;        // Pointer to stego image filename

    FILE *fptr_stego_image;          // File pointer for stego image

    /* Secret File Info */
    char output_fname[200];         // Output filename

    FILE *fptr_output;                // File pointer for output file

    long int extrn_size;               // Extracted extension size

    char extn_output_file[16];         // Extension of output file
     
    long size_output_file;                 // Size of output file

} DecodeInfo;


/* Encoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_for_decode(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Encode secret file extenstion size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_image_to_data(char *data, int size, DecodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
char decode_lsb_to_byte(char *image_buffer);

/* Function to decode LSB to int */
int decode_lsb_to_int(char *image_buffer);

#endif
