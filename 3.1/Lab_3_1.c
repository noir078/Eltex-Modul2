
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// ==========================================
// 1. ПРЕОБРАЗОВАНИЕ ПРАВ ДОСТУПА
// ==========================================

// Цифровой формат -> буквенный формат
void numeric_to_symbolic(int mode, char *result) {
    char symbols[] = "rwxrwxrwx";

    for (int i = 0; i < 9; i++) {
        result[i] = (mode & (1 << (8 - i)))
                    ? symbols[i] : '-';
    }

    result[9] = '\0';
}

// Цифровой формат -> битовый формат
void numeric_to_binary(int mode, char *result) {
    for (int i = 0; i < 9; i++) {
        result[i] = (mode & (1 << (8 - i)))
                    ? '1' : '0';
    }

    result[9] = '\0';
}

// Буквенный формат -> цифровой формат
int symbolic_to_numeric(const char *str) {
    int mode = 0;
    char expected[] = "rwxrwxrwx";

    if (strlen(str) != 9)
        return -1;

    for (int i = 0; i < 9; i++) {
        if (str[i] == expected[i]) {
            mode |= (1 << (8 - i));
        }
        else if (str[i] != '-') {
            return -1;
        }
    }

    return mode;
}

// Проверка и преобразование введённых прав
int parse_mode(const char *input) {
    if (strlen(input) == 9)
        return symbolic_to_numeric(input);

    int len = strlen(input);

    if (len < 1 || len > 4)
        return -1;

    // Проверяем, что введены только цифры 0-7
    for (int i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '7')
            return -1;
    }

    char *end;
    long value = strtol(input, &end, 8);

    if (*end != '\0' || value > 0777)
        return -1;

    return (int)value;
}

// Вывод прав в трёх форматах
void print_permissions(int mode) {
    char symbolic[10];
    char binary[10];

    numeric_to_symbolic(mode, symbolic);
    numeric_to_binary(mode, binary);

    printf("\nБуквенный формат: %s\n", symbolic);
    printf("Цифровой формат:  %03o\n", mode);
    printf("Битовый формат:   %s\n", binary);
}

// ==========================================
// 2. ИМИТАЦИЯ КОМАНДЫ CHMOD
// ==========================================

// Применение одной или нескольких команд chmod
// Пример: u+rw,g-r,o+x
int apply_chmod(int *mode, const char *command) {

    int new_mode = *mode;
    const char *p = command;

    // Обрабатываем команды через запятую
    while (*p != '\0') {

        int who = 0;
        int permissions = 0;
        char operation;

        // Определяем пользователя
        while (*p == 'u' || *p == 'g' ||
               *p == 'o' || *p == 'a') {

            if (*p == 'u')
                who |= 4;

            if (*p == 'g')
                who |= 2;

            if (*p == 'o')
                who |= 1;

            if (*p == 'a')
                who = 7;

            p++;
        }

        // Если категория не указана,
        // считаем, что выбраны все
        if (who == 0)
            who = 7;

        // Проверяем оператор +, - или =
        if (*p != '+' && *p != '-' && *p != '=') {
            printf("Ошибка: ожидался оператор +, - или =\n");
            return 0;
        }

        operation = *p;
        p++;

        // Получаем права r, w, x
        while (*p != '\0' && *p != ',') {

            int bit = 0;

            if (*p == 'r')
                bit = 4;
            else if (*p == 'w')
                bit = 2;
            else if (*p == 'x')
                bit = 1;
            else {
                printf("Ошибка: неизвестное право '%c'\n", *p);
                return 0;
            }

            permissions |= bit;
            p++;
        }

        // Для + и - обязательно нужно указать право
        if (operation != '=' && permissions == 0) {
            printf("Ошибка: после + или - укажите r, w или x\n");
            return 0;
        }

        // Создаём маски для выбранных категорий
        int permission_mask = 0;
        int clear_mask = 0;

        if (who & 4) {
            permission_mask |= permissions << 6;
            clear_mask |= 7 << 6;
        }

        if (who & 2) {
            permission_mask |= permissions << 3;
            clear_mask |= 7 << 3;
        }

        if (who & 1) {
            permission_mask |= permissions;
            clear_mask |= 7;
        }

        // Выполняем операцию
        if (operation == '+') {
            new_mode |= permission_mask;
        }

        else if (operation == '-') {
            new_mode &= ~permission_mask;
        }

        else if (operation == '=') {
            new_mode &= ~clear_mask;
            new_mode |= permission_mask;
        }

        // Переходим к следующей команде
        if (*p == ',') {
            p++;

            if (*p == '\0') {
                printf("Ошибка: пустая команда после запятой\n");
                return 0;
            }
        }
    }

    // Сохраняем результат
    *mode = new_mode;

    return 1;
}

