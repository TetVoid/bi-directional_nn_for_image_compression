#include "pch.h"
#include <iostream>
#include"nn.h"
#include "CImg-2.9.2/CImg.h"
using namespace cimg_library;
using namespace std;

int main()
{
	Network test;
	int n = 16;
	int m = 16;

	CImg<unsigned char> img("images\\shiro2.bmp");
	int width = img.width();
	int height = img.height();
	if (width%m*n != 0)
		img.resize(width- width % m*n, height);

	if (height%n*m != 0)
		img.resize(width, height-height%n);

	width = img.width();
	height = img.height();

	vector<vector<vector <double>>>img_double;
	for (int k = 0; k < 3; k++)
	{
		vector < vector <double>> height_value;
		for (int i = 0; i < width; i++)
		{
			vector<double> width_values;
			for (int j = 0; j < height; j++)
				width_values.push_back((double)img(i, j, 0, k) / 255);
			height_value.push_back(width_values);
		}
		img_double.push_back(height_value);
	}

	

	test.init_network(n*m, 64,0.1);
	vector<vector<vector<double>>>dataset;
	
	for (int k = 0; k < img_double.size(); k++)
	{
		for (int I = 0; I < img_double[k].size(); I += n)
		{
			for (int J = 0; J < img_double[k][I].size(); J += m)
			{
				vector<vector <double>>batch;
				for (int i = 0; i < n; i++)
				{
					vector<double> line;
					for (int j = 0; j < m; j++)
					{
						line.push_back(img_double[k][I+i][J+j]);
					}
					batch.push_back(line);
				}
				dataset.push_back(batch);
			}
		}
	}

	test.set_inputs(dataset);

	test.train(0.01);

	vector<vector<vector<double>>> answer;
	cout << 0;
	answer=test.compress(img_double,n,m);
	cout << 3;
	CImg<unsigned char> new_img(width / 2, height / 2,1,3);

	for (int k = 0; k < 3; k++)
		for (int i = 0; i < answer[k].size(); i++)
			for (int j = 0; j < answer[k][i].size(); j++)
				new_img(i, j, 0, k) = answer[k][i][j]*255;

	cout << 4;
	answer=test.decompress(answer,n,m);
	cout << 7;

	CImg<unsigned char> new_decompress_img(width, height, 1, 3);
	for (int k = 0; k < 3; k++)
		for (int i = 0; i < answer[k].size(); i++)
			for (int j = 0; j < answer[k][i].size(); j++)
				new_decompress_img(i, j, 0, k) = answer[k][i][j] * 255;

	(img,new_img,new_decompress_img).display();

}

