#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_SIZE 17	 // 1) точність double 15 символів 2) '\n' 3) '\0';
#define FLOAT_SIZE 9		 // 1) точність float 7 символів 2) '\n' 3) '\0';
#define INEGER_SIZE 13	 // 1) '-' 2) 10 цифр 3) '\n' 4) '\0';
#define NAME_SIZE 40		 // 39 - name; 1 - dot;
#define FILENAME_SIZE 31 // 30 - name; 1 - dot;

double getDoubleValue(char *);
float getFloatValue(char *);
int getIntegerValue(char *);
char *getNameValue(char *);
char *getFileName(char *);

double getDoubleValue(char *nameOfVariable)
{
	char parsedCorrect;
	double number;
	do
	{
		parsedCorrect = 1;

		fflush(stdin);
		char buffer[DOUBLE_SIZE];

		printf("Enter the %s: ", nameOfVariable);

		fgets(buffer, DOUBLE_SIZE, stdin);

		int i = 0;

		int length = strlen(buffer);

		if (!isdigit(buffer[i]) && buffer[i] != '-')
		{
			isspace(buffer[i]) ? fprintf(stderr, "Must be no spaces in the beginning\n")
									 : fprintf(stderr, "Must be a digit or minus in the beginning\n");
			parsedCorrect = 0;
			continue;
		}

		char double_buffer[DOUBLE_SIZE];
		int doubleChars = 0;

		if (buffer[i] == '-')
		{
			double_buffer[doubleChars] = buffer[i];
			doubleChars++;
			i++;
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Must be a digit after a minus\n");
				parsedCorrect = 0;
				continue;
			}
		}

		while (i < length && buffer[i] != '.' && buffer[i] != 'e' && buffer[i] != 'E' && !isspace(buffer[i]))
		{
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
				parsedCorrect = 0;
				break;
			}
			else
			{
				double_buffer[doubleChars] = buffer[i];
				doubleChars++;
				i++;
			}
		}

		if (!parsedCorrect)
			continue;

		if (buffer[i] == '.')
		{
			double_buffer[doubleChars] = buffer[i];
			doubleChars++;
			i++;
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Must be a digit after a point\n");
				parsedCorrect = 0;
				continue;
			}
			while (i < length && buffer[i] != 'e' && buffer[i] != 'E' && !isspace(buffer[i]))
			{
				if (!isdigit(buffer[i]))
				{
					fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
					parsedCorrect = 0;
					break;
				}
				else
				{
					double_buffer[doubleChars] = buffer[i];
					doubleChars++;
					i++;
				}
			}
			if (!parsedCorrect)
				continue;
		}

		if (buffer[i] == 'e' || buffer[i] == 'E')
		{
			double_buffer[doubleChars] = buffer[i];
			doubleChars++;
			i++;

			if (!isdigit(buffer[i]) && buffer[i] != '+' && buffer[i] != '-')
			{
				fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
				parsedCorrect = 0;
				continue;
			}
			else if (buffer[i] == '+' || buffer[i] == '-')
			{
				double_buffer[doubleChars] = buffer[i];
				doubleChars++;
				i++;
			}
			while (i < length && !isspace(buffer[i]))
			{
				if (!isdigit(buffer[i]))
				{
					fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
					parsedCorrect = 0;
					break;
				}
				else
				{
					double_buffer[doubleChars] = buffer[i];
					doubleChars++;
					i++;
				}
			}
			if (!parsedCorrect)
				continue;
		}
		// Тут потрібна перевірка саме на '\n', бо якщо написати 1111 + пробіл + enter, то п'ятим у масиві буде ' ', а isspace() вважає це за пробіл
		if (buffer[i] != '\n' || buffer[i + 1] != '\0')
		{
			if (buffer[i] == ' ' || buffer[i + 1] == '\n')
				fprintf(stderr, "Must be no spaces around the number\n");
			else
				fprintf(stderr, "The number must have no more than 15 characters\n");

			parsedCorrect = 0;
			continue;
		}
		else
		{
			for (int j = 0; j < 2; j++)
			{
				double_buffer[doubleChars] = buffer[i];
				doubleChars++;
				i++;
			}
		}

		number = atof(double_buffer);

	} while (!parsedCorrect);

	return number;
}

