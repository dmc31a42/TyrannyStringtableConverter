using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace TyrannyStringtableConverter
{
    public class TextFile : IEnumerable<KeyValuePair<string, string>>
    {
        public readonly string FolderRoot;
        public readonly string RelativePath;
        public int Count
        {
            get {
                return this.keyValuePairs.Count;
            }
        }
        private readonly XmlDocument rootDocument;
        private readonly Dictionary<string, XmlElement> keyValuePairs;
        public TextFile(string FolderRoot, string relativePath)
        {
            this.FolderRoot = FolderRoot;
            this.RelativePath = relativePath;
            keyValuePairs = new Dictionary<string, XmlElement>();
            var xmlDocument = new XmlDocument();
            xmlDocument.Load(Path.Combine(FolderRoot, RelativePath));
            if (xmlDocument["StringTableFile"]["Entries"] != null)
            {
                rootDocument = xmlDocument;
                foreach (XmlElement entry in rootDocument["StringTableFile"]["Entries"].ChildNodes)
                {
                    string id = entry["ID"].InnerText;
                    string DefaultText = entry["DefaultText"].InnerText;
                    string FemaleText = entry["FemaleText"].InnerText;
                    if (string.IsNullOrEmpty(id) == false)
                    {
                        if (string.IsNullOrEmpty(DefaultText) == false)
                        {
                            keyValuePairs.Add(id + "/" + "DefaultText", entry["DefaultText"]);
                        }
                        if (string.IsNullOrEmpty(FemaleText) == false)
                        {
                            keyValuePairs.Add(id + "/" + "FemaleText", entry["FemaleText"]);
                        }
                    }
                }
            }
        }

        public bool Save(string path)
        {
            try
            {
                this.rootDocument.Save(path);
                    return true;
            }
            catch (XmlException)
            {
                return false;
            }
        }

        public Dictionary<string, XmlElement>.KeyCollection Keys
        {
            get => this.keyValuePairs.Keys;
        }

        public string this[int key] {
            get
            {
                return keyValuePairs.Values.ElementAt(key).InnerText;
            }
            set
            {
                keyValuePairs.Values.ElementAt(key).InnerText = value;
            }
        }

        public string this[string key]
        {
            get
            {
                return keyValuePairs[key].InnerText;
            }
            set
            {
                keyValuePairs[key].InnerText = value;
            }
        }

        public IEnumerator<KeyValuePair<string, string>> GetEnumerator()
        {
            return new TextFileEnum(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return (IEnumerator)GetEnumerator();
        }
    }

    public class TextFileEnum : IEnumerator<KeyValuePair<string, string>>
    {
        private readonly TextFile textFile;
        private int position = -1;
        public TextFileEnum(TextFile textFile)
        {
            this.textFile = textFile;
        }

        object IEnumerator.Current
        {
            get
            {
                return Current;
            }
        }
        public KeyValuePair<string, string> Current {
            get
            {
                return new KeyValuePair<string, string>(textFile.Keys.ToList()[position], textFile[position]);
            }
        }

        public void Dispose()
        {
            //throw new NotImplementedException();
        }

        public bool MoveNext()
        {
            position++;
            return (position < textFile.Count);
        }

        public void Reset()
        {
            position = -1;
        }
    }
}
