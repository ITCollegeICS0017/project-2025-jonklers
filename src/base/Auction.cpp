#include "Auction.h"

Auction::Auction(std::string listing_id, std::string owner_id, std::time_t expiry, std::string last_bidder_id) : Listing(listing_id, owner_id, expiry), last_bidder_id(last_bidder_id) {}

//TODO:
void bid() {

}