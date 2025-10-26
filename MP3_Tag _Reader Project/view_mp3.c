#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void view_mp3_tags(const char* filename)
{
    //Opening the file in the read mode
    FILE *fp = fopen(filename, "r+");
    //checking the file is NULL or not.
    if (fp == NULL)
    {
        printf("ERROR : In opening file.\n");
        return;
    }
    //reading the 3 bytes from the header
    char header[4] = {0};
    if(fread(header, 1, 3, fp) != 3)
    {
        printf("ERROR : In reading the header.\n");
        fclose(fp);
        return;
    }
    header[3] = '\0';
    //checking the 3 bytes of header is ID3 or not.
    if (strcmp(header, "ID3") != 0)
    {
        printf("Error: Not a valid MP3 file with ID3 tag.\n");
        fclose(fp);
        return;
    }
    // Step 5: Skip the header 7 bytes
    fseek(fp, 7, SEEK_CUR);
    //creating the array for tags and names
    const char *tags[6] = {"TIT2", "TALB", "TPE1", "TYER", "TCOM", "TCON"};
    const char *names[6] = {"TITLE", "ALBUM", "ARTIST", "YEAR", "COMPOSER", "CONTENT"};
    //Reading the tags from the audio
    printf("\nMP3 Tag Reader Data");
    printf("\n------------------------------------------------------\n");
    for (int i = 0; i < 6; i++)
    {
        char tag[5] = {0};
        if(fread(tag, 1, 4, fp) != 4)
        {
            break;
        }
        unsigned char size_bytes[4];
        if(fread(size_bytes, 1, 4, fp) != 4)
        {
            break;
        }
        //converting the big endianess to little endianess 
        int size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8)  | size_bytes[3];
        fseek(fp, 2, SEEK_CUR); 
        if(size <= 0)
        {
            continue;
        }
        char *data = (char *)malloc(size+1);
        if (data == NULL)
        {
            printf("Memory allocation failed");
            fclose(fp);
            return;
        }
        if(fread(data, 1, size, fp) != size)
        {
            free(data);
            return;
        }
        data[size] = '\0';
        printf("%-8s : %s\n", names[i], data+1);
        free(data);
    }
    printf("------------------------------------------------------\n");
    //closing the opened file
    fclose(fp);
}