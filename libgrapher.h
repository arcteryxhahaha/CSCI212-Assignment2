/* Name: Gabriel Beedles (4197665)
* Assignment: 2
* Filename: libgrapher.h
* Date Modified: 26/4/2015
*
* Purpose: Contains the declaration for a function to print scheduling output
* to a png file
*
*/

#include <gd.h>
#include <gdfontl.h>
#include <gdfonts.h>
#include <gdfontt.h>
#include <gdfontg.h>
#include <gdfontmb.h>
#include <stdio.h>
#include <stdlib.h>

void outputImage(int timeLine[], int procNum, char filename[128]);
