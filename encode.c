// Include standard and user defined input/output header
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

// Function to get image size for BMP
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;                  // Declare width variable
    fseek(fptr_image, 18, SEEK_SET);       // Seek to the 18th byte in the file

    fread(&width, sizeof(int), 1, fptr_image);    // Read the width value from file
    printf("width = %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image);      // Read the height value from file
    printf("height = %u\n", height);

    return width * height * 3;                    // Return the calculated image capacity
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

// Function to open required files
Status open_files(EncodeInfo *encInfo)
{
    printf("Opening required files\n");

    // Open source image file for reading
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    
    // Check if file opened successfully
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    else
    {
        printf("Opened beautiful.bmp\n");
    }

    // Open secret file for reading
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    
    // Check if file opened successfully
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    else
    {
        printf("Opened secret.txt\n");
    }

    // Open stego image file for writing
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    
    // Check if file opened successfully
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    else
    {
        printf("Opened steged_img.bmp\n");
    }

    printf("Done\n");
    return e_success;
}

// Function to get file size
uint get_file_size(FILE *fptr)
{
    int size;
    fseek(fptr, 0, SEEK_END);                      // Seek to end of file
    size = ftell(fptr);                            // Get current position as size
    rewind(fptr);                                 // Rewind to beginning
    
    return size;
}

// Function to check capacity
Status check_capacity(EncodeInfo *encInfo)
{
    printf("Checking for beautiful. bmp capacity to handle secret.txt\n");

    // Get image capacity
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    rewind(encInfo->fptr_src_image);                   // Rewind source image file
    
    // Get secret file size
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    // Check if image capacity is greater than required
    if(encInfo -> image_capacity > 16 + 32 + 32 + 32 + (encInfo -> size_secret_file * 8))
    {
        printf("Image capacity is grater than secret file\n");
        printf("Done. Found OK\n");
        return e_success;
    }
    else
    {
        printf("Image capacity is lesser than secret file\n");
        return e_failure;
    }
}

// Function to copy BMP header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("Copying Image Header\n");
    
    // Seek to beginning of source
    fseek(fptr_src_image, 0, SEEK_SET);

    unsigned char fheader[60];                // Declare header buffer
    
    // Read 54 bytes from source
    if(fread(fheader, sizeof(unsigned char), 54,fptr_src_image) != 54)
    {
        return e_failure;
    }

    // Write 54 bytes to destination
    if(fwrite(fheader, sizeof(unsigned char), 54, fptr_dest_image) != 54)
    {
        return e_failure;
    }

    printf("Done\n");

    return e_success;
}

// Function to read and validate encode arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check if argv[2] is a .bmp file
    if(strstr(argv[2], ".bmp") != NULL)
    {
        // Check if it ends with .bmp
        if(strcmp(strstr(argv[2], ".bmp"), ".bmp") == 0)
        {
            // Set source image filename
            encInfo -> src_image_fname = argv[2];
        }
    }
    else
    {
        printf("argv[2] is not a .bmp file\n");
        return e_failure;
    }
      // encInfo->src_image_fname = argv[2];
    //if it is not .bmp file return e_failure.

    //check the argv[3] is .file or not
    if(strstr(argv[3], ".txt") != NULL)
    {
        // Check if it ends with .txt
        if(strcmp(strstr(argv[3], ".txt"), ".txt") == 0)
        {
            // Set secret filename
            encInfo -> secret_fname = argv[3];
        }
    }
    else
    {
        printf("argv[3] is not a .txt file\n");
        return e_failure;
    }
      // encInfo->secret_fname = argv[3];
    //if it is not .file return e_failure.

    //check argv[4] is present or not
    if(argv[4] == NULL)
    {
        printf("Output File not mentioned. Creating steged_1mg. bmp as default\n");

        // Set default stego filename
        encInfo -> stego_image_fname = "stego.bmp";
    }
    else if(strstr(argv[4], ".bmp") != NULL)
    {
        // Check if it ends with .bmp
        if(strcmp(strstr(argv[4], ".bmp"), ".bmp") == 0)
        {
            // Set stego filename
            encInfo->stego_image_fname = argv[4];
        }
    }
    else
    {
        printf("argv[4] is not a .bmp file\n");
        return e_failure;
    }

    return e_success;

    //check the argv[4] is .bmp file or not
      // encInfo->stego_image_fname = argv[4];
    //if it is not .bmp file return e_failure.

    //store default name
    //encInfo->stego_image_fname = "default.bmp";
}

