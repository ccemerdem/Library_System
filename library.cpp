#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>

using namespace std;

struct userInfo
{
  string nameSurname, password;
  int ID, totalBook;
};

struct bookInfo
{
  string bookName, authorName, name_book_author, category, status;
  int ID, owner, categoryID;
};

int activeID;
bool is_admin = false;

// Admin
void menu_admin();
void add_book();

// User
void signup();
void menu_user();
void return_books();
void list_category();
void choose_category(int amount);
void list_books(int categoryID, string category);
void choose_book(int categoryID, string category, int amount);
void assing_book(int bookID, string name_book_author);

// Common
int menu_main();
void login();
int book_status(int bookID);
void assing_book(int bookID, string name_book_author);
void unAssing_book(int bookID);
string get_ID(int userID);
bool check_bookID(int bookID);

int main()
{
  menu_main();
  return 0;
}

int menu_main()
{
  is_admin = false;
  system("cls");
  int select_menu, verify = 1;

  cout << " /-----------------------------------\\" << endl;
  cout << "|    Welcome To The Library System    |" << endl;
  cout << " \\-----------------------------------/" << endl;
  cout << "\nSelect the action you want to do;" << endl;
  cout << "\n1) Log In     2) Sign Up     3) Exit" << endl
       << endl;

  do
  {
    select_menu = getch();

    switch (select_menu)
    {
    // It's in quotes because the getch() function returns the ascii value of the input.
    case '1':
      verify = 0;

      system("cls");
      login();
      break;

    case '2':
      verify = 0;

      system("cls");
      signup();

      break;

    case '3':
      verify = 0;
      exit(0);
    }
  } while (verify);
  return 0;
}

void signup()
{
  fstream user;
  fstream admin;
  userInfo u;

  // 'IDdummy' was created to check the existence of the ID
  int IDdummy, verify, step, adminID;

  // It was created to skip the password and name surname in '.txt'
  string dummy;

  // Created to check the existence of the user.txt file
  ifstream if_user("user.txt");

  // The existence of the User.txt file is queried
  if (!if_user.good())
  {
    ofstream create_user("user.txt");
  }

  admin.close();
  user.close();

  user.open("user.txt", ios::in);
  admin.open("admin.txt", ios::in);

  do
  {
    verify = 1;
    step = 0;

    cout << "Enter a 6 digit ID: ";
    cin >> IDdummy;

    for (int temp = IDdummy; temp > 0; step++)
    {
      temp /= 10;
    }

    if (step != 6)
    {
      system("cls");

      cout << "              Warning !" << endl
           << endl
           << "This ID is not valid. Must be 6 digit"
           << endl
           << endl
           << endl;

      verify = 0;
      continue;
    }

    if (verify == 1)
    {
      user.seekg(0);
      admin.seekg(0);

      while (!user.eof() && !admin.eof())
      {
        admin >> adminID;

        user >> u.ID;

        cin.ignore(0);
        getline(user, dummy);

        if (IDdummy == u.ID || IDdummy == adminID)
        {
          system("cls");

          cout << "              Warning !" << endl
               << endl
               << "      This ID is already exists"
               << endl
               << endl
               << endl;

          verify = 0;
          break;
        }
      }
    }
  } while (!verify);

  u.ID = IDdummy;
  user.close();
  admin.close();

  user.open("user.txt", ios::app);
  user.setf(ios::left);

  cout << "Enter your name and surname: ";

  cin.ignore();
  getline(cin, u.nameSurname);

  cout << "Enter your password: ";
  cin >> u.password;

  user << setw(10) << u.ID << setw(30) << u.password << u.nameSurname << endl;

  system("cls");

  cout << "      Successfully signed up!"
       << endl
       << endl
       << "    Returning to the main menu...";

  Sleep(1700);
  system("cls");

  user.close();
  admin.close();

  menu_main();
}

