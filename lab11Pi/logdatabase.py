import sqlite3
import time
import os


def create_database():
    if os.path.exists("logs.db"):
        os.remove("logs.db")
        print("Old data base removed")
    connection = sqlite3.connect("logs.db")
    cursor = connection.cursor()
    cursor.execute("""
        CREATE TABLE logs (
            card_id text,
            time text
        )
    """)
    connection.commit()
    connection.close()
    print("New database created")


if __name__ == '__main__':
    create_database()