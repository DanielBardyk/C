#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#include "parselib.h"

#define ENTER 13
#define ESC 27
#define CTRL_C 3
#define YES 121
#define NO 110
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define getName(var) #var
#define PI 3.14159265358979323846

typedef struct
{
	double re;
	double lm;
} complex;

int continueOrExit();
int scrollOrStop();
int chooseCircuit();
double findResFreq(double, double);
double *circuit1(double, double, double, double, double);
double *circuit2(double, double, double, double, double);
double *circuit3(double, double, double, double, double);
double *circuit4(double, double, double, double, double);
double calcValues(double *(*)(double, double, double, double, double), double, double, double, double, double, double, double);
complex calcResistance(complex, double, double, double, double);
void printResistance(complex);

int main()
{
	int button, correctValues, circuitNum, indexCir;
	double f0, fmin, fmax, df, R, R1, R2, L, C, Z;
	double *(*circuits[])(double, double, double, double, double) = {circuit1, circuit2, circuit3, circuit4};
	printf("\nThe program calculates impedances\n\n");
	do
	{
		circuitNum = chooseCircuit();
		if (!circuitNum)
			return 0;

		switch (circuitNum)
		{
		case ONE:
			printf("You choosed the circuit number 1\n\n");
			indexCir = 0;
			break;
		case TWO:
			printf("You choosed the circuit number 2\n\n");
			indexCir = 1;
			break;
		case THREE:
			printf("You choosed the circuit number 3\n\n");
			indexCir = 2;
			break;
		case FOUR:
			printf("You choosed the circuit number 4\n\n");
			indexCir = 3;
			break;
		}

		// get fmin and fmax
		do
		{
			correctValues = 1;

			fmin = getDoubleValue(getName(fmin));
			if (fmin <= 0)
			{
				fprintf(stderr, "\nNumber must be > 0\n\n");
				correctValues = 0;
				continue;
			}

			do
			{
				fmax = getDoubleValue(getName(fmax));
				if (fmax <= 0)
					fprintf(stderr, "\nNumber must be > 0\n\n");
			} while (fmax <= 0);

			if ((fmin > fmax && fmin - fmax > 5e+5) || (fmin < fmax && fmax - fmin > 5e+5))
			{
				fprintf(stderr, "Set smaller interval between fmin and fmax (500 thousand maximum)\n");
				correctValues = 0;
			}
		} while (!correctValues);

		// get step
		do
		{
			df = getDoubleValue(getName(df));
			if (df == 0)
			{
				fprintf(stderr, "The step must not be equal to zero\n");
			}
			else if (fmin < fmax && df < 0)
			{
				fprintf(stderr, "If fmin < fmax the step must be > 0\n");
				df = 0;
			}
			else if (fmin > fmax && df > 0)
			{
				fprintf(stderr, "If fmin > fmax the step must be < 0\n");
				df = 0;
			}
		} while (!df);

		// get resistances
		do
		{
			correctValues = 1;
			if (circuitNum == ONE || circuitNum == TWO)
			{
				R1 = getDoubleValue(getName(R));
				if (R1 <= 0)
				{
					fprintf(stderr, "\nNumber must be > 0\n\n");
					correctValues = 0;
					continue;
				}
			}
			else
			{
				R1 = getDoubleValue(getName(R1));
				if (R1 <= 0)
				{
					fprintf(stderr, "\nNumber must be > 0\n\n");
					correctValues = 0;
					continue;
				}
				do
				{
					R2 = getDoubleValue(getName(R2));
					if (R2 <= 0)
						fprintf(stderr, "\nNumber must be > 0\n\n");
				} while (R2 <= 0);
			}
		} while (!correctValues);

		// get L
		do
		{
			L = getDoubleValue(getName(L));
			if (L <= 0)
				fprintf(stderr, "\nNumber must be > 0\n\n");
		} while (L <= 0);

		// get C
		do
		{
			C = getDoubleValue(getName(C));
			if (C <= 0)
				fprintf(stderr, "\nNumber must be > 0\n\n");
		} while (C <= 0);

		// calculate and printf f0
		f0 = findResFreq(L, C);
		printf("\nResonant frequency: %lf\n\n", f0);

		// calculate and print f and appropriate Z
		calcValues(circuits[indexCir], fmin, fmax, df, R1, R2, L, C);

		button = continueOrExit();
	} while (button == ENTER);
	return 0;
}

