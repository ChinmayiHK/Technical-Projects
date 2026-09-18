# Image Steganography using LSB Encoding and Decoding

## Project Overview

The Image Steganography project implements a method to hide a secret text file inside a BMP image using Least Significant Bit (LSB) encoding. The project supports both encoding and decoding operations through command-line arguments.

During encoding, the program copies the BMP header and embeds information into the least significant bits of image data. The embedded information includes a magic string, secret file extension, secret file size, and secret file data. During decoding, the program extracts this information from the stego image and recreates the original secret file.

## Objectives

- To understand the BMP image file structure and header.
- To implement data hiding using LSB manipulation.
- To encode a secret text file into a BMP image without changing the visible properties of the image.
- To decode the hidden data and reconstruct the original secret file.
- To gain practical experience with file operations, command-line arguments, modular programming, and bitwise data manipulation.

## Features

### Encoding
- Validates the input BMP image and secret text file.
- Checks whether the source image has sufficient capacity.
- Copies the 54-byte BMP header to the output image.
- Encodes a magic string to identify the stego image.
- Encodes the secret file extension and its size.
- Encodes the secret file size.
- Encodes the secret file data into the LSBs of image bytes.
- Copies the remaining image data to the output image.

### Decoding
- Validates the input stego BMP image.
- Skips the BMP header.
- Verifies the encoded magic string.
- Decodes the secret file extension and its size.
- Decodes the secret file size.
- Extracts the secret data from the image.
- Recreates the decoded output file.

## Technologies and Concepts Used

- Embedded C
- File Handling
- BMP File Format
- Bitwise Operations
- Pointers
- Structures
- Command-Line Arguments
- Modular Programming
- Makefiles

## Project Structure

```text
Image-Steganography/
├── main.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
└── types.h
```

## Working Principle

### Encoding Flow

```text
Input BMP Image
       |
       v
Validate Input Files
       |
       v
Check Image Capacity
       |
       v
Copy BMP Header
       |
       v
Encode Magic String
       |
       v
Encode Secret File Extension
       |
       v
Encode Secret File Size
       |
       v
Encode Secret File Data
       |
       v
Copy Remaining Image Data
       |
       v
Stego BMP Image
```

### Decoding Flow

```text
Stego BMP Image
       |
       v
Validate Input
       |
       v
Skip BMP Header
       |
       v
Decode & Verify Magic String
       |
       v
Decode File Extension
       |
       v
Decode File Size
       |
       v
Decode Secret File Data
       |
       v
Reconstructed Output File
```

## Command-Line Usage

### Encoding

```bash
./a.out -e <input.bmp> <secret.txt> [output.bmp]
```

Example:

```bash
./a.out -e beautiful.bmp secret.txt stego.bmp
```

If the output BMP name is not provided, the program uses `stego.bmp` as the default output file.

### Decoding

```bash
./a.out -d <stego.bmp> [output_file]
```

Example:

```bash
./a.out -d stego.bmp output.txt
```

## Key Challenges and Learnings

- Understood the structure of BMP files, including the image header and image data.
- Learned how to manipulate individual bits using LSB operations for data hiding and extraction.
- Implemented capacity checking to determine whether the input image can accommodate the secret data.
- Learned to preserve the BMP header and copy the remaining image data after encoding.
- Implemented encoding and decoding as separate modular operations with validation and error handling.
- Gained practical experience in file pointers, binary file operations, dynamic memory allocation, and command-line argument handling.

## Author

**Chinmayi H K**
