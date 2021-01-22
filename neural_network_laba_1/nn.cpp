#include "pch.h"
#include "nn.h"

void Network::choose_inputs()
{

	if (index >= dataset.size())
		index = 0;
	int size_of_lines = dataset[index].size();
	int size_of_rows = dataset[index][0].size();

	for (int i = 0; i < size_of_lines; i++)
		for (int j = 0; j < size_of_rows; j++)
			this->inputs_data[size_of_rows*i+j]=dataset[index][i][j];

	index++;
}

vector<vector<double>>Network::devide_for_batches(vector<vector<vector <double>>> inputs, int n, int m)
{
	vector<vector<double>>batches_for_compress;


	for (int k = 0; k < inputs.size(); k++)
	{
		for (int I = 0; I < inputs[k].size(); I += m)
		{
			for (int J = 0; J < inputs[k][I].size(); J += n)
			{
				vector<double>batch;
				for (int i = 0; i < m; i++)
				{
					for (int j = 0; j < n; j++)
					{
						batch.push_back(inputs[k][I + i][J + j]);
					}
				}
				batches_for_compress.push_back(batch);
				batch.clear();
			}
		}
	}

	return batches_for_compress;
}

vector<vector<vector <double>>>Network::forme_formation_matrix(vector<double*>inputs, int n,int m,int col_of_colums,int col_of_rows )
{
	vector<vector<vector<double>>>answer;
	vector<vector<double>> matrix;
	vector<double> line;
	for (int i = 0; i < inputs.size(); i+= col_of_rows / n)
	{
		
		for (int j = 0; j < _msize(inputs[i])/sizeof(inputs[i][0]); j+=n)
		{
			for (int I = 0; I < col_of_rows /n; I++)
			{
				for (int J = 0; J < n; J++)
				{
					line.push_back(inputs[i+I][j + J]);
				}
			}

			matrix.push_back(line);
			line.clear();
			if (matrix.size() == col_of_colums)
			{
				answer.push_back(matrix);
				matrix.clear();
			}
		}
	}

	return answer;
}

double Network::calculate_picture_error()
{
	double mean_square_error = 0;
	double temp_mean_square_error = 0;
	int inputs_size = _msize(inputs_data) / sizeof(inputs_data[0]);
	for (int I = 0; I < dataset.size(); I++)
	{
		temp_mean_square_error = 0;

		nn_layers[0].calculate(inputs_data);
		double* answer_vector = nn_layers[0].get_answer_vector();
		nn_layers[1].calculate(answer_vector);

		delete[]answer_vector;
		answer_vector = nn_layers[1].get_answer_vector();
		
		for (int i = 0; i < inputs_size; i++)
			temp_mean_square_error += pow((answer_vector[i] - inputs_data[i]), 2);

		mean_square_error += temp_mean_square_error;
	
		choose_inputs();
		delete[] answer_vector;
	}
	return mean_square_error / dataset.size() / inputs_size;
}

void Network:: init_network(int size_of_input, int size_of_output, double mean_square_error)
{
	this->mean_square_error = mean_square_error;

	Layer compress_layer;
	compress_layer.init_layer(size_of_input, size_of_output);
	Layer decompress_layer;
	decompress_layer.init_layer(size_of_output, size_of_input);

	inputs_data = new double[size_of_input];

	nn_layers = new Layer[2];
	nn_layers[0] = compress_layer;
	nn_layers[1] = decompress_layer;
}

void  Network:: set_inputs(vector< vector<vector<double>>> dataset)
{
	this->dataset = dataset;
	choose_inputs();
}

void  Network::train(double learning_step)
{
	
	double mean_square_error = 0;
	int u = 0;
	int number_of_iteration = 0;
	do{
	
	for (int I = 0; I < dataset.size(); I++)
	{
		nn_layers[0].calculate(inputs_data);
		double* answer_vector = nn_layers[0].get_answer_vector();
		nn_layers[1].calculate(answer_vector);
		
		delete[]answer_vector;
		answer_vector = nn_layers[1].get_answer_vector();

		int inputs_size = _msize(inputs_data) / sizeof(inputs_data[0]);

		double* error_layer= new double [inputs_size];
		for (int i = 0; i < inputs_size; i++)
			error_layer[i]=(answer_vector[i] - inputs_data[i]);
		
		Layer false_layer;
		nn_layers[1].set_error_layer(error_layer);
		nn_layers[1].train(learning_step, nn_layers[0]);
		nn_layers[0].train(learning_step, false_layer);
		choose_inputs();
		
		delete[] answer_vector;
		delete[] error_layer;
		number_of_iteration++;
	}
	u++;
	
	if (u % 1 == 0 || u == 1)
	{
		mean_square_error = calculate_picture_error();
		cout << mean_square_error << "  " << u << " " << number_of_iteration << endl;
	}
	
	} while (mean_square_error>this->mean_square_error);
}

vector<double*>  Network:: compress(vector<vector<vector <double>>> inputs,int n,int m)
{
	vector<double*>batches_of_answer;

	vector<vector<double>>batches_for_compress = devide_for_batches(inputs, n,m);
	for (int i = 0; i < batches_for_compress.size(); i++)
	{
		double* double_batch = new double[batches_for_compress[i].size()];
		for (int j = 0; j < batches_for_compress[i].size(); j++)
			double_batch[j] = batches_for_compress[i][j];

		nn_layers[0].calculate(double_batch);
		batches_of_answer.push_back(nn_layers[0].get_answer_vector());
	}
	return batches_of_answer;
}

vector<vector<vector <double>>> Network:: decompress(vector<double*> batches_for_decompress,int n, int m,int width,int heigth)
{
	vector<double*>batches_of_answer;

	for (int i = 0; i < batches_for_decompress.size(); i++)
	{
		int size = _msize(batches_for_decompress[i]) / sizeof(batches_for_decompress[i][0]);
		double* double_batch = new double[size];

		for (int j = 0; j < size; j++)
			double_batch[j] = batches_for_decompress[i][j];

		nn_layers[1].calculate(double_batch);
		batches_of_answer.push_back(nn_layers[1].get_answer_vector());
	}
	return forme_formation_matrix(batches_of_answer,n,m,width,heigth);
}
