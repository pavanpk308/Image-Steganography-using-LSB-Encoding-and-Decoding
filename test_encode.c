// Include standard and user defined input/output header
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>

int main(int argc, char *argv[])           // Argument count and argument value parameter for main 
{
    EncodeInfo encInfo;                  // Declare EncodeInfo structure
    
    DecodeInfo decInfo;                 // Declare DecodeInfo structure

    // Check the operation type from command line arguments
    int ret = check_operation_type(argv);

    // If operation is encode
    if(ret == e_encode)
    {
        // Check if at least 4 arguments are provided
        if(argc >=4)
        {

            printf("## Encoding Procedure Started ##\n");

            // Read and validate encode arguments
            if(read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                // Perform encoding
                if(do_encoding(&encInfo) == e_success)
                {
                    printf("Encoding done succcessfully\n");
                }
                else
                {
                    printf("Encoding is not done succcessfully\n");
                    return 0;
                }

            }
            else
            {
                printf("Error: validation failed\n");
                return 0;

            }
        }
        else
        {
            printf("Error: Enter proper arguments\n");
            return 0;
        }

    }

    // If operation is decode
    else if(ret == e_decode)
    {
       
        // Check if at least 3 arguments are provided
        if(argc >= 3)
        {
            printf("## Decoding Procedure Started ##\n");

            // Read and validate decode arguments
            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                // Perform decoding
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding done successfully\n");
                }
                else
                {
                    printf("Decoding is not done successfully\n");
                }
            }
            else
            {
                printf("Error: validation failed\n");
                return 0;
            }
        }
        else
        {
            printf("Error: Enter proper arguments\n");
            return 0;
        }


    }

    // If operation is unsupported
    else
    {
        printf("Invalid Input\nUnsupported\n");
    }
    return 0;
}

// Function to check operation type
OperationType check_operation_type(char *argv[])
{
    // If argument is "-e", return encode
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }

    // If argument is "-d", return decode
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }

    // Else return unsupported
    else
    {
        return e_unsupported;
    }
}
