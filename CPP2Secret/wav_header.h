#pragma once
#include <cstdint>
#include <vector>
#include <fstream>

struct wav_header
{
	wav_header() : riff(4), wave(4), fmt(4), data_id(4) {}
	~wav_header() = default;
	wav_header(const wav_header& h) = delete;
	wav_header(wav_header&& h) = delete;
	wav_header operator=(const wav_header& h) = delete;
	wav_header operator=(wav_header&& h) = delete;
	void read(std::ifstream& stream, bool verbose);

	std::vector<char> riff; //RIFF Header
	uint32_t file_size{}; // RIFF Chunk Size
	std::vector<char> wave; // WAVE Header
	/* "fmt" sub-chunk */
	std::vector<char> fmt; // FMT header
	uint32_t fmt_size{}; // Size of the fmt chunk
	uint16_t audio_format{}; // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t num_of_chan{}; // Number of channels 1=Mono 2=Stereo
	uint32_t samples_per_sec{}; // Sampling Frequency in Hz
	uint32_t bytes_per_sec{}; // bytes per second
	uint16_t block_align{}; // 2=16-bit mono, 4=16-bit stereo
	uint16_t bits_per_sample{}; // Number of bits per sample
	/* "data" sub-chunk */
	std::vector<char> data_id; // "data"  string
	uint32_t data_size{}; // Sampled data length
};
