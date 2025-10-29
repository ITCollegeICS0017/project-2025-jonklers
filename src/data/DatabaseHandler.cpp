#include "DatabaseHandler.h"
#include "../base/User.h"
#include "../base/Auction.h"
#include "../base/Listing.h"
#include "../base/Negotiation.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>


DatabaseHandler::DatabaseHandler() {
  usr_filepath = "storage/users.json";
  lst_filepath = "storage/listings.json";
  archive_filepath = "storage/archive.json";
  create_files();
  load_all_listings();
}
DatabaseHandler::DatabaseHandler(std::string usr_fp, std::string lst_file) {
    std::filesystem::path user_path(usr_fp);
    std::filesystem::path listing_path(lst_file);

    if(!std::filesystem::exists(user_path) || !std::filesystem::exists(listing_path)) throw std::runtime_error("Database error: Invalid file paths!");
    if(!std::filesystem::is_regular_file(user_path) || !std::filesystem::is_regular_file(listing_path)) throw std::runtime_error("Database error: Supplied arguments are not regular files!");
    usr_filepath = usr_fp;
    lst_filepath = lst_file;
    load_all_listings();
}

void DatabaseHandler::set_user_file(std::string new_path) {
    std::filesystem::path user_path(new_path);
    if(!std::filesystem::exists(user_path)) throw std::runtime_error("Database error: Invalid file path!");
    if(!std::filesystem::is_regular_file(user_path)) throw std::runtime_error("Database error: Not a file!");
    usr_filepath = new_path;
}

void DatabaseHandler::set_listing_file(std::string new_path) {
    std::filesystem::path listing_path(lst_filepath);
    if(!std::filesystem::exists(listing_path)) throw std::runtime_error("Database error: Invalid file path!");
    if(!std::filesystem::is_regular_file(listing_path)) throw std::runtime_error("Database error: Not a file!");
    lst_filepath = new_path;
}

void DatabaseHandler::set_archived_file(std::string new_path) {
    std::filesystem::path listing_path(archive_filepath);
    if(!std::filesystem::exists(listing_path)) throw std::runtime_error("Database error: Invalid file path!");
    if(!std::filesystem::is_regular_file(listing_path)) throw std::runtime_error("Database error: Not a file!");
    archive_filepath = new_path;
}

std::unique_ptr<User> DatabaseHandler::load_user(std::string id) {
    nlohmann::json j;
    std::ifstream file(usr_filepath);
    if(!file.is_open()) throw std::runtime_error("Cannot open user file!");
    file >> j;
    file.close();
    User u = j.at(id).get<User>();
    u.set_id(id);
    return std::make_unique<User>(u);
}

void DatabaseHandler::update_usr(const User& u) {
    nlohmann::json j;
    std::ifstream infile(usr_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(!j.contains(u.get_id())) throw std::runtime_error("User doesn't exist!");
    j[u.get_id()] = u;

    std::ofstream outfile(usr_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open user file!");
    outfile << j.dump(4);
    outfile.close();
}

void DatabaseHandler::delete_usr(const User& u) {
    nlohmann::json j;
    std::ifstream infile(usr_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(!j.contains(u.get_id())) throw std::runtime_error("User doesn't exist!"); //sanity check
    j.erase(u.get_id());

    std::ofstream outfile(usr_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open user file!");
    outfile << j.dump(4);
    outfile.close();
}

void DatabaseHandler::register_usr(const User& u) {
    nlohmann::json j;
    std::ifstream infile(usr_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(j.contains(u.get_id())) throw std::runtime_error("User already exist!"); //sanity check
    j[u.get_id()] = u;

    std::ofstream outfile(usr_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open user file!");
    outfile << j.dump(4);
    outfile.close();
}

/*std::unique_ptr<Listing> DatabaseHandler::load_single_listing(std::string id) {
    nlohmann::json j;
    std::ifstream infile(this->lst_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open user file!");
    infile >> j;
    infile.close();
    if(j[id].contains("last_bidder_id")) {
        return std::make_unique<Auction>(j[id].get<Auction>());
    }else if(j[id].contains("offers")) {
        return std::make_unique<Negotiation>(j[id].get<Negotiation>());
    }else {
        return std::make_unique<Listing>();
    }
}
*/

void DatabaseHandler::add_listing(std::shared_ptr<Listing> l) {
    all_listings[l->get_listing_id()] = l;
    update_listings_file();
    //nlohmann::json j;
    //std::ifstream infile(this->lst_filepath);
    //if(!infile.is_open()) throw std::runtime_error("Cannot open listing file!");
    //infile >> j;
    //infile.close();
    //j[l->get_listing_id()] = *l; 

    //std::ofstream outfile(this->lst_filepath);
    //if(!outfile.is_open()) throw std::runtime_error("Cannot open listing file!");
    //outfile << j.dump(4);
    //outfile.close();
}

void DatabaseHandler::load_all_listings() {
    nlohmann::json j;
    std::ifstream infile(lst_filepath);
    if(!infile.is_open()) throw std::runtime_error("Cannot open listing file!");
    infile >> j;
    infile.close();
    for(auto& [id, listing] : j.items()) {
        std::shared_ptr<Listing> l;
        if(listing.contains("last_bidder_id")) {
            l = std::make_shared<Listing>(listing.get<Auction>()); 
        }else if(listing.contains("offers")) {
            l = std::make_shared<Listing>(listing.get<Negotiation>()); 
        }else {
            l = std::make_shared<Listing>(listing.get<Listing>()); 
        }
        l->set_listing_id(id);
        all_listings[id] = l;
    }
}

void DatabaseHandler::update_listings_file() {
    nlohmann::json j;
    std::ofstream outfile(lst_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open listing file!");
    for(const auto& [id, obj] : all_listings) {
        j[id] = *obj;
    }
    outfile << j.dump(4);
    outfile.close();
}



void DatabaseHandler::archive_listing(std::string id) {
    if(all_listings.find(id) == all_listings.end()) throw std::runtime_error("Listing doesn't exist!"); 
    auto l = all_listings[id];
    all_listings.erase(id);
    append_archive(l);
    update_listings_file();
}

void DatabaseHandler::append_archive(std::shared_ptr<Listing> l) {
    nlohmann::json j;
    std::ifstream ifile(archive_filepath);
    if(!ifile.is_open()) throw std::runtime_error("Cannot open archive file!");
    ifile >> j;
    ifile.close();
    j.push_back(*l);
    std::ofstream outfile(archive_filepath);
    if(!outfile.is_open()) throw std::runtime_error("Cannot open archive file!");
    outfile << j.dump(4);
    outfile.close();
}

void DatabaseHandler::create_files() {
    if((std::filesystem::exists(usr_filepath) && std::filesystem::exists(lst_filepath) && std::filesystem::exists(archive_filepath))) return;
    std::filesystem::create_directories("storage");
    std::ofstream file(usr_filepath);
    file << "{}";
    file.close();
    std::ofstream file2(lst_filepath);
    file2 << "{}";
    file2.close();
    std::ofstream file3(archive_filepath);
    file3 << "{}";
    file3.close();
}