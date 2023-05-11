#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parselib.h"

#define ENTER 13
#define YES 121
#define NO 110

typedef struct
{
	char region[20];
	double area;
	int citizens;
} rec;

int createFile(FILE *, char *);
int readRecords(FILE *, char *);
int writeRecords(FILE *, char *);
int removeRecords(FILE *, char *);
int deleteFile(FILE *, char *);
int editRecords(FILE *, char *);
int sortRecords(FILE *, char *);
int insertRecords(FILE *, char *);
int chooseAction();
int moreRecords(int *);
int setSignature(FILE *);
int readSignature(FILE *);
rec getNewRecord();
rec *sort(rec *, int, int, int);
int doOrNot(char *);

int main()
{
	int operation, trigger, button, (*operations[])(FILE *, char *) = {createFile, readRecords, writeRecords, removeRecords, deleteFile, editRecords, sortRecords, insertRecords};

	do
	{
		button = 0;
		char nameOfFile[31];
		FILE *fpointer;
		operation = chooseAction() - 1;
		strcpy(nameOfFile, getFileName("bin"));
		do
		{
			if (button)
				operation = chooseAction() - 1;

			trigger = operations[operation](fpointer, nameOfFile);
			if (!trigger)
				return 0;
			else if (trigger == 2)
				break;

			button = doOrNot("\nPress ENTER to continue working with the current file / ANY button to end.\n");
		} while (button == ENTER);
		button = doOrNot("\nPress ENTER to continue working with files / ANY button to leave the program.\n");
	} while (button == ENTER);

	return 0;
}

