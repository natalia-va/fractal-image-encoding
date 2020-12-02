#include "my_image8.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <vector>



double get_mse(my_image img1, my_image img2) {
	unsigned long long int razn = 0;
	for (int i = 0; i < img1.get_height(); i++) {
		for (int j = 0; j < img1.get_width(); j++) {
			razn += pow((img1.get_ser(i, j) - img2.get_ser(i, j)), 2);
		}
	}
	return razn / (img1.get_width() * img1.get_height());
}


int main()
{
	const char* file_name_boat = "Boat.bmp";
	const char* file_name_lena = "Lena-gs.bmp";
	const char* file_name_home = "Goldhill.bmp";
	

	const char* file_name_re_boat = "Boat1.bmp";
	const char* file_name_re_lena = "Lena-gs1.bmp";
	const char* file_name_re_home = "Goldhill1.bmp";
	const char* file_name_base = file_name_boat;

	my_image BMP = my_image(file_name_lena);
	my_image BMP_l = my_image(file_name_base);
	
	BMP.fractal();
	BMP.Load_to_file(file_name_re_lena, BMP_l);
	my_image BMP_2 = my_image(file_name_re_lena);

	my_image BMP_f = my_image(file_name_lena);
	my_image BMP_s = my_image(file_name_re_lena);
	for (size_t i = 0; i < 12; i++)
	{

		BMP_f = my_image(file_name_lena);
		BMP_s= my_image(file_name_re_lena);
		std::cout << i << " Lena: " << 10 * log10((255.0 * 255.0) / get_mse(BMP_f, BMP_s)) << std::endl;

		BMP.Load_to_file(file_name_re_lena, BMP_2);
		BMP_2 = my_image(file_name_re_lena);
		
	}
	
////////////////////////////////////////////////////
	BMP = my_image(file_name_home);
	BMP_l = my_image(file_name_base);

	BMP.fractal();
	BMP.Load_to_file(file_name_re_home, BMP_l);
	BMP_2 = my_image(file_name_re_home);

	for (size_t i = 0; i < 12; i++)
	{

		BMP_f = my_image(file_name_home);
		BMP_s = my_image(file_name_re_home);
		std::cout << i << " Home: " << 10 * log10((255.0 * 255.0) / get_mse(BMP_f, BMP_s)) << std::endl;

		BMP.Load_to_file(file_name_re_home, BMP_2);
		BMP_2 = my_image(file_name_re_home);

	}

//////////////////////////////////////////////////////////////////
	file_name_base = file_name_lena;
	BMP = my_image(file_name_boat);
	BMP_l = my_image(file_name_base);

	BMP.fractal();
	BMP.Load_to_file(file_name_re_boat, BMP_l);
	BMP_2 = my_image(file_name_re_boat);

	for (size_t i = 0; i < 12; i++)
	{

		BMP_f = my_image(file_name_boat);
		BMP_s = my_image(file_name_re_boat);
		std::cout <<i << " Boat: " << 10 * log10((255.0 * 255.0) /get_mse(BMP_f, BMP_s)) << std::endl;
		BMP.Load_to_file(file_name_re_boat, BMP_2);
		BMP_2 = my_image(file_name_re_boat);

	}


	



	return 0;
}