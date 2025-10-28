#include "DatabaseHandler.h"
#include "../base/User.h"
#include "../base/Auction.h"
#include "../base/Listing.h"
#include "../base/Negotiation.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>


DatabaseHandler::DatabaseHandler(std::string usr_fp, std::string lst_file) {
    std::filesystem::path user_path(usr_fp);
    std::filesystem::path listing_path(lst_file);

    if(!std::filesystem::exists(user_path) || !std::filesystem::exists(listing_path)) throw std::runtime_error("Database error: Invalid file paths!");
    if(!std::filesystem::is_regular_file(user_path) || !std::filesystem::is_regular_file(listing_path)) throw std::runtime_error("Database error: Supplied arguments are not regular files!");
    this->usr_filepath = usr_fp;
    this->lst_filepath = lst_file;
}

void DatabaseHandler::set_user_file(std::string new_path) {
    std::filesystem::path user_path(new_path);
    if(!std::filesystem::exists(user_path)) throw std::runtime_error("Database error: Invalid file path!");
    if(!std::filesystem::is_regular_file(user_path)) throw std::runtime_error("Database error: Not a file!");
    this->usr_filepath = new_path;
}

void DatabaseHandler::set_listing_file(std::string new_path) {
    std::filesystem::path listing_path(lst_filepath);
    if(!std::filesystem::exists(listing_path)) throw std::runtime_error("Database error: Invalid file path!");
    if(!std::filesystem::is_regular_file(listing_path)) throw std::runtime_error("Database error: Not a file!");
    this->lst_filepath = new_path;
}

std::unique_ptr<User> DatabaseHandler::load_user(std::string id) {
    nlohmann::json j;
    std::ifstream file(this->usr_filepath);
    if(!file.is_open()) throw std::runtime_error("Cannot open user file!");
    file >> j;
    file.close();
    User u = j.at(id).get<User>();
    u.set_id(id);
    return std::make_unique<User>(u);
}

void DatabaseHandler::update_usr(const User& u) {
    nlohmann::json j;
    std::ifstream infile(this->usr_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(!j.contains(u.get_id())) throw std::runtime_error("User doesn't exist!");
    j[u.get_id()] = u;

    std::ofstream outfile(this->usr_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open user file!");
    outfile << j.dump(4);
    outfile.close();
}

void DatabaseHandler::delete_usr(const User& u) {
    nlohmann::json j;
    std::ifstream infile(this->usr_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(!j.contains(u.get_id())) throw std::runtime_error("User doesn't exist!"); //sanity check
    j.erase(u.get_id());

    std::ofstream outfile(this->usr_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open user file!");
    outfile << j.dump(4);
    outfile.close();
}

void DatabaseHandler::register_usr(const User& u) {
    nlohmann::json j;
    std::ifstream infile(this->usr_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(j.contains(u.get_id())) throw std::runtime_error("User already exist!"); //sanity check
    j[u.get_id()] = u;

    std::ofstream outfile(this->usr_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open user file!");
    outfile << j.dump(4);
    outfile.close();
}

std::unique_ptr<Listing> DatabaseHandler::load_single_listing(std::string id) {
    nlohmann::json j;
    std::ifstream infile(this->lst_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    return std::make_unique<Listing>(j[id]);
    if(j[id].contains("last_bidder_id")) {
        return std::make_unique<Auction>(j[id].get<Auction>());
    }else if(j[id].contains("offers")) {
        return std::make_unique<Negotiation>(j[id].get<Negotiation>());
    }else {
        return std::make_unique<Listing>();
    }
}