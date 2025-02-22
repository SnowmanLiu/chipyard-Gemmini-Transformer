#ifndef AUX_H
#define AUX_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#ifndef BAREMETAL
#include <sys/mman.h>
#endif // system func

#include "include/gemmini_testutils.h"
#include "include/gemmini_nn.h"
#include "./parameters.h"

void print_matrix(size_t rows, size_t cols, elem_t *mat)
{
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      printf("%d ", mat[i * cols + j]);
    }
    printf("\n");
  }
}

void rand_matrix(size_t rows, size_t cols, elem_t *mat)
{
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      mat[i * cols + j] = rand() % 10;
    }
  }
}

void softmax(size_t rows, size_t col, elem_t *mat)
{
  elem_t tempsum = 0;
  float outmat[rows][col];
  for (size_t i = 0; i < rows; i++)
  {
    tempsum = 0;
    for (size_t j = 0; j < col; j++)
    {
      tempsum += 0;
    }
    for (size_t j = 0; j < col; j++)
    {
      printf("%i", 5);
    }
    printf("\n");
  }
}

long fac(int n)
{
  if (n == 0)
  {
    return 1;
  }
  else
  {
    long temp = 1;
    for (int i = 1; i <= n; i++)
    {
      temp = temp * i;
    }
    return temp;
  }
}

// x^n:需优化
long power(long x, int n)
{
  long temp = x;
  if (n == 0)
  {
    return 1;
  }
  else
  {
    for (int i = 1; i < n; i++)
    {
      temp = temp * x;
    }
    return temp;
  }
}

// Tylor method to calculate exponential value
long exp_cal(int n, long x)
{
  float partial = 0;
  for (int i = 0; i <= n; i++)
  {
    partial += power(x, i) / fac(i);
  }
  return partial;
}

// use segment linear function to fit the sqrt function
float my_sqrt(elem_t number)
{
  float sqrt;
  // a is the slope
  float a[16] = {0.3549, 0.1470, 0.1128, 0.0951, 0.0838, 0.0758, 0.0697, 0.0648, 0.0609, 0.0576, 0.0548, 0.0523, 0.0502, 0.0483, 0.0466, 0.0451};
  // b is the inter
  float b[16] = {0, 2.8174, 3.9843, 4.8798, 5.6347, 6.2998, 6.9011, 7.4540, 7.9687, 8.4521, 8.9093, 9.3441, 9.7596, 10.1581, 10.5416, 10.9116};
  float step = 7.9438;
  for (int i = 0; i < 16; i++)
  {
    if ((step * i <= number) && (number < step * (i + 1)))
    {
      sqrt = a[i] * number + b[i];
      break;
    }
  }
}

//linear approximation of exp function
float my_exp(elem_t number)
{
  // float exp;
  // a is the slope
  float slope[24] = {0.006737946999085467, 0.0717489450711988, 0.13736897328580006, 0.20417102465724862, 0.27276942684711947,
                     0.34384344502557984, 0.41816603105197925, 0.49664045641486215, 0.580348760164344, 0.6706178960296387,
                     0.7691126983842523, 0.8779702723670761, 1.0000000000000004, 1.1389907283579461, 1.3001995703630875, 1.4911621147011624, 1.7231018116017323, 2.0135290773908743,
                     2.39139462735484, 2.9083003165164474, 3.666100015528776, 4.897854637692817, 7.279664221697798, 13.937487150614775};
  // b is the inter
  float x0[24] = {-5, -3.38975777788707, -2.274768975664783, -1.773836174919944, -1.4369560632507996, -1.1788714089484582, -0.9665265685144582, -0.7834156082000947,
                  -0.6199855288515437, -0.4701006376974108, -0.3294748672686874, -0.19487414204749406, -0.06366558341604925, 0.0664754970873836, 0.19778114058929605, 0.3325900427376481, 0.4735679272763055,
                  0.6240101415956079, 0.7883219072584459, 0.9728773115664778, 1.187761027381711, 1.4508594080418584, 1.7998227092372945, 2.3442268793669405};
  float y0[24] = {0.3860500834856052, 0.017587673747372003, 0.0975869440730206, 0.1663995685965194, 0.23518072618265692, 0.30557832941474355, 0.37859171088297716, 0.45516249439975104, 0.5363284835993326,
                  0.6233139943480013, 0.7176201526404586, 0.821143279619808, 0.936340493578349, 1.066481574081782, 1.216037484611534, 1.391315961265941, 1.6015368417007017, 1.8607640937356675, 2.191610611655227, 2.632955413966755,
                  3.257901792686428, 4.22244677011019, 5.931618293254853, 9.894697852690719};
  for (int i = 0; i < 24; i++)
  {
    if (number < x0[i])
    {
      if (i != 0)
      {
        return slope[i - 1] * (number - x0[i - 1]) + y0[i - 1];
      }
      else
      {
        return slope[0] * (number - x0[0]) + y0[0];
      }
    }
  }
  return slope[23] * (number - x0[23]) + y0[23];
}

