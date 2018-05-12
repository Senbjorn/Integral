#include <iostream>
#include <thread>
#include <sys/sysinfo.h>
#include <vector>
#include <cmath>

double pi = 3.1415926535897;

double func(double x);

double sinx(double x);

void integrate(double (*func)(double), double left, double right, int n, double *result);

int main(int argc, char *argv[]) { std::cout << get_nprocs() << std::endl;
	double left = 0;
	double right = 0.5 * pi;
	int n = 100000000;
	int nthreads = std::stoi(argv[1]);
	int tn = n / nthreads;
	double step = (right - left) / nthreads;
	double integral = 0;
	std::vector<std::thread*> thread_list;
	std::vector<double> pres(nthreads);
	std::cout << "Create threads" << std::endl;
	/*
	std::thread t = std::thread(integrate,
		 				func,
						left, right, n, std::ref(integral));
	t.join();
	
	*/
	
	for (int i = 0; i < nthreads; i++) {
		double ileft = left + i * step;
		double iright = left + (i + 1) * step;	
		thread_list.push_back(new std::thread(integrate,
		 				func,
						ileft, iright, tn, &(pres[i])));
	}
	for (int i = 0; i < nthreads; i++) {
		(*thread_list[i]).join();
		integral += pres[i];
	}
	std::cout << integral << std::endl;
}

double func(double x) {
	return (x*x);
}

double sinx(double x) {
	return std::sin(x);
}

void integrate(double (*func)(double), double left, double right, int n, double *result) {
	*result = 0;
	double local_res = 0;
	double step = (right - left) / n;
	for(double x = left; x < right; x += step) {
		local_res += func(x) * step;
	}
	*result = local_res;
}
