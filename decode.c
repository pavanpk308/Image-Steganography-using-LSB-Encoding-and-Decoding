// Include standard and user defined input/output header
#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

// Function to read and validate decode arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
     // Check if argv[2] is a .bmp file
    if(strstr(argv[2], ".bmp") != NULL)
    {
        // Check if it ends with .bmp
        if(strcmp(strstr(argv[2], ".bmp"), ".bmp") == 0)
        {
            // Set stego image filename
            decInfo -> stego_image_fname = argv[2];
        }
    }
    else
    {
        printf("argv[2] is not a .bmp file\n");
        return e_failure;
    }

    // Check if argv[3] is null
    if(argv[3] == NULL)
    {
        printf("Output File not mentioned. Creating output.txt as default\n");
        
        // Set default output filename
        strcpy(decInfo -> output_fname, "Output");
    }

    else if(argv[3] != NULL)
    {
        // Copy argv[3] to output filename
        strcpy(decInfo -> output_fname, argv[3]);
        
        int len = strlen(decInfo->output_fname);               // Get length

        // Loop to find dot
        for (int i = len - 1; i >= 0; i--)
        {
            // If dot found
            if (decInfo->output_fname[i] == '.')
            {
                // Set null terminator
                decInfo->output_fname[i] = '\0';
                break;
            }
        }
    }

    else
    {
        printf("argv[3] is not properly present\n");
        return e_failure;
    }

    return e_success;
}

// Function to skip BMP header
Status skip_bmp_header(FILE *fptr_stego_image)
{
    // Seek to 54th byte
    fseek(fptr_stego_image, 54, SEEK_SET);

    return e_success;

}

// Function to open files for decode
Status open_files_for_decode(DecodeInfo *decInfo)
{

    printf("Opening required files\n");
   
    // Open stego image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    
    // Check if opened
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    printf("Opened steged_beautiful.bmp\n");

    return e_success;

}

// Function to decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("Decoding Magic String Signature\n");

    char expected_magic[3];   // user-entered magic string
    char magic_data[3];        // decoded magic data

    // Read magic string from user
    printf("Enter the magic string used during encoding: ");
    scanf("%s", expected_magic);

    // Decode image to data
    if (decode_image_to_data(magic_data, 2, decInfo) == e_failure)
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }

    magic_data[2] = '\0';

    // Compare with user-entered string
    if (strcmp(expected_magic, magic_data) != 0)
    {
        printf("Given wrong magic string\n");
        return e_failure;
    }

    printf("Magic string matched\n");
    printf("Done\n");
    return e_success;
}

// Function to decode secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer_data[40];                         // Declare buffer data
    int res;                                                                                                                 

    // Read 32 bytes
    if(fread(buffer_data, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        printf("decode_secret_file_extn_size read is faild\n");
        return e_failure;
    }

    res = decode_lsb_to_int(buffer_data);         // Decode LSB to int

    decInfo->extrn_size = res;                   // Set extracted size

    return e_success;
}

// Function to decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{

    printf("Decoding Output File Extenstion\n");

    int size = decInfo->extrn_size;                      // Get size

    char decode_extn[size];                              // Declare decode extension array

    // Decode image to data
    if(decode_image_to_data(decode_extn, size, decInfo) == e_failure)
    {
        printf("decode_decret_file_extn faild\n");
        return e_failure;
    }

    decode_extn[size] = '\0'; 

    strcpy(decInfo->extn_output_file, decode_extn);                  // Copy to extension

    printf("Done\n");

    return e_success;
}

// Function to decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{

    printf("Decoding decoded. txt File Size\n");

    char file_buffer[40];                              // Declare file buffer

    // Read 32 bytes
    if(fread(file_buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        printf("decode_secret_file_size read faild\n");
        return e_failure;
    }

    decInfo->size_output_file = (decode_lsb_to_int(file_buffer));           // Decode LSB to int

    printf("Done\n");

    return e_success;
}

