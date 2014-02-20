using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.Linq;
using System.IO;
using CsvHelper;
using System.Data.SqlClient;
using LinqExtension;
using System.Data;

namespace ExperimentDatabasePopulator
{

    struct ENVIRONMENT_DESC
    {
        public string type;
        public int num_objects;
        public double ratio_gold;
        public int grid_size;
        public int sink_boundary;
    }

    struct EXPERIMENT_DESC {
	    // height and width
	    //TO DO: Change to size.
        public int height;
        public int width;

	    //Robots
        public int number_robots;
        public float forager_explorer_ratio;
        public float gold_waste_division_ratio;


	    //Objects
        public int number_objects;
        public float gold_waste_ratio;

	    //total cluster iterations
        public int total_iterations;

	    //Deprecated
        public int total_cluster_iterations;
        public int total_forage_iterations;
        public float gold_waste_division_ratio_cluster;
        public float gold_waste_division_ratio_forage;

	    //Max path that robots are to be initialized with
        public int max_path;
        public int samples;
    };

    struct AveStd
    {
        public double average { get; set; }
        public double stdDev { get; set; }
        public int algorithm { get; set; }

        public override string ToString()
        {
            return String.Format("{0} ({1})", average, stdDev);
        }
    };

    enum FluxVizParameters { Algorithm, EnvironmentType, EnvironmentSize, EnvironmentRatio, NumberObjects, Measure, NumRobots, Division };
    class Program
    {
        /*
        * Creates FluxViz file for each measure with the following items
        * Algorithm, Environment Type, Ratio, Division, Grid-size, number robots, number objects, Performance Measure
        */
        static void Create_FluxViz_All(int measureId, RobominerDataContext dc)
        {
            //TODO: Check how to average of items properly
            var results1 = dc.Results.Where(n => n.measureid == measureId);
            var results2 = results1.GroupBy(x => new { algorithmid = x.algorithmid, type = x.Environment.type, ratio = x.Environment.ratio, division = x.Experiment.division, size = x.Environment.size, objects = x.Environment.objects, robots = x.Experiment.robots });
            var results = results2.Select(x => new { algorithmid = x.Key.algorithmid, type = x.Key.type, ratio = x.Key.ratio, division = x.Key.division, size = x.Key.size, objects = x.Key.objects, robots = x.Key.robots , value = (double)x.Average(p => p.value) });

            //TODO: Check how to write to a file.
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
            string fileName = String.Format("FluxVizMeasure_Everything_{0}.txt", name);
            FileStream fileStream = File.Open(fileName, FileMode.Create);
            StreamWriter writer = new StreamWriter(fileStream);
            foreach (var r in results)
            {
                string s = String.Format("{0} {1} {2} {3}", r.algorithmid, r.type, r.ratio, r.value);
                writer.WriteLine(s);
            }
            writer.Close();
            AppendToFluxVizBatch(fileName, 4, results.Count());
        }

        /*
         * Creates FluxViz file for each measure with the following items
         * Algorithm, Environment Type, Ratio, Division, Grid-size, Performance Measure
         */
        static void Create_FluxViz_Alg_Type_Rat_Div_Env(int measureId, RobominerDataContext dc)
        {
            //Averaging Items.
            var results1 = dc.Results.Where(n => n.measureid == measureId);
            var results2 = results1.GroupBy(x => new { algorithmid = x.algorithmid, type = x.Environment.type, ratio = x.Environment.ratio, division = x.Experiment.division, size = x.Environment.size });
            var results = results2.Select(x => new { algorithmid = x.Key.algorithmid, type = x.Key.type, ratio = x.Key.ratio, division = x.Key.division, size = x.Key.size, value = (double)x.Average(p => p.value) });

            //Writing to file. 
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
            string fileName = String.Format("FluxVizMeasure_Alg_Type_Rat_Div_Env_{0}.txt", name);
            FileStream fileStream = File.Open(fileName,FileMode.Create);
            StreamWriter writer = new StreamWriter(fileStream);
            foreach (var r in results)
            {
                string s = String.Format("{0} {1} {2} {3} {4} {5}", r.algorithmid, r.type, r.ratio, r.division, r.size, r.value);
                writer.WriteLine(s);
            }
            writer.Close();
            AppendToFluxVizBatch(fileName, 6, results.Count());
        }
        /*
         * Creates FluxViz file for each measure with the following items
         * Algorithm, Environment Type, Ratio, Performance Measure
         */
        static void Create_FluxViz_Alg_Type_Ratio(int measureId, RobominerDataContext dc)
        {
            //TODO: Check how to average of items properly
            var results1 = dc.Results.Where(n => n.measureid == measureId);
            var results2 = results1.GroupBy(x => new { algorithmid = x.algorithmid, type = x.Environment.type, ratio = x.Environment.ratio });
            var results = results2.Select(x => new { algorithmid = x.Key.algorithmid, type = x.Key.type, ratio = x.Key.ratio, value = (double)x.Average(p => p.value) });

            //TODO: Check how to write to a file.
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
            string fileName = String.Format("FluxVizMeasure_Alg_Type_Ratio_{0}.txt", name.Trim());
            FileStream fileStream = File.Open(fileName, FileMode.Create);
            StreamWriter writer = new StreamWriter(fileStream);
            foreach (var r in results)
            {
                string s = String.Format("{0} {1} {2} {3}", r.algorithmid, r.type, r.ratio, r.value);
                writer.WriteLine(s);
            }
            writer.Close();
            AppendToFluxVizBatch(fileName, 4, results.Count());
        }

