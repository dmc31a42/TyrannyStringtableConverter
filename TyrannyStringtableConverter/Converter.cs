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
        [DllImport("AnemoneDllPort.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl,EntryPoint = "Anemone_TranslateText")]
        [return:MarshalAs(UnmanagedType.LPWStr)]
        static extern string TranslateText([MarshalAs(UnmanagedType.LPWStr)] string input);

        private TextFilesFolder textFilesFolder;
        public readonly MyPo myPo = new MyPo();

        public Converter()
        {
            fnWin32Project2(0);
        }

        public string[] AvailableISOAlpha2 { get { return myPo.AvailableISOAlpha2.ToArray(); } }
        

        public string unityNewLine(string input)
        {
            string[] newLines = { "\r\n", "\r", "\n" };
            string[] CRLF = input.Split(newLines,StringSplitOptions.RemoveEmptyEntries); //\r\n
            string output = "";
            foreach(string temp in CRLF)
            {
                output += temp + Environment.NewLine;
            }
            return output;
        }

        public void ReadFolder(string dirPath, string extension, string ISOAlpha2)
        {
            textFilesFolder = new TextFilesFolder(dirPath, extension, ISOAlpha2);
            foreach(KeyValuePair<string,string> keyValuePair in textFilesFolder)
            {
                myPo[keyValuePair.Key,ISOAlpha2] = unityNewLine(keyValuePair.Value);
            }
            myPo.AddISOAlpha2(ISOAlpha2);
        }

        public static string[] GetAvailableLanguageISOAlpha2InDataFolder(string dirPath)
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

        public void TranslateToKR(string sourceISOAlpha2)
        {
            foreach(string key in myPo.Keys.ToList())
            {
                myPo[key,"kr"] = unityNewLine(TranslateText(myPo[key,sourceISOAlpha2]));
            }
            myPo.AddISOAlpha2("kr");
        }

        public void SavePo(string poFilePath, string originalISOAlpha2, string targetISOAlpha2)
        {
            var catalogPot = new POCatalog
            {
                Encoding = "UTF-8"
            };
            var catalogPo = new POCatalog
            {
                Encoding = "UTF-8"
            };

            foreach (string key in myPo.Keys.ToList())
            {
                var poKey = new POKey(myPo[key,originalISOAlpha2], null, key);
                var entryPot = new POSingularEntry(poKey)
                {
                    Translation = ""
                };
                var entryPo = new POSingularEntry(poKey)
                {
                    Translation = myPo[key,targetISOAlpha2]
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

        public void LoadPo(string poFilePath, string originalISOAlpha2, string targetISOAlpha2)
        {
            var parser = new POParser();

            if(File.Exists(poFilePath))
            {
                StreamReader streamReader = File.OpenText(poFilePath);
                var result = parser.Parse(streamReader);
                if (result.Success)
                {
                    var catalog = result.Catalog;
                    foreach(string key in myPo)
                    {
                        /// ONLY TEMPERARY CODE
                        string msgId = myPo[key,originalISOAlpha2];
                        string translated = catalog.GetTranslation(new POKey(msgId, null, key));
                        if (string.IsNullOrEmpty(translated) == false)
                        {
                            myPo[key,targetISOAlpha2] = translated;
                            myPo.AddISOAlpha2(targetISOAlpha2);
                        }
                    }
                } else
                {
                    var diagnostics = result.Diagnostics;
                }
            }
        }

        public void SaveFolder(string targetDirPath, string translatedISOAlpha2)
        {
            foreach(string key in textFilesFolder.Keys.ToList())
            {
                if(myPo.ContainsKey(key))
                {
                    if(String.IsNullOrEmpty(myPo[key,translatedISOAlpha2]))
                    {
                        textFilesFolder[key] = myPo[key,translatedISOAlpha2];
                    }
                }
            }
        }
    }
}
