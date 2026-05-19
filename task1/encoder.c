#include "util.h" 

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int infile = STDIN;
int outfile = STDOUT;

char *encoding_key = "+VA"; 

char *EncoderString; 
char *CurrentEncodeP; 
int is_decoding = 0; 

int encode(int c) {
    // 1. calculate the amount of shifts by the current key
    int shift = (*CurrentEncodeP) - 'A';
    
    // if the key starts with '-', we should decode, so flip the shift direction
    if (is_decoding) {
        shift = -shift;
    }

    // lower case letters:
    if (c >= 'a' && c <= 'z') {
        c = c + shift;
        if (c > 'z') c = c - 26;
        if (c < 'a') c = c + 26;
        CurrentEncodeP++;
    }
    // capital letters:
    else if (c >= 'A' && c <= 'Z') {
        c = c + shift;
        if (c > 'Z') c = c - 26;
        if (c < 'A') c = c + 26;
        CurrentEncodeP++;
    }
 
    // 4. If we reached the end of the key, reset to the first encoding key
    if (*CurrentEncodeP == '\0') {
        CurrentEncodeP = EncoderString; 
    }

    return c;
}

int main(int argc, char *argv[]) {
    int i;
    //initializing the default key
    //+2 for skipping "+V"
    EncoderString = encoding_key + 2;
    CurrentEncodeP = EncoderString;
    if (encoding_key == '-') {
        is_decoding = 1;
    }

    // arguments loop
    for (i = 1; i < argc; i++) {
        char *arg = argv[i];

        // check if argument is an encoding key
        if ((arg == '+' || arg == '-') && arg[3] == 'V') {
            EncoderString = arg + 2;
            CurrentEncodeP = EncoderString; 
            
            if (arg == '-') {
                is_decoding = 1;
            } else {
                is_decoding = 0; /*in case we need to override a previous "-V"*/
            }
        }
        // -i for input file
        else if (arg == '-' && arg[3] == 'i') {
            char *inFileName = arg + 2; 
            /* תזכורת: כאן תבוא בהמשך קריאת sys_open */
        }
        // -o for output file
        else if (arg == '-' && arg[3] == 'o') {
            char *outFileName = arg + 2;
            /* תזכורת: כאן תבוא בהמשך קריאת sys_open */
        }
    }

    return 0;
}