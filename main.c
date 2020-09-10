#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "bmplib.h"

/* Author: Joseph Pietroluongo
// PID: 5901749
// I affirm that I wrote this program myself without any help
// from any other people or sources from the internet
// Summary: This program is designed in C language, for the purpose to
// rotate an image by 90 degrees
*/

#define TRUE = 1
/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
	    PIXEL** new, int* newrows, int* newcols)
{
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */

    *newcols = cols * scale;
    *newrows = rows * scale;


    *new = (PIXEL*)malloc( (rows * cols) * (scale * scale) * sizeof(int));
    if(!*new)
    {
        free(*new);
        fprintf(stderr, "Could not allocate memory.\n");
        return 1;
    }

    int iRows,jCol,iRowScale,jColScale;

    for(iRows = 0; iRows < *newrows; iRows++)
    {
        for(jCol = 0; jCol < *newcols; jCol++)
        {
            for(iRowScale = 0; iRowScale < scale; iRowScale++)
            {
                int posN = ((iRows+scale) * (*newcols) + jCol);
                int posO = ((iRows/scale)*((*newcols)/scale) + (jCol/scale));
                *((*new) + posN) = *(original + posO);

                for(jColScale = 0; jColScale < scale; jColScale++)
                    *((*new) + (posN + jColScale)) = *(original + posO);
            }
        }
    }

    return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
  
    *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));
    int row, col;
    if(rotation == 90 || rotation == -270)
    {
        for(row = 0; row < rows; row++)
        {
            for(col = 0; col < cols; col++)
            {
                PIXEL* o = original + (row * cols) + col;
                PIXEL* n = (*new) + ((cols - col - 1) * rows) + row;
                *n = *o;
                *newcols = cols;
                *newrows = rows;
            }
        }
    }

    else if(rotation == -90 || rotation == 270)
    {
        for(row = 0; row < rows; row++){
            for(col = 0; col < cols; col++ )
            {
                PIXEL* o = original+ (row*cols) + col;
                PIXEL* n = (*new)+(col*rows) + (rows-row-1);
                *n = *o;
                *newcols = rows;
                *newrows = cols;
            }
        }
    }

    else if(rotation == 180 || rotation == -180)
    {
        for(row = 0; row < rows; row++){
            for(col = 0; col < cols; col++ )
            {
                PIXEL* o = original+ (row*cols) + col;
                PIXEL* n = (*new)+((rows-row-1)*cols) + (cols-col-1);
                *n = *o;
                *newcols = cols;
                *newrows = rows;
            }
        }
    }

    else
    {
        fprintf(stderr,"Please only enter degrees in multiples of 90.\n");
    }
  return 0;

}

/*
 * This method vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalFlip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

    for(row = 0; row < rows; row++)
    {
        for(col = 0; col < cols; col++ )
        {
            PIXEL* o = original + (row * cols) + col;
            PIXEL* n = (*new) + ((rows - row - 1) * cols) + (cols - col - 1);
            *n = *o;  
        }
    }

  return 0;
}

/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) 
    {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

/*
* This function was created in order to help process all options seleced from user.
*/
void mainHelper(int scale, int degree, int flipp, int verticalFlipp, char *out, char *in)
{
    int row, col, functionCount=0;
    PIXEL *bmp, *nbmp;

    readFile(in, &row, &col, &bmp);

    if(scale)
    {
        functionCount++;
        enlarge(bmp, row, col, scale, &nbmp, &row, &col);
    }
    if(degree)
    {
        functionCount++;
        if(functionCount > 1)
            bmp = nbmp;
        rotate(bmp, row, col, degree, &nbmp, &row, &col);
    }
    if(flipp)
    {
        functionCount++;
        if(functionCount > 1)
            bmp = nbmp;
        flip(bmp, &nbmp, row, col);
    }
    if(verticalFlipp)
    {
        functionCount++;
        if(functionCount > 1)
            bmp = nbmp;
        verticalFlip(bmp, &nbmp, row, col);
    }

    writeFile(out, row, col, nbmp);

    free(bmp);
    free(nbmp);
}


int main(int argc, char **argv)
{
    
    int get, option[5]={0};
    extern char *optarg;
    extern int optind;
    char *in = NULL;
    char *out = NULL;

// given usage in PDF
    static char usage[] = "\nUsage: bmptool [-s scale | -r degree | -f | -v] [-o output_file] [input_file]\n";

    while((get = getopt(argc, argv, "s:r:fvo:")) != -1)
        switch(get)
        {
            case 's':
                if(option[0] >= 2)
                {
                    printf("Too many s flags entered\n");
                    exit(1);
                }
                if(sscanf(optarg, " %d", &option[0]) != 1)
                {
                    printf("Please enter a number\n");
                    exit(1);
                }
                break;
            case 'r':
                if(option[1] >= 2 || option[1] < 0)
                {
                    printf("Too many r flags entered\n");
                    exit(1);
                }
                if(sscanf(optarg, " %d", &option[1]) != 1)
                {
                    printf("Please enter a number\n");
                    exit(1);
                }
                if((option[1] % 90) != 0 || option[1] == 0 )
                {
                    printf("Please enter a number that is a multiple of 90");
                    exit(1);
                }
                break;
            case 'f':
                option[2]++;
                if(option[2] >= 2)
                {
                    printf("Too many f flags entered\n");
                    exit(1);
                }
                break;
                case 'v':
                option[3]++;
                if(option[3] >= 2)
                {
                    printf("Too many f flags entered\n");
                    exit(1);
                }
                break;
            case 'o':
                option[4]++;
                if(option[4] >= 2)
                {
                    printf("Too many o flags entered\n");
                    exit(1);
                }
                out = optarg;
                break;
            default:
                fprintf(stderr, usage);
                exit(1);
        }

    if(argv[optind])
        in = argv[optind];

    mainHelper(option[0], option[1], option[2], option[3], out, in);

    return 0;
}
