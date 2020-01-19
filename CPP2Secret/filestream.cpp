#include "filestream.h"
#include "wav_header.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <windows.h>
#include <codecvt>

void filestream::read_file(const std::string& path, bool verbose)
{
	std::ifstream input(path, std::ios::binary);
	if (input) {
		wav_header wav{};
		std::cout << "Reading file..." << std::endl;
		wav.read(input, verbose);
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
		wav.read(input, false);
	}
	int loc = input.tellg();

	std::string binary_message;
	for (unsigned char c: message)
	{
		auto encoded = char_to_utf8(c);
		for (auto& enc : encoded)
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

std::string filestream::char_to_utf8(int in)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(static_cast<char32_t>(in));
}

