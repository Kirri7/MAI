#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define UPPER_LIMIT 10000

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    UNKNOWN_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

unsigned long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

ErrorCode calculateLimitE(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double n = 1;
    long double limit0 = 0;
    long double limit1 = powl((1 + (1 / n)), n);

    while (fabsl(limit1 - limit0) > epsilon) {
        ++n;
        limit0 = limit1;
        limit1 = powl((1 + (1 / n)), n);
    }

    *result = limit1;
    return SUCCESS;
}

ErrorCode calculateRowE(long double epsilon, long double* result){
    long double n = 1;
    long double row = 0;
    long double denominator = 1;

    while (fabsl(denominator) > epsilon) {
        row += denominator;
        denominator /= n;
        ++n;
    }

    *result = row;
    return SUCCESS;
}

ErrorCode calculateLimitPi(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double n = 1;
    long double limit0 = 0;
    long double limit1 = powl((powl(2, n) * factorial(n)), 4) / (n * (powl(factorial(2*n), 2)));

    while (fabsl(limit1 - limit0) > epsilon) {
        ++n;
        limit0 = limit1;
        limit1 = powl((powl(2, n) * factorial(n)), 4) / (n * (powl(factorial(2*n), 2)));
        if (limit1 > limit0) {
            *result = limit1;
            return OVERFLOW_ERROR;
        }
    }

    *result = limit1;
    return SUCCESS;
}

ErrorCode calculateRowPi(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    unsigned int n = 1;
    long double row0 = 0;
    long double row1 = 1.0 / (2*n - 1);
    ++n;

    while (fabsl(row1 - row0) > epsilon) {  
        long double denominator = 1.0 / (2*n - 1);
        row0 = row1;
        if (n % 2 == 0)
            row1 -= denominator;
        else
            row1 += denominator;
        ++n;
    }

    *result = 4 * row1;
    return SUCCESS;
}

ErrorCode calculateLimitLn2(long double epsilon, long double* result){
    long double n = 1;
    long double limit0 = 0;
    long double limit1 = n * (powl(2, (1.0 / n)) - 1);

    while (fabsl(limit1 - limit0) > epsilon) {
        ++n;
        limit0 = limit1;
        limit1 = n * (powl(2, (1.0 / n)) - 1);
    }

    *result = limit1;
    return SUCCESS;
}

ErrorCode calculateRowLn2(long double epsilon, long double* result){
    unsigned int n = 1;
    long double ln2 = 0;
    long double temp = 1;

    while (temp > epsilon) {  
        temp = 1.0 / n;

        if (n % 2 == 0)
            ln2 -= temp;
        else
            ln2 += temp;
        ++n;
    }

    *result = ln2;
    return SUCCESS;
}

ErrorCode calculateLimitSqrt2(long double epsilon, long double* result){
    long double x = -0.5;

    for (int i = 0; i < UPPER_LIMIT; ++i) {
        long double nextX = x - (x*x / 2.0) + 1;

        if (fabsl(nextX - x) < epsilon) {
            *result = nextX;
            return SUCCESS;
        }
        x = nextX;
    }
    return OVERFLOW_ERROR;
}


ErrorCode calculateRowSqrt2(long double epsilon, long double* result){
    long double k = 2;
    long double sqrt02 = 0;
    long double sqrt2 = 1;

    do {
        sqrt02 = sqrt2;
        sqrt2 *= powl(2, 1 / powl(2.0, k));
        ++k;
    } while (fabsl(sqrt2 - sqrt02) > epsilon);

    *result = sqrt2;
    return SUCCESS;
}

ErrorCode calculateSqrt2(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double x = 2;
    while (fabsl(x * x - 2) > epsilon)
        x = 0.5 * (x + 2 / x);

    *result = x;
    return SUCCESS;
}

ErrorCode calculateE(long double epsilon, long double* result){
    long double a = 2;
    long double b = 3;

    while ((b - a) > epsilon) {
        long double m = (a + b) / 2.0;

        if ((log(m) - 1.0) * (log(a) - 1.0) < 0)
            b = m;
        else
            a = m;
    }
    *result = (a + b) / 2.0;
    return SUCCESS;
}


long double dcosx(long double x) {
    return -sin(x);
}

ErrorCode calculatePi(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double pi = 3;
    long double fx = 4;

    while (fabsl(fx) > epsilon) {
        fx = cos(pi) + 1;
        long double dfx = dcosx(pi);
        pi -= fx / dfx;
    }

    *result = pi;
    return SUCCESS;
}

ErrorCode calculateLn2(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double a = 0;
    long double b = 1;

    while ((b - a) >= epsilon) {
        long double m = (a + b) / 2.0;
        if ((exp(m) - 2) * (exp(a) - 2) < 0) 
            b = m;
        else
            a = m;
    }
    *result = (a + b) / 2.0;
    return SUCCESS;
}

