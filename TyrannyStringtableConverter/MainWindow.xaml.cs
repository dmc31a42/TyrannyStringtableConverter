using Microsoft.Win32;
using Microsoft.WindowsAPICodePack.Dialogs;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
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
            converter = new Converter();
        }

        private void StartWorkUI(string content)
        {
            gProgress.Visibility = Visibility.Visible;
            tbProgress.Text = content;
        }

        private void OtherWorkUI(string content)
        {
            tbProgress.Text = content;
        }

        private void StopWorkUI()
        {
            gProgress.Visibility = Visibility.Hidden;
            tbProgress.Text = "작업이 진행중입니다.";
        }

        private bool IsMyPoEmpty()
        {
            if(converter.myPo.Entries.Count == 0)
            {
                return true;
            } else
            {
                return false;
            }
        }

        private void RefreshDataGridMyPo()
        {
            Style wrapStyle = new Style(typeof(TextBlock));
            wrapStyle.Setters.Add(new Setter(TextBlock.TextWrappingProperty, TextWrapping.Wrap));
            //if (dgMyPo.Columns.FirstOrDefault((x) => (string)x.Header == "Key") == null)
            //{
            //    dgMyPo.Columns.Add(new DataGridTextColumn
            //    {
            //        Header = "Key",
            //        Binding = new Binding() { Path = new PropertyPath("Key") },
            //        Width = System.Windows.Controls.DataGridLength.Auto,
            //        ElementStyle = wrapStyle
            //    });
            //}
            foreach (string ISOAlpha2 in converter.myPo.AvailableISOAlpha2)
            {
                if (dgMyPo.Columns.FirstOrDefault(x => (string)x.Header == ISOAlpha2) == null)
                {
                    dgMyPo.Columns.Add(new DataGridTextColumn
                    {
                        Header = ISOAlpha2,
                        Binding = new Binding()
                        {
                            Path = new PropertyPath(ISOAlpha2),
                            Mode = BindingMode.TwoWay
                        },
                        Width = new DataGridLength(1, DataGridLengthUnitType.Star),
                        ElementStyle = wrapStyle,
                        IsReadOnly = true
                    });
                }
            }
            dgMyPo.ItemsSource = converter.myPo.Entries.Values;
            dgMyPo.UpdateLayout();
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
                if ((tyrannyPath = CheckTyrannyPath(dirToProcess)) == null)
                {
                    tbTyrannyPath.Text = "";
                } else
                {
                    tbTyrannyPath.Text = tyrannyPath;
                    cbISOAlpha2.ItemsSource = Converter.GetAvailableLanguageISOAlpha2InDataFolder(tyrannyPath);
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
                if (foundIndex >= 0)
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
            if (input == "" || input == null)
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
                    if (input != tyrannyPath)
                    {
                        tbTyrannyPath.Text = tyrannyPath;
                    }
                    cbISOAlpha2.ItemsSource = Converter.GetAvailableLanguageISOAlpha2InDataFolder(tyrannyPath);
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

        private void UpdateAvailableLanguageISOAlpha2()
        {
            cbTranslatedISOAlpha2.ItemsSource = converter.AvailableISOAlpha2;
            cbPOSaveOriginalISOAlpha2.ItemsSource = converter.AvailableISOAlpha2;
            cbPOSaveTranslatedISOAlpha2.ItemsSource = converter.AvailableISOAlpha2;
            cbPOLoadTranslatedISOAlpha2.ItemsSource = converter.AvailableISOAlpha2;
            cbAnamoneOriginalISOAlpha2.ItemsSource = converter.AvailableISOAlpha2;
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            string tyrannyFolderPath = tbTyrannyPath.Text;
            string extension = tbFileExt.Text;
            string ISOAlpha2 = cbISOAlpha2.SelectedItem.ToString();
            if (tyrannyFolderPath != "" && extension != "" && ISOAlpha2 != "")
            {
                StartWorkUI("폴더에서 문장을 읽어오고 있습니다.");
                Thread thread = new Thread(new ThreadStart(
                    delegate ()
                    {
                        converter.ReadFolder(tyrannyFolderPath, extension, ISOAlpha2);
                        this.Dispatcher.Invoke(new Action(delegate ()
                        {
                            UpdateAvailableLanguageISOAlpha2();
                            OtherWorkUI("문장 목록을 갱신하고 있습니다.");
                            RefreshDataGridMyPo();
                            StopWorkUI();
                        }));
                    }));
                thread.Start();
            }
        }

        private void Button_Click_8(object sender, RoutedEventArgs e)
        {
            string tyrannyFolderPath = tbTyrannyPath.Text;
            string extension = tbFileExt.Text;
            if (tyrannyFolderPath != "" && extension != "")
            {
                StartWorkUI("폴더에서 문장을 읽어오고 있습니다.");
                Thread thread = new Thread(new ThreadStart(
                    delegate ()
                    {
                        foreach (string ISOAlpha2 in cbISOAlpha2.ItemsSource)
                        {
                            converter.ReadFolder(tyrannyFolderPath, extension, ISOAlpha2);
                        }
                        this.Dispatcher.Invoke(new Action(delegate ()
                        {
                            UpdateAvailableLanguageISOAlpha2();
                            OtherWorkUI("문장 목록을 갱신하고 있습니다.");
                            RefreshDataGridMyPo();
                            StopWorkUI();
                        }));
                    }));
                thread.Start();
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if(IsMyPoEmpty())
            {
                MessageBox.Show("Tyranny 폴더나 Po 파일에서 문장을 불러오십시오");
                return;
            }
            if (string.IsNullOrEmpty(tbPoSavePath.Text))
            {
                MessageBox.Show("저장할 Po 파일의 이름을 지정하여 주십시오.");
                return;
            }
            if (cbPOSaveOriginalISOAlpha2.SelectedItem == null)
            {
                MessageBox.Show("Po 파일에 저장할 원문의 언어를 선택하십시오.");
                return;
            }
            if (cbPOSaveTranslatedISOAlpha2.SelectedItem == null
                || cbPOSaveTranslatedISOAlpha2.SelectedItem.ToString().Length != 2)
            {
                MessageBox.Show("Po 파일에 저장할 번역문의 언어를 선택하거나 2자리로 정확히 입력하여주십시오.");
                return;
            }
            StartWorkUI("선택한 원문과 번역문을 Po 파일에 저장하고 있습니다. (주의: 오래 걸리는 작업)");
            string poSavePath = tbPoSavePath.Text;
            string poSaveOriginalISOAlpha2 = cbPOSaveOriginalISOAlpha2.SelectedItem.ToString();
            string poSaveTranslatedISOAlpha2 = cbPOSaveTranslatedISOAlpha2.SelectedItem.ToString();
            Thread thread = new Thread(new ThreadStart(
                delegate ()
                {
                    converter.SavePo(poSavePath, poSaveOriginalISOAlpha2, poSaveTranslatedISOAlpha2);
                    this.Dispatcher.Invoke(new Action(delegate ()
                    {
                        StopWorkUI();
                    }));
                }));
            thread.Start();
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(tbPoLoadPath.Text))
            {
                MessageBox.Show("불러올 Po 파일의 이름을 지정하여 주십시오.");
                return;
            }
            if(File.Exists(tbPoLoadPath.Text) == false)
            {
                MessageBox.Show("선택한 파일이 실제로 존재하지 않습니다.");
                return;
            }
            if (cbPOLoadTranslatedISOAlpha2.SelectedItem == null
                || cbPOLoadTranslatedISOAlpha2.SelectedItem.ToString().Length != 2)
            {
                MessageBox.Show("Po 파일에서 불러올 번역문의 언어를 선택하거나 2자리로 정확히 입력하여주십시오.");
                return;
            }
            StartWorkUI("Po 파일을 불러오고 있습니다.");
            string poLoadPath = tbPoLoadPath.Text;
            string poLoadTranslatedISOAlpha2 = cbPOLoadTranslatedISOAlpha2.SelectedItem.ToString();
            Thread thread = new Thread(new ThreadStart(
                delegate ()
                {
                    converter.LoadPo(poLoadPath, poLoadTranslatedISOAlpha2);
                    this.Dispatcher.Invoke(new Action(delegate ()
                    {
                        UpdateAvailableLanguageISOAlpha2();
                        OtherWorkUI("문장 목록을 갱신하고 있습니다.");
                        RefreshDataGridMyPo();
                        StopWorkUI();
                    }));
                }));
            thread.Start();
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            if (IsMyPoEmpty())
            {
                MessageBox.Show("Tyranny 폴더나 Po 파일에서 문장을 불러오십시오");
                return;
            }
            if (cbTranslatedISOAlpha2.SelectedItem.ToString() == "" || cbTranslatedISOAlpha2.SelectedItem.ToString().Length != 2)
            {
                MessageBox.Show("번역하는 언어의 ISO Alpha-2 코드를 2자리로 입력하여주십시오.");
                return;
            }
            if (tbTargetDirPath.Text == "")
            {
                MessageBox.Show("번역물이 저장될 폴더를 먼저 선택하세요.");
                return;
            }
            StartWorkUI("게임 폴더에 적용할 수 있는 번역된 결과물을 만들고 있습니다.");
            string targetDirPath = tbTargetDirPath.Text;
            string translatedISOAlpha2 = cbTranslatedISOAlpha2.SelectedItem.ToString();
            Thread thread = new Thread(new ThreadStart(
                delegate ()
                {
                    converter.SaveFolder(targetDirPath, translatedISOAlpha2);
                    this.Dispatcher.Invoke(new Action(delegate ()
                    {
                        StopWorkUI();
                    }));
                }));
            thread.Start();
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            var dialog = new CommonOpenFileDialog
            {
                EnsurePathExists = true,
                EnsureFileExists = false,
                AllowNonFileSystemItems = false,
                IsFolderPicker = true,
                Title = "번역물이 저장될 폴더를 선택하세요. 원문이 저장된 폴더가 아닌 폴더를 추천합니다."
            };

            if (dialog.ShowDialog() == CommonFileDialogResult.Ok)
            {
                string dirToProcess = Directory.Exists(dialog.FileName) ? dialog.FileName : System.IO.Path.GetDirectoryName(dialog.FileName);
                tbTargetDirPath.Text = dirToProcess;
            }
        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {
            converter.myPo.AddISOAlpha2("en");
            converter.myPo.AddISOAlpha2("jp");
            converter.myPo.AddISOAlpha2("kr");
            RefreshDataGridMyPo();
        }

        private void Button_Click_9(object sender, RoutedEventArgs e)
        {
            if (IsMyPoEmpty())
            {
                MessageBox.Show("Tyranny 폴더나 Po 파일에서 문장을 불러오십시오");
                return;
            }
            if (cbAnamoneOriginalISOAlpha2.SelectedItem == null)
            {
                MessageBox.Show("아나모네로 번역할 원문을 선택하십시오.");
                return;
            }
            StartWorkUI("선택한 원문(" + cbAnamoneOriginalISOAlpha2.SelectedItem.ToString() + ")을 한국어(kr)로 번역하고 있습니다.(매우 오래걸리는 작업)");
            var anamoneOriginalISOAlpha2 = cbAnamoneOriginalISOAlpha2.SelectedItem.ToString();
            Thread thread = new Thread(new ThreadStart(
                delegate ()
                {
                    converter.TranslateToKR(anamoneOriginalISOAlpha2);
                    this.Dispatcher.Invoke(new Action(delegate ()
                    {
                        UpdateAvailableLanguageISOAlpha2();
                        OtherWorkUI("문장 목록을 갱신하고 있습니다.");
                        RefreshDataGridMyPo();
                        StopWorkUI();
                    }));
                }));
            thread.Start();
        }

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {
            string defaultFileName = tbPoSavePath.Text;
            if(string.IsNullOrEmpty(defaultFileName))
            {
                defaultFileName = "strings.po";
            }
            SaveFileDialog saveFileDialog = new SaveFileDialog
            {
                Title = "Po 파일을 저장할 위치를 지정하십시오.",
                FileName = defaultFileName,
            };
            if(saveFileDialog.ShowDialog() == true)
            {
                tbPoSavePath.Text = saveFileDialog.FileName;
            }
        }

        private void Button_Click_10(object sender, RoutedEventArgs e)
        {
            string defaultFileName = tbPoLoadPath.Text;
            if(string.IsNullOrEmpty(defaultFileName))
            {
                defaultFileName = "strings.po";
            }
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Title = "불러올 Po 파일을 선택하십시오.",
                FileName = defaultFileName
            };
            if(openFileDialog.ShowDialog() == true)
            {
                string loadPoFileName = openFileDialog.FileName;
                if (File.Exists(loadPoFileName))
                {
                    tbPoLoadPath.Text = loadPoFileName;
                } else
                {
                    MessageBox.Show("선택한 파일이 실제로 존재하지 않습니다.");
                }
            }
        }
    }
}
