/*
 * arg.c
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


#include "arg.h"

#define NUM_ARGS 8

bool Debug = FALSE;
bool Tabs  = FALSE;

char SpinUp[MIN_BUFFER_LENGTH]   = "SpinUp";
char SpinDown[MIN_BUFFER_LENGTH] = "SpinDown";

double FermiEnergy = 0.0;

static void arg_all(int index, int argc, char *argv[]);
static void arg_tabs(int index, int argc, char *argv[]);
static void arg_debug(int index, int argc, char *argv[]);
static void arg_output_file(int index, int argc, char *argv[]);
static void arg_fermi_energy(int index, int argc, char *argv[]);
static void arg_auto_fermi_energy(int index, int argc, char *argv[]);


static const ArgFunctions ArgCheck[NUM_ARGS] = 
{{"-all"         , &arg_all},
 {"-debug"       , &arg_debug},
 {"-tabs"        , &arg_tabs},
 {"-output-file" , &arg_output_file},
 {"-o"           , &arg_output_file},
 {"-fermi-energy", &arg_fermi_energy},
 {"-fe"          , &arg_fermi_energy},
 {"-auto"        , &arg_auto_fermi_energy}};

void ParseArgs(int argc, char *argv[]) {

	int x, y;
	for (x = 1;x < argc;x++) {
	
		for (y = 0;y < NUM_ARGS;y++) {
		
			if (strcasecmp(argv[x], ArgCheck[y].string) == 0) {
			
				ArgCheck[y].function(x, argc, argv);
				
			}
			
		}
		
	}
	
}

void arg_all(int index, int argc, char *argv[]) {

	Tabs  = TRUE;
	Debug = TRUE;
	
	arg_auto_fermi_energy(index, argc, argv);
	
}

void arg_tabs(int index, int argc, char *argv[]) {
	
	Tabs = TRUE;
	
}

void arg_debug(int index, int argc, char *argv[]) {

	Debug = TRUE;
	
}

void arg_output_file(int index, int argc, char *argv[]) {

	if ((index + 1) == argc) {
	
		printf("Did not pass output filename\n");
		exit(EXIT_FAILURE);
		
	} else {
	
		strncpy(SpinUp, argv[index + 1], MIN_BUFFER_LENGTH);
		strncat(SpinUp, "-Up", 3);
		
		strncpy(SpinDown, argv[index + 1], MIN_BUFFER_LENGTH);
		strncat(SpinUp, "-Down", 5);
		
	}
	
}

void arg_fermi_energy(int index, int argc, char *argv[]) {

	if ( (index + 1) == argc ) {
	
		printf("Did not pass fermi energy\n");
		exit(EXIT_FAILURE);
		
	} else {
		
		FermiEnergy = atof(argv[index + 1]);
		
		if (FermiEnergy == 0.0) {
		
			printf("Did not pass a good value for the Fermi energy");
			
		} else {
			
			if (Debug == TRUE) {
			
				printf("Fermi energy given = %f\n", FermiEnergy);
				
			}
			
		}
		
	}
	
}

void arg_auto_fermi_energy(int index, int argc, char *argv[]) {

	char buffer[512];

	FILE *fp = fopen("OUTCAR", "r");

	if (fp == NULL) {

		printf("No file OUTCAR found. Halting Execution \n");
		exit(0);

	}

	while (fgets(buffer, 512, fp) != NULL) {

		if (strncmp(buffer, " E-fermi :", 10) == 0) {

			char temp[6];

			temp[0] = buffer[13];
			temp[1] = buffer[14];
			temp[2] = buffer[15];
			temp[3] = buffer[16];
			temp[4] = buffer[17];
			temp[5] = buffer[18];

			FermiEnergy = atof(temp);

			if (Debug == TRUE) {

				printf("Extracted Fermi energy = %g\n", FermiEnergy);

			}

			if (FermiEnergy == 0.0) {

				printf("Failed to extract Fermi energy from OUTCAR\n");

			}

			break;
		}

	}

	fclose(fp);
}
