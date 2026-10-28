# Source Code to HTML Converter in C

## Overview
Source to HTML is a command-line utility developed in C that converts a C source file into an HTML document with syntax highlighting and line numbers.

The generated HTML uses CSS classes to distinguish C keywords, comments, strings, numeric constants, and preprocessor directives. The program also escapes HTML-sensitive characters such as `<`, `>`, and `&` to preserve source-code content in the output.

## Features
- Read a C source file from the command line
- Generate an HTML output file
- Highlight C keywords
- Highlight comments
- Highlight strings and character constants
- Highlight numeric constants
- Highlight preprocessor directives
- Display source line numbers
- Escape HTML-sensitive characters
- Handle invalid arguments and file-opening errors

## Technologies Used
- C Programming Language
- File Handling
- String Processing
- Character Classification
- HTML
- CSS

## Project Structure
- `main.c` — Handles command-line arguments and creates the HTML output
- `source2html.c` — Implements source-code processing and HTML generation
- `source2html.h` — Declares the conversion function
- `input.c` — Sample C source file for testing

## How to Run
Pass the input C file as a command-line argument:

```bash
./a.out input.c
```

On Windows, run the executable using:

```text
source2html.exe input.c
```

The program generates `input.c.html` in the same directory.

Open the generated HTML file in a web browser to view the highlighted source code.

## Concepts Demonstrated
- File handling in C
- Command-line arguments
- String manipulation
- Character classification
- HTML generation
- Basic syntax highlighting
- Error handling

## Author
Chinmayi H K
