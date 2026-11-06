
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Перевод цифровых прав в буквенный формат
void numeric_to_symbolic(int mode, char *result) {
    char symbols[] = "rwxrwxrwx";

    for (int i = 0; i < 9; i++) {
        result[i] = (mode & (1 << (8 - i))) ? symbols[i] : '-';
    }
    result[9] = '\0';
}

// Перевод цифровых прав в битовый формат
void numeric_to_binary(int mode, char *result) {
    for (int i = 0; i < 9; i++) {
        result[i] = (mode & (1 << (8 - i))) ? '1' : '0';
    }
    result[9] = '\0';
}

// Перевод буквенных прав в цифровые
int symbolic_to_numeric(char *str) {
    int mode = 0;

    if (strlen(str) != 9)
        return -1;

    char expected[] = "rwxrwxrwx";

    for (int i = 0; i < 9; i++) {
        if (str[i] == expected[i])
            mode |= (1 << (8 - i));
        else if (str[i] != '-')
            return -1;
    }

    return mode;
}

// Вывод всех представлений прав
void print_permissions(int mode) {
    char symbolic[10], binary[10];

    numeric_to_symbolic(mode, symbolic);
    numeric_to_binary(mode, binary);

    printf("Буквенный формат: %s\n", symbolic);
    printf("Цифровой формат: %03o\n", mode);
    printf("Битовый формат:  %s\n", binary);
}

int main() {
    int choice;

    do {
        printf("\n===== МАСКА ПРАВ ДОСТУПА =====\n");
        printf("1. Перевод прав доступа\n");
        printf("2. Получение прав файла (stat)\n");
        printf("3. Изменение прав (имитация chmod)\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char input[20];
            int mode;

            printf("Введите права (например, 755 или rwxr-xr-x): ");
            scanf("%19s", input);

            if (strlen(input) == 9) {
                mode = symbolic_to_numeric(input);
            } else {
                char *end;
                mode = (int)strtol(input, &end, 8);

                if (*end != '\0' || mode < 0 || mode > 0777)
                    mode = -1;
            }

            if (mode < 0) {
                printf("Ошибка! Неверный формат прав.\n");
            } else {
                print_permissions(mode);
            }
        }

        else if (choice == 2) {
            char filename[256];
            struct stat file_info;

            printf("Введите имя файла: ");
            scanf("%255s", filename);

            if (stat(filename, &file_info) == -1) {
                perror("Ошибка stat");
            } else {
                int mode = file_info.st_mode & 0777;

                printf("\nИнформация о файле: %s\n", filename);
                print_permissions(mode);

                printf("\nСравните с командой: ls -l %s\n", filename);
            }
        }

        else if (choice == 3) {
            char input[20];
            int mode;

            printf("Введите текущие права (например, 755): ");
            scanf("%19s", input);

            if (strlen(input) == 9) {
                mode = symbolic_to_numeric(input);
            } else {
                char *end;
                mode = (int)strtol(input, &end, 8);

                if (*end != '\0' || mode < 0 || mode > 0777)
                    mode = -1;
            }

            if (mode < 0) {
                printf("Ошибка! Неверные права.\n");
                continue;
            }

            printf("Введите команду (например, u+x, g-w, o+r): ");

            scanf("%19s", input);

            // Имитация изменения прав chmod
            for (int i = 0; input[i] != '\0'; i++) {
                if (input[i] == '+') {
                    int bit = 0;

                    if (i + 1 < strlen(input)) {
                        if (input[i+1] == 'r') bit = 4;
                        if (input[i+1] == 'w') bit = 2;
                        if (input[i+1] == 'x') bit = 1;
                    }

                    if (input[0] == 'u') mode |= bit << 6;
                    if (input[0] == 'g') mode |= bit << 3;
                    if (input[0] == 'o') mode |= bit;
                }

                if (input[i] == '-') {
                    int bit = 0;

                    if (i + 1 < strlen(input)) {
                        if (input[i+1] == 'r') bit = 4;
                        if (input[i+1] == 'w') bit = 2;
                        if (input[i+1] == 'x') bit = 1;
                    }

                    if (input[0] == 'u') mode &= ~(bit << 6);
                    if (input[0] == 'g') mode &= ~(bit << 3);
                    if (input[0] == 'o') mode &= ~bit;
                }
            }

            printf("\nНовые права (файл не изменён):\n");
            print_permissions(mode);
        }

    } while (choice != 0);

    return 0;
}
