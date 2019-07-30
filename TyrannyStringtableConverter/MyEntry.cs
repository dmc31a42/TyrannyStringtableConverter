using System;
using System.Collections;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TyrannyStringtableConverter
{
    public class MyPo : IEnumerable
    {
        public Dictionary<string, MyEntry> Entries { get; } = new Dictionary<string, MyEntry>();
        private readonly HashSet<string> _AvailableISOAlpha2 = new HashSet<string>();
        public string[] AvailableISOAlpha2
        {
            get { return _AvailableISOAlpha2.ToArray(); }
        }
        public void AddISOAlpha2(string ISOAlpha2)
        {
            _AvailableISOAlpha2.Add(ISOAlpha2);
        }
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
            get
            {
                if (Entries.ContainsKey(key) == false)
                {
                    Entries[key] = new MyEntry { Key = key };
                }
                return Entries[key];
            }
            set
            {
                Entries[key] = value;
            }
        }

        public string this[string key, string ISOAlpha2]
        {
            get { return (string)Entries[key][ISOAlpha2]; }
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

        public string this[int iter, string ISOAlpha2]
        {
            get
            {
                return (string)Entries[Entries.Keys.ElementAt(iter)][ISOAlpha2];
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


    public class MyEntryDEPRECATED
    {
        public Dictionary<string, string> Text { get; } = new Dictionary<string, string>();
        public string Key { get; set; }

        public string this[string ISOAlpha2]
        {
            get { return Text[ISOAlpha2]; }
            set { Text[ISOAlpha2] = value; }
        }
    }

    public class MyEntry : DynamicObject
    {
        public Dictionary<string, object> dictionary { get; } = new Dictionary<string, object>();
        public string Key { get; set; }
        // If you try to get a value of a property 
        // not defined in the class, this method is called.
        public override bool TryGetMember(
            GetMemberBinder binder, out object result)
        {
            // Converting the property name to lowercase
            // so that property names become case-insensitive.
            string name = binder.Name.ToLower();

            // If the property name is found in a dictionary,
            // set the result parameter to the property value and return true.
            // Otherwise, return false.
            return dictionary.TryGetValue(name, out result);
        }

        // If you try to set a value of a property that is
        // not defined in the class, this method is called.
        public override bool TrySetMember(
            SetMemberBinder binder, object value)
        {
            // Converting the property name to lowercase
            // so that property names become case-insensitive.
            dictionary[binder.Name.ToLower()] = value;

            // You can always add a value to a dictionary,
            // so this method always returns true.
            return true;
        }

        public object this[string ISOAlpha2]
        {
            get { return dictionary[ISOAlpha2]; }
            set { dictionary[ISOAlpha2] = value; }
        }
    }
}
