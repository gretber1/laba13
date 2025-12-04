#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <windows.h>
#include <limits>
using namespace std;

double check_double(std::string s) {
    bool t = true;
    int k = 0;
    for (long long i = 0; i < s.size(); i++) {
        if (!isdigit(s[i]) and !(s[i] == '.')) {
            t = false;
        }
        if (s[i] == '.') k++;
    }
    if (t) {
        if (k > 1 or stod(s) > 100000000 or stod(s) <= 0) {
            t = false;
        }
    }
    while (std::cin.fail() || !t) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Введено невозможное значение. Попробуйте снова: ";
        std::cin >> s;
        t = true;
        k = 0;
        for (long long i = 0; i < s.size(); i++) {
            if (!isdigit(s[i]) and !(s[i] == '.')) {
                t = false;
            }
            if (s[i] == '.') k++;
        }
        if (t) {
            if (k > 1 or stod(s) > 100000000 or stod(s) <= 0) {
                t = false;
            }
        }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return stod(s);
}

double check_int(std::string s) {
    bool t = true;
    for (long long i = 0; i < s.size(); i++) {
        if (!isdigit(s[i]) and !(i == 0 and s[i] == '-')) {
            t = false;
        }
    }
    if (t) {
        if (stoi(s) > 100000000) {
            t = false;
        }
    }
    while (std::cin.fail() || !t) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Введено невозможное значение. Попробуйте снова: ";
        std::cin >> s;
        t = true;
        for (long long i = 0; i < s.size(); i++) {
            if (!isdigit(s[i]) and !(i == 0 and s[i] == '-')) {
                t = false;
            }
        }
        if (t) {
            if (stoi(s) > 100000000) {
                t = false;
            }
        }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return stoi(s);
}

double check_bool(std::string s) {
    bool t = true;
    for (long long i = 0; i < s.size(); i++) {
        if (!isdigit(s[i]) and !(i == 0 and s[i] == '-')) {
            t = false;
        }
    }
    if (t) {
        if (stoi(s) != 0 and stoi(s) != 1) {
            t = false;
        }
    }
    while (std::cin.fail() || !t) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Введено невозможное значение. Попробуйте снова: ";
        std::cin >> s;
        t = true;
        for (long long i = 0; i < s.size(); i++) {
            if (!isdigit(s[i]) and !(i == 0 and s[i] == '-')) {
                t = false;
            }
        }
        if (t) {
            if (stoi(s) != 0 and stoi(s) != 1) {
                t = false;
            }
        }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return stoi(s);
}

// Interface (Strategy)
class IPriceStrategy {
public:
    virtual double calculatePrice(double basePrice) const = 0;
    virtual ~IPriceStrategy() = default;
};


// Strategy: обычная цена
class BasePriceStrategy : public IPriceStrategy {
public:
    double calculatePrice(double basePrice) const override {
        return basePrice;
    }
};


// Strategy: цена со скидкой
class DiscountStrategy : public IPriceStrategy {
private:
    double discountPercent;
public:
    DiscountStrategy(double discountPercent)
        : discountPercent(discountPercent)
    {
        if (discountPercent < 0 || discountPercent > 100)
            throw invalid_argument("Процент скидки должен быть от 0 до 100");
    }

    double calculatePrice(double basePrice) const override {
        return basePrice * (1 - discountPercent / 100.0);
    }
};


class Tariff {
private:
    string name;
    double basePrice;
    unique_ptr<IPriceStrategy> strategy;

public:
    Tariff(string name, double price, IPriceStrategy* strategy)
        : name(name), basePrice(price), strategy(strategy) {}

    double getPrice() const {
        return strategy->calculatePrice(basePrice);
    }

    string getName() const {
        return name;
    }
};


class Firm {
private:
    vector<unique_ptr<Tariff>> tariffs;

public:
    void addTariff(unique_ptr<Tariff> t) {
        tariffs.push_back(move(t));
    }

    void printAllTariffs() const {
        if (tariffs.empty()) {
            cout << "Список тарифов пуст!" << endl;
            return;
        }

        cout << "Доступные тарифы:" << endl;

        for (const auto& t : tariffs) {
            cout << "- " << t->getName() << " — " << t->getPrice() << " рублей" << endl;
        }
    }

    Tariff* findCheapestTariff() const {
        if (tariffs.empty())
            return nullptr;

        Tariff* minTariff = tariffs[0].get();

        for (const auto& t : tariffs) {
            if (t->getPrice() < minTariff->getPrice())
                minTariff = t.get();
        }

        return minTariff;
    }
};


int main() {
        SetConsoleOutputCP(CP_UTF8);
	    SetConsoleCP(CP_UTF8);
        Firm firm;
        std::string choice_t;
        int choice;
        firm.addTariff(make_unique<Tariff>("Обычный тариф", 900, new BasePriceStrategy));
        firm.addTariff(make_unique<Tariff>("Скидочный", 1000, new DiscountStrategy(20)));
        firm.addTariff(make_unique<Tariff>("VIP тариф", 3000, new DiscountStrategy(50)));

        do {
            cout << "\n=== МЕНЮ ===\n";
            cout << "1. Добавить тариф\n";
            cout << "2. Показать самый дешевый тариф\n";
            cout << "3. Показать все тарифы\n";
            cout << "0. Выход\n";
            cout << "Выберите действие: ";
            cin >> choice_t;

            choice = check_int(choice_t);

            switch (choice) {
                case 1: {
                    string name;
                    double price;
                    string price_t;
                    string saleorno_t;
                    int saleorno;
                    string sale_t;
                    double sale;
                    cout << "Введите цену тарифа: ";
                    cin >> price_t;
                    price = check_double(price_t);
                    cout << "Введите тип тарифа, 0 - без скидок, 1 со скидкой: ";
                    cin >> saleorno_t;
                    saleorno = check_bool(saleorno_t);
                    if (saleorno == 1) {
                        cout << "Введите размер скидки в процентах: ";
                        cin >> sale_t;
                        sale = check_double(sale_t);
                        while(sale < 0 or sale > 100) {
                            cout << "Процент скидки должен быть от 0 до 100. Введите размер скидки в процентах: ";
                            cin >> sale_t;
                            sale = check_double(sale_t);
                        }
                        cout << "Введите имя тарифа: ";
                        getline(std::cin, name);
                        firm.addTariff(make_unique<Tariff>(name, price, new DiscountStrategy(sale)));
                    }
                    else {
                        cout << "Введите имя тарифа: ";
                        getline(std::cin, name);
                        firm.addTariff(make_unique<Tariff>(name, price, new BasePriceStrategy));
                    }
                    break;
                }

                case 2: {
                    Tariff* cheapest = firm.findCheapestTariff();
                    if (cheapest == nullptr){
                        cout << "Список тарифов пуст!\n";
                        break;
                    }
                    cout << "Самый дешевый тариф: " << cheapest->getName() << " — " << cheapest->getPrice() << " рублей."<< endl;
                    break;
                }

                case 3: {
                    firm.printAllTariffs();
                    break;
                }

                case 0:
                    std::cout << "Выход из программы...\n";
                    break;

                default:
                    std::cout << "Неверный выбор!\n";
            }
        } while (choice != 0);


    return 0;
}

