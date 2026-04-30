#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
public:
    Book() = default;
    Book(int id, std::string title, std::string author, int year, std::string genre, bool available);

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getYear() const;
    const std::string& getGenre() const;
    bool isAvailable() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setYear(int year);
    void setGenre(const std::string& genre);
    void setAvailable(bool available);

    std::string serialize() const;
    static Book deserialize(const std::string& line);
    static std::string csvHeader();
    std::string toCsvRow() const;

private:
    int id_ {0};
    std::string title_;
    std::string author_;
    int year_ {0};
    std::string genre_;
    bool available_ {true};

    static std::string escape(const std::string& value);
    static std::string unescape(const std::string& value);
    static std::string csvEscape(const std::string& value);
};

#endif
