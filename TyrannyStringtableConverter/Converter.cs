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
        [return:MarshalAs(UnmanagedType.LPWStr)]
        static extern string Anemone_TranslateText([MarshalAs(UnmanagedType.LPWStr)] string input);

        static string TranslateText(string input)
        {
            return Anemone_TranslateText(input);
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
            var catalogPot = new POCatalog
            {
                Encoding = "UTF-8"
            };
            var catalogPo = new POCatalog
            {
                Encoding = "UTF-8"
            };

            foreach (KeyValuePair<string, string> keyValuePair in textFilesFolder)
            {
                var key = new POKey(keyValuePair.Value, null, keyValuePair.Key);
                var entryPot = new POSingularEntry(key)
                {
                    Translation = ""
                };
                var entryPo = new POSingularEntry(key)
                {
                    Translation = TranslateText(keyValuePair.Value)
                };
                catalogPot.Add(entryPot);
                catalogPo.Add(entryPo);
            }

            var generatorPot = new POGenerator(new POGeneratorSettings
            {
                IgnoreEncoding = true
            });
            var generatorPo = new POGenerator(new POGeneratorSettings {
                IgnoreEncoding = true
            });
            StringWriter streamWriter = new StringWriter();
            generatorPo.Generate(streamWriter, catalogPo);
            File.WriteAllText(poFilePath, streamWriter.ToString());

            string potPath = Path.ChangeExtension(poFilePath, "pot");
            StringWriter streamWriterPot = new StringWriter();
            generatorPot.Generate(streamWriterPot, catalogPot);
            DirectoryInfo directoryInfo = new DirectoryInfo(poFilePath);
            File.WriteAllText(potPath, streamWriterPot.ToString(), Encoding.UTF8);
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
                    foreach(KeyValuePair<string, string> keyValuePair in textFilesFolder.ToArray())
                    {
                        /// ONLY TEMPERARY CODE
                        string msgId = keyValuePair.Value.Replace("\n", "\r\n");
                        string translated = catalog.GetTranslation(new POKey(msgId, null, keyValuePair.Key));
                        if (string.IsNullOrEmpty(translated) == false)
                        {
                            textFilesFolder[keyValuePair.Key] = translated;
                        }
                    }
                } else
                {
                    var diagnostics = result.Diagnostics;
                }
            }
        }

        public void Save(string targetDirPath, string translatedISOAlpha2)
        {
            textFilesFolder.Save(targetDirPath, translatedISOAlpha2);
        }
    }
}
