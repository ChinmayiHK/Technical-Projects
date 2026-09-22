#ifndef MP3TAG_H
#define MP3TAG_H

#define TITLE    0
#define ARTIST   1
#define ALBUM    2
#define YEAR     3
#define COMMENT  4
#define GENRE    5
#define TRACK    6

typedef struct
{
    char title[256];
    char artist[256];
    char album[256];
    char year[32];
    char comment[512];
    char genre[256];
    char track[64];
    char version[32];

} MP3Tag;

int display_tags(const char *filename);

int show_version(const char *filename);

int edit_tag(const char *option,
             const char *value,
             const char *filename);

#endif