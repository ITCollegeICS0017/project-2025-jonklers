#include "Listing.h"

Listing::Listing(std::string listing_id, std::string owner_id, std::time_t expiry)
    : listing_id(listing_id), owner_id(owner_id), expiry(expiry) {}

void Listing::set_product(std::string name, std::string description, Category category) {
   this->product.name = name; 
   this->product.description = description; 
   this->product.category = category; 
}

Product Listing::get_product() {
    return this->product;
}

//TODO:
void Listing::conclude_sale() {

}