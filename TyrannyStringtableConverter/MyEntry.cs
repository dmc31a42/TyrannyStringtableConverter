using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Dynamic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TyrannyStringtableConverter
{
    public class MyPo : IEnumerable
    {
        public Dictionary<string, MyEntry> Entries { get; private set; } = new Dictionary<string, MyEntry>();
        private Dictionary<string, MyEntry> backupEntries;
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

    public class MyEntry : DynamicObject, IEditableObject, INotifyPropertyChanged
    {
        public Dictionary<string, object> Dictionary { get; private set; } = new Dictionary<string, object>();
        private Dictionary<string, object> backupDictionary;

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
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
            return Dictionary.TryGetValue(name, out result);
        }

        // If you try to set a value of a property that is
        // not defined in the class, this method is called.
        public override bool TrySetMember(
            SetMemberBinder binder, object value)
        {
            // Converting the property name to lowercase
            // so that property names become case-insensitive.
            Dictionary[binder.Name.ToLower()] = value;
            OnPropertyChanged(binder.Name.ToLower());
            // You can always add a value to a dictionary,
            // so this method always returns true.
            return true;
        }

        public void BeginEdit()
        {
            if(backupDictionary == null)
            {
                backupDictionary = new Dictionary<string, object>(Dictionary);
            }
        }

        public void EndEdit()
        {
            backupDictionary = null;
        }

        public void CancelEdit()
        {
            Dictionary = backupDictionary;
            backupDictionary = null;
        }

        public object this[string ISOAlpha2]
        {
            get { return Dictionary[ISOAlpha2]; }
            set { Dictionary[ISOAlpha2] = value; }
        }
    }
}
