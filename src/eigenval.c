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
#include <malloc.h>
#include <string.h>

#include "arg.h"

void initialize();
void ReadData(int iteration);
void Transpose(int iteration);

static char *Filename 			= "EIGENVAL";

static float *FirstColumn;
static float *SecondColumn;

static int NumberOfFields   = 0;
static int NumberOfSections = 0;

static FILE *fp;

int main(int argc, char *argv[]) {

	initialize();
	ParseArgs(argc, argv);

	int x;
	for (x = 0;x < NumberOfSections;x++) {
	
		ReadData((x + 1));
		Transpose((x + 1));
	
	}

	free(FirstColumn);
	free(SecondColumn);

	fclose(fp);

	return 0;
}

void initialize() {

	fp = fopen(Filename, "r");

	char Buffer[256];

	int x;
	for (x = 0;x < 5;x++)
		fgets(Buffer, 256, fp);
	fscanf(fp, "%*d%d%d\n", &NumberOfSections, &NumberOfFields);	

	if (Debug == TRUE)
		printf("NumberOfSections = %d, NumberOfFields = %d\n", NumberOfSections, NumberOfFields);	

	FirstColumn  = malloc(NumberOfFields * sizeof(float));
	SecondColumn = malloc(NumberOfFields * sizeof(float)); 

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

void Transpose(int iteration) {

	FILE *upBand = fopen(OutputFilename, "a");
	
	if (Tabs == FALSE) {
		
		fprintf(upBand, "%d,", iteration);
		
	} else {
	
		fprintf(upBand, "%d\t", iteration);
		
	}
	
	int x;
	for (x = 0;x < NumberOfFields;x++) {

		if (Tabs == FALSE) {

			if ((x + 1) == NumberOfFields)
				fprintf(upBand, "%.4f", FirstColumn[x]);
			else
				fprintf(upBand, "%.4f,", FirstColumn[x]);
		
		} else {
		
			if ((x + 1) == NumberOfFields)
				fprintf(upBand, "%.4f", FirstColumn[x]);
			else
				fprintf(upBand, "%.4f\t", FirstColumn[x]);
			
		}

	}
	fprintf(upBand, "\n");


	fclose(upBand);

}

