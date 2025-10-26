#include "base64.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono>

/*
	Code coming from https://github.com/Reynaldev/Base64.
	TODO: Modify the code syntax and structure to match with the rest of the project.
*/

// Base64 Alphabet code function
char base_alphabet(int index) {
	char alphabet[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

	return alphabet[index];
}

// Base64 Alphabet char function
int base_code(char chr) {
	if (chr == '=') {
		return 0;
	}

	char alphabet[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

	int len = sizeof(alphabet) / sizeof(char);

	for (int i = 0; i < len; i++) {
		if (chr == alphabet[i]) {
			return i;
		}
	}

	return 0;
}

std::string base64_encode(std::string text) {
	// Declaration
	std::vector<int> binary;

	// Get the ASCII code for each letter and put it inside binary vector
	for (int i = 0; i < text.length(); i++) {
		binary.push_back(int(text[i]));
	}

	// Turn ASCII code into binary
	std::string bits;
	for (int i = 0; i < binary.size(); i++) {
		// Declaration
		std::string revTemp;
		int bit = binary[i];

		// While the ASCII is bigger than 0
		while (bit > 0) {
			// We calculate the ASCII number with modulo, so the results will be 0 or 1
			int x = bit % 2;

			// Put it inside a string that we already declared
			revTemp += std::to_string(x);

			// We divide the ASCII until it reaches zero
			bit /= 2;
		}

		// If the binary number length is less than 8, we put a 0 behind
		while (revTemp.length() < 8) {
			revTemp += "0";
		}

		// We reversed the binary code, because the order is incorrect when we insert it
		for (int j = revTemp.length() - 1; j >= 0; j--) {
			bits += revTemp[j];
		}
	}

	// We clear the vector so we can use it later
	binary.clear();

	// Declare a vector to store the whole binary string
	std::vector<std::string> binaryText;

	// We divide the binaries and put it into four group of 6 elements
	while (bits.length() > 0) {
		// If a group has less than 6 element, add a 0 in the last index
		while (bits.length() < 6) {
			bits += "0";
		}

		// Take 6 elements
		std::string temp = bits.substr(0, 6);
		// Insert it into a vector
		binaryText.push_back(temp);
		// We make it simple to take 6 element without having to change the substr by erasing first six element
		bits.erase(0, 6);
	}

	// Now we convert it to decimal using a loop, this will be converted into Base64 alphabet code (It looks like ASCII code)
	// Size loop
	for (int i = 0; i < binaryText.size(); i++) {
		// Declaration
		int result = 0;

		// Inner loop
		for (int j = binaryText[i].length() - 1; j >= 0; j--) {
			// If the binary equals 1, we calculate it using the power of (binary length - 1 - index). It is the same as power of two.
			if (binaryText[i].at(j) % 2 == 1) {
				result += pow(2, binaryText[i].length() - 1 - j);
			}
		}

		// Insert it into the vector we already cleared
		binary.push_back(result);
	}

	// Declaration
	std::string output;
	// Last but not least, we combine all Base64 alphabet code into a readable words.
	for (int i = 0; i < binary.size(); i++) {
		// We call the base64_alphabet function that already have been declared.
		output += base_alphabet(binary[i]);
	}

	// Declaration
	int padding = 0;
	// We put a padding or two if one segment has less than 4 group 
	if (binaryText.size() % 4 != 0) {
		padding = 4 - (binaryText.size() % 4);
	}

	while (abs(padding) > 0) {
		output += "=";
		padding--;
	}

	// Now we return the output
	return output;
}

std::string base64_decode(std::string text) {
	// Declaration
	std::vector<int> bits;

	// Get each character Base64 code and put it in bits vector
	for (int i = 0; i < text.length(); i++) {
		bits.push_back(base_code(text[i]));
	}

	// Turn Base64 code into binary
	std::string binaryString;
	for (int i = 0; i < bits.size(); i++) {
		std::string revString;

		// Calculate Binary
		while (bits[i] > 0) {
			int remain = bits[i] % 2;
			revString += std::to_string(remain);
			bits[i] /= 2;
		}

		// If Binary length is less than 6, put a 0 behind it
		while (revString.length() < 6) {
			revString += "0";
		}

		// Reverse the string so it would match before we divide it to a group of 8
		for (int j = revString.length() - 1; j >= 0; j--) {
			binaryString += revString[j];
		}
	}

	// Clear bits vector so we could use it later
	bits.clear();

	// Divide the binary into a group with 8 elements
	std::vector<std::string> binaryTemp;
	while (binaryString.length() > 0) {
		std::string temp = binaryString.substr(0, 8);
		binaryTemp.push_back(temp);
		binaryString.erase(0, 8);
	}

	// Convert from Binary to ASCII code
	for (int i = 0; i < binaryTemp.size(); i++) {
		int result = 0;
		for (int j = binaryTemp[i].length() - 1; j >= 0; j--) {
			if (binaryTemp[i].at(j) % 2 == 1) {
				result += pow(2, binaryTemp[i].length() - 1 - j);
			}
		}

		// Put the ASCII into the bits vector
		bits.push_back(result);
	}

	// We convert the ASCII to char
	std::string output;
	for (int i = 0; i < bits.size(); i++) {
		// Only convert the ASCII to char if it's not a padding zero.
		if (bits[i] != 0) {
			output += char(bits[i]);
		}
	}

	// Return the output
	return output;
}