        /**
         Creates a FluxViz file for each measure with the following items
         * Algorithm, Environment Type, Performance Measure
         */
        static void Create_FluxViz_File_Alg_EnvType(int measureId, RobominerDataContext dc)
        {

            //TODO: Check how to average of items properly
            var results1 = dc.Results.Where(n => n.measureid == measureId);
            var results2 = results1.GroupBy(x => new {algorithmid = x.algorithmid, type = x.Environment.type });
            var results = results2.Select(x => new { algorithmid = x.Key.algorithmid, type =  x.Key.type,  value = (double)x.Average(p => p.value)});
            
            //TODO: Check how to write to a file.
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
            string fileName = String.Format("FluxVizMeasure_Alg_EnvType_{0}.txt", name);
            FileStream fileStream = File.Open(fileName, FileMode.Create);
            StreamWriter writer = new StreamWriter(fileStream);
            foreach (var r in results)
            {
                string s = String.Format("{0} {1} {2}", r.algorithmid, r.type, r.value);
                writer.WriteLine(s);
            }
            writer.Close();
            AppendToFluxVizBatch(fileName, 3, results.Count());
        }

        /**
         * Creates batch file for which fluxviz can be run repetitively 
         */
        static void AppendToFluxVizBatch(string fileName, int num_fields, int num_rows)
        {
            using (StreamWriter writer = File.AppendText("FluxViz_batch.bat"))
            {
                writer.WriteLine("FluxViz.exe {0} {1} {2}", fileName, num_rows,num_fields);
                writer.Close();
            }
        }
        /**
         * Input file to enable bulk run MWU in R
         * input: input path
         * output: output path
         * tag: parameter value
         * category: parameter name
         */
        static void AppendToMWUFile(string performanceMeasure, string input, string tag, string category)
        {
            string output = String.Format("MWU/MWU_{0}_results.csv", performanceMeasure);
            using (StreamWriter writer = File.AppendText(String.Format("MWU/MWU_{0}_batch.csv",performanceMeasure)))
            {
                writer.WriteLine("{0}, {1}, {2}, {3}", input, tag, category, output);
                writer.Close();
            }        
        }

        static void AppendToLatexInputFile(string fileName)
        {
            using (StreamWriter writer = File.AppendText("Table_include.tex"))
            {
                writer.WriteLine("\\input{{{0}}}", fileName);
                writer.Close();
            }  
        }


        /**
         * Create files for R Mann-whitney Script
         */
        static void Create_MWU_All(int measureId, RobominerDataContext dc)
        {
            var results = dc.Results.Where(n => n.measureid== measureId);
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
            string fileName = String.Format("MWU/MWU_{0}.txt", name);

            //TODO: Check how to write to a file.
            FileStream fileStream = File.Open(fileName, FileMode.Create);
            StreamWriter writer = new StreamWriter(fileStream);
            foreach (Result r in results)
            {
                string s = String.Format("{0} {1}", r.value, r.Algorithm.name.Trim());
                writer.WriteLine(s);
            }
            writer.Close();
            AppendToMWUFile(name, fileName, "all", "all");
        }

