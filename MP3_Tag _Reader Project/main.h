#ifndef MAIN_H
#define MAIN_H

// Function to display 6 MP3 tags from SAMPLE.mp3
void view_mp3_tags(const char * filename);

const char *option_frame(const char *opt);
// Function to edit 6 MP3 tags from SAMPLE.mp3
void edit_mp3_tags(const char *tag_name, const char *new_value, const char *filename);

#endif