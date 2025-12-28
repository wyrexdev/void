#include "Core/SQLite/SQLite.hpp"

namespace SQLite
{
    SQLite::SQLite()
    {
        Storage::FileStore::mkdirRecursive(Storage::FileStore::BASE_PATH + "/db/");

        std::string path = Storage::FileStore::BASE_PATH + "/db/cookies.db";
        exit = sqlite3_open(path.c_str(), &db);

        if (exit)
        {
            std::cout << "Error open DB " << sqlite3_errmsg(db) << std::endl;
            return;
        }
    }

    SQLite::~SQLite()
    {
        sqlite3_close(db);
    }

    void SQLite::checkAndCreateTables()
    {
        char *messaggeError;
        exit = sqlite3_exec(db, checkQuery.c_str(), NULL, 0, &messaggeError);

        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Checking Tables" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Checking Finished Successfully" << std::endl;
    }

    void SQLite::insert(
        const std::string &table,
        const std::vector<std::string> &columns,
        const std::vector<std::string> &values)
    {
        if (columns.size() != values.size())
        {
            std::cerr << "Insert error: columns/value size mismatch\n";
            return;
        }

        std::string cls;
        for (size_t i = 0; i < columns.size(); i++)
        {
            cls += columns[i];
            if (i + 1 < columns.size())
                cls += ", ";
        }

        std::string placeholders;
        for (size_t i = 0; i < values.size(); i++)
        {
            placeholders += "?";
            if (i + 1 < values.size())
                placeholders += ", ";
        }

        std::string query =
            "INSERT INTO " + table +
            " (" + cls + ") VALUES (" + placeholders + ");";

        sqlite3_stmt *stmt = nullptr;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        for (size_t i = 0; i < values.size(); i++)
        {
            sqlite3_bind_text(
                stmt,
                static_cast<int>(i + 1),
                values[i].c_str(),
                -1,
                SQLITE_TRANSIENT);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else
        {
            std::cout << "Row inserted successfully\n";
        }

        sqlite3_finalize(stmt);
    }

    static int selectCallback(void *data, int argc, char **argv, char **colName)
    {
        std::string *result = static_cast<std::string *>(data);

        for (int i = 0; i < argc; i++)
        {
            *result += colName[i];
            *result += "=";
            *result += (argv[i] ? argv[i] : "NULL");
            *result += " ";
        }

        *result += "\n";
        return 0;
    }

    std::string SQLite::select(std::string table, std::string where)
    {
        std::string result;
        std::string query = "SELECT * FROM " + table;

        if (!where.empty())
            query += " WHERE " + where;

        query += ";";

        char *messageError = nullptr;

        int rc = sqlite3_exec(
            db,
            query.c_str(),
            selectCallback,
            &result,
            &messageError);

        if (rc != SQLITE_OK)
        {
            std::cerr << "SELECT error: " << messageError << std::endl;
            sqlite3_free(messageError);
            return "";
        }

        return result;
    }
} // namespace