float getFloatValue(char *nameOfVariable)
{
	char parsedCorrect;
	float number;
	do
	{
		parsedCorrect = 1;

		fflush(stdin);
		char buffer[FLOAT_SIZE];

		printf("Enter %s: ", nameOfVariable);

		fgets(buffer, FLOAT_SIZE, stdin);

		int i = 0;

		int length = strlen(buffer);

		if (!isdigit(buffer[i]) && buffer[i] != '-')
		{
			isspace(buffer[i]) ? fprintf(stderr, "Must be no spaces in the beginning\n")
									 : fprintf(stderr, "Must be a digit or minus in the beginning\n");
			parsedCorrect = 0;
			continue;
		}

		char float_buffer[FLOAT_SIZE];
		int floatChars = 0;

		if (buffer[i] == '-')
		{
			float_buffer[floatChars] = buffer[i];
			floatChars++;
			i++;
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Must be a digit after a minus\n");
				parsedCorrect = 0;
				continue;
			}
		}

		while (i < length && buffer[i] != '.' && buffer[i] != 'e' && buffer[i] != 'E' && !isspace(buffer[i]))
		{
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
				parsedCorrect = 0;
				break;
			}
			else
			{
				float_buffer[floatChars] = buffer[i];
				floatChars++;
				i++;
			}
		}

		if (!parsedCorrect)
			continue;

		if (buffer[i] == '.')
		{
			float_buffer[floatChars] = buffer[i];
			floatChars++;
			i++;
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Must be a digit after a point\n");
				parsedCorrect = 0;
				continue;
			}
			while (i < length && buffer[i] != 'e' && buffer[i] != 'E' && !isspace(buffer[i]))
			{
				if (!isdigit(buffer[i]))
				{
					fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
					parsedCorrect = 0;
					break;
				}
				else
				{
					float_buffer[floatChars] = buffer[i];
					floatChars++;
					i++;
				}
			}
			if (!parsedCorrect)
				continue;
		}

		if (buffer[i] == 'e' || buffer[i] == 'E')
		{
			float_buffer[floatChars] = buffer[i];
			floatChars++;
			i++;

			if (!isdigit(buffer[i]) && buffer[i] != '+' && buffer[i] != '-')
			{
				fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
				parsedCorrect = 0;
				continue;
			}
			else if (buffer[i] == '+' || buffer[i] == '-')
			{
				float_buffer[floatChars] = buffer[i];
				floatChars++;
				i++;
			}
			while (i < length && !isspace(buffer[i]))
			{
				if (!isdigit(buffer[i]))
				{
					fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
					parsedCorrect = 0;
					break;
				}
				else
				{
					float_buffer[floatChars] = buffer[i];
					floatChars++;
					i++;
				}
			}
			if (!parsedCorrect)
				continue;
		}
		// Тут потрібна перевірка саме на '\n', бо якщо написати 1111 + пробіл + enter, то п'ятим у масиві буде ' ', а isspace() вважає це за пробіл
		if (buffer[i] != '\n' || buffer[i + 1] != '\0')
		{
			if (buffer[i] == ' ' || buffer[i + 1] == '\n')
				fprintf(stderr, "Must be no spaces around the number\n");
			else
				fprintf(stderr, "The number must have no more than 7 characters\n");

			parsedCorrect = 0;
			continue;
		}
		else
		{
			for (int j = 0; j < 2; j++)
			{
				float_buffer[floatChars] = buffer[i];
				floatChars++;
				i++;
			}
		}

		number = atof(float_buffer);

	} while (!parsedCorrect);

	return number;
}

