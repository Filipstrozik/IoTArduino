import sqlite3
import time
import os


# def create_database():
#     if os.path.exists("logs.db"):
#         os.remove("logs.db")
#         print("Old data base removed")
#     connection = sqlite3.connect("logs.db")
#     cursor = connection.cursor()
#     cursor.execute("""
#         CREATE TABLE logs (
#             card_id text,
#             time text
#         )
#     """)
#     connection.commit()
#     connection.close()
#     print("New database created")

def create_database():
    if os.path.exists("logs.db"):
        os.remove("logs.db")
        print("Old data base removed")
    connection = sqlite3.connect("logs.db")
    cursor = connection.cursor()
    cursor.execute("""
        CREATE TABLE logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            card_id text,
            time text,
            FOREIGN KEY (card_id) REFERENCES employees(card_id)
        )
    """)
    connection.commit()
    cursor.execute("""
        CREATE TABLE employees (
            card_id text PRIMARY KEY,
            name text
        )
    """)
    connection.commit()
    connection.close()
    print("New database created")


if __name__ == '__main__':
    create_database()