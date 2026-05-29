//
//  main.cpp
//  курсовая.cpp
// Гостиничный бизнес.Обслуживание номеров, платные услуги, работа с заявками
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// МАКСИМАЛЬНЫЕ РАЗМЕРЫ
const int MAX_ROOMS = 100;      // максимум номеров
const int MAX_SERVICES = 50;    // максимум услуг
const int MAX_BOOKINGS = 200;   // максимум заявок

// СТРУКТУРЫ
struct Room {
    int id;                 // номер комнаты
    string type;            // тип: "economy", "standard", "luxe"
    double pricePerNight;   // цена за ночь
    int capacity;           // вместимость
    bool isAvailable;       // true - свободен, false - занят
};

struct Service {
    string name;            // название услуги
    double price;           // цена услуги
};

struct Booking {
    int bookingId;          // уникальный номер заявки
    int roomId;             // ID номера, который забронировали
    string clientName;      // имя клиента
    int nights;             // количество ночей
    double totalPrice;      // итоговая стоимость
};

// ГЛОБАЛЬНЫЕ МАССИВЫ
Room rooms[MAX_ROOMS];
int roomsCount = 0;

Service services[MAX_SERVICES];
int servicesCount = 0;

Booking bookings[MAX_BOOKINGS];
int bookingsCount = 0;

int nextBookingId = 1;