int getIntegerValue(char *messageToPrint)
{
	char parsedCorrect;
	int number;
	do
	{
		parsedCorrect = 1;

		fflush(stdin);
		char buffer[INEGER_SIZE];

		printf("%s", messageToPrint);
		fgets(buffer, INEGER_SIZE, stdin);

		int i = 0;

		int length = strlen(buffer);

		if (!isdigit(buffer[i]) && buffer[i] != '-')
		{
			isspace(buffer[i]) ? fprintf(stderr, "Must be no spaces in the beginning\n")
									 : fprintf(stderr, "Must be a digit or minus in the beginning\n");
			parsedCorrect = 0;
			continue;
		}

		char integer_buffer[INEGER_SIZE];
		int integerChars = 0;

		if (buffer[i] == '-')
		{
			integer_buffer[integerChars] = buffer[i];
			integerChars++;
			i++;
			if (!isdigit(buffer[i]))
			{
				fprintf(stderr, "Must be a digit after a minus\n");
				parsedCorrect = 0;
				continue;
			}
		}

		for (int j = 0; j < 10; j++)
		{
			if (i == length || isspace(buffer[i]))
				break;
			else if (isdigit(buffer[i]))
			{
				integer_buffer[integerChars] = buffer[i];
				integerChars++;
				i++;
			}
			else
			{
				fprintf(stderr, "Character '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
				parsedCorrect = 0;
				break;
			}
		}
		if (!parsedCorrect)
			continue;

		// Тут потрібна перевірка саме на '\n', бо якщо ввести 10 цифр + пробіл + enter, то одинадцятим у масиві буде ' ', а isspace() вважає це за пробіл
		if (buffer[i] != '\n' || buffer[i + 1] != '\0')
		{
			if (isdigit(buffer[i]))
				fprintf(stderr, "The integer must be in range -2147483648 to 2147483647\n");
			else if (buffer[i] == ' ' || buffer[i + 1] == '\n')
				fprintf(stderr, "Must be no spaces around the number\n");

			parsedCorrect = 0;
			continue;
		}
		else if ((buffer[0] == '-' && buffer[10] == '9') || (isdigit(buffer[0]) && (buffer[9] == '8' || buffer[9] == '9')))
		{
			fprintf(stderr, "The integer must be in range -2147483648 to 2147483647\n");
			parsedCorrect = 0;
			continue;
		}
		{
			for (int j = 0; j < 2; j++)
			{
				integer_buffer[integerChars] = buffer[i];
				integerChars++;
				i++;
			}
		}

		number = atoi(integer_buffer);
	} while (!parsedCorrect);

	return number;
}

char *getNameValue(char *nameOfVariable)
{
	char parsedCorrect;
	char *string;
	string = calloc(NAME_SIZE, sizeof(char *));
	int i;
	do
	{
		parsedCorrect = 1;
		i = 0;

		fflush(stdin);

		printf("\nEnter the %s: ", nameOfVariable);
		fgets(string, NAME_SIZE, stdin);

		size_t length = strlen(string);

		if (isspace(string[i]))
		{
			fprintf(stderr, "\nMust be no spaces in the beginning\n");
			parsedCorrect = 0;
			continue;
		}
		else if (string[i] < 65 || string[i] > 90)
		{
			fprintf(stderr, "\nMust be a capital lattin letter in the beginning\n");
			parsedCorrect = 0;
			continue;
		}

		for (i; i < length; i++)
		{
			if (!isalpha(string[i]) && !isspace(string[i]))
			{
				fprintf(stderr, "\nCharacter '%c' is invalid (%d in the order)\n", string[i], i + 1);
				parsedCorrect = 0;
				break;
			}
		}

		if (!parsedCorrect)
			continue;

		if (string[i - 1] != '\n' || string[i] != '\0')
		{
			if (string[i - 1] == ' ' || string[i] == '\n')
				fprintf(stderr, "\nMust be no spaces around the name\n");
			else
				fprintf(stderr, "\nThe name must have no more than 40 characters\n");

			parsedCorrect = 0;
			continue;
		}

		if (!isspace(string[i - 2]))
			string[i - 1] = '\0';
		else
		{
			fprintf(stderr, "\nMust be no spaces in the end\n");
			parsedCorrect = 0;
		}

	} while (!parsedCorrect);
	return string;
}

char *getFileName(char *extension)
{
	char extLength = strlen(extension), parsedCorrect, *buffer;

	buffer = calloc(FILENAME_SIZE + extLength, sizeof(char));

	do
	{
		parsedCorrect = 1;

		fflush(stdin);

		printf("\nEnter the filename (.%s): ", extension);
		fgets(buffer, FILENAME_SIZE + extLength, stdin);

		int i = 0;

		if (!isalpha(buffer[i]) && !isdigit(buffer[i]))
		{
			isspace(buffer[i]) ? fprintf(stderr, "\nMust be no spaces in the beginning\n")
									 : fprintf(stderr, "\nMust be a lattin number or digit in the beginning\n", buffer[i], i + 1);
			parsedCorrect = 0;
			continue;
		}

		char name_buffer[FILENAME_SIZE];
		int nameChars = 0;

		int length = strlen(buffer) - 1;

		while (i != length && buffer[i] != '.')
		{
			name_buffer[nameChars] = buffer[i];
			nameChars++;
			i++;
		}

		if (i == length)
		{
			fprintf(stderr, "\nFilename is not available.\n", buffer[i], i + 1);
			parsedCorrect = 0;
			continue;
		}
		else
		{
			name_buffer[nameChars] = buffer[i];
			nameChars++;
			i++;
		}

		char ext_buffer[extLength + 1];
		int extChars = 0;

		while (length != i)
		{
			if (!isalpha(buffer[i]))
			{
				fprintf(stderr, "\nCharacter '%c' is invalid (%d in the order)\n", buffer[i], i + 1);
				parsedCorrect = 0;
				break;
			}
			else
			{
				ext_buffer[extChars] = buffer[i];
				extChars++;
				i++;
			}
		}
		ext_buffer[extChars] = '\0';

		if (!parsedCorrect)
			continue;
		else if (buffer[i] != '\n' || buffer[i + 1] != '\0')
		{
			if (buffer[i] == ' ' || buffer[i + 1] == '\n')
				fprintf(stderr, "\nMust be no spaces around the name\n");
			else
				fprintf(stderr, "\nThe maximum name length is 30 characters\n");

			parsedCorrect = 0;
			continue;
		}
		else if (strcmp(ext_buffer, extension) != 0)
		{
			fprintf(stderr, "\nThe file must have '%s' extension\n", extension);
			parsedCorrect = 0;
			continue;
		}

		buffer[i] = '\0';

	} while (!parsedCorrect);

	return buffer;
}