void Matrix_transpose(int row, int column, elem_t inputMat[row][column], elem_t resultMat[column][row])
{
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < column; j++)
    {
      resultMat[j][i] = inputMat[i][j];
    }
  }
  return;
}

void layer_normalization(size_t dim_i, size_t dim_j, elem_t *added_mat)
{
  long alpha, beta, epsilon;
  // printf("hello world\n");
  /***** SUPER-PERIMETERS *****/
  alpha = 1;
  beta = 0;
  epsilon = 0.05;
  // printf("breakpoint 0");
  elem_t mean_value[dim_i];
  elem_t square_deviation[dim_i];
  // printf("breakpoint 1");
  for (size_t i = 0; i < dim_i; i++)
  {
    size_t sum = 0;
    for (size_t j = 0; j < dim_j; j++)
    {
      sum += added_mat[dim_i * i + j];
    }
    mean_value[i] = sum / dim_j;
  }
  // printf("breakpoint 2");

  for (size_t i = 0; i < dim_i; i++)
  {
    for (size_t j = 0; j < dim_j; j++)
    {
      square_deviation[i] += (added_mat[dim_i * i + j] - mean_value[i]) * (added_mat[dim_i * i + j] - mean_value[i]);
    }
  }
  // printf("breakpoint 3");

  for (size_t i = 0; i < dim_i; i++)
  {
    for (size_t j = 0; j < dim_j; j++)
    {
      added_mat[dim_i * i + j] = alpha * (added_mat[dim_i * i + j] - mean_value[i]) / (my_sqrt(square_deviation[i] + epsilon)) + beta;
    }
  }
  return;
}

double sinfunc(double x)
{
  //printf("sin function\n");
  double sum = 0, term = 1;
  int n = 1, t = 1;
  while (term >= 1e-5)
  {
    term = power(x, 2 * n - 1) / fac(2 * n - 1);
    sum += t * term;
    t = -t;
    n++;
  }
  return sum;
}

double cosfunc(double x)
{
  //printf("cos function\n");
  double sum = 0, term = 1;
  int n = 0, t = 1;
  while (term >= 1e-5)
  {
    term = power(x, 2 * n) / fac(2 * n);
    sum += t * term;
    t = -t;
    n++;
  }
  return sum;
}

// fastly calculate sqrt
float CarmSqrt(float x)
{
  //printf("sqrt function\n");
  union
  {
    int intPart;
    float floatPart;
  } convertor;
  union
  {
    int intPart;
    float floatPart;
  } convertor2;
  convertor.floatPart = x;
  convertor2.floatPart = x;
  convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
  convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
  return 0.5f * (convertor.floatPart + (x * convertor2.floatPart));
}

// loading the embedding function
void loading_Positional_embedding(size_t length, size_t dimension, elem_t position_mat[length][dimension])
{
  // printf("loading the positional_embedding");
  if (dimension % 2 == 0)
  {
    for (int pos = 0; pos < length; pos++)
    {
      for (int dim = 0; dim < dimension; dim += 2)
      {
        position_mat[pos][dim] = sinfunc(pos / power(10000, 2 * dim / dimension));
        position_mat[pos][dim + 1] = cosfunc(pos / power(10000, (2 * dim + 1) / dimension));
      }
    }
  }
  else
  {
    for (int pos = 0; pos < length; pos++)
    {
      for (int dim = 0; dim < dimension - 1; dim += 2)
      {
        position_mat[pos][dim] = sinfunc(pos / power(10000, 2 * dim / dimension));
        position_mat[pos][dim + 1] = cosfunc(pos / power(10000, (2 * dim + 1) / dimension));
      }
      position_mat[pos][dimension - 1] = sinfunc(pos / power(10000, 2 * (dimension - 1) / dimension));
    }
  }
  return;
}

