#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria
    int i,j;
    Contact temp;
    if(addressBook->contactCount == 0)
    {
        printf("No contacts available\n");
        return;
    }

    for(i = 0; i < addressBook->contactCount; i++)
    { 
        for(j = 0; j < addressBook->contactCount - 1 - i; j++)
        {
            if(strcasecmp(addressBook->contacts[j].name,addressBook->contacts[j+1].name)>0)
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        } 
    }

    printf("\n-----------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-25s\n",
           "S.No", "Name", "Phone", "Email");
    printf("-------------------------------------------------------------\n");

    for(i = 0; i < addressBook->contactCount; i++)
    {
        printf("%-5d %-20s %-15s %-25s\n",i + 1,addressBook->contacts[i].name,
            addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }

    printf("-------------------------------------------------------------\n");      
}

void initialize(AddressBook *addressBook) 
{
     addressBook->contactCount = 0;
     //populateAddressBook(addressBook);
         // Load contacts from file during initialization (After files)
     loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) 
{
     saveContactsToFile(addressBook); // Save contacts to file
     exit(EXIT_SUCCESS); // Exit the program
}

int validate_name(char *name)
{
    int i;
    if(strlen(name) < 4)
    {
        printf("Error: Name must contain atleast 4 characters.\n");
        return 0;
    }
    for(i = 0; name[i] != '\0'; i++)
    {
        if(!((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] == ' ') || (name[i] == '.')))
              {
                printf("Error: Name should contain only alphabets.\n");
                return 0;
              }
    }

    return 1;
}

int validate_phone(AddressBook *addressBook, char *phone)
{
    int i;
    if(strlen(phone) != 10)
    {
        printf("Error: Phone number must contain exactly 10 digits\n\n");
        return 0;
    }
    if(!(phone[0] >= '6' && phone[0] <= '9'))
    {
        printf("Error: First digit must be between 6 and 9\n");
        return 0;
    }
    for(i = 0; phone[i] != '\0'; i++)
    {
        if(!(phone[i] >= '0' && phone[i] <= '9'))
        {
           printf("Error: Symbols are not allowed in phone number\n");
           return 0; 
        }
    }
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(phone,addressBook->contacts[i].phone) == 0)
        {
            printf("Phone number already exists\n");
            return 0;
        }
    }
    return 1;
}

int validate_email(AddressBook *addressBook, char *email)
{
    int i, at_pos = -1, dot_pos = -1;
    int at_count = 0;
    if(!(email[0] >= 'a' && email[0] <= 'z'))
    {
        return 0;
    }
    for(i = 0; email[i] != '\0'; i++)
    {
        if(!((email[i] >= 'a' && email[i] <= 'z') ||
            (email[i] >= '0' && email[i] <= '9') || 
            (email[i] == '@') || (email[i] == '.')))
            {
                printf("Invalid symbol in email\n");
                return 0;
            }
        if(email[i] == '@')
        {
            at_pos = i; 
            at_count++;
        }
        if(email[i] == '.' && email[i+1] == 'c' && email[i+2] == 'o' && email[i+3] == 'm' && 
            email[i+4] == '\0')
            {
                dot_pos = i;
            }
    }
       if(at_count > 1)
        {
            printf("Email must contain exactly one @\n");
            return 0;
        }
        if(at_pos == -1 || dot_pos == -1)
        {
            printf("Email ID must contain exactly one @ symbol and one . (dot)");
            return 0;
        }
        if(dot_pos - at_pos <= 1)
        {
            printf("Domain name missing between @ and .\n");
            return 0;
        }
        if(dot_pos < at_pos)
        {
            printf("Dot must come after @\n");
            return 0;
        }
    for(i = 0; i < addressBook -> contactCount; i++)
    {
        if(strcmp(email, addressBook -> contacts[i].email) == 0)
        {
            return 0;
        }
    }
    return 1;

}

