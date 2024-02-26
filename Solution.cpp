#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h>
#include <string>
#include <map>

using namespace std;

const double a = -2;
const double b = 2;

double f1(double x)
{
	return x * x * x * cos(3 * x - 1);
}

double f2(double x)
{
	return abs(5 * cos(3 * x) + 3);
}

vector<double> getEqualNodes(int n)
{
	vector<double> x(n + 1);
	x[0] = a;
	for (int i = 1; i <= n; i++)
	{
		x[i] = i * (b - a) / n + a;
	}

	return x;
}

vector<double> getChebyshevNodes(int n)
{
	vector<double> x(n + 1);
	for (int i = 0; i <= n; i++)
	{
		x[i] = (a + b) / 2 + (b - a) / 2 * cos((2 * i + 1) * M_PI / 2 / (n + 1));
	}

	return x;
}

vector<double> getConstants(vector<double>& x, double(*f)(double))
{
	vector<double>base(x.size());
	for (int i = 0; i < x.size(); i++)
	{
		base[i] = f(x[i]);
	}

	vector<double>res;
	res.push_back(base[0]);
	for (int cnt = x.size() - 1; cnt > 0; cnt--)
	{
		vector<double> temp(cnt);
		for (int i = 0; i < cnt; i++)
		{
			temp[i] = (base[i + 1] - base[i]) / (x[i + x.size() - cnt] - x[i]);
		}

		res.push_back(temp[0]);
		base.clear();
		base = temp;
		temp.clear();
	}

	return res;
}

double NewtonPolynom(vector<double>& scalars, vector<double>& x, double xp)
{
	double res = 0;
	double mnoz = 1;
	for (int i = 0; i < x.size(); i++)
	{
		res += scalars[i] * mnoz;
		mnoz *= (xp - x[i]);
	}

	return res;
}

void tableLine(int n, double(*f)(double))
{
	cout << n << '\t';
	double res1 = 0;
	double res2 = 0;
	vector<double>temp = getEqualNodes(n);
	vector<double>temp2 = getConstants(temp, f);

	vector<double>temp3 = getChebyshevNodes(n);
	vector<double>temp4 = getConstants(temp3, f);


	for (int i = 0; i <= 100; i++)
	{
		double xp = a + i * (b - a) / 100;
		res1 = max(res1, abs(NewtonPolynom(temp2, temp, xp) - f(xp)));
		res2 = max(res2, abs(NewtonPolynom(temp4, temp3, xp) - f(xp)));
	}

	cout << res1 << "\t\t\t\t" << res2 << '\n';
}

void outFile(ostream& out, vector<double>& scalars, vector<double>& x)
{
	for (int i = 0; i <= 100; i++)
	{
		double xp = a + i * (b - a) / 100;
		out << xp << ' ' << NewtonPolynom(scalars, x, xp) << '\n';
	}
}

const vector<int> N_DATA = { 3, 10, 20 };
const vector<double(*)(double)> FUNC = { f1, f2 };
const map<double(*)(double), string> FILE_IDENTITY_FUNC = { {f1, "f1"}, {f2, "f2"} };
const vector<vector<double>(*)(int)> NODES = { getEqualNodes, getChebyshevNodes };
const map<vector<double>(*)(int), string> FILE_IDENTITY_NODES = { {getEqualNodes, "equal"}, {getChebyshevNodes, "chebyshev"} };

int main()
{
	for (auto n : N_DATA)
		for (auto f : FUNC)
			for (auto nodes : NODES)
			{
				vector<double> x = nodes(n);
				vector<double> scalars = getConstants(x, f);
				ofstream out(FILE_IDENTITY_FUNC.at(f) + '_' + FILE_IDENTITY_NODES.at(nodes) + '_' + to_string(n) + ".txt");
				outFile(out, scalars, x);
				out.close();
			}



	for (auto f : FUNC)
	{
		cout << FILE_IDENTITY_FUNC.at(f);
		cout << "-----------TABLE------------\n";
		cout << "n\tmax difference(equal nodes)\tmax difference(chebyshev nodes)\n";
		for (int k = 3; k <= 30; k++)
		{
			tableLine(k, f);
		}

		cout << "-----------TABLE------------\n\n";
	}
}
