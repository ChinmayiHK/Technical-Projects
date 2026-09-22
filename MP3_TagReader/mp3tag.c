#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mp3tag.h"


/* ---------------------------------------------------------
   Convert 3-byte big endian value
   --------------------------------------------------------- */

static unsigned int be24(const unsigned char *p)
{
    return ((unsigned int)p[0] << 16) |
           ((unsigned int)p[1] << 8) |
           p[2];
}


/* ---------------------------------------------------------
   Convert 4-byte big endian value
   --------------------------------------------------------- */

static unsigned int be32(const unsigned char *p)
{
    return ((unsigned int)p[0] << 24) |
           ((unsigned int)p[1] << 16) |
           ((unsigned int)p[2] << 8) |
           p[3];
}


/* ---------------------------------------------------------
   Convert ID3 sync-safe integer
   --------------------------------------------------------- */

static unsigned int syncsafe32(const unsigned char *p)
{
    return ((unsigned int)(p[0] & 0x7F) << 21) |
           ((unsigned int)(p[1] & 0x7F) << 14) |
           ((unsigned int)(p[2] & 0x7F) << 7) |
           (unsigned int)(p[3] & 0x7F);
}


/* ---------------------------------------------------------
   Write 4-byte big endian value
   --------------------------------------------------------- */

static void put_be32(unsigned char *p, unsigned int value)
{
    p[0] = (unsigned char)(value >> 24);
    p[1] = (unsigned char)(value >> 16);
    p[2] = (unsigned char)(value >> 8);
    p[3] = (unsigned char)value;
}


/* ---------------------------------------------------------
   Check whether frame ID is zero
   --------------------------------------------------------- */

static int is_zero_frame_id(const unsigned char *p, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (p[i] != 0)
            return 0;
    }

    return 1;
}


/* ---------------------------------------------------------
   Copy text from frame
   --------------------------------------------------------- */

static void clean_copy(char *dest,
                       size_t capacity,
                       const unsigned char *src,
                       size_t size)
{
    size_t i = 0;
    size_t j = 0;

    if (capacity == 0)
        return;

    /* First byte of text frame = encoding */
    if (size > 0 && (src[0] == 0 || src[0] == 1))
        i = 1;

    while (i < size && j + 1 < capacity)
    {
        unsigned char ch = src[i++];

        if (ch == 0)
            break;

        if (ch >= 32 || ch == '\t')
            dest[j++] = (char)ch;
    }

    dest[j] = '\0';
}


/* ---------------------------------------------------------
   Copy normal raw text
   --------------------------------------------------------- */

static void copy_raw(char *dest,
                     size_t capacity,
                     const unsigned char *src,
                     size_t size)
{
    size_t i;
    size_t j = 0;

    if (capacity == 0)
        return;

    for (i = 0; i < size && j + 1 < capacity; i++)
    {
        if (src[i] == 0)
            break;

        if (src[i] >= 32 || src[i] == '\t')
            dest[j++] = (char)src[i];
    }

    dest[j] = '\0';
}


/* ---------------------------------------------------------
   Initialize structure
   --------------------------------------------------------- */

static void init_tag(MP3Tag *tag)
{
    memset(tag, 0, sizeof(MP3Tag));

    strcpy(tag->title, "N/A");
    strcpy(tag->artist, "N/A");
    strcpy(tag->album, "N/A");
    strcpy(tag->year, "N/A");
    strcpy(tag->comment, "N/A");
    strcpy(tag->genre, "N/A");
    strcpy(tag->track, "N/A");
}


/* ---------------------------------------------------------
   ID3v1 Genre
   --------------------------------------------------------- */

