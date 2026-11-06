
def add(a, b):
    return a + b

def subtract(a, b):
    return a - b

def multiply(a, b):
    return a * b

def divide(a, b):
    if b == 0:
        return "Ошибка! Деление на ноль."
    return a / b


# Главное меню программы
while True:
    print("\n===== КАЛЬКУЛЯТОР =====")
    print("1. Сложение")
    print("2. Вычитание")
    print("3. Умножение")
    print("4. Деление")
    print("0. Выход")

    action = input("Выберите действие: ")

    if action == "0":
        print("Программа завершена.")
        break

    if action not in ["1", "2", "3", "4"]:
        print("Неверный выбор! Попробуйте снова.")
        continue

    a = float(input("Введите первое число: "))
    b = float(input("Введите второе число: "))

    if action == "1":
        result = add(a, b)
    elif action == "2":
        result = subtract(a, b)
    elif action == "3":
        result = multiply(a, b)
    elif action == "4":
        result = divide(a, b)

    print("Результат:", result)
