# Mini-SQL-Database
#### README updated by Claude Code

A minimal SQL database engine implemented in C++11. Supports `CREATE`, `INSERT`, and `SELECT` (with full `WHERE` expressions), backed by B+ tree indices and fixed-size binary record files on disk.

---

## Features

- **DDL / DML**: `make table`, `insert into`, `select`
- **WHERE clauses**: relational operators (`=`, `<`, `>`, `<=`, `>=`) and logical operators (`AND`, `OR`) with correct precedence
- **B+ tree indexing**: every field is indexed; queries use index lookups, not table scans
- **On-disk persistence**: records stored as fixed-size binary blocks; tables survive restarts
- **Batch mode**: execute a `.batch` file of SQL commands programmatically
- **Interactive REPL**: `sql.run()` prompts for commands until quit

---

## Build & Run

Requirements: CMake 3.x, a C++11 compiler (GCC or Clang).

```bash
mkdir build && cd build
cmake ..
make
```

This produces three executables in `build/bin/`:

| Executable   | Description                    |
|--------------|--------------------------------|
| `main`       | Interactive SQL REPL           |
| `basic_test` | Google Test suite (basic)      |
| `testB`      | Google Test suite (extended)   |

Run the interactive shell:

```bash
./bin/main
```

Run a batch file from code:

```cpp
SQL sql("command.batch");   // reads, executes, and prints all commands
```

---

## SQL Syntax Reference

### Create a table

```sql
make table <name> fields <field1>, <field2>, ...

-- or equivalently:
create table <name> fields <field1>, <field2>, ...
```

Example:

```sql
make table student fields fname, lname, major, age
```

### Insert a record

```sql
insert into <table> values <val1>, <val2>, ...
```

Values with spaces must be quoted:

```sql
insert into student values "Anna Grace", "Del Rio", CS, 22
```

### Select records

```sql
-- all fields, all records
select * from student

-- specific fields only
select fname, lname from student

-- with a WHERE condition
select * from student where lname = Jackson
select * from student where age > 20
select * from student where lname >= Yang
select * from student where age <= 23
```

### WHERE with AND / OR

AND binds more tightly than OR (standard SQL precedence):

```sql
-- AND
select * from student where fname = Flo and lname = Yao

-- OR
select * from student where fname = Flo or lname = Jackson

-- mixed (AND evaluated before OR)
select * from student where fname = Flo or major = CS and age <= 23

-- complex
select * from student where age < 30 and major = CS or major = Physics and lname = Jackson
```

### Quoted strings

Wrap any value that contains spaces in double quotes:

```sql
insert into employee values "Van Gogh", "Jim Bob", "Phys Ed"
select * from employee where last = "Van Gogh"
```

### Comments

Lines starting with `//` are printed as comments and not executed:

```sql
// this is a comment
select * from student
```

---

## Architecture

A SQL command travels through these layers:

```
User input string
  └─> SQL::command()
        └─> Parser::set_string()          (DFA state machine → parse tree)
              └─> Table::insert_into()    (DDL/DML branch)
                  Table::select()         (SELECT branch)
                    └─> ShuntingYard      (infix WHERE → postfix RPN)
                          └─> Token::eval() chain
                                └─> MMap<string,long> B+ tree index lookups
                                      └─> FileRecord binary I/O
```

---

## Component Reference

### Table (`includes/table/`)

The central abstraction. A `Table` owns its field list, a B+ tree index per field, and a binary record file on disk.

**Type aliases** (`typedefs.h`):

| Alias        | Underlying type              | Used for                              |
|--------------|------------------------------|---------------------------------------|
| `vectorstr`  | `vector<string>`             | field names, values, conditions       |
| `vectorlong` | `vector<long>`               | sets of record numbers                |
| `map_sl`     | `Map<string, long>`          | field name → column index             |
| `mmap_ss`    | `MMap<string, string>`       | parser output (parse tree)            |
| `mmap_sl`    | `MMap<string, long>`         | field value → record numbers (index)  |

**Constructors:**

```cpp
Table();                                              // empty table
Table(const string& name);                            // load existing table from disk
Table(const string& name, const vector<string>& field_names); // create new table
```

**Key public methods:**

| Method | Description |
|--------|-------------|
| `insert_into(vectorstr& fields)` | Appends a record to the binary file and updates all field indices. Returns the record number. |
| `select_all()` | Returns a new Table containing every record and every field. |
| `select_all(vectorstr fields)` | Returns a new Table with every record but only the specified fields. |
| `select(fields, field, op, value)` | Single-condition query. Resolves using B+ tree index, returns result Table. |
| `select(fields, Queue<Token*> postfix)` | Multi-condition query from a postfix token queue (used internally). |
| `select(fields, vectorstr condition)` | Multi-condition query from a raw string list; converts to postfix via `ShuntingYard` internally. |
| `reindex()` | Rebuilds all B+ tree indices by replaying the binary file. Called by the loading constructor. |
| `get_fields() const` | Returns `vectorstr` of field names. |
| `is_empty() const` | True if the table has no records. |
| `Name() const` | Returns the table name string. |
| `select_recnos() const` | Returns `vector<long>` of record numbers from the most recent query. |
| `field_col_no(string field_name)` | Returns the 0-based column index for a field name. |

