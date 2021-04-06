#include "../include/matrix.h"

// _______________________Init/Out/release operations_______________________

Matrix *create_matrix_from_file(const char* path_file) {
    if (path_file == NULL) {
        return NULL;
    }

    FILE *f = fopen(path_file, "r");

    if (f == NULL) {
        return NULL;
    }

    Matrix *matrix = NULL;
    size_t rows, columns;

    if (fscanf(f, "%zu%zu", &rows, &columns) != 2) {
        fclose(f);
        return NULL;
    }
    if (rows < 1 || columns < 1) {
        fclose(f);
        return NULL;
    }

    if ((matrix = create_matrix(rows, columns)) == NULL) {
        fclose(f);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->columns; j++) {
            if (fscanf(f, "%lf", &matrix->data[i][j]) != 1) {
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);

    return matrix;
}

static void init_matrix(Matrix *const matrix, size_t rows, size_t cols) {
    matrix->data = NULL;
    matrix->rows = rows;
    matrix->columns = cols;
}

Matrix *create_matrix(size_t rows, size_t cols) {
    if (rows < 1 || cols < 1) {
        return NULL;
    }

    Matrix *matrix = (Matrix *) malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }

    init_matrix(matrix, rows, cols);

    matrix->data = (double **) malloc(matrix->rows * sizeof(double *));
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; i++) {
        matrix->data[i] = (double *) malloc(matrix->columns * sizeof(double));
        if (matrix->data[i] == NULL) {
            free_matrix(matrix);
            return NULL;
        }
    }


    return matrix;
}

int my_print_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return ERR_PRINT_MATRIX;
    }

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->columns; j++) {
            printf("%12lf", matrix->data[i][j]);
        }
        printf("\n");
    }

    return OK;
}

int free_matrix(Matrix* matrix) {
    if (matrix == NULL || matrix->data == NULL) {
        return ERR_FREE_MATRIX;
    }

    for (size_t i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }

    free(matrix->data);

    free(matrix);

    return OK;
}

// ____________________________Basic operations_____________________________

int get_rows(const Matrix* matrix, size_t* rows) {
    if (matrix == NULL || rows == NULL) {
        return ERR_GET_ROWS;
    }

    *rows = matrix->rows;

    return OK;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (matrix == NULL || cols == NULL) {
        return ERR_GET_COLS;
    }

    *cols = matrix->columns;

    return OK;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (matrix == NULL || val == NULL) {
        return ERR_GET_ELEMENT;
    }

    if (row >= matrix->rows) {
        return ERR_GET_ELEMENT;
    }
    if (col >= matrix->columns) {
        return ERR_GET_ELEMENT;
    }

    *val = matrix->data[row][col];

    return OK;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (matrix == NULL) {
        return ERR_SET_ELEMENT;
    }

    if (row >= matrix->rows) {
        return ERR_SET_ELEMENT;
    }
    if (col >= matrix->columns) {
        return ERR_SET_ELEMENT;
    }

    matrix->data[row][col] = val;

    return OK;
}

// _____________________________Math operations_____________________________

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    }

    if (matrix->rows < 1 || matrix->columns < 1) {
        return NULL;
    }

    Matrix *dst_matrix = create_matrix(matrix->rows, matrix->columns);
    if (dst_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->columns; j++) {
            dst_matrix->data[i][j] = matrix->data[i][j] * val;
        }
    }

    return dst_matrix;
}

Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    if (matrix->rows < 1 || matrix->columns < 1) {
        return NULL;
    }

    Matrix *dst_matrix = create_matrix(matrix->columns, matrix->rows);
    if (dst_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix->columns; i++) {
        for (size_t j = 0; j < matrix->rows; j++) {
            dst_matrix->data[i][j] = matrix->data[j][i];
        }
    }

    return dst_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }

    if (l->rows < 1 || l->columns < 1 || r->rows < 1 || r->columns < 1) {
        return NULL;
    }

    if (l->rows != r->rows || l->columns != r->columns) {
        return NULL;
    }

    Matrix *dst_matrix = create_matrix(l->rows, l->columns);
    if (dst_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < l->rows; i++) {
        for (size_t j = 0; j < l->columns; j++) {
            dst_matrix->data[i][j] = l->data[i][j] + r->data[i][j];
        }
    }

    return dst_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }

    if (l->rows < 1 || l->columns < 1 || r->rows < 1 || r->columns < 1) {
        return NULL;
    }

    if (l->rows != r->rows || l->columns != r->columns) {
        return NULL;
    }

    Matrix *dst_matrix = create_matrix(l->rows, l->columns);
    if (dst_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < l->rows; i++) {
        for (size_t j = 0; j < l->columns; j++) {
            dst_matrix->data[i][j] = l->data[i][j] - r->data[i][j];
        }
    }

    return dst_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }

    if (l->rows < 1 || l->columns < 1 || r->rows < 1 || r->columns < 1) {
        return NULL;
    }

    if (l->columns != r->rows) {
        return NULL;
    }

    Matrix *dst_matrix = create_matrix(l->rows, r->columns);
    if (dst_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < l->rows; i++) {
        for (size_t j = 0; j < r->columns; j++) {
            double summ = 0;

            for (size_t k = 0; k < r->rows; k++) {
                summ += l->data[i][k] * r->data[k][j];
            }
            dst_matrix->data[i][j] = summ;
        }
    }

    return dst_matrix;
}

// ____________________________Extra operations_____________________________

/*
    Gone to save the world :)

    P.S. Finish it for me :3 ples
*/

int det(const Matrix* matrix, double* val) {
    if (matrix == NULL || val == NULL) {
        return ERR_GET_DET;
    }

    return OK;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    return NULL;
}

Matrix* inv(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    return NULL;
}