        /**
         * Run Mann Whitney U file generation.
         */
        static void Create_MWU(string performanceMeasure, List<Result> results, string fileName, string tag, string category)
        {
             //Write to MWU File.
                WriteMWUFile(results, fileName);

                //Write to batch file
                AppendToMWUFile(performanceMeasure, fileName, tag, category);
        }

        /**
         * Create files for R Mann-whitney Script per environment type
         */
        static void Create_MWU_Type(int measureId, RobominerDataContext dc)
        {
            for (int i = 1; i <= dc.EnvironmentTypes.Count(); i++)
            {
                var results = dc.Results.Where(n => n.measureid == measureId && n.Environment.type == i);
                string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
                string environmentName = dc.EnvironmentTypes.Where(n => n.id == i).FirstOrDefault().name;
                string fileName = String.Format("MWU/MWU_{0}_{1}.txt", name, environmentName.Trim());

               Create_MWU(name,results.ToList<Result>(),fileName, environmentName,"Environment");
            }
        }

        /**
         * Create files for R Mann-whitney Script per environment size
         */
        static void Create_MWU_Size(int measureId, RobominerDataContext dc)
        {
            //Get all sizes in environment
            List<int?> sizes = dc.Environments.Select(n => n.size).Distinct().ToList<int?>();

            //Get all sizes
            for (int i = 0; i < sizes.Count(); i++)
            {
                //Get results for performance measure and environment size. 
                var results = dc.Results.Where(n => n.measureid == measureId && n.Environment.size == sizes[i]);

                //Get performance measure
                string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

                string fileName = String.Format("MWU/MWU_{0}_size{1}.txt", name, sizes[i] );

                Create_MWU(name, results.ToList<Result>(), fileName, Convert.ToString(sizes[i]), "sizes");

            }
        }

        /**
         * Create files for R Mann-whitney Script per environment ratio 
         */
        static void Create_MWU_Ratio(int measureId, RobominerDataContext dc)
        {
            //Get all sizes in environment
            List<double?> ratios = dc.Environments.Select(n => n.ratio).Distinct().ToList<double?>();

            //Get all sizes
            for (int i = 0; i < ratios.Count(); i++)
            {
                //Get results for performance measure and environment size. 
                var results = dc.Results.Where(n => n.measureid == measureId && n.Environment.ratio == ratios[i]);

                //Get performance measure
                string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

                string fileName = String.Format("MWU/MWU_{0}_ratio{1}.txt", name, ratios[i]);

                Create_MWU(name, results.ToList<Result>(), fileName, Convert.ToString(ratios[i]), "ratios");
            }
        }

        /**
         * Create files for R Mann-whitney Script per num robots 
         */
        static void Create_MWU_Robots(int measureId, RobominerDataContext dc)
        {
            //Get all sizes in environment
            List<int?> robots = dc.Experiments.Select(n => n.robots).Distinct().ToList<int?>();

            //Get performance measure
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

            //Get all sizes
            for (int i = 0; i < robots.Count(); i++)
            {
                //Get results for performance measure and environment size. 
                var results = dc.Results.Where(n => n.measureid == measureId && n.Experiment.robots == robots[i]);

               
                string fileName = String.Format("MWU/MWU_{0}_robots{1}.txt", name, robots[i]);

                Create_MWU(name, results.ToList<Result>(), fileName, Convert.ToString(robots[i]), "robots");
            }
        }

        /**
         * Create files for R Mann-whitney Script per num robots 
         */
        static void Create_MWU_Division(int measureId, RobominerDataContext dc)
        {
            //Get all sizes in environment
            List<double?> division = dc.Experiments.Select(n => n.division).Distinct().ToList<double?>();

            //Get performance measure
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

            //Get all sizes
            for (int i = 0; i < division.Count(); i++)
            {
                //Get results for performance measure and environment size. 
                var results = dc.Results.Where(n => n.measureid == measureId && n.Experiment.division == division[i]);


                string fileName = String.Format("MWU/MWU_{0}_division{1}.txt", name, division[i]);

                Create_MWU(name, results.ToList<Result>(), fileName, Convert.ToString(division[i]), "division");
            }
        }

