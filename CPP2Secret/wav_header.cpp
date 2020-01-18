#include "wav_header.h"

void wav_header::read(std::ifstream& stream)
{
	stream.read(riff.data(), 4);
	stream.read(reinterpret_cast<char*>(&file_size), 4);
	stream.read(wave.data(), 4);
	stream.read(fmt.data(), 4);
	stream.read(reinterpret_cast<char*>(&fmt_size), 4);
	stream.read(reinterpret_cast<char*>(&audio_format), 2);
	stream.read(reinterpret_cast<char*>(&num_of_chan), 2);
	stream.read(reinterpret_cast<char*>(&samples_per_sec), 4);
	stream.read(reinterpret_cast<char*>(&bytes_per_sec), 4);
	stream.read(reinterpret_cast<char*>(&block_align), 2);
	stream.read(reinterpret_cast<char*>(&bits_per_sample), 2);
	stream.read(data_id.data(), 4);
	stream.read(reinterpret_cast<char*>(&data_size), 4);
}
