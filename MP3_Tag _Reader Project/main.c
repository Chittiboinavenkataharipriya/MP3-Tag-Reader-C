/*Name : Chittiboina Venkata Haripriya
Admission no. : 25012_099
Batch : 25012B
Project Name : MP3 Tag Reader
Description : This project is a command-line tool written to view and edit MP3 metadata (ID3 tags).
              Users can either display tag information or update specific fields like title, artist, album, year, and genre.
              View Mode: Shows all metadata of a specified MP3 file.
                Example: ./a.out -v SAMPLE.mp3
              Edit Mode: Updates a chosen metadata field.
                Example: ./a.out -e -t "NewTitle" SAMPLE.mp3
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"main.h"

typedef enum 
{
    operation_invalid,
    operation_view,
    operation_edit
}OperationType;

OperationType check_operation_type(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("ERROR : Insufficient arguments.\n");
        printf("USAGE : ./a.out -v <file.mp3>(SAMPLE.mp3)\n");
        return operation_invalid;
    }
    if(strcmp(argv[1], "-v") == 0)
    {
        if(argc != 3)
        {
            printf("ERROR : For view option at least 3 arguments are needed.\n");
            return operation_invalid;
        }
        return operation_view;
    }
    else if(strcmp(argv[1], "-e") == 0)
    {
        if(argc != 5)
        {
            printf("ERROR : For edit option at least 5 arguments are needed.\n");
            return operation_invalid;
        }
        return operation_edit;
    }
    else
    {
        printf("ERROR : Unknown option is provided.\n");
        return operation_invalid;
    }
}
int main(int argc, char* argv[])
{
    OperationType op_type = check_operation_type(argc, argv);

    if(op_type == operation_view)
    {
        view_mp3_tags(argv[2]);
    }
    else if(op_type == operation_edit)
    {
        const char *frame = option_frame(argv[2]);
        if(!frame)
        {
            printf("Error : Invalid option.\n Usage of options are [-t] [-A] [-a] [-y] [-C] [-c]\n");
            return 1;
        }
        edit_mp3_tags(frame, argv[3], argv[4]);
    }
    else
    {
        printf("Usage :\n View : ./a.out -v SAMPLE.mp3\n Edit : ./a.out -e -t [NewTitle] SAMPLE.mp3\n");
        operation_invalid;
        return 0;
    }
}