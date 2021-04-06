#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum ret_codes {
    ERR_GET_DET = -7,
    ERR_PRINT_MATRIX,
    ERR_FREE_MATRIX,
    ERR_SET_ELEMENT,
    ERR_GET_ELEMENT,
    ERR_GET_COLS,
    ERR_GET_ROWS,
    OK
};

typedef struct {
    double **data;
    size_t rows;
    size_t columns;
} Matrix;

// Init/Out/release operations
Matrix* create_matrix_from_file(const char* path_file);
Matrix* create_matrix(size_t rows, size_t cols);
int my_print_matrix(Matrix* matrix);
int free_matrix(Matrix* matrix);

// Basic operations
int get_rows(const Matrix* matrix, size_t* rows);
int get_cols(const Matrix* matrix, size_t* cols);
int get_elem(const Matrix* matrix, size_t row, size_t col, double* val);
int set_elem(Matrix* matrix, size_t row, size_t col, double val);

// Math operations
Matrix* mul_scalar(const Matrix* matrix, double val);
Matrix* transp(const Matrix* matrix);

Matrix* sum(const Matrix* l, const Matrix* r);
Matrix* sub(const Matrix* l, const Matrix* r);
Matrix* mul(const Matrix* l, const Matrix* r);

// Extra operations
int det(const Matrix* matrix, double* val);
Matrix* adj(const Matrix* matrix);
Matrix* inv(const Matrix* matrix);

#endif  // __MATRIX_H__
