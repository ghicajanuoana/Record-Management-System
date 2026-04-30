#include "services/BookService.h"

#include <algorithm>
#include <fstream>

BookService::BookService(BookRepository repository)
    : repository_(std::move(repository)) {}

bool BookService::addBook(const Book& book, std::string& message) const {
    std::string errorMessage;
    const std::optional<Book> existingBook = repository_.findById(book.getId(), errorMessage);

    if (!errorMessage.empty()) {
        message = errorMessage;
        return false;
    }

    if (existingBook.has_value()) {
        message = "A record with that ID already exists.";
        return false;
    }

    if (!repository_.add(book, errorMessage)) {
        message = errorMessage;
        return false;
    }

    message = "Record added successfully.";
    return true;
}

std::vector<Book> BookService::getAllBooks(std::string& errorMessage) const {
    return repository_.getAll(errorMessage);
}

std::optional<Book> BookService::findBookById(int id, std::string& errorMessage) const {
    return repository_.findById(id, errorMessage);
}

bool BookService::updateBook(const Book& book, std::string& message) const {
    if (!repository_.update(book, message)) {
        return false;
    }

    message = "Record updated successfully.";
    return true;
}

bool BookService::deleteBook(int id, std::string& message) const {
    if (!repository_.remove(id, message)) {
        return false;
    }

    message = "Record deleted successfully.";
    return true;
}

std::vector<Book> BookService::getSortedBooks(SortOption option, std::string& errorMessage) const {
    std::vector<Book> books = repository_.getAll(errorMessage);
    if (!errorMessage.empty()) {
        return {};
    }

    std::sort(books.begin(), books.end(), [option](const Book& left, const Book& right) {
        switch (option) {
            case SortOption::Title:
                return left.getTitle() < right.getTitle();
            case SortOption::Year:
                if (left.getYear() == right.getYear()) {
                    return left.getTitle() < right.getTitle();
                }
                return left.getYear() < right.getYear();
            case SortOption::Author:
                if (left.getAuthor() == right.getAuthor()) {
                    return left.getTitle() < right.getTitle();
                }
                return left.getAuthor() < right.getAuthor();
        }

        return left.getTitle() < right.getTitle();
    });

    return books;
}

bool BookService::exportToCsv(const std::string& csvFilePath, std::string& message) const {
    std::string errorMessage;
    const std::vector<Book> books = repository_.getAll(errorMessage);

    if (!errorMessage.empty()) {
        message = errorMessage;
        return false;
    }

    std::ofstream csvFile(csvFilePath, std::ios::trunc);
    if (!csvFile) {
        message = "Failed to create CSV export file.";
        return false;
    }

    csvFile << Book::csvHeader() << '\n';
    for (const Book& book : books) {
        csvFile << book.toCsvRow() << '\n';
    }

    message = "CSV exported successfully.";
    return true;
}

DashboardStats BookService::getDashboardStats(std::string& errorMessage) const {
    const std::vector<Book> books = repository_.getAll(errorMessage);
    DashboardStats stats;

    if (!errorMessage.empty()) {
        return stats;
    }

    stats.totalBooks = books.size();
    for (const Book& book : books) {
        if (book.isAvailable()) {
            ++stats.availableBooks;
        } else {
            ++stats.checkedOutBooks;
        }
    }

    if (!books.empty()) {
        const auto oldestIt = std::min_element(books.begin(), books.end(), [](const Book& left, const Book& right) {
            return left.getYear() < right.getYear();
        });

        const auto newestIt = std::max_element(books.begin(), books.end(), [](const Book& left, const Book& right) {
            return left.getYear() < right.getYear();
        });

        stats.oldestTitle = oldestIt->getTitle();
        stats.newestTitle = newestIt->getTitle();
    }

    return stats;
}