void login()
{
  fstream user;
  fstream admin;
  userInfo u;

  // 'dummy' was created to skip the first and last names found in .txt
  string dummy, password;

  // 'int' and 'long int' can only take ten digits of input so using 'long long int' this limitation is circumvented
  long long ID, verify = 0, verify2 = 1, step;

  ifstream if_user("user.txt");

  // The existence of the 'User.txt' file is queried
  if (!if_user.good())
  {
    ofstream create_user("user.txt");
  }

  admin.close();
  user.close();

  admin.open("admin.txt", ios::in);
  user.open("user.txt", ios::in);

  do
  {
    user.seekg(0);
    admin.seekg(0);

    // Calculates how many digits the entered ID has
    while (verify2)
    {
      cout << "Enter your ID: ";
      cin >> ID;

      step = 0;
      for (long long temp = ID; temp > 0; step++)
      {
        temp /= 10;
      }

      if (step != 6)
      {
        system("cls");

        cout << "              Warning !" << endl
             << endl
             << "This ID is not valid. Must be 6 digit"
             << endl
             << endl
             << endl;

        continue;
      }

      verify2 = 0;
    }

    cout << "Enter your password: ";
    cin.ignore();
    getline(cin, password);

    // Checks whether the ID trying to log in belongs to any admin account

    while (!admin.eof())
    {
      admin >> u.ID >> u.password;

      if (ID == u.ID && password == u.password)
      {

        verify = 1;
        is_admin = true;

        admin.close();
        user.close();

        menu_admin();
        break;
      }
    }

    // Checks whether the ID trying to log in belongs to any user account
    while (!user.eof())
    {
      user >> u.ID >> u.password;

      // It was created to skip the password and name surname in '.txt'
      getline(user, dummy);

      if (ID == u.ID && password == u.password)
      {

        verify = 1;
        activeID = ID;

        user.close();
        admin.close();

        menu_user();

        break;
      }
    }

    if (!verify)
    {
      system("cls");

      cout << "              Warning !" << endl
           << endl
           << "        Wrong ID or password";

      Sleep(1700);

      menu_main();
    }
  } while (!verify);
}

void menu_admin()
{
  fstream book_list;
  int select_menu, verify = 1;
  system("cls");

  cout << "Select the action you want to do;" << endl;
  cout << "\n1) Add book   2) Remove a book   3) Log out" << endl
       << endl;

  do
  {
    select_menu = getch();

    switch (select_menu)
    {
    // It's in quotes because the getch() function returns the ascii value of the input.
    case '1':
      verify = 0;

      system("cls");
      add_book();
      break;

    case '2':
      verify = 0;

      system("cls");
      list_category();
      break;

    case '3':
      verify = 0;
      activeID = 0;

      system("cls");
      menu_main();
    }
  } while (verify);
}

void add_book()
{
  fstream book_list;
  bookInfo b;
  int amount, number = 1;

  book_list.close();
  book_list.open("book_list.txt", ios::out | ios::app);

  cout << "How many books do you want to add ? : ";
  cin >> amount;

  system("cls");

  while (amount != 0)
  {
    cout << "          Book " << number << endl;
    cout << "--------------------------" << endl;

    cout << "Book ID (Max. lenght is 6): ";
    cin >> b.ID;

    if (check_bookID(b.ID))
    {
      system("cls");

      cout << "already exists";

      book_list.close();
      book_list.open("book_list.txt", ios::out | ios::app);

      Sleep(1700);
      system("cls");

      continue;
    }

    cout << "Category ID : ";
    cin >> b.categoryID;

    cout << "Book name: ";
    cin.ignore();
    getline(cin, b.bookName);

    cout << "Author name: ";
    getline(cin, b.authorName);

    book_list << endl
              << left << setw(8) << b.categoryID
              << right << setw(7) << b.ID
              << " '" << b.bookName << "' by " << b.authorName;
    cout << endl;

    amount--;
    number++;
  }

  book_list.close();
  menu_admin();
}

bool check_bookID(int bookID)
{
  fstream book_list;
  bookInfo b;

  book_list.close();
  book_list.open("book_list.txt", ios::in);

  while (!book_list.eof())
  {
    book_list >> b.categoryID >> b.ID;
    getline(book_list, b.name_book_author);

    if (bookID == b.ID)
    {
      book_list.close();
      return true;
    }
  }

  book_list.close();
  return false;
}

void remove_book(int bookID)
{
  fstream book_list;
  fstream temp_book_list;
  bookInfo b;
  userInfo u;
  string dummy;

  book_list.close();
  book_list.open("book_list.txt", ios::in);

  temp_book_list.open("temp_book_list.txt", ios::out);

  while (!book_list.eof())

  {
    book_list >> b.categoryID >> b.ID;
    getline(book_list, b.name_book_author);

    if (bookID == b.ID)
    {
      continue;
    }

    temp_book_list << endl
                   << left << setw(7) << b.categoryID << right << setw(7) << b.ID << b.name_book_author;
  }

  book_list.close();
  temp_book_list.close();

  remove("book_list.txt");
  rename("temp_book_list.txt", "book_list.txt");

  unAssing_book(bookID);
  menu_admin();
}

