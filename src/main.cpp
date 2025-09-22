#include <iostream>
#include <string>

using namespace std;

class Category;
class Seller;
class Product;
class Payment;
class Buyer;

class Product {
public:
    string name;
    string description;
    Category* categories[10];
    int category_count = 0;
    Seller* seller;
    bool sold;
    int price;

    void add_category(Category* category) {
        categories[category_count++] = category;
    }

    void print_info() {
        cout << "Product: " << name << ", Description: " << description << ", Price: $" << price << endl;
    }
};

class Category {
public:
    string name;
    Product* products[1000];
    int product_count = 0;

    void add_product(Product* product) {
        products[product_count++] = product;
    }

    void print_products() {
        for (int i = 0; i < product_count; i++) {
            products[i]->print_info();
        }
    }
};

class Seller {
public:
    string name;
    Product* products[1000];
    int product_count = 0;

    Product create_product(const string& name, const string& description, int price) {
        Product product;
        product.name = name;
        product.description = description;
        product.price = price;
        product.seller = this;
        product.sold = false;
        products[product_count++] = &product;
        return product;
    }
};

class Payment {
public:
    Product* product;
    int amount;
};

class Buyer {
public:
    string name;
};

int main() {
    Seller seller;
    seller.name = "Bob";

    Product product1 = seller.create_product("Laptop", "A powerful laptop", 1500);
    Product product2 = seller.create_product("Smartphone", "A latest model smartphone", 800);

    Category electronics;
    electronics.name = "Electronics";
    electronics.add_product(&product1);
    electronics.add_product(&product2);

    electronics.print_products();

    return 0;
}