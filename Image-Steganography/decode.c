#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* Check whether input file is a BMP file */
    if(strstr(argv[2],".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    /* Check whether output file name is given */
    if(argv[3] != NULL)
    {
        char *token;
        token = strtok(argv[3], ".");
        strcpy(decInfo->output_fname,token);
    }
    else
    {
        strcpy(decInfo->output_fname, "output");
    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }

    return e_success;
}

Status decode_int_from_image(int *size, FILE *fptr_stego_image)
{
    char image_buffer[32];

    if(fread(image_buffer, 1, 32, fptr_stego_image) != 32)
    {
        return e_failure;
    }

    *size = 0;

    for (int i = 0; i < 32; i++)
    {
        *size = *size | ((image_buffer[i] & 1) << i);
    }

    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;

    for (int i = 0; i < 8; i++)
    {
        *data = *data | ((image_buffer[i] & 1) << i);
    }

    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char image_buffer[8];

    for (int i = 0; i < size; i++)
    {
        /* Read 8 bytes from the stego image */
        if(fread(image_buffer, 1, 8, fptr_stego_image) != 8)
        {
            return e_failure;
        }

        /* Decode one character */
        decode_byte_from_lsb(&data[i], image_buffer);
    }

    /* Null terminate the decoded string */
    data[size] = '\0';

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_str[strlen(MAGIC_STRING) + 1];

    decode_data_from_image(magic_str,strlen(MAGIC_STRING), decInfo->fptr_stego_image);

    // printf("Decoded Magic String = %s\n", magic_str);
    // printf("Original Magic String = %s\n", MAGIC_STRING);

    if(strcmp(magic_str, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status decode_secret_file_extn_size(int *extn_size, DecodeInfo *decInfo)
{
    if (decode_int_from_image(extn_size,decInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    //printf("Extension size = %d\n", *extn_size);
    return e_success;
}

Status decode_secret_file_extn(int extn_size, DecodeInfo *decInfo)
{
    /* Decode extension */
    if(decode_data_from_image(decInfo->extn_secret_file,extn_size, decInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }

    /* Append extension to output filename */
    strcat(decInfo->output_fname, decInfo->extn_secret_file);

    /* Open output file */
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");

    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to create file %s\n", decInfo->output_fname);

        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    return decode_int_from_image((int *)&decInfo->size_secret_file,decInfo->fptr_stego_image);
}

/* Decode secret data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char *data;


    data = malloc(decInfo->size_secret_file + 1);


    if(data == NULL)
    {
        return e_failure;
    }


    if(decode_data_from_image(data,decInfo->size_secret_file, decInfo->fptr_stego_image) == e_failure)
    {
        free(data);
        return e_failure;
    }

    data[decInfo->size_secret_file] = '\0';

    if (fwrite(data, 1, decInfo->size_secret_file, decInfo->fptr_output) != decInfo->size_secret_file)
    {
        free(data);
        return e_failure;
    }
    free(data);
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    int extn_size;

    /* Open stego image */
    if (open_decode_files(decInfo) == e_failure)
    {
        printf("ERROR: Unable to open files\n");
        return e_failure;
    }

    /* Skip BMP header */
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    /* Decode magic string */
    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: Magic string mismatch\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    /* Decode extension size */
    if (decode_secret_file_extn_size(&extn_size, decInfo) == e_failure)
    {
        printf("ERROR: Unable to decode extension size\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    /* Decode extension and create output file */
    if (decode_secret_file_extn(extn_size, decInfo) == e_failure)
    {
        printf("ERROR: Unable to decode extension\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    /* Decode secret file size */
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("ERROR: Unable to decode file size\n");
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }

    /* Decode secret file data */
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("ERROR: Unable to decode secret data\n");
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);


    return e_success;
}

