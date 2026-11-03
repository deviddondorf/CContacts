#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sqlite3.h>

sqlite3 *constructdb();
void menu(sqlite3 *db);
void drawlogo(void);

// Menu Options
void addcontact(sqlite3 *db);
void listcontacts(sqlite3 *db);
void deletecontact(sqlite3 *db);

int main(void)
{
    sqlite3 *db = constructdb();
    if (db == NULL)
    {
        printf("Failed opening db\n");
        return 1;
    }
    menu(db);
    return 0;
}

sqlite3 *constructdb()
{
    const char *homedir = getenv("HOME");
    if (homedir == NULL)
    {
        printf("Can't find Home :( \n");
    }

    char configdir[512];
    snprintf(configdir, sizeof(configdir), "%s/.config/ccontacts", homedir);
    mkdir(configdir, 0755);

    char dbpath[1024];
    snprintf(dbpath, sizeof(dbpath), "%s/Contacts.db", configdir);

    sqlite3 *db;
    int rc = sqlite3_open("Contacts.db",&db);
    if(rc!=SQLITE_OK)
    {
        printf("Error opening db\n");
        sqlite3_close(db);
        return NULL;
    }
    const char *sql = "CREATE TABLE IF NOT EXISTS Contacts ("
                      "Id INTEGER PRIMARY KEY NOT NULL, "
                      "Name VARCHAR(128) NOT NULL, "
                      "Number VARCHAR(128), "
                      "Email VARCHAR(128)"
                      ");";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    return db;
}

void menu(sqlite3 *db)
{
    drawlogo();
    char inputoption;
    while(1)
    {
        printf("[V]iew contacts, [A]dd new contact, [D]elete a contact, [Q]uit\n");
        printf(">>> ");
        inputoption = getchar();

        while (getchar() != '\n');
        printf("\n");
        switch(inputoption)
        {
            case 'A':
            case 'a':
                printf("Adding contact...\n");
                drawlogo();
                addcontact(db);
                break;
            case 'D':
            case 'd':
                printf("Deleting contact...\n");
                drawlogo();
                deletecontact(db);
                break;
            case 'Q':
            case 'q':
                printf("Quitting...\n");
                return;
            case 'V':
            case 'v':
                printf("Viewing contacts...\n");
                drawlogo();
                listcontacts(db);
                break;
            /*case 'S':
            case 's':
                printf("WORK IN PROGRESS\n");
                break;*/
        }
    }
}

void addcontact(sqlite3 *db)
{
    printf("Adding New Contact: \n");
    printf("================================================================================\n");

    printf("Name: ");
    char contactname[128];
    fgets(contactname,sizeof(contactname),stdin);
    int loc = strcspn(contactname,"\n");
    contactname[loc] = '\0';

    printf("Number: ");
    char number[128];
    fgets(number,sizeof(number),stdin);
    int loc2 = strcspn(number,"\n");
    number[loc2] = '\0';

    printf("Email: ");
    char email[128];
    fgets(email,sizeof(email),stdin);
    int loc3 = strcspn(email,"\n");
    email[loc3] = '\0';
    printf("\n");

    // PLACEHOLDER

    char sql[256];
    sprintf(sql, "insert into Contacts(Id, Name, Number, Email) values(NULL, '%s', '%s', '%s');", contactname, number, email);
    sqlite3_exec(db, sql, NULL, NULL, NULL);
}

void listcontacts(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Contacts;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("Error while executing SQL: %s\n", sqlite3_errmsg(db));
    }
    printf("%-5s%-25s%-25s%s\n","ID", "NAME", "NUMBER", "EMAIL");
    printf("================================================================================\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        char *name = sqlite3_column_text(stmt, 1);
        char *number = sqlite3_column_text(stmt, 2);
        char *email = sqlite3_column_text(stmt, 3);

        printf("%-5d%-25s%-25s%s\n",id,name,number,email);
        printf("--------------------------------------------------------------------------------\n");
    }
}

void deletecontact(sqlite3 *db)
{
    char sql[256], cid[16];
    printf("Enter Contact ID to delete: ");
    fgets(cid,sizeof(cid),stdin);

    int id = atoi(cid);
    sprintf(sql, "delete from Contacts where id=%d;", id);

    int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if(rc != SQLITE_OK)
    {
        printf("Error while trying to delete contact: %s\n", sqlite3_errmsg(db));
    }
}

void drawlogo(void)
{
    system("clear");
    printf("________/\\\\\\\\\\\\\\\\\\________/\\\\\\\\\\\\\\\\\\___________________________________________________________________________________________________\n");
    printf("_____/\\\\\\////////______/\\\\\\////////____________________________________________________________________________________________________\n");
    printf("___/\\\\\\/_____________/\\\\\\/___________________________________________/\\\\\\_______________________________________/\\\\\\___________________\n");
    printf("__/\\\\\\______________/\\\\\\_________________/\\\\\\\\\\_____/\\\\/\\\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\\\\________/\\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\\\\_\n");
    printf("_\\/\\\\\\_____________\\/\\\\\\_______________/\\\\\\///\\\\\\__\\/\\\\\\////\\\\\\__\\////\\\\\\////__\\////////\\\\\\_____/\\\\\\//////__\\////\\\\\\////__\\/\\\\\\//////__\n");
    printf("_\\//\\\\\\____________\\//\\\\\\_____________/\\\\\\__\\//\\\\\\_\\/\\\\\\__\\//\\\\\\____\\/\\\\\\________/\\\\\\\\\\\\\\\\\\\\___/\\\\\\____________\\/\\\\\\______\\/\\\\\\\\\\\\\\\\\\\\_\n");
    printf("__\\///\\\\\\___________\\///\\\\\\__________\\//\\\\\\__/\\\\\\__\\/\\\\\\___\\/\\\\\\____\\/\\\\\\_/\\\\___/\\\\\\/////\\\\\\__\\//\\\\\\___________\\/\\\\\\_/\\\\__\\////////\\\\\\_\n");
    printf("____\\////\\\\\\\\\\\\\\\\\\____\\////\\\\\\\\\\\\\\\\\\__\\///\\\\\\\\\\/___\\/\\\\\\___\\/\\\\\\____\\//\\\\\\\\\\___\\//\\\\\\\\\\\\\\\\/\\\\__\\///\\\\\\\\\\\\\\\\____\\//\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\_\n");
    printf("_______\\/////////________\\/////////_____\\/////_____\\///____\\///______\\/////_____\\////////\\//_____\\////////______\\/////____\\//////////__\n");
    printf("Version 1.0.0\n\n");
}