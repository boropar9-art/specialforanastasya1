#include "Smartphone.h"
#include <iostream>
using namespace std;

int main()
{
    cout << "=== Демонстрация работы класса Smartphone ===" << endl;
    cout << "=== с влиянием уведомлений на память и производительность ===" << endl << endl;

    // Создаем смартфон с меньшей памятью для демонстрации
    Smartphone phone("Samsung", "Galaxy A33", 5000, 80, 2.4, 8, 350, 0.01, 1024); // 1 ГБ памяти

    cout << "1. Создан смартфон:" << endl;
    phone.printInfo();
    cout << endl;

    // Устанавливаем меньший лимит для демонстрации
    phone.setNotificationLimit(8);
    cout << "2. Установлен лимит уведомлений: 8 (для демонстрации)" << endl << endl;

    // Добавляем уведомления с разным потреблением памяти
    cout << "3. Добавление уведомлений:" << endl;
    phone += Notification{ "Email", 0.3, true, 150 };
    phone += Notification{ "Сообщения", 0.2, true, 100 };
    phone += Notification{ "Погода", 0.1, false, 50 };
    phone += Notification{ "Новости", 0.4, true, 200 };
    phone += Notification{ "Календарь", 0.1, true, 80 };
    phone += Notification{ "Музыка", 0.5, true, 300 };

    cout << endl << "После добавления 6 уведомлений:" << endl;
    phone.printDetailedInfo();
    cout << endl;

    // Показываем автоматическую очистку при превышении лимита
    cout << "4. Добавляем еще уведомления (превышение лимита):" << endl;
    phone += Notification{ "Соц.сети", 0.6, true, 250 };
    phone += Notification{ "Навигация", 0.7, true, 400 };

    cout << endl << "Состояние после автоматической очистки:" << endl;
    phone.printDetailedInfo();
    cout << endl;

    // Демонстрация нехватки памяти
    cout << "5. Пытаемся добавить большое уведомление (нехватка памяти):" << endl;
    phone += Notification{ "Игра", 0.9, true, 600 };

    cout << endl << "Текущее состояние памяти:" << endl;
    cout << "Используется: " << phone.getMemoryUsagePercent() << "%" << endl;
    cout << "Доступно: " << phone.getAvailableMemory() << " МБ" << endl << endl;

    // Используем оператор [] для управления уведомлениями
    cout << "6. Работа с оператором []:" << endl;
    try {
        cout << "Первое уведомление: " << phone[0].title << endl;
        phone[0].isActive = false; // деактивируем
        cout << "Деактивировано первое уведомление (освобождено "
            << phone[0].memoryUsage << " МБ)" << endl;
    }
    catch (const out_of_range& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
    cout << endl;

    // Используем оператор -= для удаления уведомления
    cout << "7. Удаление уведомления с помощью оператора -=:" << endl;
    cout << "До удаления: " << phone << endl;
    phone -= 2; // удаляем третье уведомление
    cout << "После удаления: " << phone << endl << endl;

    // Демонстрация работы смартфона с влиянием уведомлений
    cout << "8. Использование смартфона:" << endl;
    phone.powerOn();
    cout << endl;

    // Используем телефон - показываем влияние уведомлений на производительность
    cout << "Используем телефон на 70% мощности 15 секунд:" << endl;
    if (phone.use(70, 15)) {
        cout << "Использование успешно" << endl;
    }
    else {
        cout << "Заряд закончился" << endl;
    }

    cout << endl << "Текущее состояние:" << endl;
    cout << phone << endl << endl;

    // Демонстрация оценки времени работы
    cout << "9. Оценка времени работы:" << endl;
    cout << "При 50% мощности: " << phone.getEstimatedRuntime(50) << " секунд" << endl;
    cout << "При 100% мощности: " << phone.getEstimatedRuntime(100) << " секунд" << endl << endl;

    cout << "10. Демонстрация конструктора копирования:" << endl;
    Smartphone phoneCopy = phone;
    cout << "Скопированный смартфон:" << endl;
    phoneCopy.printInfo();

    // Изменяем оригинал (копия не должна измениться)
    phone += Notification{ "Тест", 0.1, true, 50 };
    cout << endl << "После добавления уведомления в оригинал:" << endl;

    // ВАЖНО: используем оператор << вместо прямого доступа к приватному полю
    cout << "Оригинал: " << phone << endl;
    cout << "Копия:    " << phoneCopy << endl << endl;

    // Демонстрация оператора присваивания
    cout << "11. Демонстрация оператора присваивания:" << endl;
    Smartphone phone2("Apple", "iPhone 15", 4500, 100, 3.2, 6, 400, 0.008, 2048);
    cout << "До присваивания:" << endl;
    cout << "Phone2: " << phone2 << endl;

    phone2 = phone;
    cout << "После присваивания:" << endl;
    cout << "Phone2: " << phone2 << endl << endl;

    // Демонстрация заряда
    cout << "12. Зарядка телефона:" << endl;
    phone.powerOff();
    phone.charge(30); // заряжаем 30 секунд
    cout << "После зарядки: " << phone << endl;

    return 0;
}