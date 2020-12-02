#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

class my_image
{
public:
	std::vector <std::vector<unsigned char>> BMP_MATRIX_MAP;
	BITMAPFILEHEADER BMP_FILE_HEADER;
	BITMAPINFOHEADER BMP_IMAGE_HEADER;
	RGBQUAD* palitra;
	std::vector <std::vector <int>> integrall_matrix;
	std::vector <std::vector <int>> class_matrix;
	std::vector <std::vector <std::vector <float>>> transform_matrix;
	std::vector <std::vector <std::vector <float>>> transform_matrix_2_x_2;
	std::vector <std::vector <BYTE>> intensity_matrix;


	my_image(const char* name_file);
	void Load_to_file(const char* name_file, my_image BMP);
	int get_height();
	int get_width();
	void make_intensity_matrix();
	std::vector<std::vector <unsigned char>> window(int i_f, int j_f, const int hight);
	void make_integrall_matrix();
	unsigned char intensity_of_blok(unsigned int i, unsigned int j, unsigned int hight);
	unsigned int what_class_is_it(unsigned int i, unsigned int j, unsigned int hight);
	std::vector<std::vector <unsigned char>> make_small_intensity_window(int i_f, int j_f, const int hight);
	unsigned int distans(std::vector<std::vector <unsigned char>> first_blok, std::vector<std::vector <unsigned char>> second_blok);
	void fractal();
	std::vector<std::vector <unsigned char>> make_new_intensity_matrix(my_image BM);
	std::vector<std::vector <unsigned char>> make_small_intensity_window_2x2(std::vector<std::vector <unsigned char>> matr);
	int get_ser(int i, int j);
};

