#include "my_image8.h"


my_image::my_image(const char* name_file) {

	std::ifstream f1(name_file, std::ios::in | std::ios::binary);

	f1.read((char*)&BMP_FILE_HEADER, sizeof(BMP_FILE_HEADER));
	f1.read((char*)&BMP_IMAGE_HEADER, sizeof(BMP_IMAGE_HEADER));

	if (!f1.is_open()) {
		std::cout << "Error";
	}
	
	BMP_MATRIX_MAP.resize(BMP_IMAGE_HEADER.biHeight);
	for (int i = 0;i < BMP_IMAGE_HEADER.biHeight;i++)
	{
		BMP_MATRIX_MAP[i].resize(BMP_IMAGE_HEADER.biWidth);
	}

	palitra = new RGBQUAD[256];

	for (int i = 0;i < 256; i++) {
		f1.read((char*)&palitra[i], 4);
	}
	

	for (int i = 0;i < BMP_IMAGE_HEADER.biHeight;i++)
	{
		for (int j = 0; j < BMP_IMAGE_HEADER.biWidth; j++)
		{
			f1.read((char*)&BMP_MATRIX_MAP[i][j], 1);
		}
	}
	f1.close();

	make_intensity_matrix();
	make_integrall_matrix();

	transform_matrix_2_x_2.resize(BMP_MATRIX_MAP.size());
	for (size_t i = 0; i < transform_matrix_2_x_2.size(); i++)
	{
		transform_matrix_2_x_2[i].resize(transform_matrix_2_x_2.size());
		for (size_t j = 0; j < transform_matrix_2_x_2.size(); j++)
		{
			transform_matrix_2_x_2[i][j].resize(4);
			transform_matrix_2_x_2[i][j][0] = transform_matrix_2_x_2[i][j][1] = transform_matrix_2_x_2[i][j][2] = transform_matrix_2_x_2[i][j][3] =-1;
		}
	}

}
void my_image::make_integrall_matrix() {

	integrall_matrix.resize(BMP_IMAGE_HEADER.biHeight);
	for (size_t i = 0; i < integrall_matrix.size(); i++)
	{
		integrall_matrix[i].resize(integrall_matrix.size());
	}

	std::vector<std::vector <unsigned int>> s_matrix(intensity_matrix.size());
	for (size_t i = 0; i < s_matrix.size(); i++)
	{
		s_matrix[i].resize(s_matrix.size());
	}

	
	for (size_t i = 0; i < intensity_matrix.size(); i++)
	{
		for (size_t j = 0; j < intensity_matrix.size(); j++)
		{
			if (i == 0) {
				s_matrix[i][j] = intensity_matrix[i][j];
			}
			else
			{
				s_matrix[i][j] = intensity_matrix[i][j] + s_matrix[i - 1][j];
			}
		}
	}

	for (size_t i = 0; i < intensity_matrix.size(); i++)
	{
		for (size_t j = 0; j < intensity_matrix.size(); j++)
		{
			if (j == 0) {
				integrall_matrix[i][j] = s_matrix[i][j];
			}
			else
			{
				integrall_matrix[i][j] = intensity_matrix[i][j-1] + s_matrix[i][j];
			}
		}
	}

}
void my_image::make_intensity_matrix() {
	unsigned int matrix_len = BMP_IMAGE_HEADER.biHeight;
	unsigned char count_of_pixels = 0;
	
	intensity_matrix.resize(matrix_len);
	
	for (size_t k = 0; k < matrix_len; k++)
	{
		intensity_matrix[k].resize(matrix_len);
	}

	for (int i = 0; i < matrix_len; i++) {
		for (int j = 0; j < matrix_len; j++) {
			intensity_matrix[i][j] = 0.2989 * palitra[BMP_MATRIX_MAP[i][j]].rgbRed +
				0.5866 * palitra[BMP_MATRIX_MAP[i][j]].rgbGreen + 0.1144 * palitra[BMP_MATRIX_MAP[i][j]].rgbBlue;
		}
	}
}

std::vector<std::vector <unsigned char>> my_image::window(int i_f, int j_f, const int hight) {
	std::vector<std::vector <unsigned char>>  my_window(hight);
	for (size_t k = 0; k < hight; k++)
	{
		my_window[k] = std::vector<unsigned char>(hight);
	}
	for (size_t i = i_f; i < i_f + hight; i++)
	{
		for (size_t j = j_f; j < j_f + hight; j++)
		{
			my_window[i - i_f][j - j_f] = intensity_matrix[i][j];
		}
	}
	return my_window;
}

