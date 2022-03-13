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

    // Open the first rom for reading
    FILE* in1_file = fopen(rom1_filename, "rb");
    if (!in1_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Get status about the first rom file:
    struct stat in1_status;
    if (stat(rom1_filename, &in1_status) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    // Allocate some memory for reading in the first rom based on the stat structure
    char* in1_file_contents = malloc(in1_status.st_size);
    fread(in1_file_contents, in1_status.st_size, 1, in1_file);

    // Write out the first few bytes of the read info to make sure things are sane
    printf("ROM1 data: 0x%02x 0x%02x 0x%02x 0x%02x\n", in1_file_contents[0], in1_file_contents[1], in1_file_contents[2], in1_file_contents[3]);


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
    printf("Done writing.\n");

    // Close the output file
    fclose(output1_file);


    // Close the input file
    fclose(in1_file);

    // Free the allocated memory:
    free(in1_file_contents);

    return 0;
}


// The Ensoniq VFX has 16 bit samples (says Wikipedia)
//
// So, the VFX ROM's are 0x80000 bytes long. (524288 bytes)
// That means 524,288x 8-bit samples each.
// or 262,144x 16-bit samples each.
//
// There are two sample ROMs and one "NYB"/nibble ROM.... but that ROM does not appear to be on the
// same bus for some reason. But then why does the VFX have 3 ROM chips?
// Third ROM apparently holds "nibbles" to extend the other two ROMs and provide additional sample bits.
//
// So either the VFX ES5505 is loading 16+4 bit samples
// Or 8+4 bit samples
//
// VFX's ES5505 chip reads 16 bit samples from a 20-bit wide address space called "sound memory".
//
// Based on the schematic, the ROM's only have 8 data lines. And that both ROMs share those same 8 data lines.
// And we know that there are different samples on the second ROM.
//
// And we know from the labels on the data bits of the third ROM (split between HDA** and LDA**) that
//  it's for nibbles that "pad" the first and second ROMs sample bit depth
// And that is supported by the MAME source code for the es5505/es5506 driver.
//
// Buuuut the third ROM is NOT wired up to the data bus for the "sound memory", while the other two ROMs are!
//
// Which suggests that the Ensoniq VFX totally has 8-bit samples stored in a 20-bit long address space.
//
// ## Loading those into audacity
//
// Importing the raw data from both as 8-bit or 16-bit samples works as expected. But using 16-bit,
// the samples play back twice as fast.
//
// Loading them as "unsigned 8-bit pcm" NOTICABLY clips the waveform
//
// 
//



// nibble is 4 bits
// byte is 8 bits
// word is 2 bytes, 16 bits