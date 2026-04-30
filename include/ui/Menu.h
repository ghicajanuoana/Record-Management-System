#ifndef MENU_H
#define MENU_H

#include "services/BookService.h"

class Menu {
public:
    explicit Menu(BookService service);
    void run();

private:
    BookService service_;

    void showMenu() const;
    void handleAddRecord();
    void handleDisplayAllRecords() const;
    void handleSearchById() const;
    void handleUpdateRecord();
    void handleDeleteRecord();
    void handleSortRecords() const;
    void handleExportCsv() const;
    void handleDashboard() const;

    static void printBookTable(const std::vector<Book>& books);
    static void printSingleBook(const Book& book);
};

#endif
