/*************************************************************************************************************
File name:99.c
Author:KD
Version:V_1.0
Build date: 2024-04-26
Description:This C program prints the multiplication table from 1x1 to 9x9. The outer loop iterates over each
            row (1-9), while the inner loop prints the products for columns up to the current row, formatted 
            to align neatly.
Others:Usage requires preservation of original author attribution.
Log:1.First create file.
*************************************************************************************************************/

#include <stdio.h>

int main() 
{
   printf("九九乘法表：\n");
   for (int i = 1; i <= 9; ++i) {
      for (int j = 1; j <= i; ++j) {
         printf("%d*%d=%-2d  ", j, i, i * j);
      }
      printf("\n");
   }

   return 0;
}
