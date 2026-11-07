
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// Преобразование IP-адреса в число
uint32_t ipToNumber(int a, int b, int c, int d) {
    return ((uint32_t)a << 24) |
           ((uint32_t)b << 16) |
           ((uint32_t)c << 8)  |
           (uint32_t)d;
}

// Проверка принадлежности к одной подсети
int main() {
    int a, b, c, d;
    int m1, m2, m3, m4;
    int N;

    printf("Введите IP-адрес шлюза: ");
    scanf("%d.%d.%d.%d", &a, &b, &c, &d);

    printf("Введите маску подсети: ");
    scanf("%d.%d.%d.%d", &m1, &m2, &m3, &m4);

    printf("Введите количество пакетов N: ");
    scanf("%d", &N);

    if (a < 0 || a > 255 || b < 0 || b > 255 ||
        c < 0 || c > 255 || d < 0 || d > 255 ||
        m1 < 0 || m1 > 255 || m2 < 0 || m2 > 255 ||
        m3 < 0 || m3 > 255 || m4 < 0 || m4 > 255 ||
        N <= 0) {
        printf("Ошибка ввода!\n");
        return 1;
    }

    uint32_t gateway = ipToNumber(a, b, c, d);
    uint32_t mask = ipToNumber(m1, m2, m3, m4);

    int own = 0;
    int other = 0;

    srand(time(NULL));

    printf("\nРезультаты обработки пакетов:\n");

    for (int i = 0; i < N; i++) {
        int x = rand() % 256;
        int y = rand() % 256;
        int z = rand() % 256;
        int w = rand() % 256;

        uint32_t ip = ipToNumber(x, y, z, w);

        if ((gateway & mask) == (ip & mask)) {
            own++;
            printf("%d.%d.%d.%d - своя подсеть\n",
                   x, y, z, w);
        } else {
            other++;
            printf("%d.%d.%d.%d - другая сеть\n",
                   x, y, z, w);
        }
    }

    printf("\nСтатистика:\n");
    printf("Своя подсеть: %d пакетов (%.2f%%)\n",
           own, own * 100.0 / N);

    printf("Другие сети: %d пакетов (%.2f%%)\n",
           other, other * 100.0 / N);

    return 0;
}
