#ifndef __MP3_TAG_H
#define __MP3_TAG_H
/* ID3Tag info from MP3
 
 Last 128bytes of a MP3 file consists of: 
    ID3v1 identification -> 3 characters (the word "TAG")
    Song Title -> 30 characters
    Artist -> 30 characters
    Album -> 30 characters
    Year -> 4 characters
    Comment -> 30 characters
    Genre -> 1 byte 
*/

// struct for each file to hold information about the song
struct ID3Info_s{
    char songTitle[31];
    char artist[31];
    char album[31];
    char year[5];
    char comment[31];
    char genre;
};

int readID3FromMP3(const char *filename, struct ID3Info_s *ID3Info);
void printID3Tag(struct ID3Info_s *ID3Info);
#endif
