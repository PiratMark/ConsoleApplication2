#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

using namespace std;

struct Product {
    const string name;
    const unsigned int price;
};

struct OrderItem {
    const int code;
    const unsigned int quantity;
    OrderItem(const int c, const unsigned int q) : code(c), quantity(q) {}
};

const map<int, Product> menu = {
    { 1, { "Маргарита", 5 } },
    { 2, { "Гавайская", 6 } },
    { 3, { "4 сыра", 4 } },
    { 4, { "Нью-Йорк", 7 } },
    { 101, { "Сок", 2 } },
    { 102, { "Боржоми", 4 } },
    { 103, { "Кола", 3 } }
};

bool isPizza(int code) {
    return code < 100;
}

bool isDrink(int code) {
    return code >= 100;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "Меню:" << endl;
    for (auto e : menu) {
        cout << right << setw(4) << e.first << ' '
            << left << setw(20) << e.second.name
            << right << setw(4) << e.second.price << endl;
    }
    cout << "Введите заказ (код и кол-во продукта, Ctrl-Z - окончание ввода)" << endl;
    vector<OrderItem*> order;
    unsigned int totalPrice = 0;
    unsigned int totalExpDrinks = 0;
    while (true) {
        int code;
        unsigned int quantity;
        cin >> code >> quantity;
        if (cin.eof()) break;

        auto iter = menu.find(code);
        if (iter != menu.end()) {
            order.push_back(new OrderItem(code, quantity));
            totalPrice += iter->second.price * quantity;
            if (isDrink(code) && iter->second.price > 2) {
                totalExpDrinks += quantity;
            }
        }
        else {
            cout << "Неправильный код: " << code << endl;
        }
    }

    cout << "Чек:" << endl;
    unsigned int totalPizzas = 0;
    unsigned int discountedCost = 0;
    for (auto item : order) {
        auto p = menu.at(item->code);

        unsigned int freeItems = 0;
        if (isPizza(item->code)) {
            unsigned int remPizzas = totalPizzas % 5;
            freeItems = (remPizzas + item->quantity) / 5;
            totalPizzas += item->quantity;
        }

        unsigned int price = p.price * 100; // здесь цена уже в центах для расчёта скидки
        if (totalPrice > 50) {
            price -= price / 5;
        }
        else if (totalExpDrinks > 3 && isDrink(item->code) && price > 200) {
            price -= price * 3 / 20;
        }

        unsigned int itemCost = (item->quantity - freeItems) * price;
        discountedCost += itemCost;
        cout << right << setw(4) << item->code << ' '
            << left << setw(20) << p.name
            << right << setw(4) << p.price
            << setw(4) << item->quantity
            << setw(5) << (itemCost / 100) << '.' << setfill('0') << (itemCost % 100) << setfill(' ')
            << endl;
    }
    cout << "Итого:                          "
        << setw(5) << (discountedCost / 100) << '.' << setfill('0') << (discountedCost % 100) << setfill(' ')
        << endl;

    return 0;
}