
#include <stdio.h>
#include <stdlib.h>

typedef float (*Operation)(float, float);

// Функции калькулятора
float add(float a, float b) {
    return a + b;
}

float subtract(float a, float b) {
    return a - b;
}

float multiply(float a, float b) {
    return a * b;
}

float divide(float a, float b) {
    return a / b;
}

// Массив указателей на функции
Operation operations[] = {
    add,
    subtract,
    multiply,
    divide
};

// Названия операций
const char *names[] = {
    "Сложение",
    "Вычитание",
    "Умножение",
    "Деление"
};

int main() {
    char input[100];
    int action;
    float a, b, result;

    // Автоматическое определение количества операций
    int count = sizeof(operations) / sizeof(operations[0]);

    while (1) {
        printf("\n===== КАЛЬКУЛЯТОР =====\n");

        // Автоматический вывод списка команд
        for (int i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, names[i]);
        }

        printf("0. Выход\n");
        printf("Выберите действие: ");

        fgets(input, sizeof(input), stdin);
        action = atoi(input);

        if (action == 0) {
            printf("Программа завершена.\n");
            break;
        }

        if (action < 1 || action > count) {
            printf("Неверная команда!\n");
            continue;
        }

        printf("Введите первое число: ");
        fgets(input, sizeof(input), stdin);
        a = atof(input);

        printf("Введите второе число: ");
        fgets(input, sizeof(input), stdin);
        b = atof(input);

        // Проверка деления на ноль
        if (action == 4 && b == 0) {
            printf("Ошибка! Деление на ноль.\n");
            continue;
        }

        // Вызов функции через указатель
        result = operations[action - 1](a, b);

        printf("Результат операции \"%s\": %.2f\n",
               names[action - 1], result);
    }

    return 0;
}
