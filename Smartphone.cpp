#include "Smartphone.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

// Статические константы
const int Smartphone::DEFAULT_LIMIT;
const int Smartphone::AUTO_CLEANUP_COUNT;
const int Smartphone::DEFAULT_TOTAL_MEMORY;

// --------------------
// Конструкторы
// --------------------
Smartphone::Smartphone()
    : brand("Unknown"), model("Unknown"), batteryCapacity(0),
    currentChargeLevel(0), processorClockSpeed(0),
    numberOfProcessorCores(0), peakPerformance(0),
    batteryConsumptionPerGFLOP(0), isOn(false),
    totalMemory(DEFAULT_TOTAL_MEMORY), usedMemory(0),
    notifications(nullptr), notificationCount(0),
    notificationLimit(DEFAULT_LIMIT) {
}

Smartphone::Smartphone(string brand, string mdl, double capacity,
    double chargeLevel, double clockSpeed, int cores,
    double perf, double cons, int totalMem)
    : brand(brand), model(mdl), batteryCapacity(capacity),
    currentChargeLevel(chargeLevel), processorClockSpeed(clockSpeed),
    numberOfProcessorCores(cores), peakPerformance(perf),
    batteryConsumptionPerGFLOP(cons), isOn(false),
    totalMemory(totalMem), usedMemory(0),
    notifications(nullptr), notificationCount(0),
    notificationLimit(DEFAULT_LIMIT) {
}

// --------------------
// Конструктор копирования (глубокое копирование)
// --------------------
Smartphone::Smartphone(const Smartphone& other)
    : brand(other.brand), model(other.model),
    batteryCapacity(other.batteryCapacity),
    currentChargeLevel(other.currentChargeLevel),
    processorClockSpeed(other.processorClockSpeed),
    numberOfProcessorCores(other.numberOfProcessorCores),
    peakPerformance(other.peakPerformance),
    batteryConsumptionPerGFLOP(other.batteryConsumptionPerGFLOP),
    isOn(other.isOn),
    totalMemory(other.totalMemory),
    usedMemory(other.usedMemory),
    notificationLimit(other.notificationLimit),
    notifications(nullptr), notificationCount(0)
{
    notificationCount = other.notificationCount;
    if (notificationCount > 0) {
        notifications = new Notification[notificationCount];
        for (int i = 0; i < notificationCount; i++) {
            notifications[i] = other.notifications[i];
        }
    }
}

// --------------------
// Деструктор
// --------------------
Smartphone::~Smartphone()
{
    delete[] notifications;
}

// --------------------
// Оператор присваивания
// --------------------
Smartphone& Smartphone::operator=(const Smartphone& other)
{
    if (this == &other)
        return *this;

    delete[] notifications;

    brand = other.brand;
    model = other.model;
    batteryCapacity = other.batteryCapacity;
    currentChargeLevel = other.currentChargeLevel;
    processorClockSpeed = other.processorClockSpeed;
    numberOfProcessorCores = other.numberOfProcessorCores;
    peakPerformance = other.peakPerformance;
    batteryConsumptionPerGFLOP = other.batteryConsumptionPerGFLOP;
    isOn = other.isOn;
    totalMemory = other.totalMemory;
    usedMemory = other.usedMemory;
    notificationLimit = other.notificationLimit;

    notificationCount = other.notificationCount;
    if (notificationCount > 0) {
        notifications = new Notification[notificationCount];
        for (int i = 0; i < notificationCount; i++) {
            notifications[i] = other.notifications[i];
        }
    }
    else {
        notifications = nullptr;
    }

    return *this;
}

// --------------------
// Методы работы с памятью
// --------------------
int Smartphone::getAvailableMemory() const {
    return totalMemory - usedMemory;
}

double Smartphone::getMemoryUsagePercent() const {
    return (totalMemory > 0) ? (usedMemory * 100.0 / totalMemory) : 0.0;
}

bool Smartphone::hasEnoughMemory(int required) const {
    return getAvailableMemory() >= required;
}

// --------------------
// Методы управления уведомлениями
// --------------------
void Smartphone::setNotificationLimit(int limit) {
    if (limit > 0) {
        notificationLimit = limit;
    }
}