static const char *v1_genre(unsigned char genre)
{
    static const char *genres[] =
    {
        "Blues",
        "Classic Rock",
        "Country",
        "Dance",
        "Disco",
        "Funk",
        "Grunge",
        "Hip-Hop",
        "Jazz",
        "Metal",
        "New Age",
        "Oldies",
        "Other",
        "Pop",
        "R&B",
        "Rap",
        "Reggae",
        "Rock",
        "Techno",
        "Industrial",
        "Alternative",
        "Ska",
        "Death Metal",
        "Pranks",
        "Soundtrack",
        "Euro-Techno",
        "Ambient",
        "Trip-Hop",
        "Vocal",
        "Jazz+Funk",
        "Fusion",
        "Trance",
        "Classical",
        "Instrumental",
        "Acid",
        "House",
        "Game",
        "Sound Clip",
        "Gospel",
        "Noise",
        "AlternRock",
        "Bass",
        "Soul",
        "Punk",
        "Space",
        "Meditative",
        "Instrumental Pop",
        "Instrumental Rock",
        "Ethnic",
        "Gothic",
        "Darkwave",
        "Techno-Industrial",
        "Electronic",
        "Pop-Folk",
        "Eurodance",
        "Dream",
        "Southern Rock",
        "Comedy",
        "Cult",
        "Gangsta",
        "Top 40",
        "Christian Rap",
        "Pop/Funk",
        "Jungle",
        "Native American",
        "Cabaret",
        "New Wave",
        "Psychadelic",
        "Rave",
        "Showtunes",
        "Trailer",
        "Lo-Fi",
        "Tribal",
        "Acid Punk",
        "Acid Jazz",
        "Polka",
        "Retro",
        "Musical",
        "Rock & Roll",
        "Hard Rock",
        "Folk",
        "Folk-Rock",
        "National Folk",
        "Swing",
        "Fast Fusion",
        "Bebob",
        "Latin",
        "Revival",
        "Celtic",
        "Bluegrass",
        "Avantgarde",
        "Gothic Rock",
        "Progressive Rock",
        "Psychedelic Rock",
        "Symphonic Rock",
        "Slow Rock",
        "Big Band",
        "Chorus",
        "Easy Listening",
        "Acoustic",
        "Humour",
        "Speech",
        "Chanson",
        "Opera",
        "Chamber Music",
        "Sonata",
        "Symphony"
    };

    if (genre < sizeof(genres) / sizeof(genres[0]))
        return genres[genre];

    return "Unknown";
}


/* ---------------------------------------------------------
   Read ID3v1
   --------------------------------------------------------- */

