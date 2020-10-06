#include "pch.h"
#include"layer.h"
#include<time.h>
#include<stdlib.h>

double Layer::adaptive_learning_step(double max_learning_step,vector<double> answer)
{
	double rezult=0;
	for (int i = 0; i < answer.size(); i++)
		for(int j=0;j< answer.size();j++)
		rezult += answer[i] * answer[j];

	if (rezult > 1)
		rezult = 1 / rezult;
	else
		rezult / 100;
	

	//cout << rezult<<endl;

	if (rezult <= max_learning_step)
		return rezult;
	else
		return max_learning_step;
}

void Layer:: init_layer(int m, int n)
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
	{
		vector< double>temp;
		for (int j = 0; j < n; j++)
			temp.push_back((double)(-10 + rand() % (20)) / 100);
		weights.push_back(temp);
	}

	for (int i = 0; i < n; i++)
		error_layer.push_back(0);

	for (int i = 0; i < n; i++)
		answer_vector.push_back(0);
}

void Layer::calculate(vector< double>inputs)
{
	this->inputs= inputs;

	for (int i = 0; i < weights[0].size(); i++)
		answer_vector[i]=0;

	for (int i = 0; i < weights[0].size(); i++)
		for (int j = 0; j < weights.size();  j++)
			answer_vector[i] += weights[j][i] * inputs[j];


}

vector< double>Layer::get_error_layer()
{
	return error_layer;
}

vector< double>Layer::get_answer_vector()
{
	return answer_vector;
}

void  Layer::train(double learning_step,Layer prev_layer)
{
	int m = weights.size();
	int n = weights[0].size();

	if (prev_layer.weights.empty() != true)
	{
		//double step = adaptive_learning_step(learning_step, prev_layer.get_answer_vector());
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				weights[i][j] -= error_layer[j] * learning_step * prev_layer.get_answer_vector()[i];
	}
	else
	{
		//double step = adaptive_learning_step(learning_step, inputs);
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				weights[i][j] -= error_layer[j] * learning_step *inputs[i];
	}

	vector< double> new_error_layer;
	for (int i = 0; i < m; i++)
		new_error_layer.push_back(0);

	
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			new_error_layer[i] += weights[i][j]*error_layer[j];

	if (prev_layer.weights.empty() != true)
		prev_layer.set_error_layer(new_error_layer);
}

void Layer::set_error_layer(vector< double> error_layer)
{
		this->error_layer= error_layer;
}

int Layer::get_inputs_layers_size()
{
	return inputs.size();
}