void menu_user()
{
  fstream assing_list;

  int select_menu, verify = 1;
  system("cls");
  cout << "Hi " << get_ID(activeID) << " !" << endl;

  cout << "\nSelect the action you want to do;" << endl;
  cout << "\n1) Borrow books   2) Return books    3) Log out" << endl
       << endl;
  cout << "     (Max. 3)" << endl;

  do
  {
    select_menu = getch();

    switch (select_menu)
    {
    case '1':
      verify = 0;

      list_category();
      break;

    case '2':
      verify = 0;

      return_books();
      break;
    case '3':
      verify = 0;
      activeID = 0;

      system("cls");
      menu_main();
    }
  } while (verify);
}

void list_category()
{
  fstream book_category;
  bookInfo b;
  int number, amount = 0;

  system("cls");

  book_category.close();
  book_category.open("book_category.txt", ios::in);

  cout << " Categories  " << endl
       << " --------------------" << endl
       << endl;

  while (!book_category.eof())
  {
    amount++;
    book_category >> number;
    cout << number << ")";

    getline(book_category, b.category);
    cout << b.category << endl
         << endl;
  }

  cout << endl
       << amount + 1 << ") Back" << endl;
  book_category.close();

  choose_category(amount);
}

void choose_category(int amount)
{
  fstream book_category;
  bookInfo b;
  string dummy;
  int number;

  cout << endl;
  cin >> b.categoryID;

  // '+1' was added to create the back option and prevent possible larger digit entries
  if (b.categoryID > amount + 1)
  {
    list_category();
  }

  if (b.categoryID == amount + 1)
  {
    if (is_admin)
    {
      menu_admin();
    }

    menu_user();
  }

  book_category.close();
  book_category.open("book_category.txt", ios::in);

  while (!book_category.eof())
  {
    book_category >> number;

    if (b.categoryID == number)
    {
      getline(book_category, b.category);
      book_category.close();

      list_books(b.categoryID, b.category);
    }

    getline(book_category, dummy);
  }
}

void list_books(int categoryID, string category)
{
  fstream book_list;
  bookInfo b;
  string dummy;
  int status, amount = 0;

  book_list.close();
  book_list.open("book_list.txt", ios::in);

  system("cls");
  cout << category << endl
       << " --------------------" << endl;

  while (!book_list.eof())
  {
    book_list >> b.categoryID >> b.ID;
    getline(book_list, b.name_book_author);

    if (categoryID == b.categoryID)
    {
      amount++;
      status = book_status(b.ID);
      cout << endl
           << amount << ")" << left << setw(70) << b.name_book_author;

      if (status == 1)
      {
        cout << "Available" << endl;
      }
      else
      {
        cout << "Not available" << endl;
      }
    }
  }

  book_list.close();
  cout << endl
       << endl
       << amount + 1 << ")"
       << "  Back" << endl
       << endl;
  choose_book(categoryID, category, amount);
}

void choose_book(int categoryID, string category, int amount)
{
  fstream book_list;
  bookInfo b;
  string dummy;
  int number, step = 0, value = 1;

  cin >> number;

  // 'back' function
  if (number == amount + 1)
  {
    list_category();
  }

  book_list.close();
  book_list.open("book_list.txt", ios::in);

  while (!book_list.eof())
  {
    book_list >> b.categoryID >> b.ID;
    getline(book_list, b.name_book_author);

    if (b.categoryID == categoryID)
    {
      step++;
      if (step == number)
      {
        book_list.close();
        value = 0;

        // the 'remove book' function for admins
        if (is_admin)
        {
          remove_book(b.ID);
          break;
        }

        assing_book(b.ID, b.name_book_author);
        break;
      }
    }
  }

  if (value == 1)
  {
    book_list.close();
    list_books(categoryID, category);
  }
}