// Function to decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{

    printf("Decoding decoded. txt File Data\n");

    unsigned int size = decInfo->size_output_file;                // Get size
    char buffer[20];                                              // Declare buffer
    unsigned char ch;

    // Loop for each byte
    for(int i = 0; i < size; i++)
    {
        // Read 8 bytes
        fread(buffer, 1, 8, decInfo->fptr_stego_image);

        ch = decode_lsb_to_byte(buffer);                     // Decode LSB to byte
        
        // Write to output file
        if(fwrite(&ch, 1, 1, decInfo->fptr_output) != 1)
        {
            printf("decode_secret_file_data faild\n");
            return e_failure;
        }
    }

    printf("Done\n");

    return e_success;
}

// Function to decode image to data
Status decode_image_to_data(char *data, int size, DecodeInfo *decInfo)
{
    char buffer[20];                                         // Declare buffer

    // Loop for each byte
    for(int i = 0; i < size; i++)
    {
        // Read 8 bytes
        if (fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        data[i] = decode_lsb_to_byte(buffer);                   // Decode LSB to byte
        
    }

    return e_success;
}

// Function to decode LSB to byte
char decode_lsb_to_byte(char *image_buffer)
{
    // Check if buffer is null
    if(image_buffer == NULL)
    {
        return e_failure;
    }

    char character = 0;
    
    // Loop for 8 bits
    for(int i = 0; i < 8; i++)
    {
        character = character | ((image_buffer[i] & 1) << (7 - i));       // Shift and OR with LSB
    }   

    return character;
}

// Function to decode LSB to int
int decode_lsb_to_int(char *image_buffer)
{
    // Check if buffer is null    
    if (image_buffer == NULL)
    {
        return 0;                                                                          
    }

    int x = 0;
    
    // Loop for 32 bits
    for (int i = 0; i < 32; i++)
    {
        x |= ((image_buffer[i] & 1) << (31 - i));             // Shift and OR with LSB
    }

    return x;
}


// Function to perform decoding
Status do_decoding(DecodeInfo *decInfo)
{
    // Open files
    if(open_files_for_decode(decInfo) == e_success)
    {
        // Skip BMP header
        if(skip_bmp_header(decInfo->fptr_stego_image) == e_success)
        { 
            // Decode magic string
            if(decode_magic_string(decInfo) == e_success)
            {
                // Decode extension size
                if(decode_secret_file_extn_size(decInfo) == e_success)
                {
                    // Decode extension
                    if(decode_secret_file_extn(decInfo) == e_success)
                    {
                        // Concatenate filename
                        strcat(decInfo->output_fname, decInfo->extn_output_file);

                        // Open output file
                        decInfo->fptr_output = fopen(decInfo->output_fname, "w");
                        
                        // Check if opened
                        if (decInfo->fptr_output == NULL)
                        {
    	                    perror("fopen");
    	                    fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

    	                    return e_failure;
                        }

                        printf("Opened decoded. txt\n");

                        // Decode file size
                        if(decode_secret_file_size(decInfo) == e_success)
                        {
                            // Decode file data
                            if (decode_secret_file_data(decInfo) == e_success)
                            {
                                printf("## Decoding Done Successfully ##\n");
                                
                                // Close files
                                fclose(decInfo->fptr_stego_image);
                                fclose(decInfo->fptr_output);
                                return e_success;
                            }
                            else
                            {
                                // Close files
                                fclose(decInfo->fptr_stego_image);
                                fclose(decInfo->fptr_output);

                                printf("decode secret file data failed\n");

                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("decode secret file size failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("decode secreat file extn is failed\n");
                        return e_failure;
                    }
                    
                }
                else
                {
                    printf("Decode secret file extn size is failed\n");
                    return e_failure;;
                } 
            }
            else
            {
                printf("Decode magic string is failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("Skiping bmp header  failed\n");
            return e_failure;
        }
       
    }
    else
    {
        printf("Open file for decode failed\n");
        return e_failure;
    }
    
}