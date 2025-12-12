using System;
using System.Collections.Generic;
using Google.Protobuf;
using Google.Protobuf.Examples.AddressBook;
        
public class SampleAdBookDBase
{
    private AddressBook addBook;
    public SampleAdBookDBase()
    {
        // List of people details
        var peopleDetails = new List<(int Id, string Name, string Email, string PhoneNumber, Person.Types.PhoneType PhoneType)>
        {
            (1234, "John Doe", "jdoe@example.com", "555-4321", Person.Types.PhoneType.Home),
            (5678, "Jane Doe", "janedoe@example.com", "555-1234", Person.Types.PhoneType.Work),
            (9101, "Alice Smith", "alice@example.com", "555-9876", Person.Types.PhoneType.Mobile),
            (1121, "Bob Johnson", "bob@example.com", "555-0000", Person.Types.PhoneType.Unspecified)
        };

        // Create an address book as a member variable
        addBook = new AddressBook();
        // Add people to the address book
        foreach (var details in peopleDetails)
        {
            addBook.People.Add(CreatePerson(details.Id, details.Name, details.Email, details.PhoneNumber, details.PhoneType));
        }
    }

    private Person CreatePerson(int id, string name, string email, string phoneNumber, Person.Types.PhoneType phoneType)
    {
        return new Person
        {
            Id = id,
            Name = name,
            Email = email,
            Phones = { new Person.Types.PhoneNumber { Number = phoneNumber, Type = phoneType } }
        };
    }

    // public void SerializeAddressBook(string filePath)
    // {
    //     try
    //     {
    //         using (var output = System.IO.File.Create(filePath))
    //         {
    //             addBook.WriteTo(output);
    //         }
    //         Console.WriteLine("Address book serialized successfully.");
    //     }
    //     catch (Exception ex)
    //     {
    //         Console.WriteLine($"Failed to write to file: {ex.Message}");
    //     }
    // }

    // public AddressBook DeserializeAddressBook(string filePath)
    // {
    //     try
    //     {
    //         using (var input = System.IO.File.OpenRead(filePath))
    //         {
    //             addBook = AddressBook.Parser.ParseFrom(input);
    //         }
    //         return addBook;
    //     }
    //     catch (Exception ex)
    //     {
    //         Console.WriteLine($"Failed to read from file: {ex.Message}");
    //         return null;
    //     }
    // }

    // public string PrintAddressBook()
    // {
    //     string retStr = "";
    //     Console.WriteLine("Deserialized address book:");
    //     foreach (var person in addBook.People)
    //     {
    //         Console.WriteLine($"ID={person.Id}, Name={person.Name}, Email={person.Email}");
    //         retStr += "test-et";
    //         foreach (var phone in person.Phones)
    //         {
    //             Console.WriteLine($"  Phone Number={phone.Number}, Type={phone.Type}");
    //         }
    //     }
    //     return retStr;
    // }
}