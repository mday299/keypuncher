#include <iostream>
#include <fstream>
#include <string>
#include "library.pb.h"

int main() {
    // Verify the protobuf library version
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::cout << "=== Library Management System Demo ===" << std::endl << std::endl;

    // Create an Author
    library::Author author;
    author.set_id(1);
    author.set_name("Bjarne Stroustrup");
    author.set_email("bjarne@example.com");

    std::cout << "Created Author:" << std::endl;
    std::cout << "  ID: " << author.id() << std::endl;
    std::cout << "  Name: " << author.name() << std::endl;
    std::cout << "  Email: " << author.email() << std::endl << std::endl;

    // Create a Book
    library::Book book;
    book.set_id(101);
    book.set_title("The C++ Programming Language");
    book.set_author_id(author.id());
    book.set_publication_year(2013);
    book.set_isbn("978-0321563842");
    book.add_genres("Programming");
    book.add_genres("Computer Science");
    book.add_genres("Reference");
    book.set_checked_out(false);

    std::cout << "Created Book:" << std::endl;
    std::cout << "  ID: " << book.id() << std::endl;
    std::cout << "  Title: " << book.title() << std::endl;
    std::cout << "  Author ID: " << book.author_id() << std::endl;
    std::cout << "  Year: " << book.publication_year() << std::endl;
    std::cout << "  ISBN: " << book.isbn() << std::endl;
    std::cout << "  Genres: ";
    for (int i = 0; i < book.genres_size(); i++) {
        std::cout << book.genres(i);
        if (i < book.genres_size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "  Checked out: " << (book.checked_out() ? "Yes" : "No") << std::endl << std::endl;

    // Create a Borrower
    library::Borrower borrower;
    borrower.set_id(501);
    borrower.set_name("John Doe");
    borrower.set_email("john.doe@example.com");

    std::cout << "Created Borrower:" << std::endl;
    std::cout << "  ID: " << borrower.id() << std::endl;
    std::cout << "  Name: " << borrower.name() << std::endl;
    std::cout << "  Email: " << borrower.email() << std::endl << std::endl;

    // Create a Lending Record
    library::LendingRecord lendingRecord;
    lendingRecord.set_id(1001);
    lendingRecord.set_book_id(book.id());
    lendingRecord.set_borrower_id(borrower.id());
    lendingRecord.set_checkout_date("2026-03-05");
    lendingRecord.set_due_date("2026-03-19");
    lendingRecord.set_return_date("");  // Not returned yet

    std::cout << "Created Lending Record:" << std::endl;
    std::cout << "  ID: " << lendingRecord.id() << std::endl;
    std::cout << "  Book ID: " << lendingRecord.book_id() << std::endl;
    std::cout << "  Borrower ID: " << lendingRecord.borrower_id() << std::endl;
    std::cout << "  Checkout Date: " << lendingRecord.checkout_date() << std::endl;
    std::cout << "  Due Date: " << lendingRecord.due_date() << std::endl;
    std::cout << "  Return Date: " << (lendingRecord.return_date().empty() ? "(Not returned)" : lendingRecord.return_date()) << std::endl << std::endl;

    // Test AddBookRequest
    library::AddBookRequest addRequest;
    addRequest.mutable_book()->CopyFrom(book);

    std::cout << "=== Serialization Test ===" << std::endl;

    // Serialize AddBookRequest to file
    std::fstream output("add_book_request.bin", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!addRequest.SerializeToOstream(&output)) {
        std::cerr << "Failed to write AddBookRequest." << std::endl;
        return 1;
    }
    output.close();
    std::cout << "AddBookRequest serialized to add_book_request.bin" << std::endl;

    // Deserialize from file
    library::AddBookRequest loadedRequest;
    std::fstream input("add_book_request.bin", std::ios::in | std::ios::binary);
    if (!loadedRequest.ParseFromIstream(&input)) {
        std::cerr << "Failed to read AddBookRequest." << std::endl;
        return 1;
    }
    input.close();

    std::cout << "AddBookRequest deserialized successfully!" << std::endl;
    std::cout << "Loaded Book Title: " << loadedRequest.book().title() << std::endl << std::endl;

    // Test LookupBookRequest with oneof
    library::LookupBookRequest lookupByISBN;
    lookupByISBN.set_isbn("978-0321563842");

    std::cout << "=== Lookup Request Test ===" << std::endl;
    std::cout << "Query type: ";
    switch (lookupByISBN.query_case()) {
        case library::LookupBookRequest::kId:
            std::cout << "ID = " << lookupByISBN.id() << std::endl;
            break;
        case library::LookupBookRequest::kIsbn:
            std::cout << "ISBN = " << lookupByISBN.isbn() << std::endl;
            break;
        case library::LookupBookRequest::kTitle:
            std::cout << "Title = " << lookupByISBN.title() << std::endl;
            break;
        case library::LookupBookRequest::QUERY_NOT_SET:
            std::cout << "Not set" << std::endl;
            break;
    }

    // Clean up protobuf library
    google::protobuf::ShutdownProtobufLibrary();

    std::cout << std::endl << "=== Demo completed successfully! ===" << std::endl;

    return 0;
}
