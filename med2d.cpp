


#include <stdio.h>
#include <string.h>
#include "med2d.h"



#define FORWARD     1
#define BACKWARD  (-1)



unsigned char get_value(unsigned char image[ROWNUM][COLNUM], int row, int col)
{
	if(row >= 0 && col >= 0 && row < ROWNUM && col < COLNUM)
		return image[row][col];
	return 0;
}



void median2d(unsigned char dst[ROWNUM][COLNUM], unsigned char src[ROWNUM][COLNUM], int Ny, int Nx)
{
	int med, delta_l;
	int m, n;
	int prev, next;
	int h[256];
	int direction = FORWARD;
	int row1, row2, col1, col2;
	int row, col, r, c;

	int middle=((Ny*2+1)*(Nx*2+1)+1)/2;


	memset(h, 0, sizeof(h));

	// Histogram For (0,0)-element
	for(row = -Ny; row <= Ny; row++)
	{
		for(col = -Nx; col <= Nx; col++)
			h[get_value(src, row, col)]++;
	}
	// Median
	for(m=0,n=0; n < 256; n++)
	{
		m += h[n];
		if(m>=middle)
			break;
	}
	med=n;
	delta_l = m - h[n];


	// Now, Median Is Defined For (0,0)-element
	// Begin Scanning: direction - FORWARD
	dst[0][0]=med;

	// main loop
	for(col=1, row=0; row<ROWNUM; row++)
	{
		// Prepare to Horizontal Scanning
		row1=row-Ny;
		row2=row+Ny;

		for(; col>=0 && col<COLNUM; col += direction)
		{
			// Determine Previous and Next Columns
			// Pay Attention To Defined Direction !!!
			prev = col-direction*(Nx+1);
			next = col+direction*Nx;

			// Now Change Old Histogram
			// New Histogram
			// delete previous

			for(r=row1; r<=row2; r++)
			{
				int value_out = get_value(src, r, prev);
				int value_in = get_value(src, r, next);
				if(value_out == value_in)
					continue;
				h[value_out]--;
				if(value_out < med)
					delta_l--;
				h[value_in]++;
				if(value_in < med)
					delta_l++;
			}

			// Update new median
			if(delta_l >= middle)
			{
				while(delta_l>=middle)
				{
					if(h[--med] > 0)
						delta_l -= h[med];
				}
			}
			else
			{
				while(delta_l + h[med] < middle)
				{
					if(h[med] > 0)
						delta_l += h[med];
					med++;
				}
			}
			dst[row][col] = med;
		} // end of column loop


		if(row == ROWNUM-1)
			return;

		// go back to the last/first pixel of the line
		col -= direction;
		// change direction to the opposite
		direction *= -1;


		// Shift Down One Line
		prev = row1;
		next = row2+1;


		col1 = col - Nx;
		col2 = col + Nx;

		for(c=col1; c<=col2; c++)
		{
			int value_out = get_value(src, prev, c);
			int value_in = get_value(src, next, c);
			if(value_out == value_in)
				continue;
			h[value_out]--;
			if(value_out<med)
				delta_l--;
			h[value_in]++;
			if(value_in<med)
				delta_l++;
		}

		if(delta_l>=middle)
		{
			while(delta_l>=middle)
			{
				if(h[--med]>0)
					delta_l -= h[med];
			}
		}
		else
		{
			while(delta_l + h[med] < middle)
			{
				if(h[med]>0)
					delta_l += h[med];
				med++;
			}
		}
		dst[row+1][col] = med;
		col += direction;
	}
}