void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */

    char name[20];
    int valid;

    while(1)
    {
        printf("Enter the name : ");
        scanf(" %[^\n]", name);

        valid = validate_name(name);
        if(valid == 1)// valid
            break;
    }

    char mobile[11];

    while(1)
    {
        printf("Enter the phone : ");
        scanf("%s", mobile);

        valid = validate_phone(addressBook,mobile);
        if(valid == 1)// valid
            break;
    }

    char mail[30];

    while(1)
    {
        printf("Enter the email : ");
        scanf("%s", mail);

        valid = validate_email(addressBook, mail);

        if(valid)
            break;

        printf("Invalid Email ID\n");
    }

    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, mobile);
    strcpy(addressBook->contacts[addressBook->contactCount].email, mail);

    //strcpy(addressBook->contacts[0].phone, mobile);
    addressBook->contactCount++;

    printf("Contact Added Successfully\n"); 
    printf("Count = %d\n", addressBook->contactCount);
}
int search_by_phone(AddressBook *addressBook)
{
    char search_phone[20];
    int i;
    int count=0;
    printf("Enter the phone number: ");
    scanf("%s",search_phone);
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(search_phone, addressBook->contacts[i].phone)==0)
        {
            printf("%d. %s %s %s \n", i+1,addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
            return i;
        }
    }
    if(count == 0)
    {
        printf("Contact Not Found\n");
        return -1;
    }
    return -1;
}
int search_by_email(AddressBook *addressBook)
{
    char search_email[20];
    int i;
    int count = 0;
    printf("Enter the email: ");
    scanf("%s",search_email);

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(search_email, addressBook->contacts[i].email)==0)
        {
            printf("%d. %s %s %s \n", i+1,addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
            return i;
        }
    }
    if(count == 0)
    {
        printf("Contact Not Found\n");
        return -1;
    }
    return -1;
}
int search_by_name(AddressBook *addressBook)
{
    char search_name[20];
    printf("Enter the name: ");
    scanf(" %[^\n]",search_name);
    int i, count = 0, index = 0;
    int serial_no[100];
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(search_name, addressBook->contacts[i].name)==0)
        {
            printf("%d. %s %s %s \n", i+1,addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
            index = i;
            count++;
        }
    }
        if(count == 0)
        {
             printf("Contact Not Found\n");
             return -1;
        }
        if(count == 1)
        {
            return index;
        }
        if(count > 1)
        {
            int choice;
            printf("Enter contact serial number: ");
            scanf("%d", &choice);

            i = serial_no[choice - 1];
            
            printf("\nName  : %s\n", addressBook->contacts[i].name);
            printf("Phone : %s\n", addressBook->contacts[i].phone);
            printf("Email : %s\n", addressBook->contacts[i].email);
            
            return choice - 1;
        } 
        return i;
        
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    int choice;

    printf("1. Search by name\n");
    printf("2. Search by mobile number\n");
    printf("3. Search by email\n");

    printf("Enter the choice: ");
    scanf("%d",&choice);
    
    switch(choice)
    {
        case 1:
            search_by_name(addressBook);
            break;
        case 2:
            search_by_phone(addressBook);
            break;
        case 3:
            search_by_email(addressBook);
            break;
        default:
            printf("\nInvalid Input\n");
            break;
    }
}

void edit_name(AddressBook *addressBook, int res)
{
    // get the name
    int valid;
    char new_name[20];
    
    while(1)
    {
        printf("Enter the new name : ");
        scanf(" %[^\n]",new_name);

        valid = validate_name(new_name);
        if(valid == 1)// valid
            break;
    }

    strcpy(addressBook->contacts[res].name, new_name);
    printf("Name Updated Successfully\n");

}

void edit_phone(AddressBook *addressBook, int res)
{
    // get the phone number
    int valid;
    char new_phone[20];

    while(1)
    {
        printf("Enter the new mobile number : ");
        scanf("%s", new_phone);

        valid = validate_phone(addressBook, new_phone);
        if(valid == 1)// valid
            break;
    }

    strcpy(addressBook->contacts[res].phone, new_phone);
    printf("Phone Number Updated Successfully\n");

}

void edit_email(AddressBook *addressBook, int res)
{
    // get the email
    int valid;
    char new_email[20];
    
    while(1)
    {
        printf("Enter the email: ");
        scanf("%s", new_email);

        valid = validate_email(addressBook, new_email);
        if(valid == 1)// valid
            break;
    }

    strcpy(addressBook->contacts[res].email, new_email);
    printf("Email Updated Successfully\n");

}

void editContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
    int choice;
    
    printf("1. Search by name\n");
    printf("2. Search by mobile number\n");
    printf("3. Search by email\n");

    printf("Enter the choice: ");
    scanf("%d",&choice);
    int res;

    switch(choice)
    {
        case 1:
            res = search_by_name(addressBook);
            break;
        case 2: 
            res = search_by_phone(addressBook);
            break;
        case 3: 
            res = search_by_email(addressBook);
            break;
        default:
            printf("Invalid input\n");
            break;
    }

    int field;
    
    printf("1. Edit name\n");
    printf("2. Edit mobile number\n");
    printf("3. Edit email\n");

    printf("Enter the field to edit: ");
    scanf("%d",&field);

    switch(field)
    {
        case 1:
            edit_name(addressBook, res);
            break;
        case 2:
            edit_phone(addressBook, res);
            break;
        case 3:
            edit_email(addressBook, res);
            break;
        default:
            printf("\nInvalid Input\n");
            break;
    }
    saveContactsToFile(addressBook);
    printf("Contact Updated Successfully\n");
    
}

void deleteContact(AddressBook *addressBook)
{
     	/* Define the logic for deletecontact */
    int choice;
    
    printf("Search the contact to delete: \n");
    printf("1. Search by name\n");
    printf("2. Search by mobile number\n");
    printf("3. Search by email\n");
    printf("4. Exit\n");

    scanf("%d",&choice);

    int index = -1;

    switch(choice)
    {
        case 1:
            index = search_by_name(addressBook);
            break;
        case 2: 
            index = search_by_phone(addressBook);
            break;
        case 3: 
            index = search_by_email(addressBook);
            break;
        case 4:
            return;
        default:
            printf("Invalid input\n");
            break;
    }

    if(index == -1)
    {
        printf("Contact Not Found\n");
        return;
    }

    char confirm;

    printf("Are you sure to delete? (y/n): ");
    scanf(" %c", &confirm);
    
    if(confirm != 'y' && confirm != 'Y')
    {
        printf("Deletion Cancelled\n");
        return;
    }

    int i;
    for(i = index; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i+1];
    }
    addressBook->contactCount--;
    saveContactsToFile(addressBook);
    printf("Contact Deleted Successfully\n");
    printf("Count = %d\n", addressBook->contactCount);
}
