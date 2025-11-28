#include <stdarg.h>
#include <limits.h>
#include "cust_scanf.h"

typedef struct {
    const char *fmt; // текущая позиция в строке формата
    const char *input;   // Входная строка (для oversscanf)
    FILE *file;  // Файловый поток (для overfscanf)
    int input_eof;  // флаг конца ввода
    int consumed_chars;  // количество обработанных символов
    int matched_items;     // количество успешно сопоставленных элементов
    int error_code;   // Код ошибки
} parser_state_t;

#define CHECK_EOF(state) \ // проверяем конец ввода
    if ((state)->input_eof) { \
        (state)->error_code = OVER_SCANF_END_OF_INPUT; \
        return -1; \
    }

#define SET_ERROR(state, code) \ // код ошибки
    do { \
        (state)->error_code = (code); \
        return -1; \
    } while(0)

static void skip_whitespace(parser_state_t *state) {
    if (state->file) {
        int c;
        while ((c = fgetc(state->file)) != EOF && isspace(c)) {
            state->consumed_chars++;
        }
        if (c != EOF) {
            ungetc(c, state->file);
        } else {
            state->input_eof = 1;
        }
    } else {
        while (*state->input && isspace(*state->input)) {
            state->input++;
            state->consumed_chars++;
        }
        if (!*state->input) {
            state->input_eof = 1;
        }
    }
}

static int get_char(parser_state_t *state) {
    if (state->file) {
        int c = fgetc(state->file);
        if (c == EOF) {
            state->input_eof = 1;
            return EOF;
        }
        state->consumed_chars++;
        return c;
    } else {
        if (!*state->input) {
            state->input_eof = 1;
            return EOF;
        }
        state->consumed_chars++;
        return (unsigned char)*state->input++;
    }
}

static void unget_char(parser_state_t *state, int c) {
    if (state->file && c != EOF) {
        ungetc(c, state->file);
        state->consumed_chars--;
    }
}

static over_scanf_status_t parse_roman_numeral(parser_state_t *state, int *result) {
    const char *romans = "IVXLCDM";
    int values[] = {1, 5, 10, 50, 100, 500, 1000};
    int len = strlen(romans);
    int num = 0;
    int prev_value = 0;

    skip_whitespace(state);
    if (state->input_eof) return OVER_SCANF_END_OF_INPUT;

    int start_pos = state->consumed_chars;
    int c;

    while ((c = get_char(state)) != EOF) {
        int found = 0;
        for (int i = 0; i < len; i++) {
            if (toupper(c) == romans[i]) {
                int current_value = values[i];
                if (current_value > prev_value) {
                    num += current_value - 2 * prev_value;
                } else {
                    num += current_value;
                }
                prev_value = current_value;
                found = 1;
                break;
            }
        }
        if (!found) {
            unget_char(state, c);
            break;
        }
    }

    if (num == 0) {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    }

    *result = num;
    return OVER_SCANF_SUCCESS;
}

static over_scanf_status_t parse_zeckendorf(parser_state_t *state, unsigned int *result) {
    skip_whitespace(state);
    if (state->input_eof) return OVER_SCANF_END_OF_INPUT;

    int start_pos = state->consumed_chars;
    char buffer[128] = {0};
    int buf_len = 0;

    int c;
    while ((c = get_char(state)) != EOF) {
        if (c == '0' || c == '1') {
            if (buf_len < (int)(sizeof(buffer) - 1)) {
                buffer[buf_len++] = c;
            } else {
                state->consumed_chars = start_pos;
                return OVER_SCANF_BUFFER_OVERFLOW;
            }
        } else {
            unget_char(state, c);
            break;
        }
    }

    if (buf_len == 0) {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    }

    if (buffer[buf_len - 1] != '1') {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    }

    unsigned int value = 0;
    unsigned int fib_prev = 1, fib_curr = 1;
    for (int i = 0; i < buf_len; i++) {
        if (buffer[i] == '1') {
            if (i == 0) {
                value += fib_curr;
            } else {
                unsigned int next_fib = fib_prev + fib_curr;
                fib_prev = fib_curr;
                fib_curr = next_fib;
                value += fib_curr;
            }
        } else if (i > 0) {
            unsigned int next_fib = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = next_fib;
        }
    }

    *result = value;
    return OVER_SCANF_SUCCESS;
}

