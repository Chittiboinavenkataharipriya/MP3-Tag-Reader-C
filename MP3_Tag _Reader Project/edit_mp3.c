#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"main.h"

const char *option_frame(const char *opt)
{
    if (!strcmp(opt, "-t"))
    {
        return "TIT2";  // Title
    }
    if (!strcmp(opt, "-A")) 
    {
        return "TALB";  // Album
    }
    if (!strcmp(opt, "-a")) 
    {
        return "TPE1";  // Artist
    }
    if (!strcmp(opt, "-y")) 
    {
        return "TYER";  // Year
    }
    if (!strcmp(opt, "-C")) 
    {
        return "TCOM";  // Composer
    }
    if (!strcmp(opt, "-c")) 
    {
        return "TCON";  // Genre
    }
    return NULL;
}
void edit_mp3_tags(const char *tag_name, const char *new_value, const char *filename)
{
    //Opening the SAMPLE_AUDIO.mp3 file in read mode
    FILE *fp = fopen(filename, "r+");
    if (fp == NULL)
    {
        printf("ERROR : Unable to open file %s\n", filename);
        return;
    }
    //Opening the temp.mp3 file in write mode
    FILE *temp = fopen("temp.mp3", "w+");
    if (temp == NULL)
    {
        printf("ERROR : Unable to create temp file.\n");
        fclose(fp);
        return;
    }
    //Reading the header from SAMPLE_AUDIO.mp3
    char header[10];
    if(fread(header, 1, 10, fp) != 10)
    {
        printf("ERROR : Unable to read MP3 header.\n");
        fclose(fp);
        fclose(temp);
        return;
    }
    //Writing the header from SAMPLE_AUDIO.mp3 to temp.mp3
    if(fwrite(header, 1, 10, temp) != 10)
    {
        printf("ERROR : Unable to write MP3 header.\n");
        fclose(temp);
        fclose(fp);
        return;
    }
    int size_diff = 0;
    while (1)
    {
        char tag[5] = {0};
        unsigned char size_bytes[4];
        unsigned char flags[2];
        if (fread(tag, 1, 4, fp) != 4)
        {
            break;
        } 
        if(tag[0] == 0)
        {
            fwrite(tag, 1, 4, temp);
            break;
        }
        if (fread(size_bytes, 1, 4, fp) != 4)
        {
            break;
        }
        int size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
        if (fread(flags, 1, 2, fp) != 2)
        {
            break;
        }
        char *data = (char *)malloc(size);
        if (!data)
        {
            printf("Memory allocation failed.\n");
            fclose(fp);
            fclose(temp);
            return;
        }
        fread(data, 1, size, fp);
        //comparing the tag and tag name are matching or not
        if (strcmp(tag, tag_name) == 0)
        {
            int new_size = strlen(new_value) + 1; 
            size_diff += (new_size - size);
            //converting the endianess
            unsigned char new_size_bytes[4] = {(new_size >> 24) & 0xFF, (new_size >> 16) & 0xFF, (new_size >> 8) & 0xFF, new_size & 0xFF};
            fwrite(tag, 1, 4, temp);
            fwrite(new_size_bytes, 1, 4, temp);
            fwrite(flags, 1, 2, temp);
            fputc(0x00, temp);
            fwrite(new_value, 1, strlen(new_value), temp);
            printf("Edited tag %s successfully.\n", tag_name);
        }
        else
        {
            fwrite(tag, 1, 4, temp);
            fwrite(size_bytes, 1, 4, temp);
            fwrite(flags, 1, 2, temp);
            fwrite(data, 1, size, temp);
        }
        free(data);
    }
    //Copying remaining MP3 data.
    char buff[100000];
    size_t n;
    //Reading the remining data
    while((n = fread(buff, 1, sizeof(buff), fp)) > 0)
    {
        //writing the remainig data
        if(fwrite(buff, 1, n, temp) != n)
        {
            printf("Error : Failed to write remaining data.\n");
            break;
        }
    }
    //Closing the files
    fclose(fp);
    fclose(temp);
    //removing the existing file and renaming the temp.mp3 file to SAMPLE_AUDIO.mp3 file
    if (remove(filename) != 0 || rename("temp.mp3", filename) != 0)
    {
        printf("ERROR : Failed to update file.\n");
    }
    else // printing successfully message
    {
        printf("<---Edited Information Successfully--->\n");
    }
}
