
#include <stdio.h>
#include <string.h>

#define MAX_CONTACTS 100

#define FILE_NAME "contacts.txt"

typedef struct {
    char surname[50];
    char name[50];
    char patronymic[100];
    char workplace[100];
    char position[100];
    char phone[25];
    char email[100];
    char social[150];
    char messenger[150];
} Contact;

Contact contacts[MAX_CONTACTS];
int count = 0;


// =====================================
// ЧТЕНИЕ СТРОКИ С КЛАВИАТУРЫ
// =====================================

int readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return 0;
    }

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }

    return 1;
}


// =====================================
// УДАЛЕНИЕ ЦИФР ИЗ СТРОКИ
// =====================================

void removeDigits(char *str) {
    int j = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            str[j] = str[i];
            j++;
        }
    }

    str[j] = '\0';
}


// =====================================
// ПРОВЕРКА НА ПУСТУЮ СТРОКУ
// =====================================

int isEmpty(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ' && str[i] != '\t') {
            return 0;
        }
    }

    return 1;
}


// =====================================
// ОБЯЗАТЕЛЬНЫЙ ВВОД ИМЕНИ И ФАМИЛИИ
// =====================================

int readRequiredName(const char *prompt,
                     char *buffer, size_t size) {

    while (1) {
        if (!readLine(prompt, buffer, size)) {
            return 0;
        }

        removeDigits(buffer);

        if (!isEmpty(buffer)) {
            return 1;
        }

        printf("Поле обязательно! Повторите ввод.\n");
    }
}


// =====================================
// ЧТЕНИЕ ОДНОЙ СТРОКИ ИЗ ФАЙЛА
// =====================================

int readFileLine(FILE *file, char *buffer, size_t size) {

    if (fgets(buffer, size, file) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[--len] = '\0';
    } else if (!feof(file)) {
        int ch;

        while ((ch = fgetc(file)) != '\n' && ch != EOF) {
        }
    }

    // Убираем возврат каретки при Windows-формате
    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\r') {
        buffer[len - 1] = '\0';
    }

    return 1;
}


// =====================================
// СОХРАНЕНИЕ КОНТАКТОВ В ФАЙЛ
// =====================================

int saveContacts(void) {

    FILE *file = fopen(FILE_NAME, "w");

    if (file == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {

        fprintf(file, "%s\n", contacts[i].surname);
        fprintf(file, "%s\n", contacts[i].name);
        fprintf(file, "%s\n", contacts[i].patronymic);
        fprintf(file, "%s\n", contacts[i].workplace);
        fprintf(file, "%s\n", contacts[i].position);
        fprintf(file, "%s\n", contacts[i].phone);
        fprintf(file, "%s\n", contacts[i].email);
        fprintf(file, "%s\n", contacts[i].social);
        fprintf(file, "%s\n", contacts[i].messenger);
    }

    int success = !ferror(file);

    if (fclose(file) != 0) {
        success = 0;
    }

    if (success) {
        printf("Данные сохранены в файл!\n");
    } else {
        printf("Ошибка сохранения данных!\n");
    }

    return success;
}


// =====================================
// ЗАГРУЗКА КОНТАКТОВ ИЗ ФАЙЛА
// =====================================

void loadContacts(void) {

    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        printf("Файл пока не существует. "
               "Будет создан при первом сохранении.\n");
        return;
    }

    count = 0;

    while (count < MAX_CONTACTS) {

        Contact c = {0};

        if (!readFileLine(file, c.surname,
                          sizeof(c.surname))) {
            break;
        }

        if (!readFileLine(file, c.name,
                          sizeof(c.name))) break;

        if (!readFileLine(file, c.patronymic,
                          sizeof(c.patronymic))) break;

        if (!readFileLine(file, c.workplace,
                          sizeof(c.workplace))) break;

        if (!readFileLine(file, c.position,
                          sizeof(c.position))) break;

        if (!readFileLine(file, c.phone,
                          sizeof(c.phone))) break;

        if (!readFileLine(file, c.email,
                          sizeof(c.email))) break;

        if (!readFileLine(file, c.social,
                          sizeof(c.social))) break;

        if (!readFileLine(file, c.messenger,
                          sizeof(c.messenger))) break;

        if (isEmpty(c.surname) || isEmpty(c.name)) {
            printf("Некорректная запись в файле. "
                   "Загрузка остановлена.\n");
            break;
        }

        contacts[count] = c;
        count++;
    }

    fclose(file);

    printf("Загружено контактов: %d\n", count);
}


// =====================================
// ДОБАВЛЕНИЕ КОНТАКТА
// =====================================

void addContact(void) {

    if (count >= MAX_CONTACTS) {
        printf("Телефонная книга заполнена!\n");
        return;
    }

    Contact c = {0};

    if (!readRequiredName("Фамилия: ",
                          c.surname, sizeof(c.surname)))
        return;

    if (!readRequiredName("Имя: ",
                          c.name, sizeof(c.name)))
        return;

    if (!readLine("Отчество: ", c.patronymic,
                  sizeof(c.patronymic))) return;

    if (!readLine("Место работы: ", c.workplace,
                  sizeof(c.workplace))) return;

    if (!readLine("Должность: ", c.position,
                  sizeof(c.position))) return;

    if (!readLine("Телефон: ", c.phone,
                  sizeof(c.phone))) return;

    if (!readLine("Email: ", c.email,
                  sizeof(c.email))) return;

    if (!readLine("Соцсеть: ", c.social,
                  sizeof(c.social))) return;

    if (!readLine("Мессенджер: ", c.messenger,
                  sizeof(c.messenger))) return;

    contacts[count] = c;
    count++;

    printf("Контакт добавлен!\n");

    saveContacts();
}


