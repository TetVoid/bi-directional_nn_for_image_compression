#include "pch.h"
#include "nn.h"

void Network::choose_inputs()
{
	//int index = 0 + rand() % dataset.size();
	if (index >= dataset.size())
		index = 0;

	vector<double>inputs_data;
	for (int i = 0; i < dataset[index].size(); i++)
		for (int j = 0; j < dataset[index][i].size(); j++)
			inputs_data.push_back(dataset[index][i][j]);

	index++;

	this->inputs_data = inputs_data;
}

vector<vector<double>>Network::devide_for_batches(vector<vector<vector <double>>> inputs, int n, int m)
{
	vector<vector<double>>batches_for_compress;


	for (int k = 0; k < inputs.size(); k++)
	{
		for (int I = 0; I < inputs[k].size(); I += n)
		{
			for (int J = 0; J < inputs[k][I].size(); J += m)
			{
				vector<double>batch;
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < m; j++)
					{
						batch.push_back(inputs[k][I + i][J + j]);
					}
				}
				batches_for_compress.push_back(batch);
			}
		}
	}

	return batches_for_compress;
}

vector<vector<vector <double>>>Network::forme_formation_matrix(vector<vector<double>>inputs, int n,int m,int col_of_rows,int col_of_colums)
{
	vector<vector<vector<double>>>answer;
	vector<vector<double>> matrix;
	vector<double> line;
	for (int i = 0; i < inputs.size(); i+= col_of_colums / m)
	{
		
		for (int j = 0; j < inputs[i].size(); j+=m)
		{
			for (int I = 0; I < col_of_colums/m; I++)
			{
				for (int J = 0; J < m; J++)
				{
					line.push_back(inputs[i+I][j + J]);
				}
			}

			matrix.push_back(line);
			line.clear();
			if (matrix.size() == col_of_rows)
			{
				answer.push_back(matrix);
				matrix.clear();
			}
		}
	}

	return answer;
}

void Network:: init_network(int size_of_input, int size_of_output, double mean_square_error)
{
	this->mean_square_error = mean_square_error;

	Layer compress_layer;
	compress_layer.init_layer(size_of_input, size_of_output);
	Layer decompress_layer;
	decompress_layer.init_layer(size_of_output, size_of_input);

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
	double average_mean_square_error = 0;
	double mean_square_error = 0;
	int u = 0;
	do{
	mean_square_error = 0;
	/*cout << 1 << " ";
	for (int i = 0; i < inputs_data.size(); i++)
		cout << inputs_data[i] << " ";
	cout << endl;*/
	for (int I = 0; I < 10; I++)
	{
		nn_layers[0].calculate(inputs_data);
		nn_layers[1].calculate(nn_layers[0].get_answer_vector());

		vector< double> answer_vector = nn_layers[1].get_answer_vector();

		/*cout << 2 << " ";
		for (int i = 0; i < answer_vector.size(); i++)
			cout << answer_vector[i] << " ";
		cout << endl;*/

		vector< double> error_layer;
		for (int i = 0; i < inputs_data.size(); i++)
		{
			mean_square_error += pow((answer_vector[i] - inputs_data[i]), 2);
			error_layer.push_back(+answer_vector[i] - inputs_data[i]);
		}
		average_mean_square_error += mean_square_error;
		

		Layer false_layer;
		nn_layers[1].set_error_layer(error_layer);
		nn_layers[1].train(learning_step, nn_layers[0]);
		nn_layers[0].train(learning_step, false_layer);
		choose_inputs();
		u++;
	}
	mean_square_error = average_mean_square_error / 10;
	average_mean_square_error = 0;
	cout << mean_square_error <<"  "<<u << endl;
	} while ( u<100000 );
}

vector<vector<vector <double>>>  Network:: compress(vector<vector<vector <double>>> inputs,int n,int m)
{
	vector<vector<double>>batches_of_answer;

	vector<vector<double>>batches_for_compress = devide_for_batches(inputs, n,m);
	cout << 1;
	for (int i = 0; i < batches_for_compress.size(); i++)
	{
		nn_layers[0].calculate(batches_for_compress[i]);
		batches_of_answer.push_back(nn_layers[0].get_answer_vector());
	}
	cout << 2;
	return forme_formation_matrix(batches_of_answer, n/2,m/2,inputs[0].size()/2,inputs[0][0].size()/2);
}

vector<vector<vector <double>>> Network:: decompress(vector<vector<vector <double>>> inputs,int n, int m)
{
	vector<vector<double>>batches_for_decompress;
	vector<vector<double>>batches_of_answer;

	batches_for_decompress = devide_for_batches(inputs, n/2,m/2);

	cout << 5;
	for (int i = 0; i < batches_for_decompress.size(); i++)
	{
		nn_layers[1].calculate(batches_for_decompress[i]);
		batches_of_answer.push_back(nn_layers[1].get_answer_vector());
	}
	cout << 6;
	return forme_formation_matrix(batches_of_answer,n,m,inputs[0].size() * 2, inputs[0][0].size() * 2);
}
