#pragma once
#include <fstream>
#include <iostream>
#include <windows.h>

void saveModel(DWORD name) {
	std::ofstream outfile;
	outfile.open("C:\\Users\\kwat\\Documents\\LOCAL\\rdr2\\forestRedemption\\ForestRedemption\\ForestRedemption\\object_save\\new.txt", std::ios_base::app);
	outfile << std::hex << name << "\n";
	outfile.close();
}