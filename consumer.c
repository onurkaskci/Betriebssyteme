#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"

int main(int argc, char* argv[]) {
    FILE* fd;
    int status;

    // TODO
    
    for(int i = 0; i < 10; i++) {
        fd = fopen(FIFO_NAME, "rb");
        if(fd != NULL) {
            break;
        }
        else {
            sleep(1);
        }
    }
    if(fd == NULL) {
        perror("FIFO cant be opened!");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        //we start with reading the start byte
        unsigned char start_byte;
        size_t bytes_read = fread(&start_byte, 1, 1, fd);
        
        if (bytes_read != 1) {
            //we are either at the end of file (EOF) or received an error
            if(feof(fd)) {
                printf("End of file reached.\n");
                break;
            }
            else {
                perror("Error by reading the start byte");
                exit(EXIT_FAILURE);
            }
        }
        else { //if we really read a data with the length of 1 (which we expect)
            if(start_byte != 0x1) { //we received an unexpected byte, 0x means its in hexadecimal, 1 means its 1 written in hex format
                perror("Unexpected start byte");
                exit(EXIT_FAILURE);
            }
        }
        
        //reading the second field: length
        uint32_t length;
        size_t length_byte = fread(&length, sizeof(uint32_t), 1, fd);
        if(length_byte != 1) {
            perror("Error by reading length!");
            exit(EXIT_FAILURE);
        }
        
        //continue with reading id next;
        uint32_t id;
        size_t id_byte = fread(&id, sizeof(uint32_t), 1, fd);
        if(id_byte != 1) {
            perror("Error by reading id!");
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "%u, %u: ", id, length); //printing id byte
        
        //reading text marker
        unsigned char text_marker;
        size_t text_marker_size = fread(&text_marker, 1, 1, fd);
        if(text_marker_size != 1) {
            perror("Error by reading text marker!");
            exit(EXIT_FAILURE);
        }
        else {
            if(text_marker != 0x2) {
                perror("Unexpected text marker received!");
                exit(EXIT_FAILURE);
            }
        }
        
        //reading text, allocating memory dynamically
        char* text = malloc(length * sizeof(char));
        if(text == NULL) {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        size_t text_size = fread(text, sizeof(char), length, fd);
        if(text_size != length) {
            perror("Error by reading text!");
            exit(EXIT_FAILURE);
        }
        fwrite(text, sizeof(char), length, stdout); //printing text array even though its not null terminated \0
        free(text); //releasing memory
        
        //reading the end marker
        unsigned char end_marker;
        size_t end_marker_size = fread(&end_marker, 1, 1, fd);
        if(end_marker_size != 1) {
            perror("Error by reading end marker!");
            exit(EXIT_FAILURE);
        }
        else {
            if(end_marker != 0x4) {
                perror("Unexpected end marker received!");
                exit(EXIT_FAILURE);
            }
        }
        printf("\n");
        fflush(stdout);
    }
    fclose(fd);
    return 0;
}