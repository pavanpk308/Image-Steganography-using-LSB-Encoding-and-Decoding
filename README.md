# Image Steganography using LSB Encoding and Decoding

A command-line image steganography project written in C. It hides the contents of a text file inside a BMP image using the **Least Significant Bit (LSB)** technique and can later extract the hidden message from the generated stego image.

## Project Overview

Steganography is the technique of hiding a message inside another file so that the presence of the message is not immediately noticeable.

In this project:

- **Encoding** hides a `.txt` file inside a BMP image.
- **Decoding** extracts the hidden text file from the stego image.
- The LSB of image bytes is modified to store the secret information.
- A magic string (`#*`) is embedded and checked during decoding to identify an encoded image.

## Features

- Encode a text file into a BMP image.
- Decode and recover the hidden text file.
- Store the secret file extension and file size along with its contents.
- Check whether the source image has enough capacity for the secret data.
- Accept an optional output BMP filename during encoding.
- Accept an optional output filename during decoding.

## Technologies Used

- C programming language
- File handling (`fopen`, `fread`, `fwrite`, `fseek`, `fclose`)
- Structures and header files
- Bitwise operators
- Command-line arguments
- BMP image file format
- Least Significant Bit (LSB) steganography

## Project Structure

```text
C-Project-1/
├── 1-References/
│   ├── lsb_steganography.odt
│   ├── requirment_and_design.pdf
│   └── reference BMP / HEX files
├── 2-OutputImages/
│   └── sample output images
├── 3-Design/
│   └── design reference image
├── 4-SkeletonCode/
│   ├── encode.c
│   ├── encode.h
│   ├── decode.c
│   ├── decode.h
│   ├── common.h
│   ├── types.h
│   ├── test_encode.c
│   ├── beautiful.bmp
│   └── secret.txt
└── README.md
```

## How It Works

### 1. Encoding

The encoding process reads the source BMP image and the secret text file. It then embeds the secret information into the image data by changing the least significant bits.

The project stores the following information in the image:

1. Magic string (`#*`) to identify the encoded image.
2. Secret file extension size.
3. Secret file extension.
4. Secret file size.
5. Secret file contents.

The BMP header is copied to the output image, and the remaining image data is copied after the secret information has been encoded.

### 2. Decoding

The decoding process reads the generated stego image and extracts the embedded information.

It performs these steps:

1. Skips the BMP header.
2. Extracts and verifies the magic string.
3. Extracts the secret file extension size and extension.
4. Extracts the original secret file size.
5. Reads the hidden data and writes it to the recovered text file.

## Build Instructions

Open a terminal and navigate to the `4-SkeletonCode` directory.

Compile the source files using GCC:

```bash
gcc test_encode.c encode.c decode.c -o steganography
```

## Usage

### Encode a text file into a BMP image

```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

Arguments:

- `-e` — selects encoding mode.
- `beautiful.bmp` — source BMP image.
- `secret.txt` — text file to hide.
- `stego.bmp` — output image containing the hidden data.

The output image filename is optional. If omitted, the program uses `stego.bmp`.

### Decode the hidden text file

```bash
./steganography -d stego.bmp decoded.txt
```

Arguments:

- `-d` — selects decoding mode.
- `stego.bmp` — image containing the hidden data.
- `decoded.txt` — base name for the recovered file. The program uses the extension extracted from the image.

During decoding, the program asks for the magic string. Enter:

```text
#*
```

If the output filename is omitted, the program uses `Output` as the base name and appends the extracted extension.

## Example Workflow

```bash
# Compile
gcc test_encode.c encode.c decode.c -o steganography

# Encode
./steganography -e beautiful.bmp secret.txt stego.bmp

# Decode
./steganography -d stego.bmp decoded.txt
```

After decoding, open the generated text file and compare its contents with the original `secret.txt`.

## Concepts Practised

- Hiding and extracting data using LSB steganography
- Reading and writing binary files in C
- Working with BMP image data
- Bitwise AND, OR, and shift operations
- Using structures to organize encoding and decoding information
- Separating implementation into `.c` and `.h` files
- Validating command-line arguments
- Managing file pointers and file operations

## Notes and Limitations

- The current argument validation is designed for a `.bmp` source image and a `.txt` secret file.
- The implementation assumes a 24-bit BMP-style pixel data layout and uses a 54-byte BMP header when processing image data. Use a compatible BMP image.
- The source image must have enough capacity to store the secret information.
- This project demonstrates basic LSB steganography; it does not encrypt the secret text. Anyone who knows the encoding method and magic string may be able to extract the hidden data.
- Avoid editing, resizing, or recompressing the generated BMP image before decoding, as this may alter the embedded bits.

## Author

**Pavan Kumar N**

---

If you find this project useful, feel free to explore the source code and experiment with the encoding and decoding logic.
