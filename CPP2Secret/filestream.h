#pragma once
#include <string>

namespace filestream
{
	/**
	 * \brief Reads a file and if it detects a message, it prints it to standard output.
	 * @param path: Path to read the message from.
	 * @param verbose: Prints additional information to console.
	 */
	void read_file(const std::string& path, bool verbose);

	/**
	 * \brief Writes a message to a file given by path
	 * @param path: Path to the file.
	 * @param message: Message to be written.
	 */
	void write_message(const std::string& path, const std::string& message);

	std::string char_to_utf8(const int& in);
}