// =====================================
// ВЫВОД ОДНОГО КОНТАКТА
// =====================================

void printContact(const Contact *c, int index) {

    printf("\n--- Контакт %d ---\n", index + 1);

    printf("Фамилия: %s\n", c->surname);
    printf("Имя: %s\n", c->name);
    printf("Отчество: %s\n", c->patronymic);
    printf("Место работы: %s\n", c->workplace);
    printf("Должность: %s\n", c->position);
    printf("Телефон: %s\n", c->phone);
    printf("Email: %s\n", c->email);
    printf("Соцсеть: %s\n", c->social);
    printf("Мессенджер: %s\n", c->messenger);
}


// =====================================
// ПРОСМОТР ВСЕХ КОНТАКТОВ
// =====================================

void showContacts(void) {

    if (count == 0) {
        printf("Телефонная книга пуста.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printContact(&contacts[i], i);
    }
}


// =====================================
// ПОИСК КОНТАКТА ПО ФАМИЛИИ
// =====================================

int findContact(void) {

    char surname[50];

    if (!readLine("Введите фамилию: ",
                  surname, sizeof(surname))) {
        return -1;
    }

    for (int i = 0; i < count; i++) {

        if (strcmp(contacts[i].surname, surname) == 0) {
            return i;
        }
    }

    return -1;
}


// =====================================
// РЕДАКТИРОВАНИЕ ОБЫЧНЫХ ПОЛЕЙ
// =====================================

int editField(const char *prompt,
              char *field, size_t size) {

    char temp[150];

    if (!readLine(prompt, temp, sizeof(temp))) {
        return 0;
    }

    if (temp[0] != '\0') {
        snprintf(field, size, "%s", temp);
    }

    return 1;
}


// =====================================
// РЕДАКТИРОВАНИЕ ИМЕНИ И ФАМИЛИИ
// =====================================

int editName(const char *prompt,
             char *field, size_t size) {

    char temp[150];

    while (1) {

        if (!readLine(prompt, temp, sizeof(temp))) {
            return 0;
        }

        if (temp[0] == '\0') {
            return 1;
        }

        removeDigits(temp);

        if (isEmpty(temp)) {
            printf("Нужно ввести буквы! "
                   "Повторите ввод.\n");
            continue;
        }

        snprintf(field, size, "%s", temp);

        return 1;
    }
}


// =====================================
// РЕДАКТИРОВАНИЕ КОНТАКТА
// =====================================

void editContact(void) {

    int i = findContact();

    if (i == -1) {
        printf("Контакт не найден.\n");
        return;
    }

    Contact *c = &contacts[i];

    printf("Оставьте поле пустым, "
           "чтобы не менять его.\n");

    if (!editName("Новая фамилия: ",
                  c->surname, sizeof(c->surname))) return;

    if (!editName("Новое имя: ",
                  c->name, sizeof(c->name))) return;

    if (!editField("Новое отчество: ",
                   c->patronymic,
                   sizeof(c->patronymic))) return;

    if (!editField("Новое место работы: ",
                   c->workplace,
                   sizeof(c->workplace))) return;

    if (!editField("Новая должность: ",
                   c->position,
                   sizeof(c->position))) return;

    if (!editField("Новый телефон: ",
                   c->phone,
                   sizeof(c->phone))) return;

    if (!editField("Новый email: ",
                   c->email,
                   sizeof(c->email))) return;

    if (!editField("Новая соцсеть: ",
                   c->social,
                   sizeof(c->social))) return;

    if (!editField("Новый мессенджер: ",
                   c->messenger,
                   sizeof(c->messenger))) return;

    printf("Контакт изменён!\n");

    saveContacts();
}


// =====================================
// УДАЛЕНИЕ КОНТАКТА
// =====================================

void deleteContact(void) {

    int i = findContact();

    if (i == -1) {
        printf("Контакт не найден.\n");
        return;
    }

    for (int j = i; j < count - 1; j++) {
        contacts[j] = contacts[j + 1];
    }

    count--;

    printf("Контакт удалён!\n");

    saveContacts();
}


// =====================================
// ГЛАВНАЯ ФУНКЦИЯ
// =====================================

int main(void) {

    int choice;
    char input[32];
    char extra;

    // Загружаем контакты при запуске
    loadContacts();

    while (1) {

        printf("\n=== ТЕЛЕФОННАЯ КНИГА ===\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать контакты\n");
        printf("3. Редактировать контакт\n");
        printf("4. Удалить контакт\n");
        printf("0. Выход\n");

        if (!readLine("Ваш выбор: ",
                      input, sizeof(input))) {
            break;
        }

        if (sscanf(input, "%d %c",
                   &choice, &extra) != 1) {

            printf("Введите число от 0 до 4.\n");
            continue;
        }

        switch (choice) {

            case 1:
                addContact();
                break;

            case 2:
                showContacts();
                break;

            case 3:
                editContact();
                break;

            case 4:
                deleteContact();
                break;

            case 0:
                printf("Выход из программы.\n");
                return 0;

            default:
                printf("Неверный пункт меню.\n");
        }
    }

    return 0;
}