void my_image::Load_to_file(const char* file_name, my_image BMP) {

	std::ofstream f2(file_name, std::ios::out | std::ios::binary);

	if (!f2.is_open()) {
		std::cout << "Error";
	}
	f2.write((char*)&BMP_FILE_HEADER, sizeof(BITMAPFILEHEADER));
	f2.write((char*)&BMP_IMAGE_HEADER, sizeof(BITMAPINFOHEADER));

	for (int i = 0;i < 256; i++) {
		RGBQUAD a;
		a.rgbBlue = a.rgbGreen = a.rgbRed = i;
		f2.write((char*)&a, 4);
	}

	std::vector<std::vector <unsigned char>> new_intensity_matrix = make_new_intensity_matrix(BMP);
	for (int i = 0;i < BMP_IMAGE_HEADER.biHeight; i++)
	{
		for (int j = 0; j < BMP_IMAGE_HEADER.biWidth; j++)
		{
			f2.write((char*)&new_intensity_matrix[i][j], 1);
		}	
	}
	f2.close();
}


int my_image::get_height() {
	return BMP_IMAGE_HEADER.biHeight;
}
int my_image::get_width() {
	return BMP_IMAGE_HEADER.biWidth;
}

unsigned char my_image::intensity_of_blok(unsigned int i, unsigned int j, unsigned int hight) {
	hight = hight - 1;
	unsigned char blok_intensity = 0;
	if (i != 0) {
		blok_intensity = blok_intensity - integrall_matrix[i - 1][j + hight];
		if (j != 0) {
			blok_intensity = blok_intensity + integrall_matrix[i - 1][j-1];
		}
	}
	if (j != 0) {
		blok_intensity = blok_intensity - integrall_matrix[i + hight][j - 1];
	}
	blok_intensity = blok_intensity + integrall_matrix[i + hight][j + hight];
	return blok_intensity;
}
unsigned int my_image::what_class_is_it(unsigned int i, unsigned int j, unsigned int hight) {
	unsigned int blok_class = 0;
	unsigned int middle_intensiti = intensity_of_blok(i, j, hight);
	unsigned int intensiti_1_blok = intensity_of_blok(i, j, hight/2);
	if (intensiti_1_blok > middle_intensiti)
	{
		blok_class = blok_class + pow(2, 0);
	}

	unsigned int intensiti_2_blok = intensity_of_blok(i + hight/2, j, hight / 2);
	if (intensiti_2_blok > middle_intensiti)
	{
		blok_class = blok_class + pow(2, 1);
	}

	unsigned int intensiti_3_blok = intensity_of_blok(i, j + hight/2, hight / 2);
	if (intensiti_3_blok > middle_intensiti)
	{
		blok_class = blok_class + pow(2, 2);
	}

	unsigned int intensiti_4_blok = intensity_of_blok(i + hight / 2, j + hight/2, hight / 2);
	if (intensiti_4_blok > middle_intensiti)
	{
		blok_class = blok_class + pow(2, 3);
	}
	return blok_class;
}


std::vector<std::vector <unsigned char>> my_image::make_small_intensity_window(int i_f, int j_f, const int hight) {
	std::vector<std::vector <unsigned char>> small_intensity_window(hight/2);
	for (size_t i = 0; i < small_intensity_window.size(); i++){
		small_intensity_window[i].resize(hight/2);
	}

	for (size_t i = 0; i < hight; i = i + 2)
	{
		for (size_t j = 0; j < hight; j = j + 2)
		{
			small_intensity_window[i / 2][j / 2] = (intensity_matrix[i +i_f][j+ j_f] + intensity_matrix[i + 1+i_f][j + j_f] + intensity_matrix[i + 1 +i_f][j + 1 + j_f] + intensity_matrix[i+i_f][j + 1 + j_f])/4;
		}
	}
	return small_intensity_window;
}