        /**
         * Create files for R Mann-whitney Script per num robots 
         */
        static void Create_MWU_Objects(int measureId, RobominerDataContext dc)
        {
            //Get all sizes in environment
            List<int?> objects = dc.Environments.Select(n => n.objects).Distinct().ToList<int?>();

            //Get performance measure
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

            //Get all sizes
            for (int i = 0; i < objects.Count(); i++)
            {
                //Get results for performance measure and environment size. 
                var results = dc.Results.Where(n => n.measureid == measureId && n.Environment.objects == objects[i]);
                string fileName = String.Format("MWU/MWU_{0}_objects{1}.txt", name, objects[i]);
                Create_MWU(name, results.ToList<Result>(), fileName, Convert.ToString(objects[i]), "objects");
            }
        }

        static void WriteMWUFile(List<Result> results, string fileName)
        {
            //Write to File
            FileStream fileStream = File.Open(fileName, FileMode.Create);
            StreamWriter writer = new StreamWriter(fileStream);
            foreach (Result r in results)
            {
                string s = String.Format("{0} {1}", r.value, r.Algorithm.name.Trim());
                writer.WriteLine(s);
            }
            writer.Close();
        }

        /**
         * Create Latex Tables for average & standard deviations
         */

        static void CreateAverageStandardDevLatex(int measureId, RobominerDataContext dc)
        {
           var result = dc.Results.GroupBy(x => new {x.algorithmid, x.Environment.type}).
                    Select(r => new AveStd { algorithm = r.Key.algorithmid, average = (double)r.Average(p => p.value), stdDev = (double)r.StdDev(x => x.value) });
            
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();
            string fileName = String.Format("LatexAve_{0}.txt", name);

        }

        /**
         * Creates avg for
         * */
        static void Create_Avg_General(int measureId, string table, string column, RobominerDataContext dc, string format, bool trimField = false)
        {
            //Get performance measure
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

            //Create file name
            string filePath = String.Format("Avg_{3}_{0}_{1}.{2}", table, column, format, name);

            //Does the Column need to be trimmed?
            string alias = column;
            if (trimField)
            {
                table = String.Format("RTRIM({0}", table);
                column = String.Format("{0} )", column);
            }

            //Construct query
            string query = String.Format(@"SELECT * FROM (
	            SELECT RTRIM(Algorithm.name) as algorithm, {1}.{0} as {3}, AVG(value) as value
	            FROM [Experiment].[dbo].[Results] 
	            JOIN Experiment.dbo.Environment on environmentid = Experiment.dbo.Environment.id
                JOIN Experiment.dbo.Experiment on experimentid = Experiment.dbo.Experiment.id
	            JOIN Experiment.dbo.Algorithm on algorithmid = Experiment.dbo.Algorithm.id
                JOIN Experiment.dbo.EnvironmentType on Experiment.dbo.Environment.type = Experiment.dbo.EnvironmentType.id
	            WHERE measureid = {2}
	            GROUP BY Algorithm.name, {1}.{0} ) 
            AS AvgStd
            PIVOT ( 
	            MAX(value) FOR [algorithm] IN (Naive, DesertAnt,HoneyBee )
            ) AS p;", column, table, measureId, alias);

            //Create new dataset
            DataSet ds = new DataSet();

            //Execute Query
            using (SqlConnection connection = new SqlConnection("Data Source=JADE-PC;Initial Catalog=Experiment;Integrated Security=True"))
            {
                SqlCommand command = new SqlCommand(query, connection);
                SqlDataAdapter da = new SqlDataAdapter(command);
                da.Fill(ds);

            }

            //Write query
            if (ds.Tables.Count > 0)
            {
                if (format.Equals("tex"))
                {
                    LatexWriter writer = new LatexWriter(filePath);
                    writer.OutputTable(ds.Tables[0]);
                }
                else
                {
                    CsvTableWriter csvwriter = new CsvTableWriter(filePath);
                    csvwriter.OutputTable(ds.Tables[0]);
                }
            }

        }

