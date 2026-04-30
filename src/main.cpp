#include "repositories/BookRepository.h"
#include "services/BookService.h"
#include "ui/Menu.h"

int main() {
    BookRepository repository("data/books.txt", "data/deleted_books.log");
    BookService service(repository);
    Menu menu(service);
    menu.run();
    return 0;
}
