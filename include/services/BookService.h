#ifndef BOOK_SERVICE_H
#define BOOK_SERVICE_H

#include "repositories/BookRepository.h"

#include <optional>
#include <string>
#include <vector>

struct DashboardStats {
    std::size_t totalBooks {0};
    std::size_t availableBooks {0};
    std::size_t checkedOutBooks {0};
    std::string oldestTitle {"N/A"};
    std::string newestTitle {"N/A"};
};

enum class SortOption {
    Title = 1,
    Year = 2,
    Author = 3
};

class BookService {
public:
    explicit BookService(BookRepository repository);

    bool addBook(const Book& book, std::string& message) const;
    std::vector<Book> getAllBooks(std::string& errorMessage) const;
    std::optional<Book> findBookById(int id, std::string& errorMessage) const;
    bool updateBook(const Book& book, std::string& message) const;
    bool deleteBook(int id, std::string& message) const;
    std::vector<Book> getSortedBooks(SortOption option, std::string& errorMessage) const;
    bool exportToCsv(const std::string& csvFilePath, std::string& message) const;
    DashboardStats getDashboardStats(std::string& errorMessage) const;

private:
    BookRepository repository_;
};

#endif
