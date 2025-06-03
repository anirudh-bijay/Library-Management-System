#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace lms
{
    class User;
    class Borrower;
    class Student;
    class Faculty;
    class Librarian;

    class Account;
} // namespace lms

namespace services
{
    class Users;
} // namespace services

class ChangePasswordPage;
class ManageUsersPage;
class App;

class lms::User
{
    friend class ::services::Users;
    friend class ::ChangePasswordPage;
    friend class ::ManageUsersPage;
    friend class ::App;

    public:
        using userId_type   = std::uint64_t;
        using password_type = std::string;
    
    protected:
        userId_type userId;

    private:
        password_type password;

        password_type getPassword() const noexcept
        {
            return password;
        }

        void setPassword(std::string&& new_password) noexcept
        {
            password = std::move(new_password);
        }

    public:
        User() = default;
        
        User(const userId_type userId, const password_type& password)
            : userId{std::move(userId)}, password{password} {}

        User(const userId_type userId, password_type&& password)
            : userId{std::move(userId)}, password{std::move(password)} {}

        ~User() = default;

        userId_type getUserId() const noexcept
        {
            return userId;
        }
};

class lms::Borrower : public lms::User
{
    public:
        using User::User;

        Borrower() = delete;

        /// @brief Retrieves the account details of the user (student/faculty).
        ///        Virtual since separate data files are used for storage of
        ///        account details of students and faculty.
        ///        
        /// @return The account details.
        virtual Account getAccount() const = 0;
};

class lms::Student : public lms::Borrower
{
    public:
        static constexpr unsigned maxBorrowLimit = 3;
        static constexpr unsigned maxBorrowPeriod = 15;
        static constexpr unsigned fineRate = 10; // Fine per book per day for overdue books.

        using Borrower::Borrower;
};

class lms::Faculty : public lms::Borrower
{
    public:
        static constexpr unsigned maxBorrowLimit = 5;
        static constexpr unsigned maxBorrowPeriod = 30;
        static constexpr unsigned fineRate = 0; // Fine per book per day for overdue books.

        using Borrower::Borrower;
};

class lms::Librarian : public lms::User
{
    public:
        using User::User;
};