static int read_v1(const char *filename, MP3Tag *tag)
{
    FILE *fp;
    unsigned char buffer[128];

    fp = fopen(filename, "rb");

    if (fp == NULL)
        return -1;

    if (fseek(fp, -128L, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    if (fread(buffer, 1, 128, fp) != 128)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    /* Check TAG signature */
    if (memcmp(buffer, "TAG", 3) != 0)
        return 0;

    copy_raw(tag->title,
             sizeof(tag->title),
             buffer + 3,
             30);

    copy_raw(tag->artist,
             sizeof(tag->artist),
             buffer + 33,
             30);

    copy_raw(tag->album,
             sizeof(tag->album),
             buffer + 63,
             30);

    copy_raw(tag->year,
             sizeof(tag->year),
             buffer + 93,
             4);

    copy_raw(tag->comment,
             sizeof(tag->comment),
             buffer + 97,
             30);

    snprintf(tag->genre,
             sizeof(tag->genre),
             "%s (%u)",
             v1_genre(buffer[127]),
             buffer[127]);

    if (buffer[125] == 0)
    {
        snprintf(tag->track,
                 sizeof(tag->track),
                 "%u",
                 buffer[126]);

        strcpy(tag->version, "ID3v1.1");
    }
    else
    {
        strcpy(tag->version, "ID3v1.0");
    }

    return 1;
}


/* ---------------------------------------------------------
   Store ID3v2 frame data
   --------------------------------------------------------- */

static void set_v2_field(MP3Tag *tag,
                         const char *id,
                         const unsigned char *data,
                         unsigned int size)
{
    if (strcmp(id, "TIT2") == 0 ||
        strcmp(id, "TT2") == 0)
    {
        clean_copy(tag->title,
                   sizeof(tag->title),
                   data,
                   size);
    }

    else if (strcmp(id, "TPE1") == 0 ||
             strcmp(id, "TP1") == 0)
    {
        clean_copy(tag->artist,
                   sizeof(tag->artist),
                   data,
                   size);
    }

    else if (strcmp(id, "TALB") == 0 ||
             strcmp(id, "TAL") == 0)
    {
        clean_copy(tag->album,
                   sizeof(tag->album),
                   data,
                   size);
    }

    else if (strcmp(id, "TYER") == 0 ||
             strcmp(id, "TYE") == 0)
    {
        clean_copy(tag->year,
                   sizeof(tag->year),
                   data,
                   size);
    }

    else if (strcmp(id, "TCON") == 0 ||
             strcmp(id, "TCO") == 0)
    {
        clean_copy(tag->genre,
                   sizeof(tag->genre),
                   data,
                   size);
    }

    else if (strcmp(id, "TRCK") == 0 ||
             strcmp(id, "TRK") == 0)
    {
        clean_copy(tag->track,
                   sizeof(tag->track),
                   data,
                   size);
    }

    else if (strcmp(id, "COMM") == 0 ||
             strcmp(id, "COM") == 0)
    {
        clean_copy(tag->comment,
                   sizeof(tag->comment),
                   data,
                   size);
    }
}


/* ---------------------------------------------------------
   Read ID3v2
   --------------------------------------------------------- */

static int read_v2(const char *filename,
                   MP3Tag *tag,
                   int *major,
                   int *minor)
{
    FILE *fp;
    unsigned char header[10];
    unsigned int tag_size;
    unsigned int position = 0;

    fp = fopen(filename, "rb");

    if (fp == NULL)
        return -1;

    if (fread(header, 1, 10, fp) != 10)
    {
        fclose(fp);
        return 0;
    }

    /* Check ID3 signature */
    if (memcmp(header, "ID3", 3) != 0)
    {
        fclose(fp);
        return 0;
    }

    *major = header[3];
    *minor = header[4];

    tag_size = syncsafe32(header + 6);

    /* Emertxe SRS excludes ID3v2.4 */
    if (*major == 4)
    {
        fclose(fp);
        return 2;
    }

    if (*major != 2 && *major != 3)
    {
        fclose(fp);
        return 0;
    }

    snprintf(tag->version,
             sizeof(tag->version),
             "ID3v2.%d.%d",
             *major,
             *minor);

    if (fseek(fp, 10, SEEK_SET) != 0)
    {
        fclose(fp);
        return 0;
    }

    while (position < tag_size)
    {
        unsigned char frame_header[10];
        unsigned int frame_size;
        int header_size;
        char frame_id[5] = {0};
        unsigned char *data;

        if (*major == 2)
            header_size = 6;
        else
            header_size = 10;

        if (position + header_size > tag_size)
            break;

        if (fread(frame_header,
                  1,
                  header_size,
                  fp) != (size_t)header_size)
            break;

        if (is_zero_frame_id(frame_header,
                             (*major == 2) ? 3 : 4))
            break;

        if (*major == 2)
        {
            memcpy(frame_id, frame_header, 3);
            frame_size = be24(frame_header + 3);
        }
        else
        {
            memcpy(frame_id, frame_header, 4);
            frame_size = be32(frame_header + 4);
        }

        position += header_size;

        if (frame_size == 0 ||
            position + frame_size > tag_size)
            break;

        data = malloc(frame_size);

        if (data == NULL)
        {
            fclose(fp);
            return 0;
        }

        if (fread(data, 1, frame_size, fp) != frame_size)
        {
            free(data);
            break;
        }

        set_v2_field(tag,
                     frame_id,
                     data,
                     frame_size);

        free(data);

        position += frame_size;
    }

    fclose(fp);

    return 1;
}


/* ---------------------------------------------------------
   Display all tags
   --------------------------------------------------------- */

int display_tags(const char *filename)
{
    MP3Tag tag;
    int major = 0;
    int minor = 0;
    int result;

    init_tag(&tag);

    result = read_v2(filename,
                     &tag,
                     &major,
                     &minor);

    if (result == -1)
    {
        printf("ERROR: Unable to open file.\n");
        return 1;
    }

    if (result == 2)
    {
        printf("ERROR: ID3v2.4 is not supported.\n");
        return 1;
    }

    if (result == 0)
    {
        init_tag(&tag);

        result = read_v1(filename,
                         &tag);
    }

    if (result == -1)
    {
        printf("ERROR: Unable to open file.\n");
        return 1;
    }

    if (result == 0)
    {
        printf("ERROR: ID3 tag not found.\n");
        return 1;
    }

    printf("\n");
    printf("---------------------------------------------\n");
    printf("             MP3 TAG INFORMATION\n");
    printf("---------------------------------------------\n");

    printf("File    : %s\n", filename);
    printf("Version : %s\n", tag.version);
    printf("Title   : %s\n", tag.title);
    printf("Artist  : %s\n", tag.artist);
    printf("Album   : %s\n", tag.album);
    printf("Year    : %s\n", tag.year);
    printf("Comment : %s\n", tag.comment);
    printf("Genre   : %s\n", tag.genre);
    printf("Track   : %s\n", tag.track);

    printf("---------------------------------------------\n");

    return 0;
}


/* ---------------------------------------------------------
   Display ID3 version
   --------------------------------------------------------- */

int show_version(const char *filename)
{
    MP3Tag tag;
    int major = 0;
    int minor = 0;
    int result;

    init_tag(&tag);

    result = read_v2(filename,
                     &tag,
                     &major,
                     &minor);

    if (result == -1)
    {
        printf("ERROR: Unable to open file.\n");
        return 1;
    }

    if (result == 2)
    {
        printf("ID3v2.4\n");
        return 0;
    }

    if (result == 1)
    {
        printf("%s\n", tag.version);
        return 0;
    }

    init_tag(&tag);

    result = read_v1(filename,
                     &tag);

    if (result == 1)
    {
        printf("%s\n", tag.version);
        return 0;
    }

    printf("ERROR: ID3 tag not found.\n");

    return 1;
}


/* ---------------------------------------------------------
   Convert command option to index
   --------------------------------------------------------- */

static int option_to_index(const char *option)
{
    if (strcmp(option, "-t") == 0)
        return TITLE;

    if (strcmp(option, "-a") == 0)
        return ARTIST;

    if (strcmp(option, "-A") == 0)
        return ALBUM;

    if (strcmp(option, "-y") == 0)
        return YEAR;

    if (strcmp(option, "-c") == 0)
        return COMMENT;

    if (strcmp(option, "-g") == 0)
        return GENRE;

    if (strcmp(option, "-T") == 0)
        return TRACK;

    return -1;
}


/* ---------------------------------------------------------
   Get frame ID
   --------------------------------------------------------- */

static void get_frame_id(int major,
                         int index,
                         char *id)
{
    static const char *v22[] =
    {
        "TT2",
        "TP1",
        "TAL",
        "TYE",
        "COM",
        "TCO",
        "TRK"
    };

    static const char *v23[] =
    {
        "TIT2",
        "TPE1",
        "TALB",
        "TYER",
        "COMM",
        "TCON",
        "TRCK"
    };

    if (major == 2)
        strcpy(id, v22[index]);
    else
        strcpy(id, v23[index]);
}


/* ---------------------------------------------------------
   Edit ID3v1
   --------------------------------------------------------- */

static int edit_v1(const char *filename,
                   int index,
                   const char *value)
{
    FILE *fp;
    unsigned char buffer[128];
    size_t length;

    fp = fopen(filename, "r+b");

    if (fp == NULL)
        return -1;

    if (fseek(fp, -128L, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    if (fread(buffer, 1, 128, fp) != 128)
    {
        fclose(fp);
        return 0;
    }

    if (memcmp(buffer, "TAG", 3) != 0)
    {
        fclose(fp);
        return 0;
    }

    length = strlen(value);

    if (index == TITLE)
    {
        memset(buffer + 3, 0, 30);

        if (length > 30)
            length = 30;

        memcpy(buffer + 3, value, length);
    }

    else if (index == ARTIST)
    {
        memset(buffer + 33, 0, 30);

        if (length > 30)
            length = 30;

        memcpy(buffer + 33, value, length);
    }

    else if (index == ALBUM)
    {
        memset(buffer + 63, 0, 30);

        if (length > 30)
            length = 30;

        memcpy(buffer + 63, value, length);
    }

    else if (index == YEAR)
    {
        memset(buffer + 93, 0, 4);

        if (length > 4)
            length = 4;

        memcpy(buffer + 93, value, length);
    }

    else if (index == COMMENT)
    {
        memset(buffer + 97, 0, 30);

        if (length > 30)
            length = 30;

        memcpy(buffer + 97, value, length);
    }

    else if (index == TRACK)
    {
        buffer[125] = 0;
        buffer[126] = (unsigned char)atoi(value);
    }

    else if (index == GENRE)
    {
        buffer[127] = (unsigned char)atoi(value);
    }

    if (fseek(fp, -128L, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    if (fwrite(buffer, 1, 128, fp) != 128)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    return 1;
}


/* ---------------------------------------------------------
   Edit ID3v2.2 / ID3v2.3
   --------------------------------------------------------- */

static int edit_v2(const char *filename,
                   int index,
                   const char *value,
                   int major)
{
    FILE *input;
    FILE *output;

    unsigned char header[10];
    unsigned int tag_size;
    unsigned int position = 0;

    char target_id[5];

    char temp_name[512];

    get_frame_id(major,
                 index,
                 target_id);

    input = fopen(filename, "rb");

    if (input == NULL)
        return -1;

    if (fread(header, 1, 10, input) != 10)
    {
        fclose(input);
        return 0;
    }

    if (memcmp(header, "ID3", 3) != 0)
    {
        fclose(input);
        return 0;
    }

    tag_size = syncsafe32(header + 6);

    snprintf(temp_name,
             sizeof(temp_name),
             "%s.tmp",
             filename);

    output = fopen(temp_name, "wb");

    if (output == NULL)
    {
        fclose(input);
        return 0;
    }

    /*
       For a simple submission implementation,
       copy header first.
    */

    if (fwrite(header, 1, 10, output) != 10)
    {
        fclose(input);
        fclose(output);
        remove(temp_name);
        return 0;
    }

    /*
       Read and copy frames.
       If matching frame is found,
       replace its content.
    */

    while (position < tag_size)
    {
        unsigned char frame_header[10];

        int header_size;

        unsigned int frame_size;

        char frame_id[5] = {0};

        unsigned char *data;

        if (major == 2)
            header_size = 6;
        else
            header_size = 10;

        if (position + header_size > tag_size)
            break;

        if (fread(frame_header,
                  1,
                  header_size,
                  input) != (size_t)header_size)
            break;

        if (is_zero_frame_id(frame_header,
                             major == 2 ? 3 : 4))
            break;

        if (major == 2)
        {
            memcpy(frame_id,
                   frame_header,
                   3);

            frame_size = be24(frame_header + 3);
        }
        else
        {
            memcpy(frame_id,
                   frame_header,
                   4);

            frame_size = be32(frame_header + 4);
        }

        position += header_size;

        if (frame_size == 0 ||
            position + frame_size > tag_size)
            break;

        data = malloc(frame_size);

        if (data == NULL)
            break;

        if (fread(data,
                  1,
                  frame_size,
                  input) != frame_size)
        {
            free(data);
            break;
        }

        position += frame_size;

        if (strcmp(frame_id, target_id) == 0)
        {
            unsigned int new_size;
            unsigned char *new_data;

            new_size = strlen(value) + 1;

            new_data = calloc(1,
                              new_size);

            if (new_data == NULL)
            {
                free(data);
                fclose(input);
                fclose(output);
                remove(temp_name);
                return 0;
            }

            /*
               Encoding = ISO-8859-1
            */
            new_data[0] = 0;

            memcpy(new_data + 1,
                   value,
                   strlen(value));

            if (major == 2)
            {
                unsigned char new_header[6];

                memcpy(new_header,
                       frame_header,
                       6);

                new_header[3] =
                    (new_size >> 16) & 0xFF;

                new_header[4] =
                    (new_size >> 8) & 0xFF;

                new_header[5] =
                    new_size & 0xFF;

                fwrite(new_header,
                       1,
                       6,
                       output);

                fwrite(new_data,
                       1,
                       new_size,
                       output);
            }
            else
            {
                unsigned char new_header[10];

                memcpy(new_header,
                       frame_header,
                       10);

                put_be32(new_header + 4,
                         new_size);

                fwrite(new_header,
                       1,
                       10,
                       output);

                fwrite(new_data,
                       1,
                       new_size,
                       output);
            }

            free(new_data);
        }
        else
        {
            fwrite(frame_header,
                   1,
                   header_size,
                   output);

            fwrite(data,
                   1,
                   frame_size,
                   output);
        }

        free(data);
    }

    /*
       Copy remaining tag/audio data.
    */

    {
        unsigned char buffer[4096];
        size_t bytes;

        while ((bytes = fread(buffer,
                              1,
                              sizeof(buffer),
                              input)) > 0)
        {
            fwrite(buffer,
                   1,
                   bytes,
                   output);
        }
    }

    fclose(input);
    fclose(output);

    /*
       Replace original file.
    */

    remove(filename);

    if (rename(temp_name,
               filename) != 0)
    {
        remove(temp_name);
        return 0;
    }

    return 1;
}


/* ---------------------------------------------------------
   Edit tag
   --------------------------------------------------------- */

int edit_tag(const char *option,
             const char *value,
             const char *filename)
{
    MP3Tag tag;

    int index;
    int major = 0;
    int minor = 0;

    int result;

    index = option_to_index(option);

    if (index == -1)
    {
        printf("ERROR: Invalid option.\n");
        return 1;
    }

    init_tag(&tag);

    result = read_v2(filename,
                     &tag,
                     &major,
                     &minor);

    if (result == -1)
    {
        printf("ERROR: Unable to open file.\n");
        return 1;
    }

    if (result == 2)
    {
        printf("ERROR: ID3v2.4 is not supported.\n");
        return 1;
    }

    if (result == 1)
    {
        result = edit_v2(filename,
                         index,
                         value,
                         major);

        if (result == 1)
        {
            printf("Tag updated successfully.\n");
            return 0;
        }

        printf("ERROR: Unable to update tag.\n");
        return 1;
    }

    result = edit_v1(filename,
                     index,
                     value);

    if (result == -1)
    {
        printf("ERROR: Unable to open file.\n");
        return 1;
    }

    if (result == 1)
    {
        printf("Tag updated successfully.\n");
        return 0;
    }

    printf("ERROR: ID3 tag not found.\n");

    return 1;
}