std::vector<std::vector <unsigned char>> my_image::make_small_intensity_window_2x2(std::vector<std::vector <unsigned char>> matr) {
	std::vector<std::vector <unsigned char>> small_intensity_window(matr.size() / 2);
	for (size_t i = 0; i < small_intensity_window.size(); i++) {
		small_intensity_window[i].resize(matr.size() / 2);
	}

	for (size_t i = 0; i < matr.size(); i = i + 2)
	{
		for (size_t j = 0; j < matr.size(); j = j + 2)
		{
			small_intensity_window[i / 2][j / 2] = (matr[i][j] + matr[i + 1][j] + matr[i + 1][j + 1] + matr[i][j + 1]) / 4;
		}
	}
	return small_intensity_window;
}


unsigned int my_image::distans(std::vector<std::vector <unsigned char>> first_blok, std::vector<std::vector <unsigned char>> second_blok) {
	int my_distans = 0;
	for (size_t i = 0; i < first_blok.size(); i++)
	{
		for (size_t j = 0; j < first_blok.size(); j++)
		{
			my_distans = my_distans + pow(first_blok[i][j] - second_blok[i][j], 2);
		}
	}
	return sqrt(my_distans);
}


void my_image::fractal() {
	transform_matrix.resize(BMP_MATRIX_MAP.size()/4);
	for (size_t i = 0; i < transform_matrix.size(); i++)
	{
		transform_matrix[i].resize(transform_matrix.size());
		for (size_t j = 0; j < transform_matrix.size(); j++)
		{
			transform_matrix[i][j].resize(4);
		}
	}
	
	for (size_t i = 0; i < intensity_matrix.size()- 7; i = i + 4)
	{
		for (size_t j = 0; j < intensity_matrix.size()-7; j = j + 4) {
			std::vector <int> a;
			a.push_back(what_class_is_it(i, j, 8));
			a.push_back(i);
			a.push_back(j);
			class_matrix.push_back(a);
		}
	}
	
	for (size_t i = 0; i < intensity_matrix.size(); i = i + 4)
	{
		std::cout << 100*i/256 <<"% " ;
		for (size_t j = 0; j < intensity_matrix.size(); j = j + 4)
		{
			int distans_my = 1000;
			std::vector<std::vector <unsigned char>> small_blok = window(i, j, 4);
			for (size_t k = 0; k < class_matrix.size(); k++)
			{
				if (what_class_is_it(i, j, 4) == class_matrix[k][0]) {
					std::vector<std::vector <unsigned char>> big_blok = make_small_intensity_window(class_matrix[k][1], class_matrix[k][2], 8);
					int distans_new = distans(small_blok, big_blok);
					if (distans_my > distans_new)
					{
						distans_my = distans_new;
						transform_matrix[i / 4][j / 4][0] = class_matrix[k][0];
						transform_matrix[i / 4][j / 4][1] = class_matrix[k][1];
					}
				}
			}
			if (distans_my > 70) {
				
				std::vector<std::vector <unsigned char>> small_blok_1 = window(i, j, 2);
				std::vector<std::vector <unsigned char>> small_blok_2 = window(i, j+2, 2);
				std::vector<std::vector <unsigned char>> small_blok_3 = window(i+2, j, 2);
				std::vector<std::vector <unsigned char>> small_blok_4 = window(i+2, j+2, 2);
				int d_1 = 10000;
				int d_2 = 10000;
				int d_3 = 10000;
				int d_4 = 10000;


				for (size_t k_r = 0; k_r < class_matrix.size(); k_r++)
				{
					if (what_class_is_it(i, j, 2) == class_matrix[k_r][0]) {
						std::vector<std::vector <unsigned char>> big_blok = make_small_intensity_window(class_matrix[k_r][1], class_matrix[k_r][2], 8);
						big_blok = make_small_intensity_window_2x2(big_blok);
						int d1_new = distans(small_blok_1, big_blok);
						if (d_1 > d1_new)
						{
							d_1 = d1_new;
							transform_matrix_2_x_2[i][j][0] = class_matrix[k_r][0];
							transform_matrix_2_x_2[i][j][1] = class_matrix[k_r][1];
						}
					}
					
					if (what_class_is_it(i+2, j, 2) == class_matrix[k_r][0]) {
						std::vector<std::vector <unsigned char>> big_blok = make_small_intensity_window(class_matrix[k_r][1], class_matrix[k_r][2], 8);
						big_blok = make_small_intensity_window_2x2(big_blok);
						int d2_new = distans(small_blok_2, big_blok);
						if (d_2 > d2_new)
						{
							d_2 = d2_new;
							transform_matrix_2_x_2[i+2][j][0] = class_matrix[k_r][0];
							transform_matrix_2_x_2[i+2][j][1] = class_matrix[k_r][1];
						}
					}
					
					if (what_class_is_it(i, j+2, 2) == class_matrix[k_r][0]) {
						std::vector<std::vector <unsigned char>> big_blok = make_small_intensity_window(class_matrix[k_r][1], class_matrix[k_r][2], 8);
						big_blok = make_small_intensity_window_2x2(big_blok);
						int d3_new = distans(small_blok_3, big_blok);
						if (d_3 > d3_new)
						{
							d_3 = d3_new;
							transform_matrix_2_x_2[i][j +2][0] = class_matrix[k_r][0];
							transform_matrix_2_x_2[i][j +2][1] = class_matrix[k_r][1];
						}
					}
					
					if (what_class_is_it(i +2, j + 2, 2) == class_matrix[k_r][0]) {
						std::vector<std::vector <unsigned char>> big_blok = make_small_intensity_window(class_matrix[k_r][1], class_matrix[k_r][2], 8);
						big_blok = make_small_intensity_window_2x2(big_blok);
						int d4_new = distans(small_blok_4, big_blok);
						if (d_4 > d4_new)
						{
							d_4 = d4_new;
							transform_matrix_2_x_2[i +2][j + 2][0] = class_matrix[k_r][0];
							transform_matrix_2_x_2[i +2][j + 2][1] = class_matrix[k_r][1];
						}
					}
				}
			}

			std::vector<std::vector <unsigned char>> ch_blok = make_small_intensity_window(transform_matrix[i/4][j/4][0], transform_matrix[i/4][j/4][1], 8);
			if (transform_matrix_2_x_2[i][j][0] != -1 && transform_matrix_2_x_2[i][j+2][0] != -1)
			{

				std::vector<std::vector <unsigned char>> ch_blok = make_small_intensity_window(transform_matrix_2_x_2[i][j][0], transform_matrix_2_x_2[i][j][1], 8);
				ch_blok = make_small_intensity_window_2x2(ch_blok);
				std::vector<std::vector <unsigned char>> small_blok_1 = window(i, j, 2);

				double a_1 = 0;
				double b_1 = 0;
				double midle_small_1 = 0;
				double midle_big_1 = 0;
				for (size_t h = 0; h < small_blok_1.size(); h++)
				{
					for (size_t g = 0; g < small_blok_1.size(); g++)
					{
						midle_big_1 = midle_big_1 + ch_blok[h][g];
						midle_small_1 = midle_small_1 + small_blok_1[h][g];
					}
				}
				midle_big_1 = midle_big_1 / (small_blok_1.size() * small_blok_1.size());
				midle_small_1 = midle_small_1 / (small_blok_1.size() * small_blok_1.size());

				for (size_t h = 0; h < small_blok_1.size(); h++)
				{
					for (size_t g = 0; g < small_blok_1.size(); g++)
					{
						a_1 = a_1 + (ch_blok[h][g] - midle_big_1) * (small_blok_1[h][g] - midle_small_1);
						b_1 = b_1 + pow((ch_blok[h][g] - midle_big_1), 2);
					}
				}
				transform_matrix_2_x_2[i][j][2] = a_1 / b_1;
				transform_matrix_2_x_2[i][j][3] = midle_small_1 - (a_1 / b_1) * midle_big_1;


				//////////////2
				ch_blok = make_small_intensity_window(transform_matrix_2_x_2[i][j + 2][0], transform_matrix_2_x_2[i][j + 2][1], 8);
				ch_blok = make_small_intensity_window_2x2(ch_blok);
				std::vector<std::vector <unsigned char>> small_blok = window(i, j + 2, 2);

				a_1 = 0;
				b_1 = 0;
				midle_small_1 = 0;
				midle_big_1 = 0;
				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						midle_big_1 = midle_big_1 + ch_blok[h][g];
						midle_small_1 = midle_small_1 + small_blok[h][g];
					}
				}
				midle_big_1 = midle_big_1 / (small_blok.size() * small_blok.size());
				midle_small_1 = midle_small_1 / (small_blok.size() * small_blok.size());

				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						a_1 = a_1 + (ch_blok[h][g] - midle_big_1) * (small_blok[h][g] - midle_small_1);
						b_1 = b_1 + pow((ch_blok[h][g] - midle_big_1), 2);
					}
				}
				transform_matrix_2_x_2[i][j + 2][2] = a_1 / b_1;
				transform_matrix_2_x_2[i][j + 2][3] = midle_small_1 - (a_1 / b_1) * midle_big_1;


				///////////////////////////3

				ch_blok = make_small_intensity_window(transform_matrix_2_x_2[i + 2][j][0], transform_matrix_2_x_2[i + 2][j][1], 8);
				ch_blok = make_small_intensity_window_2x2(ch_blok);
				small_blok = window(i + 2, j, 2);

				a_1 = 0;
				b_1 = 0;
				midle_small_1 = 0;
				midle_big_1 = 0;
				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						midle_big_1 = midle_big_1 + ch_blok[h][g];
						midle_small_1 = midle_small_1 + small_blok[h][g];
					}
				}
				midle_big_1 = midle_big_1 / (small_blok.size() * small_blok.size());
				midle_small_1 = midle_small_1 / (small_blok.size() * small_blok.size());

				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						a_1 = a_1 + (ch_blok[h][g] - midle_big_1) * (small_blok[h][g] - midle_small_1);
						b_1 = b_1 + pow((ch_blok[h][g] - midle_big_1), 2);
					}
				}
				transform_matrix_2_x_2[i + 2][j][2] = a_1 / b_1;
				transform_matrix_2_x_2[i + 2][j][3] = midle_small_1 - (a_1 / b_1) * midle_big_1;

				////////////////////////////4
				ch_blok = make_small_intensity_window(transform_matrix_2_x_2[i + 2][j + 2][0], transform_matrix_2_x_2[i + 2][j + 2][1], 8);
				ch_blok = make_small_intensity_window_2x2(ch_blok);
				small_blok = window(i + 2, j + 2, 2);

				a_1 = 0;
				b_1 = 0;
				midle_small_1 = 0;
				midle_big_1 = 0;
				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						midle_big_1 = midle_big_1 + ch_blok[h][g];
						midle_small_1 = midle_small_1 + small_blok[h][g];
					}
				}
				midle_big_1 = midle_big_1 / (small_blok.size() * small_blok.size());
				midle_small_1 = midle_small_1 / (small_blok.size() * small_blok.size());

				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						a_1 = a_1 + (ch_blok[h][g] - midle_big_1) * (small_blok[h][g] - midle_small_1);
						b_1 = b_1 + pow((ch_blok[h][g] - midle_big_1), 2);
					}
				}
				transform_matrix_2_x_2[i + 2][j + 2][2] = a_1 / b_1;
				transform_matrix_2_x_2[i + 2][j + 2][3] = midle_small_1 - (a_1 / b_1) * midle_big_1;

			}
			else
			{
				double a = 0;
				double b = 0;
				double midle_small = 0;
				double midle_big = 0;
				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						midle_big = midle_big + ch_blok[h][g];
						midle_small = midle_small + small_blok[h][g];
					}
				}
				midle_big = midle_big / (small_blok.size() * small_blok.size());
				midle_small = midle_small / (small_blok.size() * small_blok.size());

				for (size_t h = 0; h < small_blok.size(); h++)
				{
					for (size_t g = 0; g < small_blok.size(); g++)
					{
						a = a + (ch_blok[h][g] - midle_big) * (small_blok[h][g] - midle_small);
						b = b + pow((ch_blok[h][g] - midle_big), 2);
					}
				}
				transform_matrix[i / 4][j / 4][2] = a / b;
				transform_matrix[i / 4][j / 4][3] = midle_small - (a / b) * midle_big;


			}

		}
	}
	
}

