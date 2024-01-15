#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>  

using namespace std;
using namespace chrono;
 

 // the class for books
class Book {
private:
    int bookID;
    string bookName;
    int pageCount;
    string authorFirstName;
    string authorLastName;
    string bookType;

public:
    Book(int bookID, string bookName, int pageCount, string authorFirstName, string authorLastName, string bookType)
        : bookID(bookID), bookName(bookName), pageCount(pageCount), authorFirstName(authorFirstName), authorLastName(authorLastName), bookType(bookType) {}


    int getBookID() const {
        return bookID;
    }

    string getBookName() const {
        return bookName;
    }

    int getPageCount() const {
        return pageCount;
    }

    string getAuthorFirstName() const {
        return authorFirstName;
    }

    string getAuthorLastName() const {
        return authorLastName;
    }

    string getBookType() const {
        return bookType;
    }
};

//this is member class

class Member {
private:
    int memberID;
    string firstName;
    string lastName;
    vector<int> borrowedBooks;

public:
    Member(int memberID, string firstName, string lastName)
        : memberID(memberID), firstName(firstName), lastName(lastName) {}

    int getMemberID() const {
        return memberID;
    }

    string getFirstName() const {
        return firstName;
    }

    string getLastName() const {
        return lastName;
    }

    const vector<int>& getBorrowedBooks() const {
        return borrowedBooks;
    }

    void borrowBook(int bookID) {
        borrowedBooks.push_back(bookID);
    }

    void returnBook(int bookID) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), bookID);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
        }
    }
};

// the library class 

class Library {
private:
    vector<Book> books;
    vector<Member> members;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void addMember(const Member& member) {
        members.push_back(member);
        cout << "New member added. Member ID: " << member.getMemberID() << ", Name: " << member.getFirstName() << " " << member.getLastName() << endl;
 
 
    }


 //issue and return of the book 

    void issueBook(int bookID, int memberID) {
        auto dueDate = system_clock::now() + days(3);

        auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book& book) { return book.getBookID() == bookID; });
        auto memberIt = find_if(members.begin(), members.end(), [memberID](const Member& member) { return member.getMemberID() == memberID; });

        if (bookIt != books.end() && memberIt != members.end()) {
            bookIt->getBookID();
            memberIt->borrowBook(bookID);

            cout << "Book with ID " << bookID << " issued to Member with ID " << memberID << ". Due Date: "
                 << put_time(localtime(&dueDate), "%F %T") << endl;
        } else {
            cout << "Error: Invalid book ID or member ID." << endl;
        }
    }

    void returnBook(int bookID, int memberID) {
        auto currentDate = system_clock::now();

        auto memberIt = find_if(members.begin(), members.end(), [memberID](const Member& member) { return member.getMemberID() == memberID; });

        if (memberIt != members.end()) {
            memberIt->returnBook(bookID);

            const auto& borrowedBooks = memberIt->getBorrowedBooks();
            for (int bookID : borrowedBooks) {
                auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book& book) { return book.getBookID() == bookID; });

                if (bookIt != books.end()) {
                    auto dueDate = currentDate + days(3);
                    int overdueDays = duration_cast<days>(currentDate - dueDate).count();

                    if (overdueDays > 0) {
                        double fine = overdueDays * 1.0; // 1 pound per day overdue
                        cout << "Book with ID " << bookID << " returned by Member with ID " << memberID
                             << ". Overdue by " << overdueDays << " days. Fine: £" << fixed << setprecision(2) << fine << endl;
                    } else {
                        cout << "Book with ID " << bookID << " returned by Member with ID " << memberID << ". No overdue." << endl;
                    }
                }
            }
        } else {
            cout << "Error: Invalid member ID." << endl;
        }
    }

//display of the books borrowed

    void displayBorrowedBooks(int memberID) const {
        auto memberIt = find_if(members.begin(), members.end(), [memberID](const Member& member) { return member.getMemberID() == memberID; });

        if (memberIt != members.end()) {
            const auto& borrowedBooks = memberIt->getBorrowedBooks();
            if (!borrowedBooks.empty()) {
                cout << "Books borrowed by Member with ID " << memberID << ": ";
                for (int bookID : borrowedBooks) {
                    cout << bookID << " ";
                }
                cout << endl;
            } else {
                cout << "Member with ID " << memberID << " has not borrowed any books." << endl;
            }
        } else {
            cout << "Error: Invalid member ID." << endl;
        }
    }
 
 //calculating the fine 

    void calculateFine(int memberID) const {
        auto memberIt = find_if(members.begin(), members.end(), [memberID](const Member& member) { return member.getMemberID() == memberID; });

        if (memberIt != members.end()) {
            const auto& borrowedBooks = memberIt->getBorrowedBooks();
            auto currentDate = system_clock::now();

            double totalFine = 0.0;

            for (int bookID : borrowedBooks) {
                auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book& book) { return book.getBookID() == bookID; });

                if (bookIt != books.end()) {
                    auto dueDate = currentDate + days(3);
                    int overdueDays = duration_cast<days>(currentDate - dueDate).count();

                    if (overdueDays > 0) {
                        double fine = overdueDays * 1.0; // 1 pound per day overdue for the books taken
                        totalFine += fine;
                    }
                }
            }

            if (totalFine > 0.0) {
                cout << "Total fine for Member with ID " << memberID << ": £" << fixed << setprecision(2) << totalFine << endl;
            } else {
                cout << "Member with ID " << memberID << " has no overdue fines." << endl;
            }
        } else {
            cout << "Error: Invalid member ID." << endl;
        }
    }
// display the books
    void displayBooks() const {
        for (const Book& book : books) {
            cout << "Book ID: " << book.getBookID() << ", Book Name: " << book.getBookName()
                 << ", Page Count: " << book.getPageCount() << ", Author: " << book.getAuthorFirstName() << " " << book.getAuthorLastName()
                 << ", Type: " << book.getBookType() << endl;
        }
    }
// display members 
    void displayMembers() const {
        for (const Member& member : members) {
            cout << "Member ID: " << member.getMemberID() << ", Name: " << member.getFirstName() << " " << member.getLastName() << endl;
        }
    }
};

int main() {
    Library library;

    ifstream file("library_books.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open  file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;

        vector<string> tokens;
        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 6) {
            int bookID = stoi(tokens[0]);
            string bookName = tokens[1];
            int pageCount = stoi(tokens[2]);
            string authorFirstName = tokens[3];
            string authorLastName = tokens[4];
            string bookType = tokens[5];

            Book book(bookID, bookName, pageCount, authorFirstName, authorLastName, bookType);
            library.addBook(book);
        }
    }

    file.close();

    // Displaying books from the given CSV file
    cout << "Books in the Library:" << endl;
    library.displayBooks();

    // Adding  the new members
    Member member(1, "Anakha", "jk");
    library.addMember(member);

    // Displaying the members
    cout << "\nMembers in the Library:" << endl;
    library.displayMembers();

    // Issuing  the books to the members
    library.issueBook(1, 1);

    // Returning the books from the members
    library.returnBook(1, 1);

    // Displaying all books borrowed by the members
    library.displayBorrowedBooks(1);

    // Calculating the fine for the member
    library.calculateFine(1);

    return 0;
}
