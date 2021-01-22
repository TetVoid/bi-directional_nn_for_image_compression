//http://cimg.eu/
/*
Лабораторная работа по дисциплине "Модели решения задач в интелектуальных системах"
Выполнена студентом БГУИР группы 821703
Филипповым Никитой Юрьевичем
Сжатие графической информации линейной рециркуляционной сетью
16.10.2020
Класс nn реализует общие процессы ИНС
Класс layer реализует процессы отдельного слоя ИНС
*/
#include "pch.h"
#include <iostream>
#include"nn.h"
#include "CImg-2.9.2/CImg.h"
using namespace cimg_library;
using namespace std;

vector<vector<vector <double>>> convert_to_double(CImg<unsigned char>* img,int n,int m)
{
	int width = (*img).width();
	int height = (*img).height();

	(*img).resize(width - width % m, height - height % n);

	width = (*img).width();
	height = (*img).height();


	vector<vector<vector <double>>>img_double;
	for (int k = 0; k < 3; k++)
	{
		vector < vector <double>> height_value;
		for (int i = 0; i < width; i++)
		{
			vector<double> width_values;
			for (int j = 0; j < height; j++)
				width_values.push_back((double)(*img)(i, j, 0, k) / 255);
			height_value.push_back(width_values);
		}
		img_double.push_back(height_value);
	}
	return img_double;
}

vector<vector<vector<double>>> create_dataset(vector<vector<vector<double>>> img_double,int n,int m)
{
	vector<vector<vector<double>>>dataset;
	for (int k = 0; k < img_double.size(); k++)
	{
		for (int I = 0; I < img_double[k].size(); I += m)
		{
			for (int J = 0; J < img_double[k][I].size(); J += n)
			{
				vector<vector <double>>batch;
				for (int i = 0; i < m; i++)
				{
					vector<double> line;
					for (int j = 0; j < n; j++)
					{
						line.push_back(img_double[k][I + i][J + j]);
					}
					batch.push_back(line);
				}
				dataset.push_back(batch);
			}
		}
	}
	return dataset;
}

int main(int argc, char* argv[])
{
	Network test;
	int n = 8;
	int m = 8;
	int output_neurons = 16;
	double max_step=0.01;
	double error = 0.0025;

	if (argc == 6)
	{
		n = atoi(argv[1]);
		m = atoi(argv[2]);
		output_neurons = atoi(argv[3]);
		max_step = atof(argv[4]);
		error = atof(argv[5]);
	}
	/*system("chcp 1251");
	cout << "Введите n(количество строк) ";
	cin >> n;

	cout << "Введите m(количество столбцов) ";
	cin >> m;

	cout << "Введите количество нейроннов на выхоном слое ";
	cin >> output_neurons;

	cout << "Введите максимальный шаг обучения ";
	cin >> max_step;

	cout << "Введите желаемую среднеквадратичную ошибку ";
	cin >> error;*/

	CImg<unsigned char> img("images\\mis1536X1112.bmp");
	CImg<unsigned char> img2("images\\mis1536X1112.bmp");	
	vector<vector<vector <double>>> img_double = convert_to_double(&img, n, m);
	vector<vector<vector <double>>> img_double2 = convert_to_double(&img2, n, m);
	int width = img2.width();
	int height = img2.height();
	

	vector<vector<vector<double>>>dataset1 =create_dataset(img_double,n,m);

	test.init_network(n*m, output_neurons, error);
	test.set_inputs(dataset1);
	test.train(max_step);

	vector<double*> answer;
	vector<vector<vector<double>>> new_answer;

	answer=test.compress(img_double2,n,m);
	new_answer =test.decompress(answer,n,m,width,height);

	CImg<unsigned char> new_decompress_img(width, height, 1, 3);
	for (int k = 0; k < 3; k++)
		for (int i = 0; i < new_answer[k].size(); i++)
			for (int j = 0; j < new_answer[k][i].size(); j++)
				if (new_answer[k][i][j] * 255 > 255 || new_answer[k][i][j] * 255 < 0)
					if (new_answer[k][i][j] * 255 > 255)
						new_decompress_img(i, j, 0, k) = 255;
					else
						new_decompress_img(i, j, 0, k) = 0;		
				else
					new_decompress_img(i, j, 0, k) = new_answer[k][i][j] * 255;

	(img2,new_decompress_img).display();

}