static over_scanf_status_t parse_custom_base(parser_state_t *state, int *result, int base, int uppercase_only) {
    if (base < 2 || base > 36) {
        return OVER_SCANF_INVALID_BASE;
    }

    skip_whitespace(state);
    if (state->input_eof) return OVER_SCANF_END_OF_INPUT;

    int start_pos = state->consumed_chars;
    int sign = 1;
    int c = get_char(state);
    if (c == '-') {
        sign = -1;
        c = get_char(state);
    } else if (c == '+') {
        c = get_char(state);
    }

    if (c == EOF) {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    }

    long long value = 0;
    int has_digits = 0;

    while (c != EOF) {
        if (uppercase_only) {
            if (c >= 'A' && c <= 'Z') {
                int digit = c - 'A' + 10;
                if (digit < base) {
                    value = value * base + digit;
                    has_digits = 1;
                } else {
                    unget_char(state, c);
                    break;
                }
            } else if (c >= '0' && c <= '9') {
                int digit = c - '0';
                if (digit < base) {
                    value = value * base + digit;
                    has_digits = 1;
                } else {
                    unget_char(state, c);
                    break;
                }
            } else {
                unget_char(state, c);
                break;
            }
        } else {
            if (c >= 'a' && c <= 'z') {
                int digit = c - 'a' + 10;
                if (digit < base) {
                    value = value * base + digit;
                    has_digits = 1;
                } else {
                    unget_char(state, c);
                    break;
                }
            } else if (c >= '0' && c <= '9') {
                int digit = c - '0';
                if (digit < base) {
                    value = value * base + digit;
                    has_digits = 1;
                } else {
                    unget_char(state, c);
                    break;
                }
            } else {
                unget_char(state, c);
                break;
            }
        }
        c = get_char(state);
    }

    if (!has_digits) {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    }

    if (sign == 1 && value > INT_MAX) {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    } else if (sign == -1 && value > (long long)INT_MAX + 1) {
        state->consumed_chars = start_pos;
        return OVER_SCANF_INVALID_INPUT;
    }

    *result = (int)(sign * value);
    return OVER_SCANF_SUCCESS;
}

static int parse_format(parser_state_t *state, va_list args) {
    state->matched_items = 0;
    state->error_code = OVER_SCANF_SUCCESS;

    while (*state->fmt) {
        if (*state->fmt == '%') {
            state->fmt++;

            while (*state->fmt && strchr("hlL", *state->fmt)) {
                state->fmt++;
            }

            if (*state->fmt == '\0') {
                SET_ERROR(state, OVER_SCANF_INVALID_FORMAT);
            }

            char spec = *state->fmt;
            state->fmt++;

            if (spec == 'R' && *state->fmt == 'o') {
                state->fmt++;
                int *ptr = va_arg(args, int*);
                over_scanf_status_t status = parse_roman_numeral(state, ptr);
                if (status != OVER_SCANF_SUCCESS) {
                    return -1;
                }
                state->matched_items++;
            } else if (spec == 'Z' && *state->fmt == 'r') {
                state->fmt++;
                unsigned int *ptr = va_arg(args, unsigned int*);
                over_scanf_status_t status = parse_zeckendorf(state, ptr);
                if (status != OVER_SCANF_SUCCESS) {
                    return -1;
                }
                state->matched_items++;
            } else if (spec == 'C' && *state->fmt == 'v') {
                state->fmt++;
                int *ptr = va_arg(args, int*);
                int base_arg = va_arg(args, int);
                if (base_arg < 2 || base_arg > 36) {
                    base_arg = 10;
                }
                over_scanf_status_t status = parse_custom_base(state, ptr, base_arg, 0);
                if (status != OVER_SCANF_SUCCESS) {
                    return -1;
                }
                state->matched_items++;
            } else if (spec == 'C' && *state->fmt == 'V') {
                state->fmt++;
                int *ptr = va_arg(args, int*);
                int base_arg = va_arg(args, int);
                if (base_arg < 2 || base_arg > 36) {
                    base_arg = 10;
                }
                over_scanf_status_t status = parse_custom_base(state, ptr, base_arg, 1);
                if (status != OVER_SCANF_SUCCESS) {
                    return -1;
                }
                state->matched_items++;
            } else {
                SET_ERROR(state, OVER_SCANF_UNKNOWN_FLAG);
            }
        } else {
            if (isspace(*state->fmt)) {
                skip_whitespace(state);
                state->fmt++;
            } else {
                int c = get_char(state);
                if (c == EOF) {
                    return -1;
                }
                if (c != *state->fmt) {
                    unget_char(state, c);
                    return -1;
                }
                state->fmt++;
            }
        }
    }

    return state->matched_items;
}

int overfscanf(FILE *stream, const char *format, ...) {
    if (!stream || !format) {
        errno = EINVAL;
        return EOF;
    }

    parser_state_t state = {0};
    state.file = stream;
    state.fmt = format;
    state.input_eof = 0;
    state.consumed_chars = 0;
    state.matched_items = 0;
    state.error_code = OVER_SCANF_SUCCESS;

    va_list args;
    va_start(args, format);
    int result = parse_format(&state, args);
    va_end(args);

    if (result == -1) {
        return EOF;
    }
    return result;
}

int oversscanf(const char *str, const char *format, ...) {
    if (!str || !format) {
        errno = EINVAL;
        return EOF;
    }

    parser_state_t state = {0};
    state.file = NULL;
    state.input = str;
    state.fmt = format;
    state.input_eof = 0;
    state.consumed_chars = 0;
    state.matched_items = 0;
    state.error_code = OVER_SCANF_SUCCESS;

    va_list args;
    va_start(args, format);
    int result = parse_format(&state, args);
    va_end(args);

    if (result == -1) {
        return EOF;
    }
    return result;
}
