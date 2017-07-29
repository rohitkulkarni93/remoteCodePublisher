using System.IO;
using System.Windows;
using System.Reflection;
using System;
using System.Threading;
using System.Linq;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace PublisherClient
{
    partial class MainWindow
    {
        private Bridge bridgeInterface = new Bridge();
        Thread tRcv;

        CancellationTokenSource requestTokens = new CancellationTokenSource();

        private void ExecuteCommand(Action action)
        {
            new Task(action, requestTokens.Token).Start();
        }

        private void Help_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            HelpWindow window = new HelpWindow();
            window.ShowDialog();
        }

        private void Configure_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            ConfigureDlg dlg = new ConfigureDlg();
            dlg.ShowDialog();
        }

        private void Exit_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void fileDragDropPanel_Drop(object sender, DragEventArgs e)
        {
            if (sender != null)
            {
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
                if (files.Length > 0)
                {
                    for (int i = 0; i < files.Length; i++)
                        ProcessFile(files[i]);
                }
            }
        }

        /// <summary>
        /// Upload the file to server through MsgClient.
        /// </summary>
        /// <param name="fileName"></param>
        void ProcessFile(string fileName)
        {
            StatusBarText = "Processing file " + fileName;
            string category = CategoyTextBox.Text;
            if (category == "" || (category.IndexOfAny(Path.GetInvalidFileNameChars()) >= 0))
                category = "";
            bridgeInterface.PostRequest(fileName, category, HttpMessageType.HMT_UPLOADFILE.ToString(), "POST");
        }

        private void UpdateUI(string message)
        {
            if (message.Length > 0)
            {
                int index = message.IndexOf(':');
                if (index > 0 && index < message.Length)
                {
                    string responseCommand = message.Substring(0, index);
                    HttpMessageType msgType;
                    if (Enum.TryParse<HttpMessageType>(responseCommand, out msgType))
                    {
                        ProcessResponse(msgType, message.Substring(index));
                    }
                }
            }
        }

        private void ProcessResponse(HttpMessageType command, string data)
        {
            switch (command)
            {
                case HttpMessageType.HMT_BROWSEDIR:
                    UpdateDirInfo(data);
                    break;
            }
        }

        private void UpdateDirInfo(string data)
        {
            if (data.Length > 0)
            {
                List<string> dirs = new List<string>();
                List<string> files = new List<string>();
                int fileSeperator = data.IndexOf('|');
                int index = 0;
                while (index > -1 && index < data.Length)
                {
                    int startIndex = data.IndexOf('<', index), endIndex = -1;
                    if (startIndex > 0)
                        endIndex = data.IndexOf('>', startIndex);
                    if (startIndex < endIndex)
                    {
                        if (endIndex < fileSeperator)
                            dirs.Add(data.Substring(startIndex + 1, endIndex - startIndex - 1));
                        else
                            files.Add(data.Substring(startIndex + 1, endIndex - startIndex - 1));
                    }
                    index = endIndex;
                }
                UpdateListBox(dirs, files);
            }
        }

        private void UpdateListBox(List<string> dirs, List<string> files)
        {
            this.DirectoryList.Items.Clear();
            if(dirs.Count > 1 && dirs[1] == ".")
                dirs.RemoveAt(1);
            for (int i = 0; i < dirs.Count; i++)
                this.DirectoryList.Items.Insert(i, new ListItemInfo() { Item = dirs[i], IsDirectory = true });
            for (int i = 0; i < files.Count; i++)
                this.DirectoryList.Items.Insert(DirectoryList.Items.Count,
                    new ListItemInfo() { Item = files[i], IsDirectory = false });
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            tRcv = new Thread(
              () =>
              {
                  while (true)
                  {
                      String msg = bridgeInterface.GetResponse();
                      Action<String> act = new Action<string>(UpdateUI);
                      Object[] args = { msg };
                      Dispatcher.Invoke(act, args);
                  }
              }
            );
            tRcv.Start();
        }
    }
}