int createFile(FILE *fp, char *fileName)
{
	int getButton, ordered = 0;
	if ((fp = fopen(fileName, "r")))
	{
		fprintf(stderr, "\nThe file '%s' already exists. Replace it?\n", fileName);
		do
		{
			printf("\nPress 'y' for YES / 'n' for NOT\n");
			getButton = getch();
		} while (getButton != YES && getButton != NO);
	}
	if (getButton == NO)
		return 2;

	if ((fp = fopen(fileName, "w")) == NULL)
	{
		fprintf(stderr, "\nCannot write to the file\n");
		return 0;
	}

	if (!setSignature(fp))
		return 2;

	if (fwrite(&ordered, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nCannot write to the file\n");
		return 0;
	}

	fclose(fp);

	if (getButton == YES)
		fprintf(stderr, "\nFile was succesfully replaces\n");
	else
		fprintf(stderr, "\nFile was succesfully created\n");

	return 1;
}

int readRecords(FILE *fp, char *fileName)
{
	int total, ordered;
	rec *records;

	if ((fp = fopen(fileName, "r")) == NULL)
	{
		fprintf(stderr, "\nFile doesn't exist. Create it first.\n");
		return 2;
	}

	if (!readSignature(fp))
		return 0;

	if (fread(&ordered, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "Cannot read the file\n");
		return 0;
	}

	if (fread(&total, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nFile is empty.\n");
		return 1;
	}

	if ((records = calloc(total, sizeof(rec))) == NULL)
	{
		fprintf(stderr, "\nMemory wasn't allocated\n\n");
		return 0;
	}

	if (fread(records, sizeof(rec), total, fp) != total)
	{
		free(records);
		fprintf(stderr, "Cannot read the file\n");
		return 0;
	}

	fclose(fp);

	printf("\n");
	for (int i = 0; i < total; i++)
	{
		printf("%d. Region: %s", i + 1, records[i].region);
		printf(" Area: %lf", records[i].area);
		printf(" Citizens: %d\n", records[i].citizens);
		if ((i + 1) % 15 == 0 && doOrNot("\nENTER to scroll / ANY BUTTON to stop.\n") != ENTER)
			break;
	}

	free(records);

	if (ordered == 1)
		printf("\nThe records are ordered by the region in ascending\n");
	else if (ordered == 2)
		printf("\nThe records are ordered by the region in descending\n");
	else if (ordered == 3)
		printf("\nThe records are ordered by the area in ascending\n");
	else if (ordered == 4)
		printf("\nThe records are ordered by the area in descending\n");
	else if (ordered == 5)
		printf("\nThe records are ordered by the citizens in ascending\n");
	else if (ordered == 6)
		printf("\nThe records are ordered by the citizens in descending\n");

	return 1;
}

int writeRecords(FILE *fp, char *fileName)
{
	int button, total = 0, ordered;
	rec *records;

	if ((fp = fopen(fileName, "r")) == NULL)
	{
		fprintf(stderr, "\nFile doesn't exist. Create it first.\n\n");
		return 2;
	}

	if (!readSignature(fp))
		return 1;

	if (fread(&ordered, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nCannot read the file\n\n");
		return 0;
	}

	if (fread(&total, sizeof(int), 1, fp))
	{
		if ((records = calloc(total, sizeof(rec))) == NULL)
		{
			fprintf(stderr, "\nMemory wasn't allocated\n\n");
			return 0;
		}

		if (fread(records, sizeof(rec), total, fp) != total)
		{
			fprintf(stderr, "\nCannot read the previous records\n\n");
			return 0;
		}
	}
	else if ((records = calloc(1, sizeof(rec))) == NULL)
	{
		fprintf(stderr, "\nMemory wasn't allocated\n\n");
		return 0;
	}

	fclose(fp);

	do
	{
		total++;

		if ((records = realloc(records, total * sizeof(rec))) == NULL)
		{
			fprintf(stderr, "\nMemory wasn't reallocated\n\n");
			return 0;
		}

		records[total - 1] = getNewRecord();

		printf("\nWrite more records?\n");
		do
		{
			printf("Press 'y' for YES / 'n' for NOT\n");
			button = getch();
		} while (button != YES && button != NO);
	} while (button == YES);

	if ((fp = fopen(fileName, "w")) == NULL)
	{
		fprintf(stderr, "\nCannot write to the file\n\n");
		return 0;
	}

	if (!setSignature(fp))
		return 2;

	ordered = 0;

	if (fwrite(&ordered, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "Cannot write to the file\n");
		return 0;
	}

	if (fwrite(&total, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "Cannot write to the file\n");
		return 0;
	}

	if (fwrite(records, sizeof(rec), total, fp) != total)
	{
		free(records);
		fprintf(stderr, "Cannot write to the file\n");
		return 0;
	}

	fclose(fp);

	free(records);
	return 1;
}

int removeRecords(FILE *fp, char *fileName)
{
	int button, total, recordNo, currRecord = 0, ordered, correctRec;
	rec record;
	FILE *fp2;

	do
	{
		if ((fp = fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "\nFile doesn't exist. Create it first.\n");
			return 2;
		}

		if (!readSignature(fp))
			return 2;

		if (fread(&ordered, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot read the file\n");
			return 0;
		}

		if (fread(&total, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nFile is empty.\n");
			return 1;
		}

		if (!(recordNo = moreRecords(&total)))
			return 1;
		recordNo--;
		total--;

		if ((fp2 = fopen("copy.bin", "a")) == NULL)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (!setSignature(fp2))
			return 2;

		if (total < 2)
			ordered = 0;

		if (fwrite(&ordered, sizeof(int), 1, fp2) != 1)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (total)
		{
			if (fwrite(&total, sizeof(int), 1, fp2) != 1)
			{
				fprintf(stderr, "\nCannot write to the file\n");
				return 0;
			}
		}

		while (fread(&record, sizeof(rec), 1, fp))
		{
			if (currRecord != recordNo)
				fwrite(&record, sizeof(rec), 1, fp2);
			currRecord++;
		}

		_fcloseall();

		remove(fileName);
		rename("copy.bin", fileName);

		printf("\nRecord was succesfully deleted\n");
		if (total > 1)
		{
			printf("\nDelete more records?\n");
			do
			{
				printf("Press 'y' for YES / 'n' for NOT\n");
				button = getch();
			} while (button != YES && button != NO);
		}
		else
			button == NO;
	} while (button == YES);

	return 1;
}

int deleteFile(FILE *fp, char *fileName)
{
	if ((fp = fopen(fileName, "r")) == NULL)
	{
		fprintf(stderr, "File doesn't exist. Create it first.\n");
		return 2;
	}

	if (!readSignature(fp))
		return 2;

	fclose(fp);

	if (remove(fileName) == 0)
		printf("\nFile was successfully deleted.\n");
	else
	{
		fprintf(stderr, "\nFile cannot be deleted.\n");
		return 0;
	}

	return 1;
}

int editRecords(FILE *fp, char *fileName)
{
	int button, total, recordNo, choosedField, ordered, correctRec;
	char *name;
	rec *records;

	do
	{
		if ((fp = fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "\nFile doesn't exist. Create it first.\n");
			return 2;
		}

		if (!readSignature(fp))
			return 2;

		if (fread(&ordered, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot read the file\n");
			return 0;
		}

		if (fread(&total, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nFile is empty. Add records first.\n");
			return 1;
		}

		if ((records = calloc(total, sizeof(rec))) == NULL)
		{
			fprintf(stderr, "\nMemory wasn't allocated\n\n");
			return 0;
		}

		if (fread(records, sizeof(rec), total, fp) != total)
		{
			free(records);
			fprintf(stderr, "\nCannot read the file\n");
			return 0;
		}

		fclose(fp);

		if (!(recordNo = moreRecords(&total)))
			return 1;
		recordNo--;

		printf("\nEdit by:\n");
		printf("1. Region.\n");
		printf("2. Area.\n");
		printf("3. Citizens.\n");
		do
		{
			choosedField = getIntegerValue("\nChoose the field: ");
			if (choosedField != 1 && choosedField != 2 && choosedField != 3)
				printf("Enter the number from 1 to 3 to choose the field\n\n");
		} while (choosedField != 1 && choosedField != 2 && choosedField != 3);

		if (choosedField == 1)
		{
			name = getNameValue("region");
			strcpy(records[recordNo].region, name);
		}
		else if (choosedField == 2)
		{
			do
			{
				records[recordNo].area = getDoubleValue("area");

				if (records[recordNo].area < 0.5 || records[recordNo].area > 1e22)
				{
					fprintf(stderr, "\nThe number must be in range from 0.5 to 1e+22\n");
					records[recordNo].area = 0;
				}
			} while (!records[recordNo].area);
		}
		else
			records[recordNo].citizens = getIntegerValue("Enter the number of citizens: ");

		ordered = 0;

		if ((fp = fopen(fileName, "w")) == NULL)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (!setSignature(fp))
			return 2;

		if (fwrite(&ordered, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (fwrite(&total, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (fwrite(records, sizeof(rec), total, fp) != total)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		fclose(fp);

		free(records);

		printf("\nRecord was successfully edited\n");

		if (total > 1)
		{
			printf("\nEdit more records?\n");
			do
			{
				printf("Press 'y' for YES / 'n' for NOT\n");
				button = getch();
			} while (button != YES && button != NO);
		}
		else
			button == NO;
	} while (button == YES);

	return 1;
}

int sortRecords(FILE *fp, char *fileName)
{
	int total, recordNo, ordered, order, choosedField;
	char *name;
	rec *records;

	if ((fp = fopen(fileName, "r")) == NULL)
	{
		fprintf(stderr, "\nFile doesn't exist. Create it first.\n");
		return 2;
	}

	if (!readSignature(fp))
		return 0;

	if (fread(&ordered, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nCannot read the file\n");
		return 0;
	}

	if (fread(&total, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nFile is empty. Add records first.\n");
		return 1;
	}
	else if (total == 1)
	{
		fprintf(stderr, "\nFile has only one record. Add more to sort.\n");
		return 1;
	}

	if ((records = calloc(total, sizeof(rec))) == NULL)
	{
		fprintf(stderr, "\nMemory wasn't allocated\n\n");
		return 0;
	}

	if (fread(records, sizeof(rec), total, fp) != total)
	{
		free(records);
		fprintf(stderr, "\nCannot read the file\n");
		return 0;
	}

	fclose(fp);

	printf("\nSort order: \n\n");
	printf("1. Ascending.\n");
	printf("2. Descending.\n\n");
	do
	{
		order = getIntegerValue("Choose the order: ");
		if (order != 1 && order != 2)
			printf("Enter the number from 1 or 2 to choose the order\n\n");
	} while (order != 1 && order != 2);

	printf("Sort by:\n");
	printf("1. Region.\n");
	printf("2. Area.\n");
	printf("3. Citizens.\n");
	do
	{
		choosedField = getIntegerValue("Choose the field: ");
		if (choosedField != 1 && choosedField != 2 && choosedField != 3)
			printf("Enter the number from 1 to 3 to choose the field\n\n");
	} while (choosedField != 1 && choosedField != 2 && choosedField != 3);

	records = sort(records, total, order, choosedField);

	if (choosedField == 1 && order == 1)
		ordered = 1;
	else if (choosedField == 1 && order == 2)
		ordered = 2;
	else if (choosedField == 2 && order == 1)
		ordered = 3;
	else if (choosedField == 2 && order == 2)
		ordered = 4;
	else if (choosedField == 3 && order == 1)
		ordered = 5;
	else if (choosedField == 3 && order == 2)
		ordered = 6;

	if ((fp = fopen(fileName, "w")) == NULL)
	{
		fprintf(stderr, "\nCannot write to the file\n");
		return 0;
	}

	if (!setSignature(fp))
		return 2;

	if (fwrite(&ordered, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nCannot write to the file\n");
		return 0;
	}

	if (fwrite(&total, sizeof(int), 1, fp) != 1)
	{
		fprintf(stderr, "\nCannot write to the file\n");
		return 0;
	}

	if (fwrite(records, sizeof(rec), total, fp) != total)
	{
		fprintf(stderr, "\nCannot write to the file\n");
		return 0;
	}

	printf("\nRecords were succesfully sorted\n");

	fclose(fp);

	free(records);

	return 1;
}

int insertRecords(FILE *fp, char *fileName)
{
	int button, total, ordered, rowIndex, replace;
	rec record, *records;

	do
	{
		if ((fp = fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "\nFile doesn't exist. Create it first.\n");
			return 2;
		}

		if (!readSignature(fp))
			return 0;

		if (fread(&ordered, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot read the file\n\n");
			return 0;
		}
		else if (!ordered)
		{
			fprintf(stderr, "\nRecords aren't sorted or were edited. Sort them first.\n");
			return 1;
		}

		if (fread(&total, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nThe file is empty. Add records first.\n");
			return 1;
		}

		if ((records = calloc(total, sizeof(rec))) == NULL)
		{
			fprintf(stderr, "\nMemory wasn't allocated\n\n");
			return 0;
		}

		if (fread(records, sizeof(rec), total, fp) != total)
		{
			fprintf(stderr, "\nCannot read the previous records\n\n");
			return 0;
		}

		fclose(fp);

		record = getNewRecord();

		for (int i = 0; i < total; i++)
		{
			replace = (ordered == 1 && strcmp(records[i].region, record.region) > 0) ||
									(ordered == 2 && strcmp(records[i].region, record.region) < 0) ||
									(ordered == 3 && records[i].area > record.area) ||
									(ordered == 4 && records[i].area < record.area) ||
									(ordered == 5 && records[i].citizens > record.citizens) ||
									(ordered == 6 && records[i].citizens < record.citizens)
							  ? 1
							  : 0;

			if (replace)
			{
				rec temp = records[i];
				records[i] = record;
				record = temp;
				rowIndex = i + 1;
				break;
			}

			rowIndex = i + 1;
		}

		total++;

		if ((records = realloc(records, total * sizeof(rec))) == NULL)
		{
			fprintf(stderr, "\nMemory wasn't reallocated\n\n");
			return 0;
		}
		for (int i = rowIndex; i < total; i++)
		{
			rec temp = records[i];
			records[i] = record;
			record = temp;
		}

		if ((fp = fopen(fileName, "w")) == NULL)
		{
			fprintf(stderr, "\nCannot write to the file\n\n");
			return 0;
		}

		if (!setSignature(fp))
			return 2;

		if (fwrite(&ordered, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (fwrite(&total, sizeof(int), 1, fp) != 1)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		if (fwrite(records, sizeof(rec), total, fp) != total)
		{
			fprintf(stderr, "\nCannot write to the file\n");
			return 0;
		}

		free(records);

		fclose(fp);

		printf("\nThe record was succesfully inserted\n");
		printf("Insert more records?\n\n");
		do
		{
			printf("Press 'y' for YES / 'n' for NOT\n");
			button = getch();
		} while (button != YES && button != NO);
	} while (button == YES);
	return 1;
}

int chooseAction()
{
	int action;
	printf("\nAvailable operations:\n\n");
	printf("1. Create a file.\n");
	printf("2. Read records from the file.\n");
	printf("3. Write record(s) to the file (region, area and number of citizens).\n");
	printf("4. Remove file record(s).\n");
	printf("5. Delete the file.\n");
	printf("6. Edit file record(s).\n");
	printf("7. Sort file record(s).\n");
	printf("8. Insert record(s) into the ordered file.\n");
	do
	{
		// printf("Choose the operation: ");
		action = getIntegerValue("\nChoose the operation: ");
		if (action != 1 && action != 2 && action != 3 && action != 4 && action != 5 && action != 6 && action != 7 && action != 8)
			printf("Press from 1 to 8 to choose the operation\n\n");
	} while (action != 1 && action != 2 && action != 3 && action != 4 && action != 5 && action != 6 && action != 7 && action != 8);

	return action;
}

int moreRecords(int *total)
{
	int correctRec, recordNo, button;
	do
	{
		correctRec = 1;
		recordNo = getIntegerValue("\nEnter the record number: ");
		if (recordNo < 0)
		{
			fprintf(stderr, "The number must be > 0\n");
			correctRec = 0;
			continue;
		}
		else if (*total < recordNo)
		{
			if (*total == 0)
			{
				fprintf(stderr, "\nThe file is empty.\n");
				return 0;
			}
			else
			{
				fprintf(stderr, "\nThere are only %d records in the file. Choose another one?\n", *total);
				do
				{
					printf("Press 'y' for YES / 'n' for NOT\n");
					button = getch();
				} while (button != YES && button != NO);

				if (button == NO)
					recordNo = 0;

				correctRec = 0;
				continue;
			}
		}
	} while (!correctRec);

	return recordNo;
}

int setSignature(FILE *fp)
{
	char *signature = "Bardyk";

	if (fwrite(signature, sizeof(signature), 1, fp) != 1)
	{
		fprintf(stderr, "\nNo access to this file\n");
		return 1;
	}

	return 1;
}

int readSignature(FILE *fp)
{
	char *signature = calloc(7, sizeof(char));
	if (fread(signature, sizeof(signature), 1, fp) != 1)
	{
		fprintf(stderr, "\nNo access to this file\n");
		return 0;
	}

	if (strcmp(signature, "Bardyk") != 0)
	{
		fprintf(stderr, "\nNo access to this file\n");
		return 0;
	}

	return 1;
}

rec getNewRecord()
{
	int getButton;
	rec reg;
	char *name;

	do
	{
		name = getNameValue("region");
		strcpy(reg.region, name);
		do
		{
			reg.area = getDoubleValue("area");

			if (reg.area < 0.5 || reg.area > 1e22)
			{
				fprintf(stderr, "The number must be in range from 0.5 to 1e+22");
				reg.area = 0;
			}
		} while (!reg.area);

		reg.citizens = getIntegerValue("Enter the number of citizens: ");

	} while (getButton == YES);

	return reg;
}

rec *sort(rec *recArr, int rows, int order, int choosedField)
{
	int replace;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows - 1; j++)
		{
			replace = (order == 1 && choosedField == 1 && strcmp(recArr[j].region, recArr[j + 1].region) > 0) ||
									(order == 1 && choosedField == 2 && recArr[j].area > recArr[j + 1].area) ||
									(order == 1 && choosedField == 3 && recArr[j].citizens > recArr[j + 1].citizens) ||
									(order == 2 && choosedField == 1 && strcmp(recArr[j].region, recArr[j + 1].region) < 0) ||
									(order == 2 && choosedField == 2 && recArr[j].area < recArr[j + 1].area) ||
									(order == 2 && choosedField == 3 && recArr[j].citizens < recArr[j + 1].citizens)
							  ? 1
							  : 0;
			if (replace)
			{
				rec temp = recArr[j];
				recArr[j] = recArr[j + 1];
				recArr[j + 1] = temp;
			}
		}
	}

	return recArr;
}

int doOrNot(char *message)
{
	int fButton;
	printf("%s", message);
	fButton = getch();
	return fButton;
}