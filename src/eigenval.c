/*
 * eigenval.c
 *
 * Copyright 2015 Michael Davenport <Davenport.physics@gmail.com>
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

#define SOFTWARE_VERSION .2102

void initialize();
void ReadData(int iteration);
void SetData(int iteration);
void Transpose(int iteration);
void WriteDFS();

static char *Filename = "EIGENVAL";

static float *FirstColumn;
static float *SecondColumn;

static float **AllSpinUp;
static float **AllSpinDown;

static int NumberOfFields   = 0;
static int NumberOfSections = 0;

static FILE *fp;

int main(int argc, char *argv[]) {

	printf("C-BS-VASP Version %g\n", SOFTWARE_VERSION);
	printf("Make sure that you are performing spin polarization calculations. i.e ISPIN=2\n");

	ParseArgs(argc, argv);
	initialize();

	int x;
	for (x = 0;x < NumberOfSections;x++) {

		if (Dfs == FALSE) {
			
			ReadData((x + 1));
			Transpose((x + 1));
			
		} else {
			
			ReadData((x + 1));
			SetData(x);
		}

	}
	if (Dfs == TRUE) {
	
		WriteDFS();
		
	}

	free(FirstColumn);
	free(SecondColumn);
	free(AllSpinUp);
	free(AllSpinDown);

	fclose(fp);

	return 0;
}

void initialize() {

	fp = fopen(Filename, "r");

	if (fp == NULL) {

		printf("%s file not found\n", Filename);
		exit(EXIT_FAILURE);

	}

	char Buffer[256];

	int x;
	for (x = 0;x < 5;x++)
		fgets(Buffer, 256, fp);
	fscanf(fp, "%*d%d%d\n", &NumberOfSections, &NumberOfFields);

	if (Debug == TRUE)
		printf("Number of KPOINTS = %d, Number Of Bands = %d\n", NumberOfSections, NumberOfFields);

	FirstColumn  = malloc(NumberOfFields * sizeof(float));
	SecondColumn = malloc(NumberOfFields * sizeof(float));
	
	if (Dfs == TRUE) {
	
		AllSpinUp   = malloc(NumberOfSections * sizeof(float *));
		AllSpinDown = malloc(NumberOfSections * sizeof(float *));	
		
		int i;
		for (i = 0;i < NumberOfSections;i++) {
		
			AllSpinUp[i]   = malloc(NumberOfFields * sizeof(float));
			AllSpinDown[i] = malloc(NumberOfFields * sizeof(float));

		}
		
	}

}

void ReadData(int iteration) {

	char Buffer[256];

	/*
	 * This conditional statement fixes a bug that I do not want
	 * to spend time diagnosing properly.
	 *
	 * */
	if (iteration == 1) {

		fgets(Buffer, 256, fp);

	} else {

		fgets(Buffer, 256, fp);
		fgets(Buffer, 256, fp);
		fgets(Buffer, 256, fp);

	}

	int x;
	for (x = 0;x < NumberOfFields;x++) {

		fscanf(fp, "%*d%f%f", &FirstColumn[x], &SecondColumn[x]);

		if (FermiEnergy != 0.0) {

			FirstColumn[x]  -= FermiEnergy;
			SecondColumn[x] -= FermiEnergy;

		}

	}

}

void SetData(int iteration) {

	int i;
	for (i = 0;i < NumberOfFields;i++) {
	
		AllSpinUp[iteration][i]   = FirstColumn[i];
		AllSpinDown[iteration][i] = SecondColumn[i];
		
	}
	
}

void Transpose(int iteration) {

	FILE *upBand = fopen(OutputFilename, "a");
	FILE *downBand = fopen(OutputFilenameSpinDown, "a");

	if (Tabs == FALSE) {

		fprintf(upBand, "%d,", iteration);
		fprintf(downBand, "%d,", iteration);

	} else {

		fprintf(upBand, "%d\t", iteration);
		fprintf(downBand, "%d\t", iteration);

	}

	int x;
	for (x = 0;x < NumberOfFields;x++) {

		if (Tabs == FALSE) {

			if ((x + 1) == NumberOfFields) {
				
				fprintf(upBand, "%.4f", FirstColumn[x]);
				fprintf(downBand, "%.4f", SecondColumn[x]);
				
			} else {
				
				fprintf(upBand, "%.4f,", FirstColumn[x]);
				fprintf(downBand, "%.4f,", SecondColumn[x]);
				
			}

		} else {

			if ((x + 1) == NumberOfFields) {
				
				fprintf(upBand, "%.4f", FirstColumn[x]);
				fprintf(downBand, "%.4f", SecondColumn[x]);
				
			} else {
				
				fprintf(upBand, "%.4f\t", FirstColumn[x]);
				fprintf(downBand, "%.4f\t", SecondColumn[x]);
				
			}

		}

	}
	fprintf(upBand, "\n");
	fprintf(downBand,"\n");


	fclose(upBand);
	fclose(downBand);

}

void WriteDFS() {
	
	FILE *upBand = fopen(OutputFilename, "w");
	FILE *downBand = fopen(OutputFilenameSpinDown, "w");
	
	int i;
	for (i = 0; i < NumberOfSections;i++) {
		
		if (Tabs == FALSE) {

			fprintf(upBand, "%d,", (i+1));
			fprintf(downBand, "%d,", (i+1));

		} else {

			fprintf(upBand, "%d\t", (i+1));
			fprintf(downBand, "%d\t", (i+1));

		}
		
	}
	fprintf(upBand, "\n");
	fprintf(downBand,"\n");
	
	int b;
	for (b = 0;b < NumberOfFields;b++) {
		
		for (i = 0;i < NumberOfSections;i++) {
		
			if (Tabs == FALSE) {
			
				continue;
				
			} else {
			
				fprintf(upBand, "%.4f\t", AllSpinUp[i][b]);
				fprintf(downBand,"%.4f\t", AllSpinDown[i][b]);
				
			}
			
		}
		fprintf(upBand,"\n");
		fprintf(downBand,"\n");
		
	}
	
	
	fclose(upBand);
	fclose(downBand);
		
}
