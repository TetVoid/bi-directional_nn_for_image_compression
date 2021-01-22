#include "pch.h"
#include"layer.h"


double Layer::adaptive_learning_step(double max_learning_step, double* answer)
{
	double rezult=0;
	for (int i = 0; i < _msize(answer)/sizeof(answer[0]); i++)
		rezult += answer[i] * answer[i];

	rezult = 1 / rezult;
	if (rezult <= max_learning_step)
		return rezult;
	else
		return max_learning_step;
}

void Layer:: init_layer(int m, int n)
{
	srand(time(NULL));
	weights = new double*[m];
	for (int i = 0; i < m; i++)
		weights[i] = new double[n];

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			weights[i][j]=(double)(-10 + rand() % (20)) / 100;

	error_layer = new double[n];
	answer_vector = new double[n];

	for (int i = 0; i < n; i++)
		error_layer[i]=0;

	for (int i = 0; i < n; i++)
		answer_vector[i]=0;
}


void Layer::calculate(double *inputs)
{
	this->inputs= inputs;

	for (int i = 0; i < _msize(weights[0])/sizeof(weights[0][0]); i++)
		answer_vector[i]=0;

	vector<thread> thread_vector;
	for (int i = 0; i < _msize(weights[0]) / sizeof(weights[0][0]); i++)
		for (int j = 0; j < _msize(weights) / sizeof(weights[0]); j++)
			answer_vector[i] += weights[j][i] * inputs[j];
}

 double* Layer::get_error_layer()
{
	return error_layer;
}
 double* Layer::get_answer_vector()
{
	 double *copy_of_answer = new double[_msize(answer_vector) / sizeof(answer_vector[0])];
	 for (int i = 0; i < _msize(answer_vector) / sizeof(answer_vector[0]); i++)
		 copy_of_answer[i] = answer_vector[i];
	return copy_of_answer;
}

void  Layer::train(double learning_step,Layer prev_layer)
{
	int m = _msize(weights) / sizeof(weights[0]);
	int n = _msize(weights[0]) / sizeof(weights[0][0]);

	if (prev_layer.weights != 0)
	{
		double *temp_answer_vector = prev_layer.get_answer_vector();
		double step = adaptive_learning_step(learning_step, temp_answer_vector);
		
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				weights[i][j] -= error_layer[j] * step * temp_answer_vector[i];
				
		delete[] temp_answer_vector;
	}
	else
	{
		double step = adaptive_learning_step(learning_step, inputs);
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				weights[i][j] -= error_layer[j] * step *inputs[i];
	}

	if (prev_layer.weights != 0)
	{
		for (int i = 0; i < m; i++)
		{
			double sum = 0;
			for (int j = 0; j < n; j++)
				sum += weights[i][j] * error_layer[j];
			prev_layer.get_error_layer()[i] = sum;
		}
	}
}

void Layer::set_error_layer(double* error_layer)
{
	this->error_layer = error_layer;
}

int Layer::get_inputs_layers_size()
{
	return  _msize(inputs)/sizeof(inputs[0]);
}
