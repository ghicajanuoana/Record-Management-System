# Record Management System

A professional C++17 console application for managing book records with file-based persistence. The system follows OOP principles and separates responsibilities into model, repository, service, and UI layers.

## Features

- Add, display, search, update, and delete book records
- Persistent file storage in `data/books.txt`
- Sorting by title, year, or author
- CSV export to `exports/books_export.csv`
- Input validation and user-friendly error handling
- Extra feature: dashboard summary with collection statistics
- Extra feature: deleted record audit log in `data/deleted_books.log`

## Record Type

This project manages **book records** using the following fields:

- `ID`
- `Title`
- `Author`
- `Year`
- `Genre`
- `Availability`

## Project Structure

```text
record_manager/
|-- CMakeLists.txt
|-- README.md
|-- data/
|-- docs/
|   `-- screenshots/
|-- exports/
|-- include/
|   |-- models/
|   |-- repositories/
|   |-- services/
|   `-- ui/
`-- src/
    |-- main.cpp
    |-- models/
    |-- repositories/
    |-- services/
    `-- ui/
```

## Build Instructions

### Using CMake

```powershell
cmake -S . -B build
cmake --build build
```

### Run

```powershell
.\build\record_manager.exe
```

### Portable Build Script

If a C++ compiler is not installed on your machine, use the included PowerShell build script. It downloads a local portable Zig toolchain into `tools/` and builds the project without requiring admin installation.

```powershell
.\scripts\build.ps1
```

### Demo Run and Screenshot Generation

To reproduce the demo flow and generate the screenshot assets from real program runs:

```powershell
.\scripts\run_demo.ps1
```

## Menu Overview

1. Add record
2. Display all records
3. Search record by ID
4. Update record
5. Delete record
6. Sort records
7. Export to CSV
8. Dashboard summary
0. Exit

## Storage Notes

- Main storage file: `data/books.txt`
- Deleted records log: `data/deleted_books.log`
- CSV export file: `exports/books_export.csv`

For update and delete operations, the application rewrites the storage file safely using a temporary file. This is more reliable than direct in-place editing for variable-length text records.

## Screenshots

Screenshots from actual runs are stored in `docs/screenshots/` and generated from deterministic CLI sessions:

- `run_add_display.png`
- `run_update_search.png`
- `run_sort_export_dashboard.png`

## Sample Test Flow

You can test the system with this example:

1. Add a few books
2. Display all records
3. Search by a known ID
4. Update one book
5. Delete one book
6. Sort the records
7. Export to CSV
8. Open dashboard summary

## Professional Practices Applied

- OOP design with encapsulated `Book` model
- Repository pattern for storage access
- Service layer for business rules
- Input validation for IDs, years, and yes/no responses
- Safe file handling and clear separation of concerns
