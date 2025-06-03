#include "app/app.h"

#include <filesystem>

void create_nonexistent_files()
{
    if (!std::filesystem::exists("./data")) {
        if (!std::filesystem::create_directory("./data")) {
            throw std::runtime_error("Failed to create directory: ./data");
        }
    } else if (!std::filesystem::is_directory("./data")) {
        throw std::runtime_error("Path ./data exists but is not a directory");
    }
    for (const auto& file : {"./data/books.ssv", "./data/users.ssv", "./data/accounts.ssv"}) {
        if (!std::filesystem::exists(file)) {
            std::ofstream ofs(file);
            if (!ofs) {
                throw std::runtime_error(std::string{"Failed to create file: "}.append(file));
            }
            ofs << '\n'; // Ensure a newline is written to the file
            ofs.close();
        }
    }
}

int main()
{
    create_nonexistent_files();
    App();
}