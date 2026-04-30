#include "ui/Menu.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>

namespace {
std::string fitCell(const std::string& value, std::size_t width) {
    if (width == 0) {
        return "";
    }

    if (value.size() < width) {
        return value;
    }

    if (width <= 3) {
        return value.substr(0, width);
    }

    return value.substr(0, width - 3) + "...";
}

std::string readNonEmptyString(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string value;
        std::getline(std::cin, value);

        if (!value.empty()) {
            return value;
        }

        std::cout << "Input cannot be empty. Please try again.\n";
    }
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::cout << prompt;

        std::string rawValue;
        std::getline(std::cin, rawValue);

        try {
            std::size_t index = 0;
            const int value = std::stoi(rawValue, &index);
            if (index != rawValue.size()) {
                throw std::invalid_argument("Trailing input");
            }

            if (value < minValue || value > maxValue) {
                std::cout << "Value must be between " << minValue << " and " << maxValue << ".\n";
                continue;
            }

            return value;
        } catch (const std::exception&) {
            std::cout << "Invalid number. Please try again.\n";
        }
    }
}

bool readAvailability(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string value;
        std::getline(std::cin, value);

        for (char& ch : value) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        if (value == "y" || value == "yes") {
            return true;
        }

        if (value == "n" || value == "no") {
            return false;
        }

        std::cout << "Please enter yes or no.\n";
    }
}
}

Menu::Menu(BookService service)
    : service_(std::move(service)) {}

void Menu::run() {
    bool running = true;

    while (running) {
        showMenu();
        const int choice = readInt("Select an option: ", 0, 8);
        std::cout << '\n';

        switch (choice) {
            case 1:
                handleAddRecord();
                break;
            case 2:
                handleDisplayAllRecords();
                break;
            case 3:
                handleSearchById();
                break;
            case 4:
                handleUpdateRecord();
                break;
            case 5:
                handleDeleteRecord();
                break;
            case 6:
                handleSortRecords();
                break;
            case 7:
                handleExportCsv();
                break;
            case 8:
                handleDashboard();
                break;
            case 0:
                running = false;
                std::cout << "Exiting Record Management System.\n";
                break;
        }

        std::cout << '\n';
    }
}

void Menu::showMenu() const {
    std::cout << "========================================\n";
    std::cout << "     Record Management System - Books   \n";
    std::cout << "========================================\n";
    std::cout << "1. Add record\n";
    std::cout << "2. Display all records\n";
    std::cout << "3. Search record by ID\n";
    std::cout << "4. Update record\n";
    std::cout << "5. Delete record\n";
    std::cout << "6. Sort records\n";
    std::cout << "7. Export to CSV\n";
    std::cout << "8. Dashboard summary\n";
    std::cout << "0. Exit\n";
}

void Menu::handleAddRecord() {
    std::cout << "Add New Book Record\n";
    const int id = readInt("ID: ", 1, 1000000);
    const std::string title = readNonEmptyString("Title: ");
    const std::string author = readNonEmptyString("Author: ");
    const int year = readInt("Year: ", 1450, 2100);
    const std::string genre = readNonEmptyString("Genre: ");
    const bool available = readAvailability("Available? (yes/no): ");

    std::string message;
    if (service_.addBook(Book(id, title, author, year, genre, available), message)) {
        std::cout << message << '\n';
    } else {
        std::cout << "Add failed: " << message << '\n';
    }
}

void Menu::handleDisplayAllRecords() const {
    std::string errorMessage;
    const std::vector<Book> books = service_.getAllBooks(errorMessage);

    if (!errorMessage.empty()) {
        std::cout << "Display failed: " << errorMessage << '\n';
        return;
    }

    if (books.empty()) {
        std::cout << "No records found.\n";
        return;
    }

    printBookTable(books);
}

void Menu::handleSearchById() const {
    const int id = readInt("Enter ID to search: ", 1, 1000000);

    std::string errorMessage;
    const std::optional<Book> book = service_.findBookById(id, errorMessage);

    if (!errorMessage.empty()) {
        std::cout << "Search failed: " << errorMessage << '\n';
        return;
    }

    if (!book.has_value()) {
        std::cout << "Record not found.\n";
        return;
    }

    printSingleBook(book.value());
}

