/*
 * arg.h
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

#ifndef ARG_H
#define ARG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BUFFER_LENGTH 256

typedef enum {

	FALSE = 0,
	TRUE

} bool;

typedef struct {
	
	char *string;
	void (*function)(int index,int argc, char *argv[]);
	
} ArgFunctions;


extern bool Debug;
extern bool Tabs;

extern char SpinUp[MIN_BUFFER_LENGTH];
extern char SpinDown[MIN_BUFFER_LENGTH];

extern double FermiEnergy;

/*
 * Arg parsing function
 * 
 * */
void ParseArgs(int argc, char *argv[]); 

#endif

