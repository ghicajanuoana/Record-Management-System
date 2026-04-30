#include "models/Book.h"

#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
std::vector<std::string> splitEscaped(const std::string& input, char delimiter) {
    std::vector<std::string> parts;
    std::string current;
    bool escaped = false;

    for (char ch : input) {
        if (escaped) {
            current.push_back(ch);
            escaped = false;
            continue;
        }

        if (ch == '\\') {
            escaped = true;
            continue;
        }

        if (ch == delimiter) {
            parts.push_back(current);
            current.clear();
            continue;
        }

        current.push_back(ch);
    }

    if (escaped) {
        current.push_back('\\');
    }

    parts.push_back(current);
    return parts;
}
}

Book::Book(int id, std::string title, std::string author, int year, std::string genre, bool available)
    : id_(id),
      title_(std::move(title)),
      author_(std::move(author)),
      year_(year),
      genre_(std::move(genre)),
      available_(available) {}

int Book::getId() const {
    return id_;
}

const std::string& Book::getTitle() const {
    return title_;
}

const std::string& Book::getAuthor() const {
    return author_;
}

int Book::getYear() const {
    return year_;
}

const std::string& Book::getGenre() const {
    return genre_;
}

bool Book::isAvailable() const {
    return available_;
}

void Book::setTitle(const std::string& title) {
    title_ = title;
}

void Book::setAuthor(const std::string& author) {
    author_ = author;
}

void Book::setYear(int year) {
    year_ = year;
}

void Book::setGenre(const std::string& genre) {
    genre_ = genre;
}

void Book::setAvailable(bool available) {
    available_ = available;
}

std::string Book::serialize() const {
    std::ostringstream output;
    output << id_ << '|'
           << escape(title_) << '|'
           << escape(author_) << '|'
           << year_ << '|'
           << escape(genre_) << '|'
           << (available_ ? 1 : 0);
    return output.str();
}

Book Book::deserialize(const std::string& line) {
    const std::vector<std::string> parts = splitEscaped(line, '|');
    if (parts.size() != 6) {
        throw std::runtime_error("Invalid record format.");
    }

    return Book(
        std::stoi(parts[0]),
        unescape(parts[1]),
        unescape(parts[2]),
        std::stoi(parts[3]),
        unescape(parts[4]),
        parts[5] == "1"
    );
}

std::string Book::csvHeader() {
    return "ID,Title,Author,Year,Genre,Availability";
}

std::string Book::toCsvRow() const {
    std::ostringstream output;
    output << id_ << ','
           << csvEscape(title_) << ','
           << csvEscape(author_) << ','
           << year_ << ','
           << csvEscape(genre_) << ','
           << csvEscape(available_ ? "Available" : "Checked Out");
    return output.str();
}

std::string Book::escape(const std::string& value) {
    std::string result;
    for (char ch : value) {
        if (ch == '\\' || ch == '|') {
            result.push_back('\\');
        }
        result.push_back(ch);
    }
    return result;
}

std::string Book::unescape(const std::string& value) {
    std::string result;
    bool escaped = false;

    for (char ch : value) {
        if (escaped) {
            result.push_back(ch);
            escaped = false;
            continue;
        }

        if (ch == '\\') {
            escaped = true;
            continue;
        }

        result.push_back(ch);
    }

    if (escaped) {
        result.push_back('\\');
    }

    return result;
}

std::string Book::csvEscape(const std::string& value) {
    bool needsQuotes = false;
    std::string result;

    for (char ch : value) {
        if (ch == '"' || ch == ',' || ch == '\n') {
            needsQuotes = true;
        }

        if (ch == '"') {
            result += "\"\"";
        } else {
            result.push_back(ch);
        }
    }

    if (needsQuotes) {
        return "\"" + result + "\"";
    }

    return result;
}