**Index structure:**

`_indices` is a `vector<mmap_sl>` — one `MMap<string,long>` per field. When a record is inserted, each field's value is added to its corresponding multimap pointing to that record number. Queries use B+ tree `lower_bound`/`upper_bound` to resolve operators efficiently without scanning records.

---

### SQL Engine (`includes/sql/`)

Top-level orchestrator. Owns the active `Table` and a `Map<string, Table>` of all created tables.

**Constructors:**

```cpp
SQL();                      // empty engine, ready for commands
SQL(string filename);       // batch mode: reads and executes a .batch file
```

**Public methods:**

| Method | Description |
|--------|-------------|
| `command(string input)` | Parses and executes one SQL string. Returns the result `Table`. Supports `make`/`create`, `insert`, `select`. |
| `run()` | Interactive REPL: reads commands from stdin until the user quits. |
| `select_recnos()` | Returns the record numbers matched by the last `SELECT`. |

**`command()` execution flow:**
1. Constructs a `Parser`, calls `set_string()`.
2. Reads the parse tree (an `mmap_ss`): keys `command`, `table_name`, `col`, `values`, `fields`, `condition`.
3. Branches:
   - `make`/`create` → constructs a new `Table` with given field names.
   - `insert` → loads or reuses the table, calls `insert_into()`.
   - `select` → loads or reuses the table, calls `select()` (with or without conditions).

---

### Parser (`includes/parser/`)

Converts a raw SQL string into a structured **parse tree** (`mmap_ss`) using a DFA state machine.

**Main method:**

```cpp
void set_string(char* str);
mmap_ss parse_tree();
```

**Parse tree keys:**

| Key | Contains |
|-----|----------|
| `command` | `make`, `create`, `insert`, `select` |
| `table_name` | the table being created/queried |
| `col` | field names in a `CREATE`/`MAKE` |
| `values` | values in an `INSERT` |
| `fields` | projected fields in a `SELECT` (or `*`) |
| `condition` | raw tokens after `WHERE` |
| `where` | present if a `WHERE` clause exists |

**How it works:**
1. The raw string is tokenized by `STokenizer` into a queue of tokens.
2. Tokens flow through `get_parse_tree()`, which drives a transition table (2D `int` array).
3. Each state reached triggers extraction of a token into a specific parse-tree key.
4. Quoted strings are treated as single atoms by the tokenizer step.

---

### B+ Tree & Containers (`includes/bplustree/`)

#### `BPlusTree<T>`

Self-balancing B+ tree. Leaf nodes are linked for efficient sequential and range access.

Key operations:

| Method | Description |
|--------|-------------|
| `insert(entry)` | Insert (duplicates allowed when `dups_ok=true`) |
| `remove(entry)` | Remove a key |
| `contains(entry)` | True if key exists |
| `find(key)` | Returns pointer to stored entry |
| `lower_bound(key)` | Iterator to first entry >= key |
| `upper_bound(key)` | Iterator to first entry > key |
| `begin()` / `end()` | Iterators over all leaf entries |

#### `Map<K, V>`

Wraps `BPlusTree<Pair<K,V>>`. STL-`map`-like: unique keys, `operator[]`, `insert`, `find`, `contains`, range queries. Used for field-name-to-index lookups and the table registry.

#### `MMap<K, V>`

Wraps `BPlusTree<MPair<K, vector<V>>>`. Each key maps to a `vector<V>`. `insert(k, v)` appends `v` to the existing vector for key `k`. Used as the per-field index (`mmap_sl`): field value → list of matching record numbers.

---

### Binary I/O (`includes/binary_files/`)

#### `FileRecord`

Represents one database record as a fixed-size 2D char array: `char _record[101][101]` (up to 101 fields, 100 chars each = 10,201 bytes per record).

```cpp
FileRecord();
FileRecord(vector<string> v);         // construct from field values

long write(fstream& outs);            // append record, return record number
void read(fstream& ins, long recno);  // random access read at offset recno
vector<string> record_info();         // decode to vector<string>
```

Records are stored at byte offset `recno × sizeof(_record)`, enabling O(1) random access by record number.

#### Utility functions

```cpp
bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]);    // binary R/W, create if missing
void open_fileW(fstream& f, const char filename[]);     // binary write-only
vector<string> vector_from_file(const string& filename); // read whitespace tokens
```

---

### Expression Evaluation (`includes/shunting_yard/` + `includes/token/`)

#### `ShuntingYard`

