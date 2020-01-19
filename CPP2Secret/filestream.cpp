#include "filestream.h"
#include "wav_header.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <windows.h>

void filestream::read_file(const std::string& path, bool verbose)
{
	std::ifstream input(path, std::ios::binary);
	if (input) {
		wav_header wav{};
		wav.read(input);
		std::cout << "Reading file..." << std::endl;
		if (verbose)
		{
			std::cout << "Riff: " << wav.riff[0] << wav.riff[1] << wav.riff[2] << wav.riff[3] << std::endl;
			std::cout << "File Size: " << wav.file_size << std::endl;
			std::cout << "Wave: " << wav.wave[0] << wav.wave[1] << wav.wave[2] << wav.wave[3] << std::endl;
			std::cout << "Fmt: " << wav.fmt[0] << wav.fmt[1] << wav.fmt[2] << wav.fmt[3] << std::endl;
			std::cout << "Fmt size: " << wav.fmt_size << std::endl;
			std::cout << "Audio format: " << wav.audio_format << std::endl;
			std::cout << "Num of channels: " << wav.num_of_chan << std::endl;
			std::cout << "Samples per sec: " << wav.samples_per_sec << std::endl;
			std::cout << "Bytes per sec: " << wav.bytes_per_sec << std::endl;
			std::cout << "Block align: " << wav.block_align << std::endl;
			std::cout << "Bits per sample: " << wav.bits_per_sample << std::endl;
			std::cout << "Data: " << wav.data_id[0] << wav.data_id[1] << wav.data_id[2] << wav.data_id[3] << std::endl;
			std::cout << "Data size: " << wav.data_size << std::endl;
		}
		if (strncmp(wav.data_id.data(), "data", 4) == 0 && wav.bits_per_sample >= 16 && wav.audio_format == 1)
		{
			std::vector<char> buffer(wav.data_size);
			std::string full_mess;
			// read data as a block:
			input.read(buffer.data(), wav.data_size);
			input.close();
			const auto length = wav.data_size / wav.bits_per_sample;
			int8_t b_counter = 7;
			std::bitset<8> bi;
			bool has_message = false;
			for (unsigned int i = 0; i < length; i += 2)
			{
				int16_t sample = (buffer[i + 1]) << 8 | buffer[i];
				//Set index of bitset to LSB of the sample.
				bi.set(b_counter, sample & 1);
				--b_counter;

				if (b_counter == -1)
				{
					if (bi.none())
					{
						has_message = true;
						break;
					}
					
					
						//Cast bitset to char and append to message
						full_mess += static_cast<unsigned char>(bi.to_ulong());
						bi.reset();

						b_counter = 7;
					
					

				}
			}
			if (has_message)
			{
				std::cout << "Found message: " << std::endl;
				std::cout << full_mess;
			}
			else
			{
				std::cerr << "This file contains no message." << std::endl;
			}
		}
		else
		{
			std::cerr << "Not a valid file." << std::endl;
		}
	}
	else
	{
		std::cerr << "Could not find file." << std::endl;
	}
}

void filestream::write_message(const std::string& path, const std::string& message)
{
	std::ifstream input(path, std::ios::in | std::ios::binary);
	wav_header wav{};
	if (input)
	{
		wav.read(input);
	}
	int loc = input.tellg();

	std::string binary_message;
	for (unsigned char c : message)
	{
		auto encoded = char_to_utf8(c);
		for(auto& enc: encoded)
		{
			binary_message += std::bitset<8>(enc).to_string();
		}
		
	}

	binary_message += "00000000";

	std::ofstream output(path, std::ios::in | std::ios::out | std::ios::binary);
	output.seekp(loc);
	const auto k_sample_size = wav.bits_per_sample;
	unsigned int written_bits = 0;
	for (unsigned int i = 0; i < binary_message.length(); ++i)
	{
		output.seekp(loc + i * (k_sample_size / 8), std::ofstream::beg);
		input.seekg(loc + i * (k_sample_size / 8));
		char c;
		input.read(&c, 1);
		std::bitset<8> b(c);
		b.set(0,
			binary_message[i] == '1' ? true : false
		);
		c = static_cast<char>(b.to_ulong());
		output.put(c);
	}
	output.close();
	input.close();
}

std::string filestream::char_to_utf8(const int& in)
{
	if (in == 0)
	{
		return " ";
	}
	if ((in <= 0x7F) && (in > 0x00))
	{
		std::string out("."); //placeholder
		const std::bitset<8> b(in);
		out[0] = static_cast<unsigned char>(b.to_ulong());
		return out;
	}
	if ((in >= 0x80) && (in <= 0x07FF))
	{
		std::string out(".."); //placeholder
		const auto first = (in >> 6) ^ 0xC0; // discard last 2 bytes;
		const auto second = ((in ^ 0xFFC0) | 0x80) & ~0x40; //discard last byte

		const std::bitset<8> b1(first);
		const std::bitset<8> b2(second);

		out[0] = static_cast<unsigned char>(b1.to_ulong());
		out[1] = static_cast<unsigned char>(b2.to_ulong());
		return out;
	}
	return " ";
}