int Smartphone::getNotificationLimit() const {
    return notificationLimit;
}

void Smartphone::addNotification(const Notification& n)
{
    // Проверяем, не превышен ли лимит
    if (notificationCount >= notificationLimit) {
        // Автоматически удаляем старые уведомления
        cleanupOldNotifications(notificationLimit - AUTO_CLEANUP_COUNT);
    }

    // Проверяем память для активных уведомлений
    if (n.isActive) {
        if (!hasEnoughMemory(n.memoryUsage)) {
            cout << "Недостаточно памяти (" << n.memoryUsage << " МБ требуется, "
                << getAvailableMemory() << " МБ доступно)!" << endl;
            cleanupByMemory(); // пытаемся освободить память
        }

        if (hasEnoughMemory(n.memoryUsage)) {
            usedMemory += n.memoryUsage;
        }
        else {
            cout << "Не удалось освободить достаточно памяти. Уведомление не добавлено." << endl;
            return;
        }
    }

    // Создаем новый массив на 1 элемент больше
    Notification* newArr = new Notification[notificationCount + 1];

    // Копируем существующие уведомления
    for (int i = 0; i < notificationCount; i++) {
        newArr[i] = notifications[i];
    }

    // Добавляем новое уведомление в конец
    newArr[notificationCount] = n;

    // Освобождаем старый массив и заменяем на новый
    delete[] notifications;
    notifications = newArr;
    notificationCount++;

    cout << "Добавлено уведомление: " << n.title
        << " (CPU: " << n.cpuLoad
        << ", Память: " << n.memoryUsage << " МБ)" << endl;
}

bool Smartphone::removeNotification(int index) {
    if (index < 0 || index >= notificationCount) {
        cout << "Ошибка: неверный индекс уведомления" << endl;
        return false;
    }

    // Освобождаем память, если уведомление было активным
    if (notifications[index].isActive) {
        usedMemory -= notifications[index].memoryUsage;
    }

    // Создаем новый массив на 1 элемент меньше
    Notification* newArr = new Notification[notificationCount - 1];

    // Копируем все элементы кроме удаляемого
    for (int i = 0, j = 0; i < notificationCount; i++) {
        if (i != index) {
            newArr[j++] = notifications[i];
        }
    }

    // Освобождаем старый массив и заменяем на новый
    delete[] notifications;
    notifications = newArr;
    notificationCount--;

    cout << "Удалено уведомление с индексом " << index << endl;
    return true;
}

void Smartphone::cleanupOldNotifications(int keepCount) {
    if (keepCount <= 0) {
        // Освобождаем всю память перед удалением
        for (int i = 0; i < notificationCount; i++) {
            if (notifications[i].isActive) {
                usedMemory -= notifications[i].memoryUsage;
            }
        }

        delete[] notifications;
        notifications = nullptr;
        notificationCount = 0;
        cout << "Удалены все уведомления" << endl;
        return;
    }

    if (notificationCount <= keepCount) {
        return; // ничего не делаем
    }

    int removeCount = notificationCount - keepCount;

    // Освобождаем память удаляемых активных уведомлений
    for (int i = 0; i < removeCount; i++) {
        if (notifications[i].isActive) {
            usedMemory -= notifications[i].memoryUsage;
        }
    }

    // Создаем новый массив только с последними keepCount уведомлениями
    Notification* newArr = new Notification[keepCount];

    // Копируем только последние keepCount уведомлений
    for (int i = removeCount, j = 0; i < notificationCount; i++, j++) {
        newArr[j] = notifications[i];
    }

    delete[] notifications;
    notifications = newArr;
    notificationCount = keepCount;

    cout << "Автоматически удалено " << removeCount
        << " старых уведомлений (оставлено " << keepCount << ")" << endl;
}

