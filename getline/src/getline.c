#include "getline.h"

static char *getBuf(void) {
    char *buf = (char *) calloc(CALLOC_CONST + 1, sizeof(char));

    return buf;
}

static err_t reallocBuf(char **const buf, const int buf_size) {
    if (buf == NULL) {
        return ERR_GETLINE;
    }

    char *tempp = (char *) realloc(*buf, buf_size + 1);
    if (!tempp) {
        free(buf);
        return ERR_GETLINE;
    }

    *buf = tempp;

    return OK;
}

static err_t reallocLinep(char **const linep, const size_t linelenp) {
    if (linep == NULL) {
        return ERR_GETLINE;
    }

    char *tempp = (char *) realloc(*linep, linelenp + 1);
    if (!tempp) {
        free(linep);
        return ERR_GETLINE;
    }

    *linep = tempp;

    return OK;
}

static err_t fillBuf(char **const buf, FILE *const stream) {
    if (buf == NULL || stream == NULL) {
        return ERR_GETLINE;
    }

    size_t bufc = 0;
    char chr;

    for (size_t buf_size = CALLOC_CONST; (chr = fgetc(stream)) != EOF && chr != '\n'; bufc++) {
        if (bufc == buf_size) {
            buf_size *= 2;

            if (reallocBuf(buf, buf_size) != OK) {
                return ERR_GETLINE;
            }
        }

        strncat(*buf, &chr, 1);
    }

    if (chr == EOF && bufc == 0) {
        free(*buf);
        return ERR_GETLINE;
    }

    return OK;
}

ssize_t cgetline(char **const linep, size_t *const linelenp, FILE *const stream) {
    if (!linep || !linelenp || !stream) {
        return ERR_GETLINE;
    }

    char *buf = getBuf();
    if (buf == NULL) {
        return ERR_GETLINE;
    }

    if (fillBuf(&buf, stream) != OK) {
        return ERR_GETLINE;
    }

    size_t buf_len = strlen(buf);

    if (!(*linep) || *linelenp != buf_len) {
        *linelenp = buf_len;

        if (reallocLinep(linep, *linelenp) != OK) {
            free(buf);
            return ERR_GETLINE;
        }
    }

    strncpy(*linep, buf, strlen(buf) + 1);
    free(buf);

    return *linelenp;
}
