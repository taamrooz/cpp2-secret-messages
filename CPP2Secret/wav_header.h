#pragma once
#include <cstdint>

struct WAV_HEADER
{
	unsigned char riff[4]; //RIFF Header
	uint32_t file_size; // RIFF Chunk Size
	unsigned char wave[4]; // WAVE Header
	/* "fmt" sub-chunk */
	unsigned char fmt[4]; // FMT header
	uint32_t fmt_size; // Size of the fmt chunk
	uint16_t audio_format; // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t num_of_chan; // Number of channels 1=Mono 2=Stereo
	uint32_t samples_per_sec; // Sampling Frequency in Hz
	uint32_t bytes_per_sec; // bytes per second
	uint16_t block_align; // 2=16-bit mono, 4=16-bit stereo
	uint16_t bits_per_sample; // Number of bits per sample
	/* "data" sub-chunk */
	unsigned char data_id[4]; // "data"  string
	uint32_t data_size; // Sampled data length
};
