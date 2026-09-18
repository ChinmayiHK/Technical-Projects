#include <stdio.h>
#include <stddef.h>

#include "types.h"
#include "functions.h"

int main(int argc, char *argv[])
{
    MainNode *head[27] = {NULL};
    FileNode *file_head = NULL;

    int choice;
    int database_created = 0;

    if (read_and_validate(argc, argv) == 0)
    {
        return 0;
    }

    while (1)
    {
        printf("\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Database\n");
        printf("4. Save Database\n");
        printf("5. Update Database\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:

                if (database_created == 1)
                {
                    printf("INFO: Database already created.\n");
                }
                else
                {
                    create_database(argc, argv, head);

                    database_created = 1;

                    printf("Database created successfully.\n");
                }

                break;

            case 2:

                display_database(head);

                break;

            case 3:

                search_database(head);

                break;

            case 4:

                save_database(head);

                break;

            case 5:

                update_database(argc, argv, head, &file_head);

                break;

            case 6:

                return 0;

            default:

                printf("Invalid choice.\n");
        }
    }

    return 0;
}