using System;
using System.Windows;
using System.Windows.Controls;
using System.Threading;
using System.Threading.Tasks;
using System.IO;

namespace PublisherClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        class ListItemInfo
        {
            public string Item { get; set; }
            public bool IsDirectory { get; set; }
            public override string ToString()
            {
                return Item;
            }
        }

        private string StatusBarText
        {
            get { return this.statusLbl.Text; }
            set { this.statusLbl.Text = "Status: " + value; }
        }

        public MainWindow()
        {
            InitializeComponent();
            SubscribeToEvents();
            InitializeState();
        }

        private void InitializeState()
        {
            this.bridgeInterface.Configure("localhost", 8060);
        }

        internal void SubscribeToEvents()
        {
            this.tabControl1.SelectionChanged += TabControl1_SelectionChanged;
            this.Loaded += Window_Loaded;
            this.DirectoryList.MouseDoubleClick += DirectoryList_MouseDoubleClick;
        }

        private void DirectoryList_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.Source is ListBox && (ListBox)e.Source == DirectoryList && DirectoryList.SelectedIndex > -1)
            {
                ListItemInfo selectedItem = DirectoryList.Items.GetItemAt(DirectoryList.SelectedIndex) as ListItemInfo;
                if (selectedItem.IsDirectory)
                {
                    string baseDirectory = (DirectoryList.Items[0] as ListItemInfo).Item;
                    string browseDirectory = selectedItem.Item;
                    if (selectedItem.Item == ".." || DirectoryList.SelectedIndex == 0)
                        browseDirectory = baseDirectory + "\\" + "../";
                    else
                        browseDirectory = baseDirectory + "\\" + browseDirectory;
                    new Task(() => { this.bridgeInterface.PostRequest(browseDirectory, "", HttpMessageType.HMT_BROWSEDIR.ToString(), "GET"); }, requestTokens.Token).Start();
                }
            }
        }

        private void TabControl1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.Source is TabControl && ((TabControl)e.Source).SelectedIndex == 1 && DirectoryList.Items.Count == 0)
                bridgeInterface.PostRequest("$", "", "HMT_BROWSEDIR", "GET");
        }

        private void Publish_Button_Click(object sender, RoutedEventArgs e)
        {
            if (DirectoryList.SelectedIndex > -1)
            {
                ListItemInfo directoryToPublish = (ListItemInfo)DirectoryList.Items[DirectoryList.SelectedIndex];
                if (directoryToPublish.IsDirectory && DirectoryList.SelectedIndex > 1)
                {
                    string baseDirectory = (DirectoryList.Items[0] as ListItemInfo).Item;
                    string publishDirectory = baseDirectory + "\\" + directoryToPublish.Item;
                    bridgeInterface.PostRequest(publishDirectory, "", HttpMessageType.HMT_PUBLISH.ToString(), "POST");
                    bridgeInterface.PostRequest(baseDirectory, "", "HMT_BROWSEDIR", "GET");
                }
            }
        }

        private void Upload_Button_Click(object sender, RoutedEventArgs e)
        {
            string text = UploadTextbox.Text;
            string category = CategoyTextBox.Text;
            if(text.Length > 0 && Directory.Exists(text))
            {
                if(category != "" && !(category.IndexOfAny(Path.GetInvalidFileNameChars()) >= 0))
                    bridgeInterface.PostRequest(text, category, HttpMessageType.HMT_UPLOADDIR.ToString(), "POST");
            }
        }

        private void Delete_Button_Click(object sender, RoutedEventArgs e)
        {
            if (DirectoryList.SelectedIndex > -1)
            {
                ListItemInfo itemToDelete = (ListItemInfo)DirectoryList.Items[DirectoryList.SelectedIndex];
                if (DirectoryList.SelectedIndex > 1)
                {
                    string baseDirectory = (DirectoryList.Items[0] as ListItemInfo).Item;
                    string deleteDirectory = baseDirectory + "\\" + itemToDelete.Item;
                    bridgeInterface.PostRequest(deleteDirectory, "",
                        (itemToDelete.IsDirectory ? HttpMessageType.HMT_DELETEFOLDER.ToString()
                        : HttpMessageType.HMT_DELETEFILE.ToString()), "POST");
                    bridgeInterface.PostRequest(baseDirectory, "", "HMT_BROWSEDIR", "GET");
                }
                else
                    StatusBarText = "Cant delete parent folder. Please navigate back.";
            }
        }

        private void Download_Button_Click(object sender, RoutedEventArgs e)
        {
            if (DirectoryList.SelectedIndex > -1)
            {
                ListItemInfo itemToDownload = (ListItemInfo)DirectoryList.Items[DirectoryList.SelectedIndex];
                if (DirectoryList.SelectedIndex > 1 && !itemToDownload.IsDirectory)
                {
                    string baseDirectory = (DirectoryList.Items[0] as ListItemInfo).Item;
                    string downloadDirectory = baseDirectory + "\\" + itemToDownload.Item;
                    bridgeInterface.PostRequest(downloadDirectory, "", HttpMessageType.HMT_DOWNLOADFILE.ToString(), "GET");
                }
                else
                    StatusBarText = "Cant download directories. Please select file.";
            }
        }
    }
}