// ==========================================
// 3. ПОЛУЧЕНИЕ ПРАВ ФАЙЛА ЧЕРЕЗ STAT
// ==========================================

void get_file_permissions() {
    char filename[256];
    struct stat file_info;

    printf("Введите имя файла: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL)
        return;

    filename[strcspn(filename, "\n")] = '\0';

    if (stat(filename, &file_info) == -1) {
        perror("Ошибка stat");
        return;
    }

    int mode = file_info.st_mode & 0777;

    printf("\nИнформация о файле: %s\n", filename);

    print_permissions(mode);

    printf("\nДля сравнения выполните:\n");
    printf("ls -l \"%s\"\n", filename);
}

// ==========================================
// 4. РЕЖИМ ПОСЛЕДОВАТЕЛЬНОГО ИЗМЕНЕНИЯ
// ==========================================

void chmod_simulator() {
    char input[100];
    char command[100];

    int initial_mode;
    int mode;

    printf("\nВведите начальные права (например, 777): ");

    if (fgets(input, sizeof(input), stdin) == NULL)
        return;

    input[strcspn(input, "\n")] = '\0';

    initial_mode = parse_mode(input);

    if (initial_mode < 0) {
        printf("Ошибка: неправильные права доступа.\n");
        return;
    }

    // Текущие права
    mode = initial_mode;

    printf("\nНачальные права:\n");
    print_permissions(mode);

    // Бесконечный цикл изменения прав
    while (1) {

        printf("\n===== CHMOD СИМУЛЯТОР =====\n");
        printf("Текущие права: %03o\n", mode);

        printf("\nДоступные команды:\n");
        printf("u+rw       - добавить права владельцу\n");
        printf("g-w        - убрать запись у группы\n");
        printf("a+x        - добавить выполнение всем\n");
        printf("u=rw,g=r   - заменить права нескольким группам\n");
        printf("show       - показать права\n");
        printf("reset      - вернуть начальные права\n");
        printf("back       - вернуться в главное меню\n");

        printf("\nВведите команду: ");

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        // Показать текущие права
        if (strcmp(command, "show") == 0) {
            print_permissions(mode);
            continue;
        }

        // Вернуть начальные права
        if (strcmp(command, "reset") == 0) {
            mode = initial_mode;

            printf("Права восстановлены.\n");
            print_permissions(mode);
            continue;
        }

        // Выход в главное меню
        if (strcmp(command, "back") == 0) {
            break;
        }

        // Применение команды chmod
        if (apply_chmod(&mode, command)) {
            printf("\nКоманда успешно применена!\n");
            printf("Новые права:\n");
            print_permissions(mode);
        }
        else {
            printf("Права не изменены.\n");
        }
    }
}

// ==========================================
// 5. ГЛАВНАЯ ФУНКЦИЯ
// ==========================================

int main() {
    char input[20];
    int choice;

    while (1) {

        printf("\n========== МЕНЮ ==========\n");
        printf("1. Перевод прав доступа\n");
        printf("2. Получение прав файла (stat)\n");
        printf("3. Имитация chmod\n");
        printf("0. Выход\n");

        printf("\nВаш выбор: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        choice = atoi(input);

        switch (choice) {

            case 1: {
                char permissions[20];
                int mode;

                printf("Введите права (755 или rwxr-xr-x): ");

                if (fgets(permissions, sizeof(permissions), stdin) == NULL)
                    break;

                permissions[strcspn(permissions, "\n")] = '\0';

                mode = parse_mode(permissions);

                if (mode < 0) {
                    printf("Ошибка: неправильный формат прав.\n");
                }
                else {
                    print_permissions(mode);
                }

                break;
            }

            case 2:
                get_file_permissions();
                break;

            case 3:
                chmod_simulator();
                break;

            case 0:
                printf("Программа завершена.\n");
                return 0;

            default:
                printf("Неверный пункт меню.\n");
        }
    }

    return 0;
}
