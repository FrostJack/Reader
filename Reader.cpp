// Reader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

class Exception : public std::exception {
	std::string msg;
public:
	Exception(const std::string& _msg) : msg(_msg) {};
	virtual const char* what() const noexcept override {
		return msg.c_str();
	}
};

int main()
{
	std::ifstream input("example.txt", std::ios::in);
	double n;
	int k = 0;
	std::vector<std::string> names;
	std::vector<std::vector<double>> values;
	std::string s, t;

	if (!input.is_open()) {
		//std::cerr << "There was a problem opening the input file!\n";
		perror("There was a problem opening the input file");
		exit(-1);
	}

	std::getline(input, s);
	names.reserve(100);
	try {
		std::istringstream ss(s);
		while (ss && !ss.eof()) {
			if (!(ss >> t))
				throw Exception("Wrong type of data recieved, when reading " + std::to_string(k+1) + " column name");
			++k;
			names.push_back(t);
		}
		if (ss.bad()) {
			std::cerr << "Error while reading line of names" << std::endl;
			return -2;
		}
	}
	catch (Exception& fail) {
		std::cout << fail.what() << std::endl;
		//perror("Error reading line of names: ");
		return -1;
	}



	values.reserve(1000);
	int j, m = 0;
	try {
		while (std::getline(input, s)) {
			++m;
			j = 0;
			std::istringstream ss(s);
			std::vector<double> row;
			row.reserve(k);
			while (ss && !(ss.eof())) {
				if (!(ss >> n)) {
					//ss.clear();
					//ss >> t;
					//std::cout << t << std::endl;
					throw Exception("Wrong type of data recieved, when reading value in line " + std::to_string(m+1)
					+ " column " + std::to_string(j+1));
				}
				++j;
				row.push_back(n);
				//std::cout << n << std::endl;
			}
			if (ss.bad()) {
				std::cerr << "Error reading " << m << " line of values: " << std::endl;
				//perror(("Reading  line of values: ");
				return -3;
			}
			if (j < k) {
				throw Exception("Not enough values in line " + std::to_string(m+1));
			}
			values.push_back(row);
		}
	}
	catch (Exception& fail) {
		std::cout << fail.what() << std::endl;
		//perror("Error reading line of names: ");
		return -4;
	}

	for (int i = 0; i < k; ++i)
		std::cout << names[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < k; ++j)
			std::cout << values[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << "File has been read without errors" << std::endl;
	return 0;
}