void assing_book(int bookID, string name_book_author)
{
  fstream assing_list;
  bookInfo b;
  userInfo u;
  string dummy;
  int amount = 0, value = 1;

  assing_list.close();
  assing_list.open("assing_list.txt", ios::in);

  while (!assing_list.eof())
  {
    assing_list >> u.ID >> dummy;
    getline(assing_list, dummy);

    // Created to limit the number of books that a user can borrow
    if (u.ID == activeID)
    {
      amount++;

      if (amount == 3)
      {
        system("cls");

        cout << "              Warning !" << endl
             << endl
             << "       You have already 3 books";

        Sleep(1700);

        assing_list.close();
        value = 0;

        menu_user();
        break;
      }
    }

    //  Checks to whether the chosen book has already been taken or not
    if (!book_status(bookID))
    {
      system("cls");

      cout << "              Warning !" << endl
           << endl
           << "       The book is already taken";

      Sleep(1700);

      assing_list.close();
      value = 0;

      menu_user();
      break;
    }
  }

  if (value)
  {
    assing_list.close();
    assing_list.open("assing_list.txt", ios::app);

    assing_list << endl
                << left << setw(8) << activeID << right << setw(6) << bookID << left << name_book_author;

    assing_list.close();

    menu_user();
  }
}

void unAssing_book(int bookID)
{
  fstream assing_list;
  fstream temp_assing_list;
  bookInfo b;
  userInfo u;

  assing_list.close();
  assing_list.open("assing_list.txt", ios::in);

  temp_assing_list.open("temp_assing_list.txt", ios::out);

  while (!assing_list.eof())
  {
    assing_list >> u.ID >> b.ID;
    getline(assing_list, b.name_book_author);

    if (bookID == b.ID)
    {
      continue;
    }

    temp_assing_list << endl
                     << left << setw(8) << u.ID << right << setw(6) << b.ID << left << b.name_book_author;
  }
  assing_list.close();
  temp_assing_list.close();

  remove("assing_list.txt");
  rename("temp_assing_list.txt", "assing_list.txt");
}

// Created to determine and print whether is book available (not borrowed) or not
int book_status(int bookID)
{
  fstream assing_list;
  bookInfo b;
  string dummy;
  int value = 0;

  assing_list.close();
  assing_list.open("assing_list.txt", ios::in);

  while (!assing_list.eof())
  {
    assing_list >> dummy >> b.ID;
    getline(assing_list, dummy);

    if (b.ID == bookID)
    {
      value = 1;
      assing_list.close();

      return 0;
      break;
    }
  }
  if (value == 0)
  {
    assing_list.close();
    return 1;
  }
  return -1;
}

void return_books()
{
  fstream assing_list;
  bookInfo b;
  userInfo u;
  string dummy;
  int value = 0, amount = 0, number, step = 0;

  system("cls");

  assing_list.close();
  assing_list.open("assing_list.txt", ios::in);

  // Sorts and prints the books that user owns. (Inventory System)
  while (!assing_list.eof())
  {
    assing_list >> u.ID >> dummy;
    getline(assing_list, b.name_book_author);

    if (u.ID == activeID)
    {
      value = 1;
      amount++;

      cout << amount << ")" << b.name_book_author << endl;
    }
  }

  if (value == 0)
  {
    cout << "          You don't have any book "
         << endl
         << endl
         << "       Returning to the main menu... ";

    Sleep(1700);
    assing_list.close();
    menu_user();
  }

  cout << endl
       << amount + 1 << ")"
       << " Back" << endl
       << endl;

  assing_list.seekg(0);

  if (value == 1)
  {
    // Getting input from user
    cin >> number;

    // Repeats the process if the input exceeds the number of books listed.
    if (number > amount + 1)
    {
      assing_list.close();
      return_books();
    }

    // 'back' function
    if (number == amount + 1)
    {
      assing_list.close();
      menu_user();
    }

    while (!assing_list.eof())
    {
      assing_list >> u.ID >> b.ID;
      getline(assing_list, dummy);

      if (u.ID == activeID)
      {
        step++;
      }

      if (step == number)
      {
        assing_list.close();

        cout << b.ID;

        unAssing_book(b.ID);
        menu_user();
      }
    }
    assing_list.close();
  }
}

// Created to get user's ID who is logged in
string get_ID(int userID)
{
  fstream user;
  string dummy, name;
  userInfo u;

  user.close();
  user.open("user.txt", ios::in);

  while (!user.eof())
  {
    user >> u.ID;

    if (u.ID == userID)
    {
      user >> dummy >> name;

      user.close();

      return name;
    }

    getline(user, dummy);
  }
  return 0;
}

// Created to check whether entered book ID already exists or not
bool check_bookID(int bookID)
{
  fstream book_list;
  bookInfo b;

  book_list.close();
  book_list.open("book_list.txt", ios::in);

  while (!book_list.eof())
  {
    book_list >> b.categoryID >> b.ID;
    getline(book_list, b.name_book_author);

    if (bookID == b.ID)
    {
      book_list.close();
      return true;
    }
  }

  book_list.close();
  return false;
}