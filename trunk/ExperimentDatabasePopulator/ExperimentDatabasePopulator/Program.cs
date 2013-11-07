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


    class Program
    {
        /**
         Creates a FluxViz file for each measure with the following items
         * Algorithm, Environment Type, Size, Performance Measure
         */
        static void CreateFluxVizFile(int measureId, RobominerDataContext dc)
        {

            //TODO: Check how to average of items properly
            var results1 = dc.Results.Where(n => n.measureid == measureId);
            var results2 = results1.GroupBy(x => new {algorithmid = x.algorithmid, type = x.Environment.type, size = x.Environment.size  });
            var results = results2.Select(x => new { algorithmid = x.Key.algorithmid, type =  x.Key.type, size = x.Key.size, value = (double)x.Average(p => p.value)});
            
            //TODO: Check how to write to a file.
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault();
            string fileName= String.Format("FluxVizMeasure_{0}.txt",name);
            StreamWriter writer = File.AppendText(fileName);
            foreach (var r in results)
            {
                string s = String.Format("{0} {1} {2} {3}", r.algorithmid, r.type, r.size, r.value);
                writer.WriteLine(s);
            }
            writer.Close();
        }

        /**
         * Create files for R Mann-whitney Script
         */
        static void CreateMannWhitneyUFile(int measureId, RobominerDataContext dc)
        {
            var results = dc.Results.Where(n => n.measureid== measureId);
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault();
            string fileName = String.Format("MWU_{0}.txt", name);

            //TODO: Check how to write to a file.
            StreamWriter writer = File.AppendText(fileName);
            foreach (Result r in results)
            {
                string s = String.Format("{0} {1}", r.value, r.Algorithm.id);
                writer.WriteLine(s);
            }
            writer.Close();
        }

        /**
         * Create Latex Tables for average & standard deviations
         */

        static void CreateAverageStandardDevLatex(int measureId, RobominerDataContext dc)
        {
            //var result = dc.Results.Where(n => n.measureid == measureId);
           // var result = dc.Results.GroupBy(x => new {x.algorithmid, x.Environment.type}).
           //         Select(r => new AveStd { algorithm = r.Key.algorithmid, average = (double)r.Average(p => p.value), stdDev = (double)r.StdDev(x => x.value) });
           var result = dc.Results.GroupBy(x => new {x.algorithmid, x.Environment.type}).
                    Select(r => new AveStd { algorithm = r.Key.algorithmid, average = (double)r.Average(p => p.value), stdDev = (double)r.StdDev(x => x.value) });
            
            string name = dc.PerformanceMeasures.Where(n => n.id == measureId).Select(n => n.name).FirstOrDefault();
            string fileName = String.Format("LatexAve_{0}.txt", name);

        }

        static List<AveStd> CreateAvgDevPerAlgorithmId(int measureId, int algorithmId, RobominerDataContext dc)
        {
            //var result = dc.Results.Where(n => n.measureid == measureId);
            // var result = dc.Results.GroupBy(x => new {x.algorithmid, x.Environment.type}).
            //         Select(r => new AveStd { algorithm = r.Key.algorithmid, average = (double)r.Average(p => p.value), stdDev = (double)r.StdDev(x => x.value) });
            var result2 = dc.Results.Where(x => x.algorithmid == algorithmId && x.measureid == measureId).GroupBy(x => x.Environment.type);            
            var result = result2.Select(r => new AveStd { algorithm = algorithmId, average = (double)r.Average(p => p.value), stdDev = (double)r.StdDev<Result,doub>(x => x.value) });
            //Bvar result3 = result2.StdDev(x => x.value);
            return result.ToList<AveStd>();
            
        }

        /**
         * Creates latex table of averages and deviations for a specific performance measure
         * 
         */
        static void CreateLatexAvgDevGroupByEnvironment(int measureId, RobominerDataContext dc)
        {
            string performanceMeasure =  dc.PerformanceMeasures.Where(x => x.id == measureId).FirstOrDefault().name;
            List<Tuple<string, List<AveStd>>> results = new List<Tuple<string, List<AveStd>>>();
            foreach (Algorithm alg in dc.Algorithms)
            {
                List<AveStd> r = CreateAvgDevPerAlgorithmId(measureId, alg.id, dc);
                Tuple<string, List<AveStd>> t = new Tuple<string, List<AveStd>>(alg.name,r);
                results.Add(t);
            }
            
            List<string> columnNames = new List<string>();
            columnNames.AddRange( dc.EnvironmentTypes.Select( x => x.name ));

            LatexWriter writer = new LatexWriter(String.Format("Latex_AlgByEnv_{0}.tex",performanceMeasure));
            writer.OutputTable(3,columnNames,results);
        }

        static void Main(string[] args)
        {
            RobominerDataContext dc = new RobominerDataContext("Data Source=DEEPTHOUGHT;Initial Catalog=Experiment;Integrated Security=True");
            CreateFluxVizFile(4, dc);
            CreateMannWhitneyUFile(1, dc);
            //CreateLatexAvgDevGroupByEnvironment(1, dc);
        }

    }
}
