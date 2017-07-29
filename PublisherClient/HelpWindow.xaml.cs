using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace PublisherClient
{
    /// <summary>
    /// Interaction logic for HelpWindow.xaml
    /// </summary>
    public partial class HelpWindow : Window
    {
        public HelpWindow()
        {
            InitializeComponent();
            this.HelpTextBox.Text
                = "Help\n"
                + "1) Use Configure to change Server IP and Port.\n"
                + "2) Use Browse tab to browse server Directory.\n"
                + "3) To upload files, they can be dragged dropped.\n"
                + "4) To upload directory, copy paste path.\n"
                + "5) Use publish button to publish all files.\n"
                + "6) Click download to download files and folders.\n"
                + "7) Click download by selecting.";
        }
    }
}
