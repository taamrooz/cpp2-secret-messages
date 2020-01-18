#include "filestream.h"
#include "wav_header.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>

void filestream::read_file(const std::string& path, bool verbose)
{
	std::ifstream input(path, std::ios::binary);
	if (input) {
		wav_header wav{};
		input.read(wav.riff.data(), 4);
		input.read(reinterpret_cast<char*>(&wav.file_size), 4);
		input.read(wav.wave.data(), 4);
		input.read(wav.fmt.data(), 4);
		input.read(reinterpret_cast<char*>(&wav.fmt_size), 4);
		input.read(reinterpret_cast<char*>(&wav.audio_format), 2);
		input.read(reinterpret_cast<char*>(&wav.num_of_chan), 2);
		input.read(reinterpret_cast<char*>(&wav.samples_per_sec), 4);
		input.read(reinterpret_cast<char*>(&wav.bytes_per_sec), 4);
		input.read(reinterpret_cast<char*>(&wav.block_align), 2);
		input.read(reinterpret_cast<char*>(&wav.bits_per_sample), 2);
		input.read(wav.data_id.data(), 4);
		input.read(reinterpret_cast<char*>(&wav.data_size), 4);

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
		if (strncmp(wav.data_id.data(), "data", 4) == 0 && wav.bits_per_sample >= 16)
		{
			std::vector<char> buffer(wav.data_size);
			std::string full_mess;
			// read data as a block:
			input.read(buffer.data(), wav.data_size);
			input.close();
			const auto length = wav.data_size / wav.bits_per_sample;
			int8_t b_counter = 7;
			std::bitset<8> bi;
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
						break;
					}
					//Cast bitset to char and append to message
					full_mess += static_cast<char>(bi.to_ulong());
					bi.reset();

					b_counter = 7;

				}
			}
			std::cout << full_mess;
		}
		else
		{
			std::cout << "Not a valid file." << std::endl;
		}
	}
}

void filestream::write_message(const std::string& path, const std::string& message)
{
	std::ifstream input(path, std::ios::in | std::ios::binary);
	wav_header wav{};
	if (input)
	{
		input.read(wav.riff.data(), 4);
		input.read(reinterpret_cast<char*>(&wav.file_size), 4);
		input.read(wav.wave.data(), 4);
		input.read(wav.fmt.data(), 4);
		input.read(reinterpret_cast<char*>(&wav.fmt_size), 4);
		input.read(reinterpret_cast<char*>(&wav.audio_format), 2);
		input.read(reinterpret_cast<char*>(&wav.num_of_chan), 2);
		input.read(reinterpret_cast<char*>(&wav.samples_per_sec), 4);
		input.read(reinterpret_cast<char*>(&wav.bytes_per_sec), 4);
		input.read(reinterpret_cast<char*>(&wav.block_align), 2);
		input.read(reinterpret_cast<char*>(&wav.bits_per_sample), 2);
		input.read(wav.data_id.data(), 4);
		input.read(reinterpret_cast<char*>(&wav.data_size), 4);
	}
	int loc = input.tellg();

	std::string binary_message;
	for (auto c : message)
	{
		binary_message += std::bitset<8>(c).to_string();
	}
	binary_message += "00000000";

	std::ofstream output(path, std::ios::in | std::ios::out | std::ios::binary);
	output.seekp(loc);
	const auto k_sample_size = wav.bits_per_sample;
	unsigned int written_bits = 0;
	for (unsigned int i = 0; i < binary_message.length(); ++i)
	{
		output.seekp(loc + i * (k_sample_size / 8), output.beg);
		input.seekg(loc + i * (k_sample_size / 8));
		char c;
		input.read(&c, 1);
		std::bitset<8> b(c);
		b.set(0,
			binary_message[i] == '1' ? true : false
		);
		c = static_cast<char>(b.to_ulong());
		output.put(c);
		written_bits++;
	}
	std::cout << written_bits / 8 << std::endl;
	output.close();
	input.close();
}

