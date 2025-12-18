using Avalonia.Controls;
using Google.Protobuf.WellKnownTypes;
using ProtoModels = Google.Protobuf.Examples.AddressBook;
using System;
using System.Linq;

namespace protobuf2;

public partial class MainWindow : Window
{
    private ProtoModels.AddressBook _addressBook = new ProtoModels.AddressBook();

    public MainWindow()
    {
        InitializeComponent();
    }
}
