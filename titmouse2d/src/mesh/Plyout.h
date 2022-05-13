#pragma once

#include <iostream>
#include<string>
#include <fstream>
#include <sstream>

class Plyout {
public:
	Plyout(std::string path, std::string _name, int num) :myfile(path + _name + ".ply", std::ios::app) {
		myfile << "ply" << std::endl;
		myfile << "format ascii 1.0" << std::endl;
		myfile << "comment created by titmouse2d_plywriter" << std::endl;
		myfile << "element vertex " + std::to_string(num) << std::endl;
		myfile << "property float x" << std::endl;
		myfile << "property float y" << std::endl;
		myfile << "property float z" << std::endl;
		myfile << "end_header" << std::endl;
	}

	Plyout(std::string path, std::string _name, int num, std::string attribute) :myfile(path + _name + ".ply", std::ios::app) {
		myfile << "ply" << std::endl;
		myfile << "format ascii 1.0" << std::endl;
		myfile << "comment created by titmouse2d_plywriter" << std::endl;
		myfile << "element vertex " + std::to_string(num) << std::endl;
		myfile << "property float x" << std::endl;
		myfile << "property float y" << std::endl;
		myfile << "property float z" << std::endl;
		myfile << "property float " + attribute << std::endl;
		myfile << "end_header" << std::endl;
	}

	void write_in_ply(double x, double y, double z) {
		myfile << x << " " << y << " " << " " << z << std::endl;
	}

	void write_in_ply(double x, double y, double z, double r) {
		myfile << x << " " << y << " " << " " << z << " " << r << std::endl;
	}

public:
	std::ofstream myfile;
};