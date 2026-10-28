#include "tree.h"

int main(void)
{
    Tree_t *root = NULL;
    int option, data, result;

    while (1)
    {
        printf("\n--- RED-BLACK TREE MENU ---\n");
        printf("1. Insert\n");
        printf("2. Find minimum\n");
        printf("3. Find maximum\n");
        printf("4. Search\n");
        printf("5. Delete\n");
        printf("6. Inorder traversal\n");
        printf("7. Preorder traversal\n");
        printf("8. Postorder traversal\n");
        printf("9. Display inorder with colors\n");
        printf("10. Delete minimum\n");
        printf("11. Delete maximum\n");
        printf("12. Exit\n");
        printf("Enter option: ");

        if (scanf("%d", &option) != 1)
        {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (option)
        {
            case 1:
                printf("Enter data to insert: ");
                if (scanf("%d", &data) != 1)
                    goto input_error;
                result = insert_into_RBT(&root, data);
                if (result == SUCCESS)
                    printf("Inserted successfully.\n");
                else if (result == DUPLICATE)
                    printf("Duplicate value; not inserted.\n");
                else
                    printf("Insertion failed: memory allocation error.\n");
                break;

            case 2:
                if (root == NULL)
                    printf("Tree is empty.\n");
                else
                    printf("Minimum value: %d\n", findmin(root));
                break;

            case 3:
                if (root == NULL)
                    printf("Tree is empty.\n");
                else
                    printf("Maximum value: %d\n", findmax(root));
                break;

            case 4:
                printf("Enter data to search: ");
                if (scanf("%d", &data) != 1)
                    goto input_error;
                result = search_RBT(root, data);
                if (result == SUCCESS)
                    printf("Data found.\n");
                else if (result == FAILURE)
                    printf("Tree is empty.\n");
                else
                    printf("Data not found.\n");
                break;

            case 5:
                printf("Enter data to delete: ");
                if (scanf("%d", &data) != 1)
                    goto input_error;
                result = delete_RBT(&root, data);
                if (result == SUCCESS)
                    printf("Data deleted successfully.\n");
                else if (result == NOELEMENT)
                    printf("Data not found.\n");
                else
                    printf("Tree is empty.\n");
                break;

            case 6:
                printf("Inorder: ");
                inorder(root);
                printf("\n");
                break;

            case 7:
                printf("Preorder: ");
                preorder(root);
                printf("\n");
                break;

            case 8:
                printf("Postorder: ");
                postorder(root);
                printf("\n");
                break;

            case 9:
                printf("Inorder with colors: ");
                display_inorder_colored(root);
                printf("\n");
                break;

            case 10:
                result = delete_minimum(&root);
                if (result == SUCCESS)
                    printf("Minimum node deleted successfully.\n");
                else
                    printf("Tree is empty.\n");
                break;

            case 11:
                result = delete_maximum(&root);
                if (result == SUCCESS)
                    printf("Maximum node deleted successfully.\n");
                else
                    printf("Tree is empty.\n");
                break;

            case 12:
                free_tree(root);
                return SUCCESS;

            default:
                printf("Invalid option.\n");
        }
    }

    free_tree(root);
    return SUCCESS;

input_error:
    printf("Invalid input. Exiting.\n");
    free_tree(root);
    return FAILURE;
}
