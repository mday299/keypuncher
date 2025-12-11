using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using Google.Protobuf.Examples.AddressBook;
using static Google.Protobuf.Examples.AddressBook.Person.Types;

namespace protobuf2;

public partial class App : Application
{
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow();
        }

        Person john = new Person
        {
            Id = 1234,
            Name = "John Doe",
            Email = "jdoe@example.com",
            Phones = { new PhoneNumber { Number = "555-4321", Type = Person.Types.PhoneType.Home } }
        };

        base.OnFrameworkInitializationCompleted();
    }
}