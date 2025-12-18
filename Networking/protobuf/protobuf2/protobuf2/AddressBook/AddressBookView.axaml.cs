using Avalonia.Controls;
namespace protobuf2.AddressBook;
using ProtoModels = Google.Protobuf.Examples.AddressBook;
using Google.Protobuf.WellKnownTypes;
using System;
using System.Linq;

public partial class AddressBookView : UserControl
{
    private ProtoModels.AddressBook _addressBook = new ProtoModels.AddressBook();

    public AddressBookView()
    {
        InitializeComponent();
        AddButton.Click += OnAddPerson;
        LoadAddressBook();
    }

    private void OnAddPerson(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
        var person = new ProtoModels.Person
        {
            Name = NameBox.Text ?? "",
            Id = _addressBook.People.Count + 1,
            Email = EmailBox.Text ?? "",
            LastUpdated = Timestamp.FromDateTime(DateTime.UtcNow)
        };

        var phone = new ProtoModels.Person.Types.PhoneNumber
        {
            Number = PhoneBox.Text ?? "",
            Type = PhoneTypeBox.SelectedIndex switch
            {
                0 => ProtoModels.Person.Types.PhoneType.Mobile,
                1 => ProtoModels.Person.Types.PhoneType.Home,
                2 => ProtoModels.Person.Types.PhoneType.Work,
                _ => ProtoModels.Person.Types.PhoneType.Unspecified
            }
        };

        person.Phones.Add(phone);
        _addressBook.People.Add(person);

        PeopleList.ItemsSource = _addressBook.People
            .Select(p => $"{p.Name} ({p.Email})").ToList();

        SaveAddressBook();
    }

    private void SaveAddressBook() 
    {
        using var output = System.IO.File.Create("addressbook.bin");
        using var coded = new Google.Protobuf.CodedOutputStream(output);
        _addressBook.WriteTo(coded);
        coded.Flush();
    }

    private void LoadAddressBook()
    {
        if (System.IO.File.Exists("addressbook.bin"))
        {
            using var input = System.IO.File.OpenRead("addressbook.bin");
            _addressBook = ProtoModels.AddressBook.Parser.ParseFrom(input);

            PeopleList.ItemsSource = _addressBook.People
                .Select(p => $"{p.Name} ({p.Email})").ToList();
        }
    }
}
