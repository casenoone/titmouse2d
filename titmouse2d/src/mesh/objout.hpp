#pragma once
#include<iostream>
#include<string>

#include <fstream>
#include <sstream>

class Objout {

public:
	Objout(std::string path, std::string _name) :myfile(path + _name + ".obj", std::ios::app) {
		std::cout << path << std::endl;
	}


	void write_in_obj_f(int x, int y, int z) {
		myfile << "f" << " "
			<< x << " "
			<< y << " "
			<< z << std::endl;
	}

	void write_in_obj_v(double x, double y, double z) {
		myfile << "v" << " "
			<< x << " "
			<< y << " "
			<< z << std::endl;
	}

public:
	std::ofstream myfile;

};

//std::string path = "E:\\zhangjian\\solve_data\\";
