/*
	Display a 2x2 Matrix
*/
void mat_display2x2(float mult[][2], int rowFirst, int columnSecond)
{
	int i, j;

	Serial.println("Output Matrix:");
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			Serial.print(mult[i][j]);
			Serial.print(" ");
			if(j == columnSecond - 1)
				Serial.println("");
		}
	}
}

/*
  Multiply two Matrices A and B and store the result in C.
*/
void mat_multiply(float A[][3], float B[][2], float C[][2], int rowFirst, int columnFirst, int rowSecond, int columnSecond)
{
	int i, j, k;

	// Initializing elements of matrix C to 0.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			C[i][j] = 0;
		}
	}

	// Multiplying matrix A and C and storing in array C.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			for(k=0; k<columnFirst; ++k)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

/*
	Multiply two 2x2 Matrices
*/
void mat_multiply2x2(float firstMatrix[][2], float secondMatrix[][2], float mult[][2], int rowFirst, int columnFirst, int rowSecond, int columnSecond)
{
	int i, j, k;

	// Initializing elements of matrix mult to 0.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			mult[i][j] = 0;
		}
	}

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			for(k=0; k<columnFirst; ++k)
			{
				mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}

/*
	Multiply Matrix by column vector
*/
void mat_multiplyCol(float firstMatrix[][2], float secondMatrix[][1], float mult[][1])
{
	int i, j, k;
  int rowFirst = 2; int columnFirst = 2;
  int columnSecond = 1;
	// Initializing elements of matrix mult to 0.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			mult[i][j] = 0;
		}
	}

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			for(k=0; k<columnFirst; ++k)
			{
				mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}

/*
  Transopose a Matrix A into B
*/
void mat_transpose(float A[][3], float B[][2], int rows, int columns)
{
  int i,j;
  // Finding transpose of matrix a[][] and storing it in array trans[][].
  for(i = 0; i < rows; i++)
      for(j = 0; j < columns; j++)
      {
        B[j][i]=A[i][j];
      }
}

/*
  Transopose a 2x2 Matrix A into B
*/
void mat_transpose2x2(float A[][2], float B[][2], int rows, int columns)
{
  int i,j;
  // Finding transpose of matrix a[][] and storing it in array trans[][].
  for(i = 0; i < rows; i++)
      for(j = 0; j < columns; j++)
      {
        B[j][i]=A[i][j];
      }
}

/*
	Take A and invert it into B
*/
void mat_inverse2x2(float A[2][2], float B[2][2])
{
  float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];

  B[0][0] = A[1][1]/det;
  B[0][1] = -A[0][1]/det;
  B[1][0] = -A[1][0]/det;
  B[1][1] = A[0][0]/det;
}
