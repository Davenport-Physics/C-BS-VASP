/*
 * eigenval.c
 *
 * Copyright 2015-2016 Emma Davenport <Davenport.physics@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "arg.h"

static char *SOFTWARE_VERSION = ".241";

void initialize(int argc, char *argv[]);
void check_for_existing_files();
void delete_existing_file(char File[MIN_BUFFER_LENGTH]);
void ReadData(int iteration);
void Transpose(int iteration);

static char *Filename = "EIGENVAL";

static float *FirstColumn;
static float *SecondColumn;

static int NumberOfBands = 0;
static int NumberOfGrids = 0;

static FILE *fp;

int main(int argc, char *argv[]) {

	printf("C-BS-VASP Version %s\n", SOFTWARE_VERSION);
	printf("Make sure that you are performing spin polarization calculations. i.e ISPIN=2\n");

	initialize(argc, argv);

	int x;
	for (x = 0;x < NumberOfGrids;x++) {

		ReadData((x + 1));
		Transpose((x + 1));

	}

	free(FirstColumn);
	free(SecondColumn);

	fclose(fp);

	return 0;
}

void initialize(int argc, char *argv[]) {

	ParseArgs(argc, argv);
	
	fp = fopen(Filename, "r");

	if (fp == NULL) {

		printf("%s file not found\n", Filename);
		exit(EXIT_FAILURE);

	}
	
	check_for_existing_files();

	char Buffer[MIN_BUFFER_LENGTH];

	int x;
	for (x = 0;x < 5;x++)
		fgets(Buffer, MIN_BUFFER_LENGTH, fp);
	fscanf(fp, "%*d%d%d\n", &NumberOfGrids, &NumberOfBands);

	if (Debug == TRUE)
		printf("Number of grids (Related to KPOINTS) = %d, Number of Bands = %d\n", NumberOfGrids, NumberOfBands);

	FirstColumn  = malloc(NumberOfBands * sizeof(float));
	SecondColumn = malloc(NumberOfBands * sizeof(float));

}
void check_for_existing_files() {

	FILE *fp = fopen(SpinUp, "r");
	
	if (fp != NULL) {
	
		fclose(fp);
		delete_existing_file(SpinUp);
		
	}
	
	fp = fopen(SpinDown, "r");
	if (fp != NULL) {
		
		fclose(fp);
		delete_existing_file(SpinDown);
		
	}
	
}

void delete_existing_file(char File[MIN_BUFFER_LENGTH]) {
	
	printf("Deleting prexisting %s\n", File);
	char buffer[512] = {'\0'};
	strncat(buffer, "rm ", 3);
	strncat(buffer, File, MIN_BUFFER_LENGTH);
	FILE *rm = popen(buffer, "r");
	pclose(rm);
	
}

void ReadData(int iteration) {

	char Buffer[MIN_BUFFER_LENGTH];

	/*
	 * This conditional statement fixes a bug that I do not want
	 * to spend time diagnosing properly.
	 *
	 * */
	if (iteration == 1) {

		fgets(Buffer, MIN_BUFFER_LENGTH, fp);

	} else {

		fgets(Buffer, MIN_BUFFER_LENGTH, fp);
		fgets(Buffer, MIN_BUFFER_LENGTH, fp);
		fgets(Buffer, MIN_BUFFER_LENGTH, fp);

	}

	int x;
	for (x = 0;x < NumberOfBands;x++) {

		fscanf(fp, "%*d%f%f", &FirstColumn[x], &SecondColumn[x]);

		if (FermiEnergy != 0.0) {

			FirstColumn[x]  -= FermiEnergy;
			SecondColumn[x] -= FermiEnergy;

		}

	}

}

void Transpose(int iteration) {

	FILE *upBand   = fopen(SpinUp, "a");
	FILE *downBand = fopen(SpinDown, "a");

	if (Tabs == FALSE) {

		fprintf(upBand, "%d,", iteration);
		fprintf(downBand, "%d,", iteration);

	} else {

		fprintf(upBand, "%d\t", iteration);
		fprintf(downBand, "%d\t", iteration);

	}

	int x;
	for (x = 0;x < NumberOfBands;x++) {

		if (Tabs == FALSE) {

			if ((x + 1) == NumberOfBands) {
				
				fprintf(upBand, "%.4f", FirstColumn[x]);
				fprintf(downBand, "%.4f", SecondColumn[x]);
				
			} else {
				
				fprintf(upBand, "%.4f,", FirstColumn[x]);
				fprintf(downBand, "%.4f,", SecondColumn[x]);
				
			}

		} else {

			if ((x + 1) == NumberOfBands) {
				
				fprintf(upBand, "%.4f", FirstColumn[x]);
				fprintf(downBand, "%.4f", SecondColumn[x]);
				
			} else {
				
				fprintf(upBand, "%.4f\t", FirstColumn[x]);
				fprintf(downBand, "%.4f\t", SecondColumn[x]);
				
			}

		}

	}
	fprintf(upBand, "\n");
	fprintf(downBand, "\n");


	fclose(upBand);
	fclose(downBand);

}
