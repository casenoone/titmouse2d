#pragma once

#include <iostream>
#include<string>
#include <fstream>
#include <sstream>

class Plyout {
public:
	Plyout(std::string path, std::string _name) :myfile(path + _name + ".obj", std::ios::app) {
		std::cout << path << std::endl;
	}



public:
	std::ofstream myfile;


};