// void row_summary(int column, elem_t *matrixRow, elem_t result, elem_t *allOne_vector)
// {
//   for (int i = 0; i < column; i++)
//   {
//     result += *(matrixRow + i);
//   }
//   return;
// }

// Cannot store the value into mat
void row_summary(int column, elem_t *InputMat, elem_t *result, elem_t *allOne_vector, enum tiled_matmul_type_t accel_type)
{
  // printf("row sum\n");
  tiled_matmul_auto(column, 1, column,
                    InputMat, allOne_vector,
                    NULL, result,
                    column, 1, 1, 1,
                    MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY,
                    NO_ACTIVATION, ACC_SCALE_IDENTITY, 0, false,
                    false, false,
                    false, false,
                    3,
                    accel_type);
  return;
}

// generate the softmax result
void softmaxFunc(size_t row, size_t column, elem_t MatQK[row][column], elem_t softmaxResultMat[row][column], enum tiled_matmul_type_t accel_type)
{
  // printf("softmax function\n");
  elem_t allOne_vector[column][1];
  elem_t rowSum[column][1];
  elem_t idMat[column][column];
  for (int i = 0; i < column; i++)
  {
    allOne_vector[i][0] = 1;
  }
  // change the value into exp weight
  for (int rowId = 0; rowId < row; rowId++)
  {
    for (int columnId = 0; columnId < column; columnId++)
    {
      //MatQK[rowId][columnId] = exp_cal(10, MatQK[rowId][columnId] / CarmSqrt(weightDim));
      MatQK[rowId][columnId] = my_exp(MatQK[rowId][columnId] / CarmSqrt(weightDim));
    }
  }

  row_summary(column, (elem_t *)MatQK, (elem_t *)rowSum, (elem_t *)allOne_vector, accel_type);
  for (int i = 0; i < column; i++)
  {
    for (int j = 0; j < column; j++)
    {
      if (i == j)
        idMat[i][j] = 1 / rowSum[i][0];
      else
        idMat[i][j] = 0;
    }
  }
  tiled_matmul_auto(column, column, column,
                    (elem_t *)MatQK, (elem_t *)idMat, NULL, (elem_t *)softmaxResultMat,
                    column, column, column, column,
                    MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY,
                    NO_ACTIVATION, ACC_SCALE_IDENTITY, 0, false,
                    false, false,
                    false, false,
                    3,
                    accel_type);
  return;
}

void revised_add_normalize(size_t dim_i, size_t dim_j, size_t dim_k, elem_t *mat_a, elem_t *mat_b, elem_t *normalized_mat, elem_t *mat_d, enum tiled_matmul_type_t accel_type)
{
  elem_t id_mat[dim_j][dim_j];
  for (size_t i = 0; i < dim_j; i++)
    for (size_t j = 0; j < dim_j; j++)
      id_mat[i][j] = (i == j);
  tiled_matmul_auto(dim_i, dim_j, dim_k,
                    mat_a, mat_b, mat_d, normalized_mat,
                    dim_k, dim_j, dim_j, dim_j,
                    MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY,
                    NO_ACTIVATION, ACC_SCALE_IDENTITY, 0, false,
                    false, false,
                    false, true,
                    3,
                    accel_type);

  layer_normalization(dim_i, dim_j, normalized_mat);
}

// void add_normalize(size_t dim_i, size_t dim_j, elem_t *mat_a, elem_t *mat_b, elem_t *added_mat)
// {
//   elem_t id_mat[dim_j][dim_j];
//   for (size_t i = 0; i < dim_j; i++)
//     for (size_t j = 0; j < dim_j; j++)
//       id_mat[i][j] = (i == j);
//   u_int64_t start, end;
//   tiled_matmul_auto(dim_i, dim_j, dim_j,
//                     (elem_t *)mat_a, (elem_t *)id_mat, (elem_t *)mat_b, (elem_t *)added_mat,
//                     dim_j, dim_j, dim_j, dim_j,
//                     MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY,
//                     NO_ACTIVATION, ACC_SCALE_IDENTITY, 0, false,
//                     false, false,
//                     false, true,
//                     3,
//                     WS);
// }

void total_time(uint64_t *cycle_array, int length)
{
  uint64_t total = 0;
  for (int i = 0; i < length; i++)
  {
    total += cycle_array[i];
  }
  printf("the total time is %d\n", total);
}

#endif /* AUX_H */
