#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"
#define RWRWRW 0666

int main(int argc, char* argv[]) {
    srand(time(NULL));
    FILE* fd = NULL;
    int status = 0;
    uint32_t msg_id = 1;

    // TODO
    if(mkfifo(FIFO_NAME, RWRWRW) != 0 && errno != EEXIST) { //created the fifo between processes
        perror("FIFO cant be created!");
        exit(EXIT_FAILURE);
    }
    
    fd = fopen(FIFO_NAME, "wb"); //opened the file with write
    if(fd == NULL) {
        perror("Cant open file!");
        exit(EXIT_FAILURE);
    }
    
    //Creating the messages
    //now we have to loop 10 times and generate a random string with 1-10 characters each
    //then the string should be written to the file fd
    unsigned char start = 0x1; //start marker variable to point at
    unsigned char text_marker = 0x2; //text marker variable to point at
    unsigned char end = 0x4; //end marker to point at
    
    for(int i = 0; i < 10; i++) {
        int length = (rand() % 10) + 1;
        char* text = malloc(length * sizeof(char));
        if (text == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        
        for(int i = 0; i < length; i++) {
            text[i] = (rand() % 95) + 32; //the ASCII range we want is between 32 to 126, which is 95 char in total
        }
        fwrite(&start, 1, 1, fd);
        fwrite(&length, sizeof(uint32_t), 1, fd);
        fwrite(&msg_id, sizeof(uint32_t), 1, fd);
        fwrite(&text_marker, 1, 1, fd);
        fwrite(text, 1, length, fd);
        fwrite(&end, 1, 1, fd);
        fflush(fd);
        msg_id += 1;
    }
    
    
    fclose(fd);
    unlink(FIFO_NAME);

    return 0;
}