ErrorCode calculateRowGamma(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT; 
    long double pi = atanl(1.0) * 4.0;
    long long k = 2;

    long double first_fraction = 1;
    long double second_fraction = 1 / 2.0;
    long double gamma = 0;

    while(fabsl(first_fraction - second_fraction) > epsilon) {
        if (floorl(sqrtl(k)) == sqrtl(k)) {
            k++;
            continue;
        }

        first_fraction = powl(floorl(sqrtl(k)), -2);
        second_fraction = 1.0 / k;
        gamma += first_fraction;
        gamma -= second_fraction;
        k++;
    }

    *result = gamma - ((pi * pi) / 6.0);
    return SUCCESS;
}

int isPrime(int num) {
    if (num <= 1) {
        return 0;
    }
    if (num <= 3) {
        return 1;
    }
    if (num % 2 == 0) {
        return 0;
    }
    for (int i = 5; i * i <= num; i += 2) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

ErrorCode eqGammaLimit(long double eps, long double* limit) { 
    int primes[UPPER_LIMIT];
    long long t = 0;
    for (int i = 2; i < UPPER_LIMIT; ++i) {
        if (isPrime(i))
            primes[t++] = i;
    }

    t = 0;
    long double result = 1;
    long double prev = 2;
    while (fabsl(prev - result) > eps) {
        if (result > prev)
            return OVERFLOW_ERROR;
        prev = result;
        result *= (primes[t] - 1.0) / primes[t];
        ++t;
        if (t >= UPPER_LIMIT) {
            return OVERFLOW_ERROR;
        }
    }

    *limit = result * logl(primes[t]);
    return SUCCESS;
}

long double fgamma(long double x, long double limit) {
    return expl(-x) - limit;
}

long double dfgamma(long double x) {
    return -expl(x);
}

ErrorCode calculateGamma(long double eps, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double limit;
    ErrorCode code = eqGammaLimit(eps, &limit);

    long double gamma = 0.5L;
    long double temp = gamma - (fgamma(gamma, limit) / dfgamma(gamma));
    int t = 0;
    while (fabsl(temp - gamma) > eps) {
        ++t;
        if (t > UPPER_LIMIT)
            break;
        gamma = temp;
        temp = gamma - (fgamma(gamma, limit) / dfgamma(gamma));
    }
    *result = gamma;
    return code;
}

ErrorCode calculateLimitGamma(long double epsilon, long double* result) {
    if (!result)
        return INCORRECT_INPUT;
    long double gamma = 0;

    for (unsigned int i = 1; i < UPPER_LIMIT; ++i) {
        long double temp = 1.0 / i - logl(1.0 + 1.0 / i);
        if (fabsl(temp) < epsilon) {
            break;
        }
        gamma += temp;
    }

    *result = gamma;
    return SUCCESS;
}

void printError(ErrorCode code) {
    if (code == SUCCESS)
        return;
    printf("%s\n", errorMessages[code]);
    fflush(stdout);
}

int main( int argc, char *argv[]) {
    if (argc != 2){
        printf("Программа.exe <epsilon>\n");
        return INCORRECT_INPUT;
    }

    char *endptr;
    long double epsilon = strtold(argv[1], &endptr);
    if (*endptr != '\0') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    printf("epsilon = %Lf\n", epsilon);
    long double result;
    ErrorCode code;
    printf("---- 𝑒 ----\n");
    printError(calculateLimitE(epsilon, &result));
    printf("как предел = %Lf\n", result);
    printError(calculateRowE(epsilon, &result));
    printf("как ряд = %Lf\n", result);
    printError(calculateE(epsilon, &result));
    printf("как функция = %Lf\n", result);

    printf("---- π ----\n");
    printError(calculateLimitPi(epsilon, &result));
    printf("как предел = %Lf\n", result);
    printError(calculateRowPi(epsilon, &result));
    printf("как ряд = %Lf\n", result);
    printError(calculatePi(epsilon, &result));
    printf("как функция = %Lf\n", result);

    printf("---- ln2 ----\n");
    printError(calculateLimitLn2(epsilon, &result));
    printf("как предел = %Lf\n", result);
    printError(calculateRowLn2(epsilon, &result));
    printf("как ряд = %Lf\n", result);
    printError(calculateLn2(epsilon, &result));
    printf("как функция = %Lf\n", result);

    printf("---- √2 ----\n");
    printError(calculateLimitSqrt2(epsilon, &result));
    printf("как предел = %Lf\n", result);
    printError(calculateRowSqrt2(epsilon, &result));
    printf("как ряд = %Lf\n", result);
    printError(calculateSqrt2(epsilon, &result));
    printf("как функция = %Lf\n", result);
    
    printf("---- γ ----\n");
    printError(calculateLimitGamma(epsilon, &result));
    printf("как предел = %Lf\n", result);
    printError(calculateRowGamma(epsilon, &result));
    printf("как ряд = %Lf\n", result);
    printError(calculateGamma(epsilon, &result));
    printf("как функция = %Lf\n", result);
    
    return SUCCESS;
}

