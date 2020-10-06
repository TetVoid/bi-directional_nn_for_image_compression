#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Layer
{
private:
	vector<vector<double>>weights;
	vector<double> error_layer;
	vector<double>answer_vector;
	vector<double>inputs ;
	double adaptive_learning_step(double max_learning_step, vector<double> answer);
public:
	void init_layer(int m, int n);
	void calculate(vector<double>inputs);
	vector< double> get_error_layer();
	void train(double learning_step, Layer prev_layer);
	void set_error_layer(vector<double>error_layer);
	vector< double> get_answer_vector();
	int get_inputs_layers_size();
};