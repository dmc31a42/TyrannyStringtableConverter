using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TyrannyStringtableConverter
{
    public class MyPo : IEnumerable
    {
        public Dictionary<string, MyEntry> Entries { get; } = new Dictionary<string, MyEntry>();
        public void Add(MyEntry myEntry)
        {
            Entries.Add(myEntry.Key, myEntry);
            
        }
        public bool ContainsKey(string key)
        {
            return Entries.ContainsKey(key);
        }

        public Dictionary<string, MyEntry>.KeyCollection Keys => Entries.Keys;

        public MyEntry this[string key]
        {
            get { return Entries[key]; }
            set
            {
                if (Entries.ContainsKey(key) == false) {
                    Entries.Add(key, new MyEntry { Key = key });
                }
                Entries[key] = value;
            }
        }

        public string this[string key, string ISOAlpha2]
        {
            get { return Entries[key][ISOAlpha2]; }
            set
            {
                if (Entries.ContainsKey(key) == false)
                {
                    Entries[key] = new MyEntry
                    {
                        Key = key
                    };
                }
                Entries[key][ISOAlpha2] = value;
            }
        }

        public MyEntry this[string key, MyEntry myEntry]
        {
            get { return Entries[key]; }
            set { Entries[key] = myEntry; }
        }

        public string this[int iter, string ISOAlpha2]
        {
            get
            {
                return Entries[Entries.Keys.ElementAt(iter)][ISOAlpha2];
            }
        }

        public IEnumerator GetEnumerator()
        {
            return Entries.GetEnumerator();
        }
    }

    public class MyPoEnum : IEnumerator
    {
        private MyPo myPo;
        private int position = -1;
        private string ISOAlpha2;

        public MyPoEnum(MyPo myPo, string ISOAlpha2)
        {
            throw new NotImplementedException();
        }

        public object Current => throw new NotImplementedException();

        public bool MoveNext()
        {
            throw new NotImplementedException();
        }

        public void Reset()
        {
            throw new NotImplementedException();
        }
    }


    public class MyEntry
    {
        public Dictionary<string, string> Text { get; } = new Dictionary<string, string>();
        public string Key { get; set; }

        public string this[string ISOAlpha2]
        {
            get { return Text[ISOAlpha2]; }
            set { Text[ISOAlpha2] = value; }
        }
    }
}
