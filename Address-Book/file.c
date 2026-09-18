#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr;

    fptr = fopen("contacts.csv", "w");

    if(fptr == NULL)
    {
        printf("Unable to open file\n");
        return;
    }

    fprintf(fptr,"#%d\n",addressBook->contactCount);

    int i;

    for(i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr,"%s, %s, %s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);

    //printf("Contacts saved successfully\n");
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr;

    fptr = fopen("contacts.csv", "r");

    if(fptr == NULL)
    {
        return;
    }

    int count;

    fscanf(fptr, "#%d\n", &count);

    addressBook->contactCount = 0;

    while(fscanf(fptr,
                 "%[^,], %[^,], %[^\n]\n",
                 addressBook->contacts[addressBook->contactCount].name,
                 addressBook->contacts[addressBook->contactCount].phone,
                 addressBook->contacts[addressBook->contactCount].email) == 3)
    {
        addressBook->contactCount++;
    }

    fclose(fptr);
}