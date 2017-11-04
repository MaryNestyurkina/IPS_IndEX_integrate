#include <iostream>
#include <chrono>
#include <cmath>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

using namespace std::chrono;
// Интегрируемая функция
double function(double x)
{
	return 4 / sqrt(4 - pow(x, 2));
}

// Вычисление интеграла методом прямоугольников
// Последовательный код
double integralSerial(const int N, const int upperBound, const int lowerBound)
{

	const double step = (double)(upperBound - lowerBound) / N; // Вычисление шага 

	double fun = 0;

	for (int i = 0; i < N - 1; ++i)
	{
		fun += 4 / sqrt(4 - pow(lowerBound + i*step, 2));
	}

	double result = fun*step; // Результат вычислений
	return result;

}

// Вычисление интеграла методом прямоугольников
// Параллельный код
double integralParallel(const int N, const int upperBound, const int lowerBound)
{

	const double step = (double)(upperBound - lowerBound) / N; // Вычисление шага

	cilk::reducer_opadd<double> fun;
	fun.set_value(0);

	cilk_for(int i = 0; i < N - 1; ++i)
	{
		fun += 4 / sqrt(4 - pow(lowerBound + i*step, 2));
	}

	double result = fun.get_value()*step;// Результат вычислений
	return result;

}



int main()
{
	const int upperBound = 1; // Верхняя граница интеграла
	const int lowerBound = 0; // Нижняя граница интеграла
	const int N = 1000000; // Число разбиений отрезка

	double resultS, // Результат вычислений (последовательный код)
		resultP; // Результат вычислений (параллельный код)
	duration<double> duration;
	double timeP, // Время вычислений (параллельный код)
		timeS; // Время вычислений (последовательный код)

	high_resolution_clock::time_point startP = high_resolution_clock::now();
	resultP = integralParallel(N, upperBound, lowerBound); // Вычисление интеграла
	high_resolution_clock::time_point endP = high_resolution_clock::now();
	duration = endP - startP;
	timeP = duration.count(); // Расчет времени вычислений (параллельный код)

	high_resolution_clock::time_point startS = high_resolution_clock::now();
	resultS = integralSerial(N, upperBound, lowerBound);// Вычисление интеграла
	high_resolution_clock::time_point endS = high_resolution_clock::now();
	duration = endS - startS;
	timeS = duration.count(); // Расчет времени вычислений (последовательный код)

	// Вывод результатов
	printf("Serial calculated result\n");
	printf("The result %lf is calculated in %lf seconds\n", resultS, timeS);
	printf("Parallel calculated result\n");
	printf("The result %lf is calculated in %lf seconds\n", resultP, timeP);

	getchar();
	return 0;
}