std::vector<std::vector <unsigned char>> my_image::make_new_intensity_matrix(my_image BM) {
	std::vector<std::vector<unsigned char>> new_intensity_matrix(BMP_MATRIX_MAP.size());
	for (size_t i = 0; i < BMP_MATRIX_MAP.size(); i++)
	{
		new_intensity_matrix[i].resize(BMP_MATRIX_MAP.size());
	}
	for (size_t i = 0; i < BMP_MATRIX_MAP.size(); i = i + 4)
	{
		for (size_t j = 0; j < BMP_MATRIX_MAP.size(); j = j + 4)
		{
			
			if(transform_matrix_2_x_2[i][j][0] != -1 && transform_matrix_2_x_2[i][j+2][0] != -1)
			{/*
				for (size_t index_i = 0; index_i < 4; index_i = index_i + 2)
				{
					for (size_t index_j = 0; index_j < 4; index_j = index_j + 2)
					{
						std::vector<std::vector <unsigned char>> big_block;
						big_block = BM.make_small_intensity_window(transform_matrix_2_x_2[i / 2 + index_i/2][j / 2 + index_j / 2][0], transform_matrix_2_x_2[i / 2 + index_i / 2][j / 2 + index_j / 2][1], 8);
						
						big_block = make_small_intensity_window_2x2(big_block);
						
						for (size_t k = i + index_i; k < 2 + i + index_i; k++) {
							for (size_t h = j +index_j; h < 2 + j +index_j; h++) {
								new_intensity_matrix[k][h] = big_block[k - i - index_i][h - j - index_j]* transform_matrix_2_x_2[i / 2 + index_i / 2][j / 2 + index_j / 2][2] + transform_matrix_2_x_2[i / 2 + index_i / 2][j / 2 + index_j / 2][3];
							}

						}

					}
				}*/

				std::vector<std::vector <unsigned char>> big_block;
				big_block = BM.make_small_intensity_window(transform_matrix_2_x_2[i][j][0], transform_matrix_2_x_2[i][j][1], 8);
				big_block = make_small_intensity_window_2x2(big_block);

				for (size_t k = i; k < 2+i; k++)
				{
					for (size_t h = j; h < 2+j; h++)
					{
						new_intensity_matrix[k][h] = big_block[k - i][h - j]*transform_matrix_2_x_2[i][j][2]+ transform_matrix_2_x_2[i][j][3];
					}
				}

				big_block = BM.make_small_intensity_window(transform_matrix_2_x_2[i][j+2][0], transform_matrix_2_x_2[i][j+2][1], 8);
				big_block = make_small_intensity_window_2x2(big_block);

				for (size_t k = i; k < 2 + i; k++)
				{
					for (size_t h = j; h < 2 + j; h++)
					{
						new_intensity_matrix[k][h+2] = big_block[k - i][h - j] * transform_matrix_2_x_2[i][j+2][2] + transform_matrix_2_x_2[i][j+2][3];
					}
				}

				big_block = BM.make_small_intensity_window(transform_matrix_2_x_2[i+2][j][0], transform_matrix_2_x_2[i+2][j][1], 8);
				big_block = make_small_intensity_window_2x2(big_block);

				for (size_t k = i; k < 2 + i; k++)
				{
					for (size_t h = j; h < 2 + j; h++)
					{
						new_intensity_matrix[k+2][h] = big_block[k - i][h - j] * transform_matrix_2_x_2[i+2][j][2] + transform_matrix_2_x_2[i+2][j][3];
					}
				}

				big_block = BM.make_small_intensity_window(transform_matrix_2_x_2[i + 2][j+2][0], transform_matrix_2_x_2[i + 2][j+2][1], 8);
				big_block = make_small_intensity_window_2x2(big_block);

				for (size_t k = i; k < 2 + i; k++)
				{
					for (size_t h = j; h < 2 + j; h++)
					{
						new_intensity_matrix[k+2][h+2] = big_block[k - i][h - j] * transform_matrix_2_x_2[i + 2][j+2][2] + transform_matrix_2_x_2[i + 2][j+2][3];
					}
				}
			}
			else {
				std::vector<std::vector <unsigned char>> big_block;
				big_block = BM.make_small_intensity_window(transform_matrix[i / 4][j / 4][0], transform_matrix[i / 4][j / 4][1], 8);
				for (size_t k = i; k < 4 + i; k++) {
					for (size_t h = j; h < 4 + j; h++) {		
						new_intensity_matrix[k][h] = big_block[k - i][h - j] * transform_matrix[i / 4][j / 4][2] + transform_matrix[i / 4][j / 4][3];
						
					}
				}
			}
		}
		
	}
	return new_intensity_matrix;
}


int my_image::get_ser(int i, int j) {
	return  intensity_matrix[i][j];
}