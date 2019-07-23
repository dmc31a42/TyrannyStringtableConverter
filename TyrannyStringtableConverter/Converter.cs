using Karambolo.PO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TyrannyStringtableConverter
{

    public class Converter
    {
        [DllImport("AnemoneDllPort.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int fnWin32Project2(int value);
        [DllImport("AnemoneDllPort.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr Anemone_TranslateText([MarshalAs(UnmanagedType.LPWStr)] string input);

        static string TranslateText(string input)
        {
            IntPtr intPtr = Anemone_TranslateText(input);
            return Marshal.PtrToStringUni(intPtr);
        }

        private TextFilesFolder textFilesFolder;

        public Converter()
        {
            fnWin32Project2(0);
        }
        public void ReadFolder(string dirPath, string extension, string ISOAlpha2)
        {
            textFilesFolder = new TextFilesFolder(dirPath, extension, ISOAlpha2);
        }

        public static string[] GetAvailableLanguageISOAlpha2(string dirPath)
        {
            string[] sampleISOAlpha2s = { "en", "EN", "de", "DE" , "jp", "JP"};
            string foundDirectory = "";
            foreach(string ISOAlpha2 in sampleISOAlpha2s)
            {
                var found = Directory.GetDirectories(dirPath, ISOAlpha2, SearchOption.AllDirectories);
                if(found.Length != 0)
                {
                    foundDirectory = found[0];
                    break;
                }
            }
            if(foundDirectory != "")
            {
                var founds = new DirectoryInfo(foundDirectory).Parent.GetDirectories();
                return (from found in founds
                        select found.Name).ToArray();

            }
            return new string[] { };
        }

        public void SavePo(string poFilePath)
        {
            var catalog = new POCatalog
            {
                Encoding = "UTF-8"
            };

            foreach (KeyValuePair<string, string> keyValuePair in textFilesFolder)
            {
                var key = new POKey(keyValuePair.Value, null, keyValuePair.Key);
                var entry = new POSingularEntry(key)
                {
                    Translation = TranslateText(keyValuePair.Value)
                };
                catalog.Add(entry);
            }

            var generator = new POGenerator(new POGeneratorSettings {
                IgnoreEncoding = true
            });
            StringWriter streamWriter = new StringWriter();
            generator.Generate(streamWriter, catalog);

            File.WriteAllText(poFilePath, streamWriter.ToString());
        }

        public void LoadPo(string poFilePath)
        {
            var parser = new POParser();

            if(File.Exists(poFilePath))
            {
                StreamReader streamReader = File.OpenText(poFilePath);
                var result = parser.Parse(streamReader);
                if (result.Success)
                {
                    var catalog = result.Catalog;
                } else
                {
                    var diagnostics = result.Diagnostics;
                }
            }
        }
    }
}
