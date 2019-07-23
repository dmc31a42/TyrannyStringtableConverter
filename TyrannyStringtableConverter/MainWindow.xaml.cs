using Microsoft.WindowsAPICodePack.Dialogs;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TyrannyStringtableConverter
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        

        private Converter converter;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new CommonOpenFileDialog
            {
                EnsurePathExists = true,
                EnsureFileExists = false,
                AllowNonFileSystemItems = false,
                IsFolderPicker = true,
                Title = "Tyranny 게임 폴더를 선택하세요. Tyranny.exe 파일이 있는 폴더나 그 폴더의 Data 폴더를 선택하세요."
            };

            if (dialog.ShowDialog() == CommonFileDialogResult.Ok)
            {
                string dirToProcess = Directory.Exists(dialog.FileName) ? dialog.FileName : System.IO.Path.GetDirectoryName(dialog.FileName);
                string tyrannyPath;
                if((tyrannyPath = CheckTyrannyPath(dirToProcess)) == null)
                {
                    tbTyrannyPath.Text = "";
                } else
                {
                    tbTyrannyPath.Text = tyrannyPath;
                    cbISOAlpha2.ItemsSource = Converter.GetAvailableLanguageISOAlpha2(tyrannyPath);
                    if (cbISOAlpha2.Items.IndexOf("jp") != -1)
                    {
                        cbISOAlpha2.SelectedItem = "jp";
                    }
                    else if (cbISOAlpha2.Items.IndexOf("en") != -1)
                    {
                        cbISOAlpha2.SelectedItem = "en";
                    } else
                    {
                        cbISOAlpha2.SelectedIndex = 0;
                    }
                }
            }
        }

        private string CheckTyrannyPath(string path)
        {
            var fileExtention = tbFileExt.Text;
            if (fileExtention.IndexOf("*.") == -1)
            {
                fileExtention = "*." + fileExtention;
            }
            var filesFound = Directory.GetFiles(path, fileExtention, SearchOption.AllDirectories);
            if (filesFound.Length == 0)
            {
                MessageBox.Show("Tyranny 폴더가 아닙니다. 폴더를 다시 선택하거나 확장자 옵션을 확인하세요.");
                return null;
            }
            else
            {
                string samplePath = filesFound[0];
                var foundIndex = samplePath.IndexOf(@"\data\");
                if(foundIndex >= 0)
                {
                    string dirPath = samplePath.Substring(0, foundIndex);
                    return dirPath;
                } else
                {
                    return null;
                }
                
            }
        }

        private void TbTyrannyPath_TextChanged(object sender, TextChangedEventArgs e)
        {
            var input = tbTyrannyPath.Text;
            if(input == "" || input == null)
            {

            } else
            {
                string tyrannyPath;
                if ((tyrannyPath = CheckTyrannyPath(input)) == null)
                {
                    tbTyrannyPath.Text = "";
                }
                else
                {
                    if(input != tyrannyPath)
                    {
                        tbTyrannyPath.Text = tyrannyPath;
                        cbISOAlpha2.ItemsSource = Converter.GetAvailableLanguageISOAlpha2(tyrannyPath);
                        if (cbISOAlpha2.Items.IndexOf("jp") != -1)
                        {
                            cbISOAlpha2.SelectedItem = "jp";
                        }
                        else if (cbISOAlpha2.Items.IndexOf("en") != -1)
                        {
                            cbISOAlpha2.SelectedItem = "en";
                        }
                        else
                        {
                            cbISOAlpha2.SelectedIndex = 0;
                        }
                    }
                }
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            string tyrannyFolderPath = tbTyrannyPath.Text;
            string extension = tbFileExt.Text;
            string ISOAlpha2 = cbISOAlpha2.SelectedItem.ToString();
            if(tyrannyFolderPath != "" && extension != "" && ISOAlpha2 != "")
            {
                if(converter == null)
                {
                    converter = new Converter();
                }
                converter.ReadFolder(tyrannyFolderPath, extension, ISOAlpha2);
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if(converter != null)
            {
                if (tbPoSavePath.Text != "")
                    converter.SavePo(tbPoSavePath.Text);
            } else
            {
                MessageBox.Show("Tyranny 폴더를 먼저 지정한 후 폴더를 불러오십시오.");
            }
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            if(converter != null)
            {
                if(tbPoLoadPath.Text != "")
                {
                    converter.LoadPo(tbPoLoadPath.Text);
                } else
                {
                    MessageBox.Show("Tyranny 폴더를 먼저 지정한 후 폴더를 불러오십시오.");
                }
            }
        }
    }
}
