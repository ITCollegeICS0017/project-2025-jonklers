#pragma once
#include <string>
#include <chrono>


enum class Category { ELECTRONICS, FASHION, BOOKS, HOME, GARDEN };

struct Product {
    std::string name;
    std::string description;
    Category category;
};


class Listing {
public:
    Listing(std::string listing_id, double price, std::string owner_id, std::time_t expiry);

    void conclude_sale();
    void set_product(std::string name, std::string description, Category category);
    Product get_product();

protected:
    double price;
    std::string listing_id;
    Product product;
    std::string owner_id;
    std::time_t expiry;
};