// ФУНКЦИЯ ДЛЯ ПРОВЕРКИ ВВОДА ЧИСЛА
int getIntInput(int minValue, int maxValue) {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value < minValue || value > maxValue) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Введите число от " << minValue << " до " << maxValue << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// ФУНКЦИИ ДЛЯ РАБОТЫ С НОМЕРАМИ
void addRoom() {
    if (roomsCount >= MAX_ROOMS) {
        cout << "Ошибка! Достигнут лимит номеров (" << MAX_ROOMS << ").\n";
        return;
    }

    Room newRoom;

    cout << "Введите ID номера: ";
    cin >> newRoom.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очищаем буфер после ввода числа

    for (int i = 0; i < roomsCount; i++) {
        if (rooms[i].id == newRoom.id) {
            cout << "Ошибка! Номер с ID " << newRoom.id << " уже существует.\n";
            return;
        }
    }

    cout << "Введите тип (economy/standard/luxe): ";
    getline(cin, newRoom.type);

    while (newRoom.type != "economy" && newRoom.type != "standard" && newRoom.type != "luxe") {
        cout << "Ошибка! Тип должен быть: economy, standard или luxe. Повторите ввод: ";
        getline(cin, newRoom.type);
    }

    if (newRoom.type == "economy") {
        newRoom.pricePerNight = 1000;
    } else if (newRoom.type == "standard") {
        newRoom.pricePerNight = 2500;
    } else if (newRoom.type == "luxe") {
        newRoom.pricePerNight = 5000;
    }

    cout << "Цена автоматически установлена: " << newRoom.pricePerNight << " руб. за ночь\n";

    cout << "Введите вместимость (количество человек): ";
    cin >> newRoom.capacity;

    while (newRoom.capacity < 1 || newRoom.capacity > 8) {
        cout << "Ошибка! Вместимость должна быть от 1 до 8 человек. Повторите ввод: ";
        cin >> newRoom.capacity;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // чистим буфер перед возвратом в меню

    newRoom.isAvailable = true;
    rooms[roomsCount] = newRoom;
    roomsCount++;

    cout << "Номер " << newRoom.id << " успешно добавлен!\n";
}

void deleteRoom() {
    if (roomsCount == 0) {
        cout << "Нет номеров для удаления.\n";
        return;
    }

    int idToDelete;
    cout << "Введите ID номера для удаления: ";
    cin >> idToDelete;

    int index = -1;
    for (int i = 0; i < roomsCount; i++) {
        if (rooms[i].id == idToDelete) {
            index = i;
            break;
        }
    }

    while (index == -1) {
        cout << "Номер с таким ID не найден. Повторите ввод: ";
        cin >> idToDelete;
        for (int i = 0; i < roomsCount; i++) {
            if (rooms[i].id == idToDelete) {
                index = i;
                break;
            }
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < bookingsCount; i++) {
        if (bookings[i].roomId == idToDelete) {
            cout << "Ошибка! Номер " << idToDelete << " имеет активную заявку. Сначала отмените заявку.\n";
            return;
        }
    }

    for (int i = index; i < roomsCount - 1; i++) {
        rooms[i] = rooms[i + 1];
    }
    roomsCount--;

    cout << "Номер " << idToDelete << " успешно удалён.\n";
}

void showAllRooms() {
    if (roomsCount == 0) {
        cout << "Нет добавленных номеров.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "             СПИСОК НОМЕРОВ\n";
    cout << "ID  | Тип       | Цена за ночь | Вместимость | Статус\n";
    cout << "----------------------------------------\n";

    for (int i = 0; i < roomsCount; i++) {
        cout << rooms[i].id << "   | ";

        if (rooms[i].type == "economy") {
            cout << "economy    | ";
        } else if (rooms[i].type == "standard") {
            cout << "standard   | ";
        } else if (rooms[i].type == "luxe") {
            cout << "luxe       | ";
        }

        cout << rooms[i].pricePerNight << " руб.    | "
             << rooms[i].capacity << " чел.     | ";

        if (rooms[i].isAvailable) {
            cout << "СВОБОДЕН";
        } else {
            cout << "ЗАНЯТ";
        }
        cout << endl;
    }
}

void showAvailableRooms() {
    if (roomsCount == 0) {
        cout << "Нет добавленных номеров.\n";
        return;
    }

    bool found = false;
    cout << "\n========================================\n";
    cout << "          СВОБОДНЫЕ НОМЕРА\n";
    cout << "ID  | Тип       | Цена за ночь | Вместимость\n";
    cout << "----------------------------------------\n";

    for (int i = 0; i < roomsCount; i++) {
        if (rooms[i].isAvailable) {
            found = true;
            cout << rooms[i].id << "   | ";
            if (rooms[i].type == "economy") {
                cout << "economy    | ";
            } else if (rooms[i].type == "standard") {
                cout << "standard   | ";
            } else if (rooms[i].type == "luxe") {
                cout << "luxe       | ";
            }
            cout << rooms[i].pricePerNight << " руб.    | "
                 << rooms[i].capacity << " чел." << endl;
        }
    }

    if (!found) {
        cout << "Свободных номеров нет.\n";
    }
}

// ФУНКЦИИ ДЛЯ РАБОТЫ С УСЛУГАМИ
void addService() {
    if (servicesCount >= MAX_SERVICES) {
        cout << "Ошибка! Достигнут лимит услуг (" << MAX_SERVICES << ").\n";
        return;
    }

    Service newService;

    cout << "Введите название услуги: ";
    // Убрали проблемный cin.ignore(), так как getIntInput уже очистил буфер меню!
    getline(cin, newService.name);

    cout << "Введите цену услуги: ";
    cin >> newService.price;

    while (newService.price < 0) {
        cout << "Ошибка! Цена не может быть отрицательной. Повторите ввод: ";
        cin >> newService.price;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // чистим буфер после ввода цены

    services[servicesCount] = newService;
    servicesCount++;

    cout << "Услуга \"" << newService.name << "\" успешно добавлена!\n";
}

void deleteService() {
    if (servicesCount == 0) {
        cout << "Нет услуг для удаления.\n";
        return;
    }

    cout << "\nСписок услуг:\n";
    for (int i = 0; i < servicesCount; i++) {
        cout << i + 1 << ". " << services[i].name << " - " << services[i].price << " руб.\n";
    }

    int index;
    cout << "Введите номер услуги для удаления: ";
    cin >> index;

    while (index < 1 || index > servicesCount) {
        cout << "Ошибка! Введите номер от 1 до " << servicesCount << ": ";
        cin >> index;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = index - 1; i < servicesCount - 1; i++) {
        services[i] = services[i + 1];
    }
    servicesCount--;

    cout << "Услуга успешно удалена!\n";
}

void showAllServices() {
    if (servicesCount == 0) {
        cout << "Нет добавленных услуг.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "              СПИСОК УСЛУГ\n";
    for (int i = 0; i < servicesCount; i++) {
        cout << i + 1 << ". " << services[i].name << " - " << services[i].price << " руб.\n";
    }
}

// ФУНКЦИИ ДЛЯ РАБОТЫ С ЗАЯВКАМИ
void createBooking() {
    if (roomsCount == 0) {
        cout << "Нет добавленных номеров. Сначала добавьте номера.\n";
        return;
    }

    showAvailableRooms();

    int roomId;
    cout << "Введите ID номера для бронирования: ";
    cin >> roomId;

    int roomIndex = -1;
    for (int i = 0; i < roomsCount; i++) {
        if (rooms[i].id == roomId && rooms[i].isAvailable) {
            roomIndex = i;
            break;
        }
    }

    while (roomIndex == -1) {
        cout << "Номер не найден или уже занят. Повторите ввод: ";
        cin >> roomId;
        for (int i = 0; i < roomsCount; i++) {
            if (rooms[i].id == roomId && rooms[i].isAvailable) {
                roomIndex = i;
                break;
            }
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем поток после roomId перед getline

    Booking newBooking;
    newBooking.bookingId = nextBookingId++;
    newBooking.roomId = roomId;

    cout << "Введите имя клиента: ";
    getline(cin, newBooking.clientName);

    cout << "Введите количество ночей: ";
    cin >> newBooking.nights;
    while (newBooking.nights < 1) {
        cout << "Ошибка! Количество ночей должно быть больше 0. Повторите ввод: ";
        cin >> newBooking.nights;
    }

    double servicesTotal = 0;
    if (servicesCount > 0) {
        showAllServices();
        cout << "Введите номер услуги для добавления (0 - закончить): ";
        int serviceChoice;
        cin >> serviceChoice;
        while (serviceChoice != 0) {
            if (serviceChoice >= 1 && serviceChoice <= servicesCount) {
                servicesTotal += services[serviceChoice - 1].price;
                cout << "Услуга \"" << services[serviceChoice - 1].name << "\" добавлена. Сумма услуг: " << servicesTotal << " руб.\n";
            } else if (serviceChoice != 0) {
                cout << "Ошибка! Нет услуги с таким номером.\n";
            }
            cout << "Введите номер услуги (0 - закончить): ";
            cin >> serviceChoice;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Чистим буфер в конце

    newBooking.totalPrice = rooms[roomIndex].pricePerNight * newBooking.nights + servicesTotal;

    cout << "\n========================================\n";
    cout << "          ИТОГОВАЯ СТОИМОСТЬ\n";
    cout << "Проживание: " << rooms[roomIndex].pricePerNight << " руб. × " << newBooking.nights << " ночей = " << rooms[roomIndex].pricePerNight * newBooking.nights << " руб.\n";
    cout << "Услуги: " << servicesTotal << " руб.\n";
    cout << "ИТОГО: " << newBooking.totalPrice << " руб.\n";

    bookings[bookingsCount] = newBooking;
    bookingsCount++;
    rooms[roomIndex].isAvailable = false;

    cout << "Заявка #" << newBooking.bookingId << " успешно создана!\n";
}

void cancelBooking() {
    if (bookingsCount == 0) {
        cout << "Нет активных заявок.\n";
        return;
    }

    cout << "\nСписок заявок:\n";
    for (int i = 0; i < bookingsCount; i++) {
        cout << "Заявка #" << bookings[i].bookingId << " | Номер: " << bookings[i].roomId << " | Клиент: " << bookings[i].clientName << " | Сумма: " << bookings[i].totalPrice << " руб.\n";
    }

    int bookingId;
    cout << "Введите ID заявки для отмены: ";
    cin >> bookingId;

    int index = -1;
    for (int i = 0; i < bookingsCount; i++) {
        if (bookings[i].bookingId == bookingId) {
            index = i;
            break;
        }
    }

    while (index == -1) {
        cout << "Заявка с таким ID не найдена. Повторите ввод: ";
        cin >> bookingId;
        for (int i = 0; i < bookingsCount; i++) {
            if (bookings[i].bookingId == bookingId) {
                index = i;
                break;
            }
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int roomId = bookings[index].roomId;
    for (int i = 0; i < roomsCount; i++) {
        if (rooms[i].id == roomId) {
            rooms[i].isAvailable = true;
            break;
        }
    }

    for (int i = index; i < bookingsCount - 1; i++) {
        bookings[i] = bookings[i + 1];
    }
    bookingsCount--;

    cout << "Заявка #" << bookingId << " успешно отменена. Номер освобождён.\n";
}

void showAllBookings() {
    if (bookingsCount == 0) {
        cout << "Нет активных заявок.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "              СПИСОК ЗАЯВОК\n";
    for (int i = 0; i < bookingsCount; i++) {
        cout << "Заявка #" << bookings[i].bookingId << " | Номер: " << bookings[i].roomId
             << " | Клиент: " << bookings[i].clientName << " | Ночей: " << bookings[i].nights
             << " | Сумма: " << bookings[i].totalPrice << " руб.\n";
    }
}

// ФУНКЦИИ ОТЧЁТОВ
void showTotalIncome() {
    if (bookingsCount == 0) {
        cout << "Нет завершённых заявок. Доход = 0 руб.\n";
        return;
    }

    double total = 0;
    for (int i = 0; i < bookingsCount; i++) {
        total += bookings[i].totalPrice;
    }
    cout << "Общий доход гостиницы: " << total << " руб.\n";
}

void showOccupancyReport() {
    if (roomsCount == 0) {
        cout << "Нет добавленных номеров.\n";
        return;
    }

    int occupied = 0;
    for (int i = 0; i < roomsCount; i++) {
        if (!rooms[i].isAvailable) {
            occupied++;
        }
    }
    int free = roomsCount - occupied;
    double percent = (double)occupied / roomsCount * 100;

    cout << "           ЗАГРУЗКА НОМЕРОВ\n";
    cout << "Всего номеров: " << roomsCount << "\n";
    cout << "Занято: " << occupied << "\n";
    cout << "Свободно: " << free << "\n";
    cout << "Загрузка: " << percent << "%\n";
}

// ГЛАВНОЕ МЕНЮ
void showMenu() {
    cout << "        ГОСТИНИЧНЫЙ БИЗНЕС\n";
    cout << "         Добро пожаловать!\n";
    cout << "1. Добавить номер\n";
    cout << "2. Удалить номер\n";
    cout << "3. Показать все номера\n";
    cout << "4. Показать свободные номера\n";
    cout << "5. Добавить услугу\n";
    cout << "6. Удалить услугу\n";
    cout << "7. Показать все услуги\n";
    cout << "8. Создать заявку на бронирование\n";
    cout << "9. Отменить заявку\n";
    cout << "10. Показать все заявки\n";
    cout << "11. Показать общий доход\n";
    cout << "12. Показать загрузку номеров\n";
    cout << "0. Выход\n";
    cout << "Ваш выбор: ";
}

// ОСНОВНАЯ ПРОГРАММА
int main() {
    int choice;
    
    do {
        showMenu();
        choice = getIntInput(0, 12);
        
        switch (choice) {
            case 1: addRoom(); break;
            case 2: deleteRoom(); break;
            case 3: showAllRooms(); break;
            case 4: showAvailableRooms(); break;
            case 5: addService(); break;
            case 6: deleteService(); break;
            case 7: showAllServices(); break;
            case 8: createBooking(); break;
            case 9: cancelBooking(); break;
            case 10: showAllBookings(); break;
            case 11: showTotalIncome(); break;
            case 12: showOccupancyReport(); break;
            case 0:
                cout << "Программа завершена. До свидания!\n";
                break;
        }
    } while (choice != 0);
    
    return 0;
}
