#pragma once
#include "Listing.h"

class Auction: Listing {
public:
    Auction(std::string listing_id, std::string owner_id, std::time_t expiry, std::string last_bidder_id);

    void bid();
private:
    std::string last_bidder_id;
};