Converts an infix WHERE condition to a postfix (RPN) `Queue<Token*>` using the Dijkstra shunting-yard algorithm.

```cpp
ShuntingYard(vectorstr list);              // from raw string tokens
ShuntingYard(Queue<Token*> infix);         // from pre-built token queue
Queue<Token*> postfix();                   // returns converted postfix queue
```

Operator precedence enforced: **AND > OR**.

#### Token hierarchy

All tokens derive from `Token` (abstract base in `includes/token/token.h`):

| Class | Type | Role |
|-------|------|------|
| `TokenStr` | 1 | Operand: field name or literal value |
| `Result_set` | 2 | Wraps `vector<long>` of matching record numbers |
| `LeftParen` | 5 | `(` grouping |
| `RightParen` | 6 | `)` grouping |
| `Logical` | 7 | `AND` / `OR` |
| `Relational` | 8 | `=`, `<`, `>`, `<=`, `>=` |

**Evaluation:**

```cpp
// Relational: query B+ tree index, return matching record numbers
Result_set* Relational::eval(TokenStr* field, TokenStr* value,
                              vector<mmap_sl>& indices, map_sl& lookup);

// Logical: combine two Result_sets
Result_set* Logical::eval(Result_set* left, Result_set* right);
// AND → intersection, OR → union
```

`Table::cond()` drives a stack-based postfix evaluator over the queue produced by `ShuntingYard::postfix()`.

---

### Tokenizers (`includes/tokenizer/`)

#### `STokenizer`

DFA-based lexer for null-terminated C-strings.

```cpp
STokenizer(char str[]);
void set_string(char str[]);
bool done();
STokenizer& operator>>(Tokenog& t);    // extract next token
```

Token types produced (`Tokenog`): `TOKEN_ALPHA`, `TOKEN_NUMBER`, `TOKEN_SPACE`, `TOKEN_OPERATOR`, `TOKEN_PUNC`, `TOKEN_QUOTE`, `TOKEN_LEFT`, `TOKEN_RIGHT`, `TOKEN_STAR`.

#### `FTokenizer`

File-level tokenizer. Wraps `STokenizer` and reads files in `MAX_BUFFER`-byte blocks, handling block boundaries transparently.

```cpp
FTokenizer(const char* fname);
bool more();
FTokenizer& operator>>(Tokenog& t);
```

---

### Data Structures (`includes/node/`, `includes/queue/`, `includes/stack/`)

#### `node<T>`

Doubly-linked list node with `_item`, `_next`, and `_prev` pointers. Used internally by `MyQueue` and `MyStack`.

#### `MyQueue<T>`

FIFO queue backed by a doubly-linked list.

```cpp
void push(T item);   // enqueue at rear
T    pop();          // dequeue from front
T    front();
bool empty();
```

Used throughout the engine to hold token sequences.

#### `MyStack<T>`

LIFO stack backed by a singly-linked list.

```cpp
void push(T item);   // insert at top
T    pop();          // remove from top
T    top();
bool empty();
int  size();
```

Used by `ShuntingYard` for the operator stack and by `Table::cond()` for postfix evaluation.

---

## Project Layout

```
Mini-SQL-Database/
├── main.cpp                          # Entry point: creates SQL and calls sql.run()
├── CMakeLists.txt
├── command.batch                     # Example SQL script
├── includes/
│   ├── table/
│   │   ├── table.h / table.cpp       # Table class (core)
│   │   └── typedefs.h                # vectorstr, mmap_sl, etc.
│   ├── sql/
│   │   └── sql.h / sql.cpp           # SQL orchestrator
│   ├── parser/
│   │   └── parser.h / parser.cpp     # DFA SQL parser
│   ├── bplustree/
│   │   ├── bplustree.h               # BPlusTree<T>
│   │   ├── map.h                     # Map<K,V>
│   │   └── multimap.h                # MMap<K,V>
│   ├── binary_files/
│   │   ├── file_record.h / .cpp      # FileRecord (binary persistence)
│   │   └── utilities.h / .cpp        # file open/exist helpers
│   ├── shunting_yard/
│   │   └── shunting_yard.h / .cpp    # Infix → postfix conversion
│   ├── token/
│   │   ├── token.h / token.cpp       # Token base class
│   │   └── child_tokens.h / .cpp     # TokenStr, Relational, Logical, Result_set
│   ├── tokenizer/
│   │   ├── stokenize.h / .cpp        # STokenizer (string lexer)
│   │   ├── ftokenize.h / .cpp        # FTokenizer (file lexer)
│   │   └── token.h / token.cpp       # Tokenog (lexical token)
│   ├── queue/
│   │   └── MyQueue.h
│   ├── stack/
│   │   └── MyStack.h
│   └── node/
│       └── node.h
└── _tests/
    └── _test_files/
        ├── basic_test.cpp
        └── testB.cpp
```
