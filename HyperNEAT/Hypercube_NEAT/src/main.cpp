#include "HCUBE_Defines.h"
#include "JGTL_CommandLineParser.h"


#include "HCUBE_ExperimentRun.h"

#include "Experiments/HCUBE_Experiment.h"
//Tal: 3-10 add next line
#include "Experiments/HCUBE_SoftbotsExperiment.h"

//#include "Experiments/HCUBE_FindClusterExperiment.h"
//#include "Experiments/HCUBE_CheckersExperiment.h"

//Tal: removed ONAMA 3-10

#define PRINT_GENCHAMPFITNESSVIAPOSTHOCEVAL (0)

//Tal: next commented 3-10
//#define EPOCHS_PER_PRINT (100000)

//Tal: next function not used? 3-10

void printNetwork(NEAT::FastNetwork<double> &testNetwork) {
    return;

    cout << "Network links:\n";
    cout << testNetwork.getLink("Input0", "Hidden1")->weight << endl;
    cout << testNetwork.getLink("Input0", "Hidden2")->weight << endl;
    cout << endl;

    cout << testNetwork.getLink("Input1", "Hidden1")->weight << endl;
    cout << testNetwork.getLink("Input1", "Hidden2")->weight << endl;
    cout << endl;

    cout << testNetwork.getLink("Input2", "Hidden1")->weight << endl;
    cout << testNetwork.getLink("Input2", "Hidden2")->weight << endl;
    cout << endl;

    cout << testNetwork.getLink("Hidden0", "Output0")->weight << endl;
    cout << testNetwork.getLink("Hidden1", "Output0")->weight << endl;
    cout << testNetwork.getLink("Hidden2", "Output0")->weight << endl;
    cout << endl;

    CREATE_PAUSE("");
}
using namespace HCUBE;

int HyperNEAT_main(int argc, char **argv) {
    CommandLineParser commandLineParser(argc, argv);
    try {
        /*
         * job = 0: no parameter, create random individual
         * job = 1: 1 parameter, create individual
         * job = 2; 2 parameters, create offspring
         */
        int job = 0;

        if (commandLineParser.HasSwitch("-ORG")) {
            cout << "Switch -ORG\n";
            string org1 = commandLineParser.GetSafeArgument("-ORG", 0, "");
            cout << "first=" << org1 << endl;
            string org2 = commandLineParser.GetSafeArgument("-ORG", 1, "");
            cout << "second=" << org2 << endl;
            if ((org1.empty() && org2.empty())) {
                cout << "new individual\n";
            } else if ((!org1.empty() && org2.empty())) {
                job = 1;
                cout << "mutated individual\n";
            } else {
                job = 2;
                cout << "new offspring\n";
            }
        }
        if (commandLineParser.HasSwitch("-I")) //Tal: for parameter[dat] file 3-10
        {
            string paramFileName = commandLineParser.GetSafeArgument("-I", 0, "input.dat");
            ifstream paramFile;
            paramFile.open(paramFileName.c_str());
            if (!paramFile) {
                cout << "The following parameter file does not exist: " << paramFileName << endl;
                exit(3);
            } else
                cout << "Using the following parameter file: " << paramFileName << endl;
        }
        if (commandLineParser.HasSwitch("-O")) {
            string inputFile = commandLineParser.GetSafeArgument("-I", 0, "input.dat"); //ACTION-1: 2 names for the parameter file 
            string outputFilePrefix = commandLineParser.GetSafeArgument("-O", 0, "output.xml").c_str();

            NEAT::Globals::init(inputFile);
            NEAT::Globals::getSingleton()->setOutputFilePrefix(outputFilePrefix); //set the name of the outputPrefixFile
        }


        int experimentType = int(GET_PARAMETER("ExperimentType") + 0.001);

        cout << "Loading Experiment: " << experimentType << endl;
        cout << "With Generations: " << GET_PARAMETER("MaxGenerations") << endl;

        /*
         * BEGIN WEIRD_FILE  can't delete next section, otherwise get weird error message
         */
        ofstream output_file;
        std::ostringstream tmpName;
        tmpName << "Softbots--" << NEAT::Globals::getSingleton()->getOutputFilePrefix() << "---gen-Genchamp-AvgFit.txt";
        string outoutFileName = tmpName.str();
        output_file.open(outoutFileName.c_str(), ios::trunc);
        output_file.close();
        /*
         * END WEIRD_FILE
         */

        HCUBE::ExperimentRun experimentRun;

        string outputFilePrefix = commandLineParser.GetSafeArgument("-O", 0, "output.xml").c_str();
        experimentRun.setupExperiment(experimentType, outputFilePrefix);

        cout << "Experiment set up\n";


        /*
         *
         * insert custom Evolutionary Computing 2014 code here:
         *
         */
        if (job == 0) {
            //create population (size 1 - no, size 1 doesn't work, because the first N individuals might be faulty)
            experimentRun.createPopulation();
            //serialize this population
            experimentRun.start();
        } else if (job == 1) {
            //TODO: read input genotype
            //TODO: mutate genotype
            //TODO: output genotype and voxelyze file
        } else if (job == 2) {
            //TODO: read input genotype 1
            //TODO: read input genotype 2
            //TODO: create offspring
            //TODO: output genotype and voxelyze file
        }


        // experimentRun.createPopulation();
        // experimentRun.setCleanup(false);
        // cout << "Population Created\n";
        // experimentRun.start();

    } catch (string s) {
        cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
        cout << "An exception has occured: " << s << endl;
    } catch (LocatedException e) {
        cout << "CAUGHT Exception: " << e.what() << endl;
    } catch (...) {
        cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
        cout << "An error has occured!\n";
    }

    NEAT::Globals::deinit();

    return 0;
}
//main function

int main(int argc, char **argv) {
    if (argc < 5) {
        cout << "You must pass the parameters and the output file as command parameters!\n";
        cout << "Syntax for passing command-line options to HyperNEAT:[..] are optional\n";
        cout << "./HyperNEAT -I (parameterfile) -O (output/individual-file) [-ORG org1 org2] \n";
        cout << "-ORG org1 org2 - if -ORG or both org1 and org2 are absent new individual is produced \n";
        cout << "-ORG org1 org2 - if -ORG org1 this individual is mutated \n";
        cout << "-ORG org1 org2 - if -ORG org1 org2 than they are considered as parents anf after Xover and mutation they produce one offspring \n";
    } else
        HyperNEAT_main(argc, argv);
}