int chooseCircuit()
{
	int choosedCir;
	printf("Press '1', '2', '3' or '4' to choose the circuit\n\n");
	do
	{
		choosedCir = getch();
		if (choosedCir == ESC || choosedCir == CTRL_C)
			return 0;
		else if (choosedCir != ONE && choosedCir != TWO && choosedCir != THREE && choosedCir != FOUR)
			printf("Press '1', '2', '3' or '4' to choose the circuit\n\n");
	} while (choosedCir != ONE && choosedCir != TWO && choosedCir != THREE && choosedCir != FOUR);

	return choosedCir;
}

double findResFreq(double L, double C)
{
	double f0;
	f0 = 1 / (2 * PI * sqrt(L * C));
	return f0;
}

double *circuit1(double w, double R, double R2, double L, double C)
{
	double *p;
	size_t arrSize = 4;
	p = calloc(arrSize, sizeof(double));
	p[0] = L / C;
	p[1] = -R / (w * C);
	p[2] = R;
	p[3] = w * L - 1 / (w * C);
	return p;
}

double *circuit2(double w, double R, double R2, double L, double C)
{
	double *p;
	size_t arrSize = 4;
	p = calloc(arrSize, sizeof(double));
	p[0] = L / C;
	p[1] = R / (w * C);
	p[2] = R;
	p[3] = w * L - 1 / (w * C);
	return p;
}

double *circuit3(double w, double R1, double R2, double L, double C)
{
	double *p;
	size_t arrSize = 4;
	p = calloc(arrSize, sizeof(double));
	p[0] = R1 * R2;
	p[1] = R1 * (w * L - 1 / (w * C));
	p[2] = R1 + R2;
	p[3] = w * L - 1 / (w * C);
	return p;
}

double *circuit4(double w, double R1, double R2, double L, double C)
{
	double *p;
	size_t arrSize = 4;
	p = calloc(arrSize, sizeof(double));
	p[0] = R1 * R2 + L / C;
	p[1] = w * L * R1 - R2 / (w * C);
	p[2] = R1 + R2;
	p[3] = w * L - 1 / (w * C);
	return p;
}

double calcValues(double *(*circuit)(double, double, double, double, double), double fmin, double fmax, double df, double R1, double R2, double L, double C)
{
	int i = 0;
	double w, f = fmin, valuesChecked = 0, *parts;
	complex Z;

	printf("%9s %17s\n", "--f--", "--Z--");
	do
	{
		if (f == fmax)
			valuesChecked = 1;
		else if ((fmin < fmax && f >= fmax) || (fmin > fmax && f <= fmax))
		{
			f = fmax;
			valuesChecked = 1;
		}

		w = 2 * PI * f;

		if (w * C == 0)
		{
			fprintf(stderr, "\nSolution is currently not supported for this values");
			return 0;
		}

		parts = circuit(w, R1, R2, L, C);

		if (pow(parts[2], 2) + pow(parts[3], 2) == 0)
		{
			fprintf(stderr, "\nSolution is currently not supported for this values");
			return 0;
		}

		Z = calcResistance(Z, parts[0], parts[1], parts[2], parts[3]);
		free(parts);
		printf("%10lf", f);
		printResistance(Z);

		f += df;

		if (!valuesChecked && ++i % 15 == 0 && scrollOrStop() != ENTER)
			break;

	} while (!valuesChecked);
	return f;
}

complex calcResistance(complex Z, double a, double b, double c, double d)
{
	Z.re = (a * c + b * d) / (pow(c, 2) + pow(d, 2));
	Z.lm = (b * c - a * d) / (pow(c, 2) + pow(d, 2));
	return Z;
}

void printResistance(complex Z)
{
	if (Z.lm > 0)
	{
		if (Z.re < 1e-6)
			printf("\t%e+i%lf\n", Z.re, Z.lm);
		else
		{
			printf("\t%lf+i%lf\n", Z.re, Z.lm);
		}
	}
	else
	{
		if (Z.re < 1e-6)
			printf("\t%e-i%lf\n", Z.re, Z.lm);
		else
		{
			printf("\t%lf-i%lf\n", Z.re, Z.lm);
		}
	}
};

int scrollOrStop()
{
	int seeMoreBtn;
	printf("\nENTER to scroll / ANY BUTTON to stop.\n\n");
	seeMoreBtn = getch();
	return seeMoreBtn;
}

int continueOrExit()
{
	int getButton;
	printf("\nPress ENTER to continue / ANY button to leave the program.\n\n");
	getButton = getch();
	return getButton;
}
