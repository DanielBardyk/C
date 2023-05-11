#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

#include "parselib.h"

#define ENTER 13
#define ESC 27
#define CTRL_C 3
#define YES 121
#define NO 110
#define ONE 49
#define TWO 50
#define getName(var) #var

int checkOrNot();
int continueOrExit();
double firstEquation(double, double);
double secondEquation(double, double);
int chooseEquation();
int chooseMethod();
int checkApplicability(double (*)(double, double), double, double, double);
double *bisectorsMethod(double (*)(double, double), double, double, double, double, char);
double *NewtonMethod(double (*)(double, double), double, double, double, char);
void checkResult(double (*)(double, double), double, double, double);

int main()
{
	char indexEq, applicableInterval;
	int button, equationNum, methodNum;
	double a, b, t, e, *res;
	double (*equations[])(double, double) = {firstEquation, secondEquation};

	printf("\nThe program calculates two Nonlinear Equations\n\n");

	do
	{
		// choose equation
		equationNum = chooseEquation();
		if (!equationNum)
			return 0;
		else if (equationNum == ONE)
			printf("You choosed the equation number 1\n\n");
		else
			printf("You choosed the equation number 2\n\n");
		indexEq = (equationNum == ONE) ? 0 : 1;

		// choose method
		methodNum = chooseMethod();
		if (!methodNum)
			return 0;
		else if (methodNum == ONE)
			printf("You choosed the Bisectors Method\n\n");
		else
			printf("You choosed the Newton Method\n\n");

		// get the values from the user
		printf("Set the interval of the root [a; b]\n\n");
		do
		{
			applicableInterval = 1;
			a = getDoubleValue(getName(a));

			if (!a)
			{
				fprintf(stderr, "\nNumber must not be equal to zero\n\n");
				applicableInterval = 0;
				continue;
			}
			else if (equationNum == TWO && methodNum == ONE && a <= 0)
			{
				fprintf(stderr, "\nNumber must be > 0\n\n");
				applicableInterval = 0;
				continue;
			}

			// set number b
			do
			{
				applicableInterval = 1;
				b = getDoubleValue(getName(b));
				if (b <= a)
				{
					fprintf(stderr, "\nb must be > a\n\n");
					applicableInterval = 0;
					break;
				}
				else if (equationNum == ONE && methodNum == ONE && a + b == 0)
				{
					fprintf(stderr, "\na + b must not be equal to zero\n\n");
					applicableInterval = 0;
					break;
				}
				else if (equationNum == ONE && methodNum == TWO && b == 0)
				{
					fprintf(stderr, "\nNumber must not be equal to zero\n\n");
					applicableInterval = 0;
					continue;
				}
				else if (equationNum == TWO && methodNum == TWO && b <= 0)
				{
					fprintf(stderr, "\nNumber must not be > 0\n\n");
					applicableInterval = 0;
					continue;
				}
			} while (!applicableInterval);

			if (!applicableInterval)
				continue;

			// set number t
			printf("\nSet number t and precision\n\n");
			t = getDoubleValue(getName(t));

			// check applicability
			if (!(applicableInterval = checkApplicability(equations[indexEq], a, b, t)))
				break;

			// set precision
			do
			{
				printf("\nAvailiable notations for precision: 0.001, 1e-3 (1E-3), 3\n\n");
				e = getDoubleValue(getName(e));
				if (e < 1e-6 || e > 6)
				{
					fprintf(stderr, "Precision must be in range 1e-6 to 6\n");
					printf("E notation means: 1e-6 = 0.000001\n");
					e = 0;
				}
				else if (e >= 1)
					e = pow(0.1, e);
			} while (!e);

			// calculate the result
			if (methodNum == ONE)
				res = bisectorsMethod(equations[indexEq], a, b, t, e, equationNum);
			else
				res = NewtonMethod(equations[indexEq], b, t, e, equationNum);

			if (res[1])
			{
				printf("\nResult: %lf\n", res[0]);

				// check the result
				checkResult(equations[indexEq], res[0], t, equationNum);
			}
			else
				applicableInterval = 0;
		} while (!applicableInterval);

		free(res);
		button = continueOrExit();
	} while (button == ENTER);
	return 0;
}

double firstEquation(double num_x, double num_t)
{
	return (cos(num_t / num_x) - 2 * sin(1 / num_x) + 1 / num_x);
}

double secondEquation(double num_x, double num_t)
{
	return (sin(log(num_x)) - cos(log(num_x)) + num_t * log(num_x));
}

