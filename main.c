#include <stdio.h> // for file i/o
#include <stdlib.h> // for things like EXIT_FAILURE
#include <sys/stat.h> // for filesystem status like stat()

int main(int argc, char *argv[]) {
    printf("Hello world\n");

    const char* rom1_filename = "u14.bin"; // Low Wavetable ROM for "sound memory"
    const char* rom2_filename = "u15.bin"; // High Wavetable ROM for "sound memory"
    const char* rom3_filename = "u16.bin"; // Nibble ROM, to pad the samples of the other two ROMs, in theory

    const char* combined_rom1_output = "low_output.bin";
    const char* combined_rom2_output = "high_output.bin";

    //=====
    // Open the first rom for reading
    FILE* in1_file = fopen(rom1_filename, "rb");
    if (!in1_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Get status about the first rom file:
    struct stat in1_status;
    if (stat(rom1_filename, &in1_status) == -1) {
        perror("stat rom1");
        exit(EXIT_FAILURE);
    }

    // Allocate some memory for reading in the first rom based on the stat structure
    unsigned char* in1_file_contents = malloc(in1_status.st_size);
    fread(in1_file_contents, in1_status.st_size, 1, in1_file);

    // Write out the first few bytes of the read info to make sure things are sane
    printf("ROM1 data: 0x%02x 0x%02x 0x%02x 0x%02x\n", in1_file_contents[0], in1_file_contents[1], in1_file_contents[2], in1_file_contents[3]);
    //=====

    //=====
    // Open the SECOND rom for reading
    FILE* in2_file = fopen(rom2_filename, "rb");
    if (!in2_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Get status about the first rom file:
    struct stat in2_status;
    if (stat(rom2_filename, &in2_status) == -1) {
        perror("stat rom2");
        exit(EXIT_FAILURE);
    }

    // Allocate some memory for reading in the first rom based on the stat structure
    unsigned char* in2_file_contents = malloc(in2_status.st_size);
    fread(in2_file_contents, in2_status.st_size, 1, in2_file);

    // Write out the first few bytes of the read info to make sure things are sane
    printf("ROM2 data: 0x%02x 0x%02x 0x%02x 0x%02x\n", in2_file_contents[0], in2_file_contents[1], in1_file_contents[2], in1_file_contents[3]);
    //=====

    //=====
    // Open NIBBLE ROM
    FILE* nibble_file = fopen(rom3_filename, "rb");
    if (!nibble_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    // Get nibble file size (just in case)
    struct stat nibble_status;
    if (stat(rom3_filename, &nibble_status) == -1) {
        perror("stat nibble");
        exit(EXIT_FAILURE);
    }
    // Allocate some memory for reading nibble file
    unsigned char* nibble_contents = malloc(nibble_status.st_size);
    fread(nibble_contents, nibble_status.st_size, 1, nibble_file);

    // Write out the first few bytes of nibble to make sure we're sane
    printf("Nibble data: 0x%02x 0x%02x 0x%02x 0x%02x\n", nibble_contents[0], nibble_contents[1], nibble_contents[2], nibble_contents[3]);





    // Open the first rom combined output for writing
    FILE* output1_file = fopen(combined_rom1_output, "wb+");
    if (!output1_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fwrite(&in1_file_contents[0], 1, 1, output1_file);
    fwrite(&in1_file_contents[1], 1, 1, output1_file);
    fwrite(&in1_file_contents[2], 1, 1, output1_file);
    fwrite(&in1_file_contents[3], 1, 1, output1_file);

    // for (int i = 0; i++; i<in1_status.st_size) {
    // }

    // For each 8-bit byte sample...
    for (int j = 0; j < 10; j = j + 1) {
        //... Print the bytes of the sample you'd expect:
        u_int16_t new_sample_low = in2_file_contents[j];
        u_int8_t nibble_low = 0xbe; //nibble_contents[j] & 0x0f) << 4;

        printf("ROM1 sample %i: 0x%02x 0x%02x\t", j, new_sample_low, nibble_low);

        u_int16_t new_sample_high = in1_file_contents[j];
        u_int8_t nibble_high = 0xbf; //(nibble_contents[j] & 0xf0);

        printf("ROM2 sample %i: 0x%02x 0x%02x\n", j, new_sample_high, nibble_high);

    }

    printf("Done writing.\n");
    // Close the output file
    fclose(output1_file);


    // Close the input files
    fclose(nibble_file);
    fclose(in1_file);

    // Free the allocated memory:
    free(nibble_contents);
    free(in1_file_contents);

    return 0;
}
