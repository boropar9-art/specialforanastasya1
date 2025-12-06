#pragma once
#ifndef SMARTPHONE_H
#define SMARTPHONE_H

#include <string>
#include <iostream>
using namespace std;

// Структура уведомления с влиянием на память
struct Notification {
    string title;
    double cpuLoad;      // нагрузка на процессор (0.0-1.0)
    bool isActive;       // активно ли уведомление
    int memoryUsage;     // потребление памяти в МБ (10-500)
    
    // Конструктор для удобства
    Notification(string t = "", double cpu = 0.0, bool active = false, int mem = 0)
        : title(t), cpuLoad(cpu), isActive(active), memoryUsage(mem) {}
};

class Smartphone {
private:
    string brand;
    string model;
    double batteryCapacity;           // мАч
    double currentChargeLevel;        // %
    double processorClockSpeed;       // ГГц
    int numberOfProcessorCores;
    double peakPerformance;           // GFLOPS
    double batteryConsumptionPerGFLOP; // %/GFLOP*сек
    bool isOn;
    
    // Память
    int totalMemory;                  // общая память в МБ
    int usedMemory;                   // используемая память в МБ
    
    // Уведомления
    Notification* notifications;      // динамический массив уведомлений
    int notificationCount;            // текущее количество уведомлений
    int notificationLimit;            // лимит уведомлений
    
    // Константы
    static const int DEFAULT_LIMIT = 100;
    static const int AUTO_CLEANUP_COUNT = 20;
    static const int DEFAULT_TOTAL_MEMORY = 8192; // 8 ГБ

public:
    // Конструкторы и деструктор
    Smartphone();
    Smartphone(string brand, string mdl, double capacity, double chargeLevel,
               double clockSpeed, int cores, double perf, double cons,
               int totalMem = DEFAULT_TOTAL_MEMORY);
    Smartphone(const Smartphone& other);        // конструктор копирования
    ~Smartphone();                               // деструктор

    // Оператор присваивания
    Smartphone& operator=(const Smartphone& other);

    // Основные методы
    void powerOn();
    void powerOff();
    void charge(double seconds);
    bool use(double percent, double seconds);  // учитывает активные уведомления
    double getEstimatedRuntime(double percent) const;
    void printInfo() const;
    void printDetailedInfo() const;
    
    // Методы работы с памятью
    int getAvailableMemory() const;
    double getMemoryUsagePercent() const;
    bool hasEnoughMemory(int required) const;
    
    // Методы работы с уведомлениями
    void addNotification(const Notification& n);
    bool removeNotification(int index);
    void cleanupOldNotifications(int keepCount);
    void cleanupByMemory(); // очистка при нехватке памяти
    void setNotificationLimit(int limit);
    int getNotificationLimit() const;
    
    // Подсчёт суммарной нагрузки активных уведомлений
    double totalActiveCpuLoad() const;
    int totalActiveMemoryUsage() const;
    
    // Влияние уведомлений на производительность
    double getCurrentPerformance() const; // текущая производительность с учетом нагрузки
    
    // Перегрузка операторов
    friend ostream& operator<<(ostream& os, const Smartphone& phone);
    Smartphone& operator+=(const Notification& n);  // добавить уведомление
    Smartphone& operator-=(int index);              // удалить уведомление по индексу
    Notification& operator[](int index);             // доступ по индексу
    const Notification& operator[](int index) const; // константный доступ
};

#endif