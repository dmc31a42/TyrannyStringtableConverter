using Microsoft.VisualStudio.TestTools.UnitTesting;
using Karambolo.PO;
using System.IO;

namespace UnitTestProject1
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            var catalog = new POCatalog { Encoding = "UTF-8"};
            var key = new POKey("\"フェイトバインダー、よく戻った！\"", null, "data\\exported\\localized\\jp\\text\\conversations\\factionquestlines\\rebel\\fq_rb_arris_rebelpathintro.stringtable/1/DefaultText");
            var entry = new POSingularEntry(key) { Translation = "" };

            catalog.Add(entry);

            using (StreamWriter streamWriter = File.CreateText("test.po"))
            {
                var generator = new POGenerator();
                generator.Generate(streamWriter, catalog);

            }
            using(StreamReader streamReader = new StreamReader("test.po"))
            {
                var parser = new POParser();
                var result = parser.Parse(streamReader);
                if(result.Success)
                {
                    var catalog2 = result.Catalog;
                } else
                {
                    var diagnostics = result.Diagnostics;
                }
            }
            

            
        }
    }
}
