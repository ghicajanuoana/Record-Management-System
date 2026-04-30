#ifndef BOOK_REPOSITORY_H
#define BOOK_REPOSITORY_H

#include "models/Book.h"

#include <optional>
#include <string>
#include <vector>

class BookRepository {
public:
    explicit BookRepository(std::string dataFilePath, std::string deletedLogPath);

    bool add(const Book& book, std::string& errorMessage) const;
    std::vector<Book> getAll(std::string& errorMessage) const;
    std::optional<Book> findById(int id, std::string& errorMessage) const;
    bool update(const Book& updatedBook, std::string& errorMessage) const;
    bool remove(int id, std::string& errorMessage) const;

private:
    std::string dataFilePath_;
    std::string deletedLogPath_;

    bool ensureStorageReady(std::string& errorMessage) const;
    bool writeAll(const std::vector<Book>& books, std::string& errorMessage) const;
};

#endif
