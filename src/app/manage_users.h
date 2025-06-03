#pragma once

#include "../services/users.h"
#include "../services/accounts.h"

class ManageUsersPage
{ 
    private:
        // This page is stateless.

        /// @brief Table of books.
        services::Users& users_service;
        services::Accounts& accounts_service;

        // lms::Book& book;
        bool& record_created;

        /// @section Private methods
        void buildAddRecordForm();
        void buildRemoveRecordForm();

    public:
        ManageUsersPage(
            services::Users& users_service,
            services::Accounts& accounts_service,
            bool& record_created,
            bool build = true
        );
        ~ManageUsersPage() = default;

        void build();
};

inline ManageUsersPage::ManageUsersPage(
    services::Users& users_service,
    services::Accounts& accounts_service,
    bool& record_created,
    const bool build
)
    : users_service{users_service},
    accounts_service{accounts_service},
    record_created{record_created}
{
    if (build)
        this->build();
}
