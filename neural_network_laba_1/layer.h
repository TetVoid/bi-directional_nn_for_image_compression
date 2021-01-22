#pragma once
#include <iostream>
#include <vector>
#include<time.h>
#include<stdlib.h>
#include <thread>
using namespace std;

class Layer
{
private:
	double** weights=0;
	double* error_layer;
	double* answer_vector;
	double* inputs ;
	double adaptive_learning_step(double max_learning_step, double* answer);
public:
	void init_layer(int m, int n);
	void calculate(double *inputs);
	double* get_error_layer();
	void train(double learning_step, Layer prev_layer);
	void set_error_layer(double*error_layer);
	double* get_answer_vector();
	int get_inputs_layers_size();
};