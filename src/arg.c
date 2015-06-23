/*
 * arg.c
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


#include "arg.h"

bool Debug = TRUE;
bool Tabs  = FALSE;

char OutputFilename[128] = "upBand";

double FermiEnergy = 0.0;


#define NUM_ARGS 3

static const ArgFunctions ArgCheck[NUM_ARGS] = 
{{"-tabs"        , &arg_tabs},
 {"-output-file" , &arg_output_file},
 {"-fermi-energy", &arg_fermi_energy}};

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

void arg_tabs(int index, int argc, char *argv[]) {
	
	Tabs = TRUE;
	
}

void arg_output_file(int index, int argc, char *argv[]) {

	if ((index + 1) == argc) {
	
		printf("Did not pass output filename");
		
	} else {
	
		strncpy(OutputFilename, argv[index + 1], 128);
		
	}
	
}

void arg_fermi_energy(int index, int argc, char *argv[]) {

	if ( (index + 1) == argc ) {
	
		printf("Did not pass fermi energy");
		
	} else {
		
		FermiEnergy = atof(argv[index + 1]);
		
		if (FermiEnergy == 0.0) {
		
			printf("Did not pass a good value for the Fermi energy");
			
		}
		
	}
	
}
