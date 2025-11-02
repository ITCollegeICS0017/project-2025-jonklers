#pragma once
#include "Listing.h"
#include <memory>
#include <vector>

struct Offer {
    std::string sender_id;
    double neg_amount;
};

class Negotiation: public Listing {
public:
    Negotiation() = default;
    Negotiation(double price, std::string owner_id, Product p);

    std::string type() const override { return "Negotiation";}

    std::vector<Offer> get_offers() const { return this->offers;}
    void delete_offer(std::string sender_id);
    void set_offers(std::vector<Offer> o) { this->offers = o;}

    void add_offer(Offer o) {offers.push_back(o);}
private:
    std::vector<Offer> offers;
};

void to_json(nlohmann::json& j, const Offer& obj);
void from_json(const nlohmann::json& j, Offer& obj);

void to_json(nlohmann::json& j, const Negotiation& obj);
void from_json(const nlohmann::json& j, Negotiation& obj);
