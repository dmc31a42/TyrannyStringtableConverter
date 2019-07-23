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
    public class TextFilesFolder : IEnumerable<KeyValuePair<string, string>>
    {
        public readonly string FolderRoot;
        public readonly string Extension;
        public readonly string ISOAlpha2;
        private readonly Dictionary<string, TextFile> textFiles = new Dictionary<string, TextFile>();
        private readonly Dictionary<string, string> keyList = new Dictionary<string, string>();
        public int Count
        {
            get
            {
                return keyList.Count;
            }
        }
        public TextFilesFolder(string folderRoot, string extension, string ISOAlpha2)
        {
            FolderRoot = folderRoot;
            int foundIndex = extension.IndexOf("*.");
            if (foundIndex != -1)
            {
                extension = extension.Substring(foundIndex);
            }
            Extension = extension;
            this.ISOAlpha2 = ISOAlpha2;
            
            var filesFound = Directory.GetFiles(FolderRoot, "*." + Extension, SearchOption.AllDirectories);
            foreach(string filePath in filesFound)
            {
                if(filePath.Contains(Path.DirectorySeparatorChar + ISOAlpha2 + Path.DirectorySeparatorChar))
                {
                    string relativePath = filePath.Replace(FolderRoot + Path.DirectorySeparatorChar, "");
                    var textFile = new TextFile(folderRoot, relativePath);
                    textFiles.Add(relativePath, textFile);
                    foreach(string key in textFile.Keys)
                    {
                        keyList.Add(relativePath + "/" + key, relativePath);
                    }
                }
            }
        }

        public void Save(string targetDirPath, string translatedISOAlpha2)
        {
            foreach (TextFile textFile in textFiles.Values)
            {
                string relativePath = textFile.RelativePath;
                string changedRelativePath = relativePath.Replace(Path.DirectorySeparatorChar + ISOAlpha2 + Path.DirectorySeparatorChar
                    , Path.DirectorySeparatorChar + translatedISOAlpha2 + Path.DirectorySeparatorChar);
                string changedFullPath = Path.Combine(targetDirPath, changedRelativePath);
                string FolderPath = Path.GetDirectoryName(changedFullPath);
                Directory.CreateDirectory(FolderPath);
                textFile.Save(changedFullPath);
            }
        }

        public string this[int key]
        {
            get
            {
                return this[this.Keys.ElementAt(key)];
            }
            set
            {
                this[this.Keys.ElementAt(key)] = value;
            }
        }

        public string this[string key]
        {
            get
            {
                var relativePath = this.keyList[key];
                var localKey = key.Replace(relativePath + "/", "");
                return this.textFiles[relativePath][localKey];
            }
            set
            {
                var relativePath = this.keyList[key];
                var localKey = key.Replace(relativePath + "/", "");
                this.textFiles[relativePath][localKey] = value;
            }
        }

        public Dictionary<string, string>.KeyCollection Keys
        {
            get
            {
                return this.keyList.Keys;
            }
        }

        public IEnumerator<KeyValuePair<string, string>> GetEnumerator()
        {
            return new TextFilesFolderEnum(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return (IEnumerator)GetEnumerator();
        }
    }

    public class TextFilesFolderEnum : IEnumerator<KeyValuePair<string, string>>
    {
        private readonly TextFilesFolder textFilesFolder;
        private int position = -1;

        public TextFilesFolderEnum(TextFilesFolder textFilesFolder)
        {
            this.textFilesFolder = textFilesFolder;
        }

        public KeyValuePair<string, string> Current
        {
            get
            {
                return new KeyValuePair<string, string>(textFilesFolder.Keys.ElementAt(position), textFilesFolder[position]);
            }
        }

        object IEnumerator.Current
        {
            get
            {
                return Current;
            }
        }

        public void Dispose()
        {
            //throw new NotImplementedException();
        }

        public bool MoveNext()
        {
            position++;
            return (position < textFilesFolder.Count);
        }

        public void Reset()
        {
            position = -1;
        }
    }
}
