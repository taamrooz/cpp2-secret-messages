#include "wav_header.h"
#include <iostream>

void wav_header::read(std::ifstream& stream, bool verbose)
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
	if (verbose)
	{
		std::cout << "Riff: " << riff[0] << riff[1] << riff[2] << riff[3] << std::endl;
		std::cout << "File Size: " << file_size << std::endl;
		std::cout << "Wave: " << wave[0] << wave[1] << wave[2] << wave[3] << std::endl;
		std::cout << "Fmt: " << fmt[0] << fmt[1] << fmt[2] << fmt[3] << std::endl;
		std::cout << "Fmt size: " << fmt_size << std::endl;
		std::cout << "Audio format: " << audio_format << std::endl;
		std::cout << "Num of channels: " << num_of_chan << std::endl;
		std::cout << "Samples per sec: " << samples_per_sec << std::endl;
		std::cout << "Bytes per sec: " << bytes_per_sec << std::endl;
		std::cout << "Block align: " << block_align << std::endl;
		std::cout << "Bits per sample: " << bits_per_sample << std::endl;
		std::cout << "Data: " << data_id[0] << data_id[1] << data_id[2] << data_id[3] << std::endl;
		std::cout << "Data size: " << data_size << std::endl;
	}
}
