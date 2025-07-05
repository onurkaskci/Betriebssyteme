#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHK(PTR, ERR, EX) ({if (!(PTR)) {fprintf(stderr, "\033[31m\033[1mError:\033[0m %s\n\r", ERR); exit(EX);}})
#define CHK0(ARGC, ARGV) ({if (ARGC < 3) {fprintf(stderr, "\033[31m\033[1mError:\033[0m Not enough arguments given. Usage: %s input1.wav input2.wav ... output.wav\n", ARGV[0]); exit(1);}})
#define CHK1(PTR) CHK(PTR, "Memory allocation failed", 125)
#define ERR0(PATHNAME) ({fprintf(stderr, "\033[31m\033[1mError:\033[0m Output file '%s' already exists.\n\r", PATHNAME); exit(1);})
#define ERR1(PATHNAME) ({fprintf(stderr, "\033[31m\033[1mError:\033[0m Could not open input file '%s'.\n\r", PATHNAME); exit(1);})
#define ERR2(PATHNAME) ({fprintf(stderr, "\033[31m\033[1mError:\033[0m Could not read WAV header from '%s'.\n\r", PATHNAME); exit(1);})
#define ERR3(PATHNAME) ({fprintf(stderr, "\033[31m\033[1mError:\033[0m Incompatible WAV file: '%s'.\n\r", PATHNAME); exit(1);})
#define ERR4(PATHNAME) ({fprintf(stderr, "\033[31m\033[1mError:\033[0m Could not create output file '%s'.\n\r", PATHNAME); exit(1);})
#define MIXING_COMPLETED(PATHNAME) printf("\033[32m\033[1mInfo:\033[0m Mixing completed. Output written to '%s'.\n", PATHNAME);

#define HEADER_SIZE 44

// WAV header structure
typedef struct {
    char riff[4];            // "RIFF"
    unsigned int size;
    char wave[4];            // "WAVE"
    char fmt[4];             // "fmt "
    unsigned int fmt_size;
    unsigned short audio_format;
    unsigned short num_channels;
    unsigned int sample_rate;
    unsigned int byte_rate;
    unsigned short block_align;
    unsigned short bits_per_sample;
    char data[4];            // "data"
    unsigned int data_size;
} WavHeader;

int headers_are_compatible(WavHeader* a, WavHeader* b) { return memcmp(a, b, 22) == 0; }

int main(int argc, char* argv[]) {
    CHK0(argc, argv);
    WavHeader reference_header;
    WavHeader header;
    int input_count = argc - 2;
    const char* output_path = argv[argc - 1];
    FILE* test;
    FILE* output;
    FILE** inputs = malloc(sizeof(FILE*) * input_count); CHK1(inputs);
    int16_t* samples = malloc(sizeof(int16_t) * input_count); CHK1(samples);
    
    // Check that output file does not exist
    ///TODO1

    // Open all input files and check headers
    for (int i = 0; i < input_count; ++i) {
        // Check and open input file --> ERR1: Could not open input file
        ///TODO2
        
        // Rewind + Check Header --> ERR2: Could not read WAV header
        ///TODO3
        
        // Check compatibility --> ERR3: Incompatible WAV file
        ///TODO4
    }

    // Create output file
    ///TODO5

    // Write placeholder header
    ///TODO6

    // Calculate channels and sample count 
    int sample_count = reference_header.data_size / (reference_header.bits_per_sample / 8);
    int channels = reference_header.num_channels;
    int total_samples = sample_count / channels;
    int16_t sample = 0;
    int16_t mixed = 0;
    int sum = 0;
    int count = 0;
    long int final_pos = 0;
    
    // Mix samples
    ///TODO7

    // Calculate correct data size
    final_pos = ftell(output);
    reference_header.data_size = final_pos - HEADER_SIZE;
    reference_header.size = final_pos - 8;

    // Update header with calculated size
    ///TODO8

    // Cleanup
    ///TODO9

    // Mixing completed
    ///TODO10
    
    return EXIT_SUCCESS;
}
