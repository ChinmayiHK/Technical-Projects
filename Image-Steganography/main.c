#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    // check arv[1] is -e  => return e_encode
    // check argv[1] is -d => return e_decode
    // else => return e_unsupported.
    if(!(strcmp(argv[1],"-e")))
    {
        return e_encode;
    }
    else if(!(strcmp(argv[1],"-d")))
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("ERROR: Insufficient arguments\n");
        printf("Encoding : ./a.out -e <input.bmp> <secret.txt> [output.bmp]\n");
        printf("Decoding : ./a.out -d <stego.bmp> [output_file]\n");
        return 1;
    }
    
    OperationType res = check_operation_type(argv);

    if (res == e_encode)
    {
        EncodeInfo encInfo;

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("INFO: Validation Successful\n");

            if (do_encoding(&encInfo) == e_success)
            {
                printf("INFO: Encoding completed successfully\n");
            }
            else
            {
                printf("ERROR: Encoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("ERROR: Invalid command line arguments\n");
            printf("Usage: ./a.out -e <input.bmp> <secret.txt> [output.bmp]\n");
            return 1;
        }
    }
    else if (res == e_decode)
    {
        DecodeInfo decInfo;

        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("INFO: Validation Successful\n");

            if (do_decoding(&decInfo) == e_success)
            {
                printf("INFO: Decoding completed successfully\n");
            }
            else
            {
                printf("ERROR: Decoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("ERROR: Invalid command line arguments\n");
            printf("Usage: ./a.out -d <stego.bmp> [output_file]\n");
            return 1;
        }
    }
    else
    {
        printf("ERROR: Unsupported operation\n");
        printf("Encoding : ./a.out -e <input.bmp> <secret.txt> [output.bmp]\n");
        printf("Decoding : ./a.out -d <stego.bmp> [output_file]\n");
        return 1;
    }

    return 0;
}