// Function to perform encoding
Status do_encoding(EncodeInfo *encInfo)
{
    // Open files
    if(open_files(encInfo) == e_success)
    {
        printf("File opened successfully\n");

        // Check capacity
        if(check_capacity(encInfo) == e_success)
        {
            // Copy BMP header
            if(copy_bmp_header(encInfo -> fptr_src_image , encInfo -> fptr_stego_image) == e_success)
            {
                // Encode magic string
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    // Get extension size
                    int ext_size = strlen(".txt");

                    // Encode extension size
                    if(encode_secret_file_extn_size(ext_size, encInfo) == e_success)
                    {
                        // Get dot position
                        char *dot = strstr(encInfo -> secret_fname, ".");

                        // Encode extension
                        if(encode_secret_file_extn(dot, encInfo) == e_success)
                        {
                            // Get file size
                            long int file_size = get_file_size(encInfo -> fptr_secret);
                            
                            // Encode file size
                            if(encode_secret_file_size(file_size, encInfo) == e_success)
                            {
                                // Encode file data
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    // Copy remaining data
                                    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
                                    {
                                        printf("## Encoding Done Successfully ##\n");

                                        // Close files
                                        fclose(encInfo -> fptr_src_image);
                                        fclose(encInfo -> fptr_secret);
                                        fclose(encInfo -> fptr_stego_image);
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("copy_remaining_img_data failed\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("encode_secret_file_data failed\n");
                                    return e_failure;
                                }

                            }
                            else
                            {
                                printf("encode_secret_file_size failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("encode_secret_file_extn failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("encode_secret_file_extn_size failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to encode magic string\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Copy bmp header failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("check capacity failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("Error: unable to open file function\n");
        return e_failure;
    }
}

// Function to encode magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{

    printf("Encoding Magic String Signature\n");
    
    // Encode data to image
    if(encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING), encInfo->fptr_src_image, encInfo -> fptr_stego_image) == e_failure)
    {
        return e_failure;
    }

    printf("Done\n");

    return e_success;

}

// Function to encode data to image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    unsigned char data_image[8];                         // Declare image buffer

    // Loop for each byte in data
    for(int i = 0; i < size; i++)
    {
        // Read 8 bytes from source
        fread(data_image, sizeof(unsigned char), 8, fptr_src_image);

        // Encode byte to LSB
        if(encode_byte_to_lsb(data[i], data_image) == e_failure)
        {
            return e_failure;
        }

        // Write 8 bytes to stego
        fwrite(data_image, sizeof(unsigned char), 8, fptr_stego_image);
    }

    return e_success;

    //loop for size times
    //read 8 bytes of data from src file
    //one by one data from file 
    //encode_bytes_to_lsb(data[0], image_buffer);
    //write 8 bytes of image_buffer into stego.bmp file
}

// Function to encode byte to LSB
Status encode_byte_to_lsb(char data, unsigned char *image_buffer)
{
    // Check if buffer is null
    if(image_buffer == NULL)
    {
        return e_failure;
    }

    // Loop for 8 bits
    for(int i=0; i<8; i++)
    {
        int msb = ((data >> (7 - i)) & 1);                   // Get MSB of data
        image_buffer[i] = (image_buffer[i] & ~1) | msb;     // Set LSB of image buffer
    }

    return e_success;
}

// Function to encode int to LSB
Status encode_int_to_lsb(int data, unsigned char *image_buffer)
{
    // Check if buffer is null
    if(image_buffer == NULL)
    {
        return e_failure;
    }

    // Loop for 32 bits
    for(int i=0; i<32; i++)
    {
        int msb = ((data >> (31 - i)) & 1);                  // Get MSB of data
        image_buffer[i] = (image_buffer[i] & ~1) | msb;     // Set LSB of image buffer
    }

    return e_success;
}

// Function to encode secret file extension size
Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo)
{
    unsigned char secret_buffer[50];              // Declare secret buffer
    
    // Read 32 bytes from source
    fread(secret_buffer,sizeof(unsigned char), 32, encInfo -> fptr_src_image);

    // Encode int to LSB
    if(encode_int_to_lsb(file_extn_size, secret_buffer) == e_failure)
    {
        return e_failure;
    }

    // Write 32 bytes to stego
    fwrite(secret_buffer,sizeof(unsigned char), 32, encInfo ->fptr_stego_image);
    return e_success;
    //read 32 bytes from src file
    //call function int to lsb function
    //write 32 bytes in stego file   
}

// Function to encode secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{

    printf("Encoding secret.txt File Extenstion\n");

    // Get extension size
    int file_extn_size = strlen(file_extn);
   
    // Encode data to image
    if(encode_data_to_image(file_extn,file_extn_size, encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        return e_success;
    }

    printf("Done\n");

    return e_failure;
}

// Function to encode secret file size
Status encode_secret_file_size(long int file_size, EncodeInfo *encInfo)
{
    printf("Encoding secret.txt File Size\n");

    unsigned char secret_buffer[40];                     // Declare secret buffer

    // Read 32 bytes from source
    fread(secret_buffer, sizeof(unsigned char), 32, encInfo->fptr_src_image);
    
    // Encode int to LSB
    if(encode_int_to_lsb(file_size, secret_buffer) == e_failure)
    {
        return e_failure;
    }
    
    // Write 32 bytes to stego
    fwrite(secret_buffer, sizeof(unsigned char), 32, encInfo->fptr_stego_image);

    printf("Done\n");

    return e_success;
}


// Function to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("Encoding secret.txt File Data\n");

    unsigned int size = get_file_size(encInfo->fptr_secret);       // Get file size

    // Rewind secret file
    rewind(encInfo->fptr_secret);

    char character;
    unsigned char buffer[10];           // Declare buffer

    // Loop for each character
    for(int i = 0; i < size; i++)
    {
        // Read character from secret file
        fread(&character, sizeof(unsigned char), 1, encInfo->fptr_secret);

        // Read 8 bytes from source
        fread(buffer, sizeof(unsigned char), 8, encInfo->fptr_src_image);

        // Encode byte to LSB
        if(encode_byte_to_lsb(character, buffer) == e_failure)
        {
            return e_failure;
        }

        // Write 8 bytes to stego
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image); 

    }

    printf("Done\n");

    return e_success;

}

// Function to copy remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("Copying Left Over Data\n");

    unsigned char ch;

    // Loop while reading from source
    while(fread(&ch, sizeof(unsigned char), 1, fptr_src) > 0)
    {
        
        // Write to destination
        fwrite(&ch, sizeof(unsigned char), 1, fptr_dest);

    }

    printf("Done\n");

    return e_success;
}