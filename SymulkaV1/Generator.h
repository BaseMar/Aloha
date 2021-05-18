#ifndef GENERATOR_H
#define GENERATOR_H
#include <math.h>
class Generator
{
public:

	Generator();
	~Generator();
	double Floor(double number_)
	{
		double fraction_ = number_;
		double integer_ = 0;
		fraction_ = modf(number_, &integer_);
		if (number_ >= 0.0)
		{
			return integer_;
		}
		else
		{
			return integer_ - 1.0;
		}

	}
	double Uniform( int& x) // from 0 to 1
	{
		double h = Floor(x / Q);
		x = A * (x - Q * h) - R * h;
		if (x < 0.0)
		{
			x += M;
		}
		return x / M;
	}
	double Uniform(int& x, double min, double max)
	{
		return Uniform(x)* (max-min)+min;
	}
	 double Exponential(double lambda,int& x)
	{
		lambda = 1 / lambda;
		double ret = -log(Uniform(x)) / lambda;
		return ret;
	}
	 double ZeroOne(double p, int& x)
	 {
		 double k_ = 0.0;
		 k_=Uniform(x);
		 if (k_ < p)
			 return 1.0;
		 else
			 return 0.0;
	 }
private:
	double M = 2147483647.0;
	double A = 16807.0;
	const int Q = 127773;
	const int R = 2836;
	const int Range = 2147483647; //2^31-1
	const double k_Pi_ = (double) 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428;// Dotychczas poznane rozwiniêcie liczby PI wg wikipedia.org.pl
};

#endif // ! GENERATOR_H