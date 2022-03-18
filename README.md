# Ensoniq VFX Transwave dumper

This is (the start of) an attempt to write a tool to dump the Transwaves (and all other samples) from the Ensoniq VFX.

These transwaves are somewhat unique (in my opinion) and being able to export then and use them on other wavetable synths is tempting.


## How the Ensoniq VFX samples are stored and accessed by the hardware

The Ensoniq VFX has 16 bit samples (says Wikipedia)

So, the VFX ROM's are 0x80000 bytes long. (524288 bytes)
That means 524,288x 8-bit samples each.
or 262,144x 16-bit samples each.

There are two sample ROMs and one "NYB"/nibble ROM.... but that ROM does not appear to be on the
same bus for some reason. But then why does the VFX have 3 ROM chips?
Third ROM apparently holds "nibbles" to extend the other two ROMs and provide additional sample bits.

So either the VFX ES5505 is loading 16+4 bit samples
Or 8+4 bit samples

VFX's ES5505 chip reads 16 bit samples from a 20-bit wide address space called "sound memory".

Based on the schematic, the ROM's only have 8 data lines. And that both ROMs share those same 8 data lines.
And we know that there are different samples on the second ROM.

And we know from the labels on the data bits of the third ROM (split between HDA** and LDA**) that it's for nibbles that "pad" the first and second ROMssample bit depth

And that is supported by the MAME source code for the es5505/es5506 driver.

Buuuut the third ROM is NOT wired up to the data bus for the "sound memory", while the other two ROMs are!

Which suggests that the Ensoniq VFX totally has 8-bit samples stored in a 20-bit long address space.

## Loading those into audacity

Importing the raw data from both as 8-bit or 16-bit samples works as expected. But using 16-bit,
the samples play back twice as fast.

Loading them as "unsigned 8-bit pcm" NOTICABLY clips the waveform

 


 nibble is 4 bits
 byte is 8 bits
 word is 2 bytes, 16 bits