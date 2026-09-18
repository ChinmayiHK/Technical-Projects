# Address Book

## Project Overview

The Address Book project is a command-line application developed to manage contact information. It provides functionalities to create, search, edit, delete, and list contacts. Contact information is maintained using a structured data representation and stored in a CSV file for persistence.

## Objectives

- To develop a command-line application for managing contacts.
- To implement contact creation, searching, editing, deletion, and listing.
- To validate contact details such as name, phone number, and email.
- To store and retrieve contact information using file operations.
- To understand modular program organization using multiple source and header files.

## Features

### Create Contact
- Adds a new contact to the address book.
- Accepts name, phone number, and email address.
- Validates the entered contact information.
- Prevents duplicate phone numbers and email addresses.

### Search Contact
Contacts can be searched using:
- Name
- Mobile number
- Email address

### Edit Contact
- Searches for an existing contact.
- Allows modification of:
  - Name
  - Mobile number
  - Email address

### Delete Contact
- Searches for the contact to be deleted.
- Requests confirmation before deletion.
- Updates the contact list after deletion.

### List Contacts
- Displays all available contacts.
- Sorts contacts alphabetically by name.
- Displays the contact serial number, name, phone number, and email.

### File Storage
- Contacts are loaded from `contacts.csv` when the application starts.
- Updated contacts are saved back to the CSV file.
- Contact information is retained between program executions.

## Technologies and Concepts Used

- C
- Structures
- File Handling
- File I/O Operations
- String Handling
- Functions
- Modular Programming
- Command-Line Interface

## Project Structure

```text
Address-Book/
├── main.c
├── contact.c
├── contact.h
├── file.c
├── file.h
└── contacts.csv

## Working Flow

```text
Start Application
       |
       v
Initialize Address Book
       |
       v
Load Contacts from File
       |
       v
Display Menu
       |
       +----> Create Contact
       |
       +----> Search Contact
       |
       +----> Edit Contact
       |
       +----> Delete Contact
       |
       +----> List Contacts
       |
       +----> Save and Exit
                    |
                    v
             Save Contacts
                    |
                    v
                   Exit