void Smartphone::cleanupByMemory() {
    cout << "Очистка памяти..." << endl;

    // Сначала пытаемся деактивировать уведомления
    bool deactivated = false;
    for (int i = 0; i < notificationCount; i++) {
        if (notifications[i].isActive && notifications[i].memoryUsage > 0) {
            usedMemory -= notifications[i].memoryUsage;
            notifications[i].isActive = false;
            deactivated = true;
            cout << "Деактивировано: " << notifications[i].title << endl;
        }
    }

    if (deactivated) {
        cout << "Деактивированы некоторые уведомления для освобождения памяти" << endl;
        return;
    }

    // Если деактивации недостаточно, удаляем неактивные уведомления
    int inactiveCount = 0;
    for (int i = 0; i < notificationCount; i++) {
        if (!notifications[i].isActive) {
            inactiveCount++;
        }
    }

    if (inactiveCount > 0) {
        // Создаем новый массив только с активными уведомлениями
        Notification* newArr = new Notification[notificationCount - inactiveCount];
        int newIndex = 0;

        for (int i = 0; i < notificationCount; i++) {
            if (notifications[i].isActive) {
                newArr[newIndex++] = notifications[i];
            }
        }

        delete[] notifications;
        notifications = newArr;
        notificationCount -= inactiveCount;

        cout << "Удалено " << inactiveCount << " неактивных уведомлений" << endl;
    }
    else {
        cout << "Не удалось освободить память - все уведомления активны" << endl;
    }
}

// --------------------
// Влияние уведомлений на производительность
// --------------------
double Smartphone::totalActiveCpuLoad() const {
    double total = 0.0;
    for (int i = 0; i < notificationCount; i++) {
        if (notifications[i].isActive) {
            total += notifications[i].cpuLoad;
        }
    }
    return min(total, 1.0); // ограничиваем максимальной нагрузкой 100%
}

int Smartphone::totalActiveMemoryUsage() const {
    int total = 0;
    for (int i = 0; i < notificationCount; i++) {
        if (notifications[i].isActive) {
            total += notifications[i].memoryUsage;
        }
    }
    return total;
}

double Smartphone::getCurrentPerformance() const {
    double basePerformance = peakPerformance;

    // Уведомления снижают производительность:
    // - Каждое активное уведомление снижает производительность на 5%
    // - При нехватке памяти (<10% свободно) - дополнительное снижение 20%
    double performanceMultiplier = 1.0;

    // Влияние CPU нагрузки от уведомлений
    performanceMultiplier -= totalActiveCpuLoad() * 0.3; // до 30% снижения

    // Влияние нехватки памяти
    if (getMemoryUsagePercent() > 90.0) {
        performanceMultiplier -= 0.2; // дополнительно 20% при нехватке памяти
    }

    return max(0.1, basePerformance * performanceMultiplier); // минимум 10% производительности
}

// --------------------
// Основные методы телефона
// --------------------
void Smartphone::powerOn() {
    if (currentChargeLevel > 0) {
        // При включении проверяем память
        if (getAvailableMemory() < 100) { // нужно минимум 100 МБ для работы системы
            cout << "Мало памяти для загрузки! Очистка..." << endl;
            cleanupByMemory();
        }

        if (getAvailableMemory() >= 100) {
            isOn = true;
            cout << brand << " " << model << " включен" << endl;
        }
        else {
            cout << "Недостаточно памяти для включения!" << endl;
        }
    }
    else {
        cout << "Невозможно включить: разряжен аккумулятор" << endl;
    }
}

void Smartphone::powerOff() {
    if (isOn) {
        // При выключении сохраняем активные уведомления
        cout << "Выключение... Сохранено " << notificationCount << " уведомлений" << endl;
        isOn = false;
    }
}

void Smartphone::charge(double seconds) {
    double rate;
    if (isOn)
        rate = 0.2; // медленнее при работе
    else
        rate = 0.4; // быстрее при выключенном

    currentChargeLevel += rate * seconds;
    if (currentChargeLevel > 100) currentChargeLevel = 100;

    cout << "Зарядка: " << seconds << " сек. Текущий заряд: "
        << currentChargeLevel << "%" << endl;
}

