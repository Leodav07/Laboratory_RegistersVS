## LAB 1 | VARIABLE LENGTH RECORDS & SIMPLE PRIMARY INDEX

###### University: Unversidad Tecnológica Centroamericana UNITEC
###### Professor: Ing. Iván De Jesús Deras Tábora
###### Course: Estructura de Datos II - Q2 2026
###### Student: Leonardo David López Velásquez 

## LAB Description

#### Command-line application in C++ that manages student records stored in a binary file using variable length records and a simple primary index loaded entirely in memory. Every method is from scratch, there aren't external libraries or database used.

### Project Structure

```
LaboratorioRTV/
├── src/
│   ├── main.cpp
│   ├── FileManager.cpp
│   └── CliArgs.cpp
├── include/
│   ├── FileManager.hpp
│   ├── CliArgs.hpp
│   ├── CliCommand.hpp
│   ├── Student.hpp
│   ├── Index.hpp
│   └── json.hpp
├── test/
│   ├── students_batch.json
│   ├── student_single.json
│   └── student_update.json
├── CMakeLists.txt
└── README.md

```

## How to Compile

```bash
# 1. Clone the repository
git clone https://github.com/Leodav07/Laboratory_RegistersVS.git
cd <Laboratory_RegistersVS>

# 2. Create the build directory
mkdir build
cd build

# 3. Generate build files
cmake ..

# 4. Compile
cmake --build .
```
---

## How to Run

The program is executed from the terminal with the following syntax:

```
./rtv_LAB --data <file.data> [--index <file.idx>] <command> [argument]
```

### Flags

| Flag | Required | Description |
|------|----------|-------------|
| --data <file.data> | Yes | Binary data file where student records are stored |
| --index <file.idx> | No | Index file. If not specified, it is deduced from the data file name |

### Commands

| Command | Argument | Description |
|---------|----------|-------------|
| --add | <file.json> | Add one or more student records from a JSON file |
| --search | <account> | Search and display a student record by account number |
| --remove | <account> | Soft-delete a student record by account number |
| --update | <file.json> | Update an existing student record from a JSON file |
| --clean-up | — | Rebuild the data file removing all soft-deleted records |

---

## Usage Examples

### Add multiple students from a JSON array
```bash
./rtv_LAB --data students.data --add students_batch.json
```

### Add a single student
```bash
./rtv_LAB --data students.data --add student_single.json
```

### Search a student by account number
```bash
./rtv_LAB --data students.data --search 2020-00055
```

### Remove a student (soft delete)
```bash
./rtv_LAB --data students.data --remove 2023-00099
```

### Update a student record
```bash
./rtv_LAB --data students.data --update student_update.json
```

### Clean up soft-deleted records
```bash
./rtv_LAB --data students.data --clean-up
```

### Using a custom index file
```bash
./rtv_LAB --data students.data --index custom.idx --search 2019-00010
```

---

## JSON File Format

### Adding students (array or single object)

```json
[
    {
        "account": "2020-00001",
        "name": "Juan Carlos Perez Lopez",
        "phone": "502-1234-567",
        "age": 22,
        "entryDate": "20200115"
    }
]
```

### Updating a student (single object, account must already exist)

```json
{
    "account": "2020-00001",
    "name": "Juan Carlos Perez Actualizado",
    "phone": "502-9999-000",
    "age": 23,
    "entryDate": "20200115"
}
```

### Field constraints

| Field | Type | Size | Notes |
|-------|------|------|-------|
| account | string | Exactly 10 characters | Primary key, unique |
| name | string | Variable | No size restriction |
| phone | string | Exactly 12 characters | |
| age | integer | — | Positive integer |
| entryDate | string | Exactly 8 characters | Format: YYYYMMDD |

---

## Deletion Strategy

This project implements Soft Delete.

When a student record is deleted with --remove, it is not physically removed from the data file. Instead:

- The first byte of the record (the isActive flag) is overwritten with 0 (inactive) directly in the binary file.
- The corresponding index entry is also marked as inactive in memory and persisted to the index file.
- The record remains physically in the file but is completely ignored by all operations (search, update).

To physically remove all soft-deleted records, the --clean-up command must be executed. It:

1. Reads all active records from the original data file.
2. Writes them to a temporary file, recalculating all offsets.
3. Replaces the original data file with the temporary one.
4. Rebuilds the index from scratch with the new offsets.
5. Removes all inactive entries from the index permanently.

---

## Test Files

| File | Purpose |
|------|---------|
| students_batch.json | Add 3 students in a single command (array format) |
| student_single.json | Add a single student (object format) |
| student_update.json | Update an existing student record |

These JSON files can be used to test every command.
---

## Error Handling

The application validates all input before performing any operation:

- Account number must be exactly 10 characters
- Phone number must be exactly 12 characters  
- Entry date must be exactly 8 characters
- Duplicate account numbers are rejected on insert
- Non-existent or inactive accounts are rejected on update and search
- All binary file I/O errors are reported with descriptive messages