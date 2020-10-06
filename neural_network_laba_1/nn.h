#pragma once
#include <iostream>
#include <math.h>
#include<stdlib.h>
#include"layer.h"
using namespace std;

class Network
{
private:
	vector<vector<vector<double>>> dataset;
	vector< double> inputs_data;
	Layer *nn_layers;
	double mean_square_error;
	void choose_inputs();
	int index=0;
	vector<vector<double>> devide_for_batches(vector<vector<vector <double>>> inputs, int n,int m);
	vector<vector<vector <double>>> forme_formation_matrix(vector<vector<double>>inputs, int n,int m, int col_of_rows, int col_of_colums);
public:
	void init_network(int size_of_input, int size_of_output, double mean_square_error);
	void set_inputs(vector< vector<vector<double>>> dataset);
	void train(double learning_step);
	vector<vector<vector <double>>> compress(vector<vector<vector <double>>> inputs,int n,int m);
	vector<vector<vector <double>>> decompress(vector<vector<vector <double>>> inputs, int n, int m);
};