int chooseEquation()
{
	int choosedEq;
	printf("Press '1' or '2' to choose the equation\n\n");
	printf("1) cos(t/x) - 2sin(1/x) + 1/x = 0\n\n");
	printf("2) sin(ln(x)) - cos(ln(x)) + tln(x) = 0\n\n");
	do
	{
		choosedEq = getch();
		if (choosedEq == ESC || choosedEq == CTRL_C)
			return 0;
		else if (choosedEq != ONE && choosedEq != TWO)
			printf("Press '1' or '2' to choose the equation\n\n");
	} while (choosedEq != ONE && choosedEq != TWO);

	return choosedEq;
}

int chooseMethod()
{
	int choosedMet;
	printf("Press '1' or '2' to choose the solving method\n\n");
	printf("1) Division by half method\n\n");
	printf("2) Newton method\n\n");
	do
	{
		choosedMet = getch();
		if (choosedMet == ESC || choosedMet == CTRL_C)
			return 0;
		else if (choosedMet != ONE && choosedMet != TWO)
			printf("Press '1' or '2' to choose the solving method\n\n");
	} while (choosedMet != ONE && choosedMet != TWO);

	return choosedMet;
}

int checkApplicability(double (*equation)(double, double), double num_a, double num_b, double num_t)
{
	int applicable = 1;

	if (equation(num_a, num_t) * equation(num_b, num_t) >= 0)
	{
		fprintf(stderr, "\nInterval is not applicable to this method\n\n");
		applicable = 0;
	}

	return applicable;
}

double *bisectorsMethod(double (*equation)(double, double), double num_a, double num_b, double num_t, double eps, char eq_num)
{
	double x, *result, correct = 1;
	result = (double *)calloc(2, sizeof(double));

	do
	{
		x = (num_a + num_b) / 2;
		if (num_a == 0 || x == 0)
		{
			fprintf(stderr, "\n\nUnexpected division by zero. Please, try the other interval\n\n");
			correct = 0;
			break;
		}
		(equation(num_a, num_t) * equation(x, num_t) > 0) ? (num_a = x) : (num_b = x);
	} while (fabs(num_b - num_a) > eps);

	result[0] = x;
	result[1] = correct;
	return result;
}

double *NewtonMethod(double (*equation)(double, double), double num_b, double num_t, double eps, char eq_num)
{
	double fx, df, *x, alpha = 1e-14, delta, root, *result;
	int i = 0, correct = 1;
	size_t arrSize = 1;
	x = (double *)calloc(arrSize, sizeof(double));
	result = (double *)calloc(arrSize, sizeof(double));
	x[0] = num_b;

	do
	{
		if (x[i] == 0 || x[i] + alpha == 0)
		{
			fprintf(stderr, "\nInterval is not applicable to this method\n\n");
			correct = 0;
			break;
		}
		else if (eq_num == TWO)
		{
			if (x[i] < 0 || x[i] + alpha < 0)
			{
				fprintf(stderr, "\nInterval is not applicable to this equation\n\n");
				correct = 0;
				break;
			}
		}
		fx = equation(x[i], num_t);
		df = (equation(x[i] + alpha, num_t) - fx) / alpha;
		delta = fx / df;
		arrSize += 1;
		x = realloc(x, arrSize * sizeof(double));
		x[++i] = x[i] - delta;
	} while (fabs(delta) > eps);

	result[0] = x[i];
	free(x);
	result[1] = correct;

	return result;
}

void checkResult(double (*equation)(double, double), double result, double num_t, double equationNum)
{
	int button, checkResult;
	do
	{
		button = checkOrNot();

		if (button == YES)
		{
			checkResult = equation(result, num_t);
			if (equationNum == ONE)
				printf("cos(%lf/%lf) - 2sin(1/%lf) + 1/%lf = %lf", num_t, result, result, result, checkResult);
			else
				printf("sin(log(%lf)) - cos(log(%lf)) + %lflog(%lf) = %lf", result, result, num_t, result, checkResult);
		}
		else if (button == ESC || button == CTRL_C)
			break;

	} while (button != YES && button != NO);
}

int checkOrNot()
{
	int getButton;
	printf("\nDo you want to check the result?\n");
	printf("\nPress 'y' for YES / 'n' for NOT\n\n");
	getButton = getch();
	return getButton;
}

int continueOrExit()
{
	int getButton;
	printf("\n\nPress ENTER to continue / ANY button to leave the program.\n\n");
	getButton = getch();
	return getButton;
}
