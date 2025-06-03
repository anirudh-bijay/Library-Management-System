cmake_minimum_required(VERSION 3.10.0)
project(Library-Management-System VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(lms src/main.cpp
                   src/app/app.cpp
                   src/app/borrow_book.cpp
                   src/app/edit_books.cpp
                   src/app/login.cpp
                   src/app/manage_users.cpp
                   src/app/payment.cpp
                   src/app/return_book.cpp
                   src/app/search_books.cpp
                   src/services/accounts.cpp
                   src/services/books.cpp
                   src/services/users.cpp)
