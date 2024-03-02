//=================================================================
// Copyright 2024 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

/*
Please fill in the following
 Student Name: Joshua Maxi
 Date: 2/16/2024

ECE 2035 Project 1-2

This is the only file that should be modified for the C implementation
of Project 1.

Do not include any additional libraries.
-----------------------------------------------
          Top of Pile

This program loads a pile of overlapping parts, each part having a different
color, and finds the color of the topmost part. */

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0 // RESET THIS TO 0 BEFORE SUBMITTING YOUR CODE

int  Load_Mem(char *, int *);

int main(int argc, char *argv[]) {
   int	             PileInts[1024];
   int	             NumInts, TopColor=0;
   // This allows you to access the pixels (individual bytes)
   // as byte array accesses (e.g., Pile[25] gives pixel 25):
   char *Pile = (char *)PileInts;

   if (argc != 2) {
     printf("usage: ./P1-1 valuefile\n");
     exit(1);
   }
   NumInts = Load_Mem(argv[1], PileInts);
   if (NumInts != 1024) {
      printf("valuefiles must contain 1024 entries\n");
      exit(1);
   }

   if (DEBUG){
     printf("Pile[0] is Pixel 0: 0x%02x\n", Pile[0]);
     printf("Pile[107] is Pixel 107: 0x%02x\n", Pile[107]);

     printf("PileInts[26] packs 4 Pixels: 0x%08x\n", PileInts[26]);
     printf("Pile[26*4] is Pixel 104: 0x%02x\n", Pile[104]);
     printf("Pile[26*4+1] is Pixel 105: 0x%02x\n", Pile[105]);
     printf("Pile[26*4+2] is Pixel 106: 0x%02x\n", Pile[106]);
     printf("Pile[26*4+3] is Pixel 107: 0x%02x\n", Pile[107]);
   }

   /* Your program goes here */

   // Variables
   // -------------------------------------------------------------------------------------------
   
   // Intializing an array named Wrong to include all colors not at the top
   int Wrong[7];

   // Main Body
   // -------------------------------------------------------------------------------------------
   
   // A for loop that goes through all 4096 pixels to determine the current colors
   for (int i = 0; i < 4096; i++) {

      // When the top color is non zero the for loop ends
      if (TopColor != 0) {
         // printf("%d: Top color does not equal 0\n", i);
         break;
      }

      // When there is black space the current i is instantly skipped
      else if (Pile[i] == 0) {
         // printf("%d: black space skipped\n", i);
         continue;
      }

      // A for loop to look through the Wrong array and determine if the current Pile[i] is in the Wrong array
      int skip = 0;
      for (int w = 0; w < 7; w++) {
         if (Pile[i] == Wrong[w]) {
            skip = 1;
            break;
         }
      }

      // If Pile[i] is in the Wrong array then this current i is skipped
      if (skip == 1) {
         // printf("%d: Pile from wrong ignored\n", i);
         continue;
      }
      
      // If the Pixel to the right of Pile[i] is the same, then the for loop runs through the entire width of the 64x64 area to determine if there are any overlapping sections for the current Pile[i]
      // If the next Pixel is the same then continue, if the next is black then break, if the next is a different color and the color after is the original then add Pile[i] to the Wrong array and break
      else if (Pile[i] == Pile[i+1]) {
         for (int j = 0; j < 64; j++) {
            if (Pile[i+j] == Pile[i+j+1]) {
               // printf("%d: Continue from if1\n", i);
               continue;
            }
            else if (Pile[i+j+1] == 0) {
               // printf("%d: break from elseif1\n", i);
               break;
            }
            else if ((Pile[i+j+1] != Pile[i+j]) && (Pile[i+j] == Pile[i+j+2])) {
               Wrong[Pile[i]-1] = Pile[i];
               // printf("%d\n", Wrong[Pile[i]-1]);
               // printf("%d: break from elseif2\n", i);
               break;
            }
         }
      }

      // If the Pixel to the below of Pile[i] is the same, then the for loop runs through the entire height of the 64x64 area to determine if there are any overlapping sections for the current Pile[i]
      // If the next Pixel is the same then continue, if the next is black then break, if the next is a different color and the color after is the original then add Pile[i] to the Wrong array and break
      else if (Pile[i] == Pile[i+64]) {
         for (int j = 0; j < 64; j++) {
            if (Pile[i+(j*64)] == Pile[i+((j+1)*64)]) {
               // printf("%d: Continue from if2\n", i);
               continue;
            }
            else if (Pile[i+((j+1)*64)] == 0) {
               // printf("%d: break from elseif3\n", i);
               break;
            }
            else if((Pile[i+((j+1)*64)] != Pile[i+(j*64)]) && (Pile[i+(j*64)] == Pile[i+((j+2)*64)])) {
               Wrong[Pile[i]-1] = Pile[i];
               // printf("%d\n", Wrong[Pile[i]-1]);
               // printf("%d: break from elseif4\n", i);
               break;
            }
         }
      }
   }

   // The for loop goes through the Wrong with a new intialized variable to see if Wrong[q] is equal to 0 to determine which color is not represented in wrong
   // When Wrong is equal to zero it is known that q+1 is the number that represents the topmost part color
   for (int q = 0; q < 7; q++) {
      // printf("%d\n", Wrong[q]);
      if (Wrong[q] == 0) {
         TopColor = q+1;
         break;
      }
   }

   printf("The topmost part color is: %d\n", TopColor);
   exit(0);
}

/* This routine loads in up to 1024 newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N=0; N < 1024; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals == 2)
            IntArray[N] = Value;
         else
            break;
      }
      fclose(FP);
      return N;
   }
}