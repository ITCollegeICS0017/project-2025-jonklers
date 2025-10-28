#include "DatabaseHandler.h"
#include "../base/User.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <future>
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
    User u = j.at(id).get<User>();
    u.set_id(id);
    return std::make_unique<User>(u);
}
