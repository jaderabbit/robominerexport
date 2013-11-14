using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExperimentDatabasePopulator
{
    class CsvTableWriter
    {
        public string Path { get; set; }

        public CsvTableWriter(string _Path)
        {
            Path = _Path;
        }

        public void OutputTable(DataTable table)
        {
            using (StreamWriter writer = new StreamWriter(Path))
            {
                //Create table specification and column header
                string header = table.Columns[0].ColumnName + " ";
                for (int i = 1; i < table.Columns.Count; i++)
                {
                    header += String.Format(",{0} ", table.Columns[i].ColumnName);
                }

                //Write Header
                writer.WriteLine(header);

                //Output column headers
                foreach (DataRow row in table.Rows)
                {
                    foreach (object item in row.ItemArray)
                    {
                        writer.Write(item + ", ");
                    }
                    writer.WriteLine("");
                }
            }
        }
    }
}
