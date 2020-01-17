#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string>

/* --- PRINTF_BYTE_TO_BINARY macro's --- */
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)    \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 \
    PRINTF_BINARY_PATTERN_INT8              PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) \
    PRINTF_BYTE_TO_BINARY_INT8((i) >> 8),   PRINTF_BYTE_TO_BINARY_INT8(i)
/* --- end macros --- */

struct WAV_HEADER
{
	unsigned char riff[4]; //RIFF Header
	uint32_t file_size; // RIFF Chunk Size
	unsigned char wave[4]; // WAVE Header
	/* "fmt" sub-chunk */
	unsigned char fmt[4]; // FMT header
	uint32_t fmt_size; // Size of the fmt chunk
	uint16_t audio_format; // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t num_of_chan; // Number of channels 1=Mono 2=Sterio
	uint32_t samples_per_sec; // Sampling Frequency in Hz
	uint32_t bytes_per_sec; // bytes per second
	uint16_t block_align; // 2=16-bit mono, 4=16-bit stereo
	uint16_t bits_per_sample; // Number of bits per sample
	/* "data" sub-chunk */
	unsigned char data_id[4]; // "data"  string
	uint32_t data_size; // Sampled data length
};

int main(int argc, char* argv[])
{
	int counter = 0;
	std::string file;
	while (counter < argc) {
		
		std::cout << argv[counter] << std::endl;
		if (counter == 1) {
			file = argv[1];
		}
		++counter;
	}
	std::ifstream input(file, std::ios::binary);
	if (input) {
		WAV_HEADER wav{};
		input.read(reinterpret_cast<char*>(wav.riff), 4);
		std::cout << "Riff: " << wav.riff[0] << wav.riff[1] << wav.riff[2] << wav.riff[3] << std::endl;
		input.read(reinterpret_cast<char*>(&wav.file_size), 4);
		std::cout << "File Size: " <<  wav.file_size << std::endl;
		input.read(reinterpret_cast<char*>(wav.wave), 4);
		std::cout << "Wave: " << wav.wave[0] << wav.wave[1] << wav.wave[2] << wav.wave[3] << std::endl;
		input.read(reinterpret_cast<char*>(wav.fmt), 4);
		std::cout << "Fmt: " << wav.fmt[0] << wav.fmt[1] << wav.fmt[2] << wav.fmt[3] << std::endl;
		input.read(reinterpret_cast<char*>(&wav.fmt_size), 4);
		std::cout << "Fmt size: " << wav.fmt_size << std::endl;
		input.read(reinterpret_cast<char*>(&wav.audio_format), 2);
		std::cout << "Audio format: " << wav.audio_format << std::endl;
		input.read(reinterpret_cast<char*>(&wav.num_of_chan), 2);
		std::cout << "Num of channels: " << wav.num_of_chan << std::endl;
		input.read(reinterpret_cast<char*>(&wav.samples_per_sec), 4);
		std::cout << "Samples per sec: " << wav.samples_per_sec << std::endl;
		input.read(reinterpret_cast<char*>(&wav.bytes_per_sec), 4);
		std::cout << "Bytes per sec: " << wav.bytes_per_sec << std::endl;
		input.read(reinterpret_cast<char*>(&wav.block_align), 2);
		std::cout << "Block align: " << wav.block_align << std::endl;
		input.read(reinterpret_cast<char*>(&wav.bits_per_sample), 2);
		std::cout << "Bits per sample: " << wav.bits_per_sample << std::endl;

		input.read(reinterpret_cast<char*>(wav.data_id), 4);
		std::cout << "Data: " << wav.data_id[0] << wav.data_id[1] << wav.data_id[2] << wav.data_id[3] << std::endl;
		input.read(reinterpret_cast<char*>(&wav.data_size), 4);
		std::cout << "Data size: " << wav.data_size << std::endl;
		if(strncmp(reinterpret_cast<char*>(wav.data_id), "data", 4) == 0 && wav.bits_per_sample >= 16)
		{
			
			// allocate memory:
			char* buffer = new char[wav.data_size];
			std::vector<std::string> full_mess;
			std::string mess;
			
			uint16_t mask = 0b0000'0000'0000'0001;
			// read data as a block:
			input.read(buffer, wav.data_size);
			input.close();
			const auto length = wav.data_size / wav.bits_per_sample;
			std::size_t b_counter = 0;
			std::bitset<8> bi;
			for(unsigned int i = 0; i < length; i += 2)
			{
				int16_t sample = (static_cast<char>(buffer[i + 1]) << 8) | static_cast<char>(buffer[i]);
				//printf(PRINTF_BINARY_PATTERN_INT16 "\n", PRINTF_BYTE_TO_BINARY_INT16(sample));
				bi.set(b_counter, sample & 1);
				//std::cout << (buffer[i] & 1) << std::endl;
				//for(auto j = 0; j < wav.bits_per_sample; ++j)
				//{
				//	//b.emplace_back(((buffer[i]) & (1U << j)) >> j);
				//	
				//	//std::cout << b[j];
				//}
				//b.flip();
				//std::cout << std::endl;
				//std::cout << " " << b[0] << std::endl;
				//std::cout << bi << std::endl;
				//message.emplace_back(b[0]);
				++b_counter;
				//printf("point " PRINTF_BINARY_PATTERN_INT16 "\n", PRINTF_BYTE_TO_BINARY_INT16(buffer[i]));
				if(sample == 0b0000'0000'0000'0000)
				{
					
					full_mess.push_back(mess);
					mess = {};
					//b_counter = 0;
					//std::cout << "Found nul byte at position: " << i << std::endl;
					//std::cout << "Message: " << result.back() << std::endl;
					
					
				}
				if(b_counter == 8)
				{
					std::cout << bi.to_string() << std::endl;
					bi.reset();
					mess.push_back(static_cast<char>(bi.to_ulong()));
					b_counter = 0;
				}
			}
			for(auto& str: full_mess)
			{
				std::cout << str << " ";
			}
			std::cout << full_mess.size();
			//std::cout.write(buffer, wav.data_size);
			delete[] buffer;
		} else
		{
			std::cout << "Not a valid file." << std::endl;
		}
		
		
		
		//std::cout << buffer << std::endl;
		// print content:
		//std::cout.write(buffer, length);
	}
}