bool Smartphone::use(double percent, double seconds) {
    if (!isOn) {
        cout << "Телефон выключен!" << endl;
        return false;
    }

    // Используем текущую производительность (учитывает уведомления)
    double currentPerf = getCurrentPerformance();
    double perf = currentPerf * (percent / 100.0);
    double consumption = perf * batteryConsumptionPerGFLOP * seconds;

    // Добавляем потребление от активных уведомлений
    consumption += totalActiveCpuLoad() * seconds * 0.5;

    if (consumption >= currentChargeLevel) {
        currentChargeLevel = 0;
        isOn = false;
        cout << "Заряд закончился! Телефон выключен." << endl;
        return false;
    }

    currentChargeLevel -= consumption;

    // Показываем влияние уведомлений на производительность
    double performanceRatio = getCurrentPerformance() / peakPerformance;
    cout << "Использование: " << seconds << " сек. на " << percent << "% мощности" << endl;
    cout << "Текущая производительность: " << (performanceRatio * 100)
        << "% от максимальной" << endl;
    cout << "Осталось заряда: " << currentChargeLevel << "%" << endl;

    return true;
}

double Smartphone::getEstimatedRuntime(double percent) const {
    if (!isOn || currentChargeLevel <= 0) return 0;

    // Учитываем текущую производительность с уведомлениями
    double currentPerf = getCurrentPerformance();
    double perf = currentPerf * (percent / 100.0);
    double rate = perf * batteryConsumptionPerGFLOP + totalActiveCpuLoad() * 0.5;

    if (rate <= 0) return 1e9;
    return currentChargeLevel / rate;
}

// --------------------
// Вывод информации
// --------------------
void Smartphone::printInfo() const {
    cout << brand << " " << model << endl;
    cout << "Заряд: " << currentChargeLevel << "%" << endl;
    cout << "Статус: " << (isOn ? "Включен" : "Выключен") << endl;
    cout << "Память: " << usedMemory << "/" << totalMemory
        << " МБ (" << getMemoryUsagePercent() << "%)" << endl;
    cout << "Уведомлений: " << notificationCount << "/" << notificationLimit << endl;
    cout << "Текущая производительность: "
        << (getCurrentPerformance() / peakPerformance * 100) << "%" << endl;
}

void Smartphone::printDetailedInfo() const {
    printInfo();

    if (notificationCount > 0) {
        cout << "\nДетали уведомлений:" << endl;
        cout << "==================" << endl;
        for (int i = 0; i < notificationCount; i++) {
            cout << "[" << i << "] " << notifications[i].title
                << " | CPU: " << notifications[i].cpuLoad
                << " | Память: " << notifications[i].memoryUsage << " МБ"
                << " | Статус: " << (notifications[i].isActive ? "Активно" : "Неактивно")
                << endl;
        }
        cout << "Суммарная нагрузка CPU: " << totalActiveCpuLoad() * 100 << "%" << endl;
        cout << "Используемая память уведомлениями: " << totalActiveMemoryUsage() << " МБ" << endl;
    }
    else {
        cout << "\nУведомлений нет" << endl;
    }
}

// --------------------
// Перегрузка операторов
// --------------------

// Перегрузка += для добавления уведомления
Smartphone& Smartphone::operator+=(const Notification& n)
{
    addNotification(n);
    return *this;
}

// Перегрузка -= для удаления уведомления по индексу
Smartphone& Smartphone::operator-=(int index)
{
    removeNotification(index);
    return *this;
}

// Перегрузка [] для доступа к уведомлениям (неконстантная версия)
Notification& Smartphone::operator[](int index)
{
    if (index < 0 || index >= notificationCount)
        throw out_of_range("Некорректный индекс уведомления");
    return notifications[index];
}

// Перегрузка [] для доступа к уведомлениям (константная версия)
const Notification& Smartphone::operator[](int index) const
{
    if (index < 0 || index >= notificationCount)
        throw out_of_range("Некорректный индекс уведомления");
    return notifications[index];
}

// Перегрузка << для вывода в поток
ostream& operator<<(ostream& os, const Smartphone& phone)
{
    os << phone.brand << " " << phone.model
        << " | Заряд: " << phone.currentChargeLevel << "%"
        << " | Память: " << phone.getMemoryUsagePercent() << "%"
        << " | Уведомлений: " << phone.notificationCount << "/" << phone.notificationLimit
        << " | Производительность: "
        << (phone.getCurrentPerformance() / phone.peakPerformance * 100) << "%";
    return os;
}