        /**
         * Creates an average with standard deviation latex table grouping by any specified table and any specified column.
         * trimField = True if the column needs to be trimmed.
         */
        static void Create_AvgDev_General(int measureId, string table, string column, RobominerDataContext dc, string format, bool trimField = false)
        {
            //Get performance measure
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault().Trim();

            //Create file name
            string filePath = String.Format(format +"\\AvgDev_{3}_{0}_{1}.{2}", table, column,format,name);

            //Does the Column need to be trimmed?
            string alias = column;
            if (trimField)
            {
                table = String.Format("RTRIM({0}", table);
                column = String.Format("{0} )", column);
            }

            //Construct query
            string query = String.Format(@"SELECT * FROM (
	            SELECT RTRIM(Algorithm.name) as algorithm, {1}.{0} as {3}, ( CAST(AVG(value) as VARCHAR(20) )+ ' (' +CAST(STDEV(value) as VARCHAR(20))+') ') as value
	            FROM [Experiment].[dbo].[Results] 
	            JOIN Experiment.dbo.Environment on environmentid = Experiment.dbo.Environment.id
                JOIN Experiment.dbo.Experiment on experimentid = Experiment.dbo.Experiment.id
	            JOIN Experiment.dbo.Algorithm on algorithmid = Experiment.dbo.Algorithm.id
                JOIN Experiment.dbo.EnvironmentType on Experiment.dbo.Environment.type = Experiment.dbo.EnvironmentType.id
	            WHERE measureid = {2}
	            GROUP BY Algorithm.name, {1}.{0} ) 
            AS AvgStd
            PIVOT ( 
	            MAX(value) FOR [algorithm] IN (Naive, DesertAnt,HoneyBee )
            ) AS p;", column, table, measureId, alias);

            //Create new dataset
            DataSet ds = new DataSet();

            //Execute Query
            using (SqlConnection connection = new SqlConnection("Data Source=JADE-PC;Initial Catalog=Experiment;Integrated Security=True"))
            {
                SqlCommand command = new SqlCommand(query, connection);
                SqlDataAdapter da = new SqlDataAdapter(command);
                da.Fill(ds);

            }

            //Write query
            if (ds.Tables.Count > 0)
            {
                if (format.Equals("tex"))
                {
                    LatexWriter writer = new LatexWriter(filePath);
                    writer.OutputTable(ds.Tables[0]);
                }
                else
                {
                    CsvTableWriter csvwriter = new CsvTableWriter(filePath);
                    csvwriter.OutputTable(ds.Tables[0]);
                }
            }

        }
        /**
         * Creates latex table of averages and deviations for a specific performance measure
         * 
         */

        static void Main(string[] args)
        {
            RobominerDataContext dc = new RobominerDataContext("Data Source=JADE-PC;Initial Catalog=Experiment;Integrated Security=True");
           
            foreach (PerformanceMeasure a in dc.PerformanceMeasures)
            {
                //MWU
                Create_MWU_All(a.id, dc);
                Create_MWU_Division(a.id, dc);
                Create_MWU_Objects(a.id, dc);
                Create_MWU_Ratio(a.id, dc);
                Create_MWU_Robots(a.id, dc);
                Create_MWU_Size(a.id, dc);
                Create_MWU_Type(a.id, dc);
/*
                //FluxViz
                Create_FluxViz_All(a.id, dc);
                Create_FluxViz_File_Alg_EnvType(a.id, dc);
                Create_FluxViz_Alg_Type_Ratio(a.id, dc);
                Create_FluxViz_Alg_Type_Rat_Div_Env(a.id, dc);
                
                //Latex
                string format = "csv";
                Create_AvgDev_General(a.id, "EnvironmentType", "name", dc, format, true);
                Create_AvgDev_General(a.id, "Environment", "ratio", dc, format);
                Create_AvgDev_General(a.id, "Environment", "size", dc, format);
                Create_AvgDev_General(a.id, "Environment", "objects", dc, format);
                Create_AvgDev_General(a.id, "Experiment", "division", dc, format);
                Create_AvgDev_General(a.id, "Experiment", "robots", dc, format);

                format = "tex";
                Create_Avg_General(a.id, "EnvironmentType", "name", dc, format, true);
                Create_Avg_General(a.id, "Environment", "ratio", dc, format);
                Create_Avg_General(a.id, "Environment", "size", dc, format);
                Create_Avg_General(a.id, "Environment", "objects", dc, format);
                Create_Avg_General(a.id, "Experiment", "division", dc, format);
                Create_Avg_General(a.id, "Experiment", "robots", dc, format);
                */
            }
            dc.Dispose();
        }

    }
}