void Menu::handleUpdateRecord() {
    const int id = readInt("Enter ID to update: ", 1, 1000000);

    std::string errorMessage;
    const std::optional<Book> existingBook = service_.findBookById(id, errorMessage);

    if (!errorMessage.empty()) {
        std::cout << "Update failed: " << errorMessage << '\n';
        return;
    }

    if (!existingBook.has_value()) {
        std::cout << "Record not found.\n";
        return;
    }

    std::cout << "Current record:\n";
    printSingleBook(existingBook.value());

    std::cout << "\nEnter updated values.\n";
    const std::string title = readNonEmptyString("Title: ");
    const std::string author = readNonEmptyString("Author: ");
    const int year = readInt("Year: ", 1450, 2100);
    const std::string genre = readNonEmptyString("Genre: ");
    const bool available = readAvailability("Available? (yes/no): ");

    Book updatedBook(id, title, author, year, genre, available);
    std::string message;

    if (service_.updateBook(updatedBook, message)) {
        std::cout << message << '\n';
    } else {
        std::cout << "Update failed: " << message << '\n';
    }
}

void Menu::handleDeleteRecord() {
    const int id = readInt("Enter ID to delete: ", 1, 1000000);
    std::string message;

    if (service_.deleteBook(id, message)) {
        std::cout << message << '\n';
    } else {
        std::cout << "Delete failed: " << message << '\n';
    }
}

void Menu::handleSortRecords() const {
    std::cout << "Sort Options\n";
    std::cout << "1. Title\n";
    std::cout << "2. Year\n";
    std::cout << "3. Author\n";

    const int sortChoice = readInt("Sort by: ", 1, 3);

    std::string errorMessage;
    const std::vector<Book> books = service_.getSortedBooks(static_cast<SortOption>(sortChoice), errorMessage);

    if (!errorMessage.empty()) {
        std::cout << "Sort failed: " << errorMessage << '\n';
        return;
    }

    if (books.empty()) {
        std::cout << "No records found.\n";
        return;
    }

    printBookTable(books);
}

void Menu::handleExportCsv() const {
    const std::filesystem::path exportPath = std::filesystem::path("exports") / "books_export.csv";
    std::filesystem::create_directories(exportPath.parent_path());

    std::string message;
    if (service_.exportToCsv(exportPath.string(), message)) {
        std::cout << message << '\n';
        std::cout << "File: " << exportPath.string() << '\n';
    } else {
        std::cout << "Export failed: " << message << '\n';
    }
}

void Menu::handleDashboard() const {
    std::string errorMessage;
    const DashboardStats stats = service_.getDashboardStats(errorMessage);

    if (!errorMessage.empty()) {
        std::cout << "Dashboard failed: " << errorMessage << '\n';
        return;
    }

    std::cout << "Library Snapshot\n";
    std::cout << "Total books: " << stats.totalBooks << '\n';
    std::cout << "Available: " << stats.availableBooks << '\n';
    std::cout << "Checked out: " << stats.checkedOutBooks << '\n';
    std::cout << "Oldest title: " << stats.oldestTitle << '\n';
    std::cout << "Newest title: " << stats.newestTitle << '\n';
}

void Menu::printBookTable(const std::vector<Book>& books) {
    std::cout << std::left
              << std::setw(6) << "ID"
              << std::setw(28) << "Title"
              << std::setw(22) << "Author"
              << std::setw(8) << "Year"
              << std::setw(18) << "Genre"
              << std::setw(14) << "Status"
              << '\n';

    std::cout << std::string(96, '-') << '\n';

    for (const Book& book : books) {
        std::cout << std::left
                  << std::setw(6) << book.getId()
                  << std::setw(28) << fitCell(book.getTitle(), 27)
                  << std::setw(22) << fitCell(book.getAuthor(), 21)
                  << std::setw(8) << book.getYear()
                  << std::setw(18) << fitCell(book.getGenre(), 17)
                  << std::setw(14) << (book.isAvailable() ? "Available" : "Checked Out")
                  << '\n';
    }
}

void Menu::printSingleBook(const Book& book) {
    std::cout << "ID: " << book.getId() << '\n';
    std::cout << "Title: " << book.getTitle() << '\n';
    std::cout << "Author: " << book.getAuthor() << '\n';
    std::cout << "Year: " << book.getYear() << '\n';
    std::cout << "Genre: " << book.getGenre() << '\n';
    std::cout << "Status: " << (book.isAvailable() ? "Available" : "Checked Out") << '\n';
}
