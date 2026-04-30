#include "repositories/BookRepository.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

BookRepository::BookRepository(std::string dataFilePath, std::string deletedLogPath)
    : dataFilePath_(std::move(dataFilePath)),
      deletedLogPath_(std::move(deletedLogPath)) {}

bool BookRepository::add(const Book& book, std::string& errorMessage) const {
    if (!ensureStorageReady(errorMessage)) {
        return false;
    }

    std::ofstream file(dataFilePath_, std::ios::app);
    if (!file) {
        errorMessage = "Failed to open data file for writing.";
        return false;
    }

    file << book.serialize() << '\n';
    return true;
}

std::vector<Book> BookRepository::getAll(std::string& errorMessage) const {
    std::vector<Book> books;

    if (!ensureStorageReady(errorMessage)) {
        return books;
    }

    std::ifstream file(dataFilePath_);
    if (!file) {
        errorMessage = "Failed to open data file for reading.";
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        try {
            books.push_back(Book::deserialize(line));
        } catch (const std::exception&) {
            errorMessage = "Failed to parse one or more records from storage.";
            return {};
        }
    }

    return books;
}

std::optional<Book> BookRepository::findById(int id, std::string& errorMessage) const {
    const std::vector<Book> books = getAll(errorMessage);
    if (!errorMessage.empty()) {
        return std::nullopt;
    }

    for (const Book& book : books) {
        if (book.getId() == id) {
            return book;
        }
    }

    return std::nullopt;
}

bool BookRepository::update(const Book& updatedBook, std::string& errorMessage) const {
    std::vector<Book> books = getAll(errorMessage);
    if (!errorMessage.empty()) {
        return false;
    }

    bool found = false;
    for (Book& book : books) {
        if (book.getId() == updatedBook.getId()) {
            book = updatedBook;
            found = true;
            break;
        }
    }

    if (!found) {
        errorMessage = "Record ID not found.";
        return false;
    }

    return writeAll(books, errorMessage);
}

bool BookRepository::remove(int id, std::string& errorMessage) const {
    std::vector<Book> books = getAll(errorMessage);
    if (!errorMessage.empty()) {
        return false;
    }

    std::vector<Book> remainingBooks;
    std::optional<Book> deletedBook;

    for (const Book& book : books) {
        if (book.getId() == id) {
            deletedBook = book;
        } else {
            remainingBooks.push_back(book);
        }
    }

    if (!deletedBook.has_value()) {
        errorMessage = "Record ID not found.";
        return false;
    }

    if (!writeAll(remainingBooks, errorMessage)) {
        return false;
    }

    std::ofstream deletedLog(deletedLogPath_, std::ios::app);
    if (deletedLog) {
        deletedLog << deletedBook->serialize() << '\n';
    }

    return true;
}

bool BookRepository::ensureStorageReady(std::string& errorMessage) const {
    try {
        fs::create_directories(fs::path(dataFilePath_).parent_path());
        fs::create_directories(fs::path(deletedLogPath_).parent_path());
    } catch (const fs::filesystem_error&) {
        errorMessage = "Failed to create data directories.";
        return false;
    }

    if (!fs::exists(dataFilePath_)) {
        std::ofstream createFile(dataFilePath_);
        if (!createFile) {
            errorMessage = "Failed to initialize data file.";
            return false;
        }
    }

    return true;
}

bool BookRepository::writeAll(const std::vector<Book>& books, std::string& errorMessage) const {
    if (!ensureStorageReady(errorMessage)) {
        return false;
    }

    const std::string tempFilePath = dataFilePath_ + ".tmp";
    std::ofstream tempFile(tempFilePath, std::ios::trunc);

    if (!tempFile) {
        errorMessage = "Failed to create temporary file for save operation.";
        return false;
    }

    for (const Book& book : books) {
        tempFile << book.serialize() << '\n';
    }

    tempFile.close();

    try {
        fs::rename(tempFilePath, dataFilePath_);
    } catch (const fs::filesystem_error&) {
        try {
            fs::remove(dataFilePath_);
            fs::rename(tempFilePath, dataFilePath_);
        } catch (const fs::filesystem_error&) {
            errorMessage = "Failed to replace data file after update.";
            return false;
        }
    }

    return true;
}
