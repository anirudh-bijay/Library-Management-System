#include "../objects/users.h"
#include "../services/users.h"
#include "../utils/terminal_colours.h"
#include "manage_users.h"

#include <algorithm>
#include <iostream>
#include <limits>

void ManageUsersPage::build()
{
    while (true) {
        std::cout << '\n' <<
            "----------------------------------------\n"
            "                Actions                 \n"
            "----------------------------------------\n"
            "                                        \n"
            " 1. Add a record                        \n"
            " 2. Remove a record                     \n"
            " 3. Go back                             \n"
            "                                        \n"
            " Select a choice (1-5): ";

        unsigned choice;
        std::cin >> choice;
        while (!std::cin) {
            std::cin.clear();
            choice = 0;            
        }
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                buildAddRecordForm();
                break;

            case 2:
                buildRemoveRecordForm();
                break;

            case 3:
                return;

            default:
                std::cout <<
                    ANSI_COLOR_MAGENTA << "Invalid choice\n" << ANSI_COLOR_RESET;
        }
    }
}

void ManageUsersPage::buildAddRecordForm()
{
    lms::User::userId_type userId;

    std::cout << '\n' <<
        "----------------------------------------\n"
        "              Add a record              \n"
        "----------------------------------------\n"
        "                                        \n"
        " User ID: ";
    
    std::cin >> userId;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " User ID: ";
        std::cin >> userId;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    
    for (services::Users::size_type i = 0; i < users_service.size(); ++i) {
        if (std::get <0>(users_service.get(i)) == userId) {
            std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID is already assigned.\n" << ANSI_COLOR_RESET;
            return;
        }
    }

    std::string username;

    std::cout << " Username (avoid special characters): ";
    std::cin >> username;
    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    while (std::find(username.cbegin(), username.cend(), '\\') != username.cend()) {
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Username (avoid special characters): ";
        std::cin >> username;
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    }

    for (services::Users::size_type i = 0; i < users_service.size(); ++i) {
        if (std::get <1>(users_service.get(i)) == username) {
            std::cout << ANSI_COLOR_CYAN << "\nSorry, this username is already assigned.\n" << ANSI_COLOR_RESET;
            return;
        }
    }

    std::cout << " Role (1: Librarian | 2: Faculty | default: Student): ";
    unsigned status;
    std::cin >> status;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Role (1: Librarian | 2: Faculty | default: Student): ";
        std::cin >> status;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    services::Users::UserType role;

    switch (status) {
        case 1: 
            role = services::Users::UserType::LIBRARIAN;
            break;

        case 2:
            role = services::Users::UserType::FACULTY;
            break;

        default:
            role = services::Users::UserType::STUDENT;
    }

    std::string password;

    std::cout << " Password: ";
    std::getline(std::cin, password);

    users_service.append({userId, username, role, password});
    accounts_service.append({userId, lms::Account()});
    record_created = true;
    std::cout << ANSI_COLOR_GREEN << "\nSuccessfully registered request to add record\n" << ANSI_COLOR_RESET;
}

void ManageUsersPage::buildRemoveRecordForm()
{
    lms::User::userId_type userId;

    std::cout << '\n' <<
        "----------------------------------------\n"
        "            Remove a record             \n"
        "----------------------------------------\n"
        "                                        \n"
        " User ID: ";
    
    std::cin >> userId;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " User ID: ";
        std::cin >> userId;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    
    for (services::Users::size_type i = 0; i < users_service.size(); ++i) {
        if (std::get <0>(users_service.get(i)) == userId) {
            users_service.erase(i);

            for (services::Accounts::size_type i = 0; i < accounts_service.size(); ++i) {
                if (accounts_service.get <0>(i) == userId) {
                    accounts_service.erase(i);
                }
            }

            record_created = true;
            std::cout << ANSI_COLOR_GREEN << "\nSuccessfully registered request to delete record\n" << ANSI_COLOR_RESET;
            return;
        }
    }

    std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID does not exist.\n" << ANSI_COLOR_RESET;
    return;
}
