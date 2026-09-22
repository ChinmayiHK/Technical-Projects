#include <stdio.h>
#include <string.h>
#include "mp3tag.h"

static void print_help(void)
{
    printf("Usage:\n");
    printf("  ./mp3tag filename.mp3\n");
    printf("  ./mp3tag -v filename.mp3\n");
    printf("  ./mp3tag -t \"value\" filename.mp3   Modify Title\n");
    printf("  ./mp3tag -T \"value\" filename.mp3   Modify Track\n");
    printf("  ./mp3tag -a \"value\" filename.mp3   Modify Artist\n");
    printf("  ./mp3tag -A \"value\" filename.mp3   Modify Album\n");
    printf("  ./mp3tag -y \"value\" filename.mp3   Modify Year\n");
    printf("  ./mp3tag -c \"value\" filename.mp3   Modify Comment\n");
    printf("  ./mp3tag -g \"value\" filename.mp3   Modify Genre\n");
    printf("  ./mp3tag -h                         Help\n");
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        print_help();
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "-v") == 0) {
        return show_version(argv[2]);
    }

    if (argc == 2) {
        return display_tags(argv[1]);
    }

    if (argc == 4) {
        return edit_tag(argv[1], argv[2], argv[3]);
    }

    fprintf(stderr, "Invalid arguments. Use -h for help.\n");
    return 1;
}
