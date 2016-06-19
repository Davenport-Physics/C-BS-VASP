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
#include <unistd.h>

#include "arg.h"

static char *SOFTWARE_VERSION = ".25";
static char *Filename         = "EIGENVAL";

static float *SpinUpColumn;
static float *SpinDownColumn;

static int NumberOfBands = 0;
static int NumberOfGrids = 0;

static FILE *fp;

static void initialize(int argc, char *argv[]);
static void check_for_existing_files();
static void delete_existing_file(char File[MIN_BUFFER_LENGTH]);
static void ReadData(int iteration);
static void Transpose(int iteration);

int main(int argc, char *argv[]) {

	printf("C-BS-VASP Version %s\n", SOFTWARE_VERSION);
	printf("Make sure that you are performing spin polarization calculations. i.e ISPIN=2\n");

	initialize(argc, argv);

	int x;
	for (x = 0;x < NumberOfGrids;x++) {

		ReadData((x + 1));
		Transpose((x + 1));

	}

	free(SpinUpColumn);
	free(SpinDownColumn);

	fclose(fp);

	return 0;
}

static char *RegularFloatString = "%.4f";
static char *TabsFloatString    = "%.4f\t";
static char *CommaFloatString   = "%.4f,";
static char *FloatString;

static char *TabsIntString  = "%d\t";
static char *CommaIntString = "%d,";
static char *IntString;

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

	SpinUpColumn  = malloc(NumberOfBands * sizeof(float));
	SpinDownColumn = malloc(NumberOfBands * sizeof(float));
	
	if (Tabs == TRUE) {
	
		FloatString = TabsFloatString;
		IntString   = TabsIntString;
		
	} else {
		
		FloatString = CommaFloatString;
		IntString   = CommaIntString;
		
	}

}
void check_for_existing_files() {

	FILE *localfp = fopen(SpinUp, "r");
	
	if (localfp != NULL) {
	
		fclose(localfp);
		delete_existing_file(SpinUp);
		
	}
	
	localfp = fopen(SpinDown, "r");
	if (localfp != NULL) {
		
		fclose(localfp);
		delete_existing_file(SpinDown);
		
	}
	
}

void delete_existing_file(char File[MIN_BUFFER_LENGTH]) {
	
	int code = unlink(File);
	
	if (code == 0) {
	
		printf("Deleted old %s successfully.\n", File);
		
	} else if (code == 1) {
	
		printf("There was an error deleting file %s.\n", File);
		printf("Please delete the file manually.\n");
		
	}
	
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

		fscanf(fp, "%*d%f%f", &SpinUpColumn[x], &SpinDownColumn[x]);

		if (FermiEnergy != 0.0) {

			SpinUpColumn[x]  -= FermiEnergy;
			SpinDownColumn[x] -= FermiEnergy;

		}

	}

}

void Transpose(int iteration) {

	FILE *SpinUpBand   = fopen(SpinUp, "a");
	FILE *SpinDownBand = fopen(SpinDown, "a");

	fprintf(SpinUpBand, IntString, iteration);
	fprintf(SpinDownBand, IntString, iteration);

	int x;
	for (x = 0;x < NumberOfBands;x++) {

		if ((x + 1) == NumberOfBands) {
				
			fprintf(SpinUpBand, RegularFloatString, SpinUpColumn[x]);
			fprintf(SpinDownBand, RegularFloatString, SpinDownColumn[x]);
				
		} else {
				
			fprintf(SpinUpBand, FloatString, SpinUpColumn[x]);
			fprintf(SpinDownBand, FloatString, SpinDownColumn[x]);
				
		}

	}
	fprintf(SpinUpBand, "\n");
	fprintf(SpinDownBand, "\n");


	fclose(SpinUpBand);
	fclose(SpinDownBand);

}
