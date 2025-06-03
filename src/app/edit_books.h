#pragma once

#include "../services/books.h"

class EditBooksPage
{ 
    private:
        // This page is stateless.

        /// @brief Table of books.
        services::Books& books_service;

        bool& record_created;

        /// @section Private methods
        void buildAddRecordForm();
        void buildRemoveRecordForm();
        void buildEditRecordForm();
        void buildChangeStatusForm();

    public:
        EditBooksPage(
            services::Books& books_service,
            bool& record_created,
            bool build = true
        );
        ~EditBooksPage() = default;

        void build();
};

inline EditBooksPage::EditBooksPage(
    services::Books& books_service,
    bool& record_created,
    const bool build
)
    : books_service{books_service},
    record_created{record_created}
{
    if (build)
        this->build();
}
