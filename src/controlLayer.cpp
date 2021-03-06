/*
 * =====================================================================================
 *
 *       Filename:  controlLayer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/25/2017 09:38:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Baoxing Song (songbx.me), songbaoxing168@163.com
 *
 * =====================================================================================
 */

/*************************************************************************




 ************************************************************************/

#include "controlLayer.h"

std::string softwareName = "gean";

int getGenomeSequence(int argc, char** argv, std::map<std::string, std::string>& parameters){
    std::stringstream usage;
    usage << "Usage: "<<softwareName<<" pseudogeno -r reference -v variants -o output" << std::endl<<
        "Options" << std::endl <<
        " -h         produce help message" << std::endl <<
        " -r FILE    reference genome in fasta format" << std::endl <<
        " -v FILE    variant calling result in vcf/sdi format" << std::endl <<
        " -p STRING  prefix for vcf records" << std::endl <<
        " -o FILE    output pseudo genome in fasta format" << std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-prefix")){
        vcfFix = inputParser.getCmdOption("-prefix");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o")  ){
        std::string reference = inputParser.getCmdOption("-r");
        std::string variants = inputParser.getCmdOption("-v");
        std::string output = inputParser.getCmdOption("-o");
        getPseudoGenomeSequence(reference, variants, output, vcfFix, parameters);
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int getChangedFromReference( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage << "Usage: "<<softwareName<<" lift -v variants -c chromosome -p position " << std::endl<<
        "Options" << std::endl <<
        " -h         produce help message" << std::endl <<
        " -r FILE    reference genome in fasta format" << std::endl <<
        " -v FILE    variant calling result in vcf/sdi format" << std::endl <<
        " -f STRING  prefix for vcf records" << std::endl <<
        " -c STRING  chromosome" << std::endl <<
        " -p INT     the position/coordinate in reference genome" <<std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-f")){
        vcfFix = inputParser.getCmdOption("-f");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-c") && inputParser.cmdOptionExists("-p")  ){
        std::string reference = inputParser.getCmdOption("-r");
        std::string variantsFile = inputParser.getCmdOption("-v");
        std::string chromosome = inputParser.getCmdOption("-c");
        std::string coordinateS = inputParser.getCmdOption("-p");
        int coordinate = std::stoi(coordinateS);
        int liftCoordinate = myGetChangedFromReference( variantsFile, chromosome, coordinate, vcfFix, reference );
        std::cout << chromosome << " " << liftCoordinate << std::endl;
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}


int getReferenceFromChanged( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage << "Usage: "<<softwareName<<" revlift -v variants -c chromosome -p position " << std::endl<<
          "Options" << std::endl <<
          " -h         produce help message" << std::endl <<
          " -r FILE    reference genome in fasta format" << std::endl <<
          " -v FILE    variant calling result in vcf/sdi format" << std::endl <<
          " -f STRING  prefix for vcf records" << std::endl <<
          " -c STRING  chromosome, should be consistent with the chromosome information in sdi file (The coordinate starts from 1)" << std::endl <<
          " -p INT     the position/coordinate in re-sequenced genome" <<std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-f")){
        vcfFix = inputParser.getCmdOption("-f");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-c") && inputParser.cmdOptionExists("-p")  ){
        std::string reference = inputParser.getCmdOption("-r");
        std::string variantsFile = inputParser.getCmdOption("-v");
        std::string chromosome = inputParser.getCmdOption("-c");
        std::string coordinateS = inputParser.getCmdOption("-p");
        int coordinate = std::stoi(coordinateS);
        int liftCoordinate = myGetReferenceFromChanged( variantsFile, chromosome, coordinate, vcfFix, reference);
        std::cout << chromosome << " " << liftCoordinate << std::endl;
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int gffCoordinateLiftOver( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage << "Usage: "<<softwareName<<" liftgff -v variants -i inputGffFile -o outputGffFile " << std::endl<<
        "Options" << std::endl <<
        " -h         produce help message" << std::endl <<
        " -r FILE    reference genome in fasta format" << std::endl <<
        " -v FILE    variant calling result in vcf/sdi format" << std::endl <<
        " -i FILE    the input GFF/GTF file of reference line/accession" << std::endl <<
        " -f STRING  prefix for vcf records" << std::endl <<
        " -o         the output GFF/GTF file of target line/accession" << std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-f")){
        vcfFix = inputParser.getCmdOption("-f");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if(inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-o") ){
        std::string reference = inputParser.getCmdOption("-r");
        std::string variantsFile = inputParser.getCmdOption("-v");
        std::string inputGff = inputParser.getCmdOption("-i");
        std::string outputGff = inputParser.getCmdOption("-o");
        myGffCoordinateLiftOver(variantsFile, inputGff, outputGff, vcfFix, reference);
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int revGffCoordinateLiftOver( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage << "Usage: "<<softwareName<<" revliftgff -v variants -i inputGffFile -o outputGffFile " << std::endl<<
          "Options" << std::endl <<
          " -h         produce help message" << std::endl <<
          " -r FILE    reference genome in fasta format" << std::endl <<
          " -v FILE    variant calling result in vcf/sdi format" << std::endl <<
          " -i FILE    the input GFF/GTF file of non-reference line/accession" << std::endl <<
          " -f STRING  prefix for vcf records" << std::endl <<
          " -o         the output GFF/GTF file of reference line/accession" << std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-f")){
        vcfFix = inputParser.getCmdOption("-f");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if(inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-o") ){
        std::string reference = inputParser.getCmdOption("-r");
        std::string variantsFile = inputParser.getCmdOption("-v");
        std::string inputGff = inputParser.getCmdOption("-i");
        std::string outputGff = inputParser.getCmdOption("-o");
        myRevGffCoordinateLiftOver(variantsFile, inputGff, outputGff, vcfFix, reference);
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int Reanva( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" reanva -i inputGffFile -r inputGenome -a similar segments -s new genome sequence -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   GFF/GTF file" << std::endl <<
          " -r FILE   reference genome sequence" << std::endl <<
          " -v FILE   variant calling result in vcf/sdi format" << std::endl <<
          " -f STRING prefix for vcf records" << std::endl <<
          " -o FILE   output file" << std::endl <<
          " -m INT    minimum intron size" << std::endl << std::endl;

    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") ){
        std::string gffFilePath = inputParser.getCmdOption("-i");
        std::string reference = inputParser.getCmdOption("-r");
        std::string variantsFile = inputParser.getCmdOption("-v");
        std::string outPutFilePath=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        std::string vcfFix = "";
        if( inputParser.cmdOptionExists("-f")){
            vcfFix = inputParser.getCmdOption("-f");
        }
        updateVariants( gffFilePath, reference, variantsFile, vcfFix, minIntron,
                          parameters, outPutFilePath  );

        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int getSequences( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage << "Usage: "<<softwareName<<" gff2seq -i inputGffFile -r inputGenome -p outputProteinSequences -c outputCdsSequences -g outputGenomeSequences " << std::endl<<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   reference genome in GFF/GTF format" << std::endl <<
          " -r FILE   genome sequence in fasta format" << std::endl <<
          " -m INT    minimum intron size for a functional ORF" << std::endl <<
          " -p FILE   output file of protein sequence in fasta format" << std::endl <<
          " -c FILE   output file of CDS (without intron) in fasta format" << std::endl <<
          " -g FILE   output file of CDS (with intron) in fasta frormat" << std::endl << std::endl;
    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-p") && inputParser.cmdOptionExists("-c") && inputParser.cmdOptionExists("-g") ){
        std::string inputGffFile = inputParser.getCmdOption("-i");
        std::string genome = inputParser.getCmdOption("-r");
        std::string outputProteinSequences = inputParser.getCmdOption("-p");
        std::string outputCdsSequences = inputParser.getCmdOption("-c");
        std::string outputGenomeSequences = inputParser.getCmdOption("-g");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }

        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        getSequences( inputGffFile, genome, outputProteinSequences, outputCdsSequences, outputGenomeSequences, parameters, minIntron);
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}


int myPrepareForMsa( int argc, char** argv, std::map<std::string, std::string>& parameters ){

    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" premsa -i inputGffFile -r referenceGenomeSequence -v variants" << std::endl <<
          "Options" << std::endl <<
          " -h         produce help message" << std::endl <<
          " -i FILE    the input GFF/GTF file of reference line/accession" << std::endl <<
          " -r FILE    reference genome" << std::endl <<
          " -v FILE    list of variant calling results files" << std::endl <<
          " -f STRING  prefix for vcf records" << std::endl <<
          " -m INT     minimum intron size" << std::endl <<
          " -t INT     number of threads, default: 4 "<< std::endl <<
          " -w INT     window size, default: 10000 "<< std::endl <<
          " -s INT     window overlap size, default: 500 "<< std::endl <<
          " -p INT     output catch size (default 100)" << std::endl <<
          " -l INT     longest transcript to align. default(50000)"<< std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-f")){
        vcfFix = inputParser.getCmdOption("-f");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
        return 1;
    } else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") ){
        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
        std::string inputGffFile = inputParser.getCmdOption("-i");
        std::string variantsList = inputParser.getCmdOption("-v");

        int threads;
        if( inputParser.cmdOptionExists("-t") ){
            threads = std::stoi( inputParser.getCmdOption("-t") );
        }else{
            threads = 4;
        }
        int lengthThread;
        if( inputParser.cmdOptionExists("-l") ){
            lengthThread = std::stoi( inputParser.getCmdOption("-l") );
        }else{
            lengthThread=50000;
        }
        int msaWindowSize;
        if( inputParser.cmdOptionExists("-w") ){
            msaWindowSize = std::stoi( inputParser.getCmdOption("-w") );
        }else{
            msaWindowSize=10000;
        }
        int msaWindowOverlap;
        if( inputParser.cmdOptionExists("-s") ){
            msaWindowOverlap = std::stoi( inputParser.getCmdOption("-s") );
        }else{
            msaWindowOverlap=500;
        }
        std::map<std::string, std::string> sdiFiles;
        getSdiList(variantsList, sdiFiles);
        bool append = false;

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        int outputPoolSize;
        if( inputParser.cmdOptionExists("-p") ){
            outputPoolSize = std::stoi( inputParser.getCmdOption("-p") );
        }else{
            outputPoolSize=100;
        }

        prepareForMsa( referenceGenomeSequence, inputGffFile, sdiFiles, threads, lengthThread, vcfFix, parameters,
                       append, msaWindowSize, msaWindowOverlap, minIntron, outputPoolSize);
        return 0;
    } else{
        std::cerr << usage.str();
        return 0;
    }
}


int myConstructSdiFromMsa( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" msatosdi -a accessionList -c chromosomeLi -m MSAresultFolder -o outputFolder  -r referenceGenomeSequence -v variants" << std::endl <<
          "Options" << std::endl <<
          " -h         produce help message" << std::endl <<
          " -c FILE    chromosome list" << std::endl <<
          " -m FOLDER  folder of MSA result" << std::endl <<
          " -o FOLDER  output folder" << std::endl <<
          " -r FILE    reference genome in fasta format" << std::endl <<
          " -t INT     number of threads, default: 4 "<< std::endl <<
          " -v FILE    list of variant calling results files" << std::endl <<
          " -f STRING  prefix for vcf records" << std::endl;

    InputParser inputParser (argc, argv);

    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
        return 1;
    } else if( inputParser.cmdOptionExists("-c") && inputParser.cmdOptionExists("-m") && inputParser.cmdOptionExists("-o")
               && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") ){

        std::vector<std::string> chromosomes;
        std::string chromosomeListFile = inputParser.getCmdOption("-c");
        getListFromTextFile(chromosomeListFile, chromosomes);

        std::string folder = inputParser.getCmdOption("-m");
        std::string outputFolder = inputParser.getCmdOption("-o");

        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");

        std::string variantsList = inputParser.getCmdOption("-v");
        std::map<std::string, std::string> sdiFiles;
        getSdiList(variantsList, sdiFiles);

        std::string vcfFix = "";
        if( inputParser.cmdOptionExists("-f")){
            vcfFix = inputParser.getCmdOption("-f");
        }
        int threads;
        if( inputParser.cmdOptionExists("-t") ){
            threads = std::stoi( inputParser.getCmdOption("-t") );
        }else{
            threads = 4;
        }
        constructSdiFromMsa( chromosomes, folder, outputFolder, referenceGenomeSequence, sdiFiles, vcfFix, parameters, threads);
        return 0;
    } else{
        std::cerr << usage.str();
        return 0;
    }
}

int reAnnotationAndExonerateAndNovo( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" annowgr -i inputGffFile -r referenceGenomeSequence -v variants -o outputGffFile" << std::endl <<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   reference GFF/GTF file" << std::endl <<
          " -n FILE   the de novo annotation GFF of the target accession" << std::endl <<
          " -r FILE   reference genome in fasta format" << std::endl <<
          " -v FILE   variant calling result in vcf/sdi format" << std::endl <<
          " -o FILE   the output GFF/GTF file" << std::endl <<
          " -m INT    minimum intron size" << std::endl <<
          " -d        remove reference ORF shift transcripts (default false)" << std::endl <<
          " -f STRING prefix for vcf records" << std::endl <<
          " -t INT    number of threads, default: 4 "<< std::endl <<
          " -l INT    longest transcript to align. default(50000)"<< std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-prefix")){
        vcfFix = inputParser.getCmdOption("-prefix");
    }
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
        return 1;
    } else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-n") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") ){
        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
        std::string inputGffFile = inputParser.getCmdOption("-i");
        std::string novoGffFilePath = inputParser.getCmdOption("-n");
        std::string variants = inputParser.getCmdOption("-v");
        std::string outputGffFile = inputParser.getCmdOption("-o");
        int threads;
        if( inputParser.cmdOptionExists("-t") ){
            threads = std::stoi( inputParser.getCmdOption("-t") );
        }else{
            threads = 4;
        }
        int lengthThreadhold;
        if( inputParser.cmdOptionExists("-l") ){
            lengthThreadhold = std::stoi( inputParser.getCmdOption("-l") );
        }else{
            lengthThreadhold=50000;
        }

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }

        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }

        bool remove_reference_orf_shift;
        if( inputParser.cmdOptionExists("-d") ){
            remove_reference_orf_shift = true;
        }else{
            remove_reference_orf_shift = false;
        }

        myReAnnotationAndExonerateAndNovo( referenceGenomeSequence, inputGffFile, novoGffFilePath,
                                           variants, outputGffFile, threads, lengthThreadhold, vcfFix, parameters, minIntron, remove_reference_orf_shift);
        return 0;
    } else{
        std::cerr << usage.str();
        return 0;
    }
}

int TransGff( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" transgff -i inputGffFile -r inputGenome -a similar segments -s new genome sequence -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   reference GFF/GTF file" << std::endl <<
          " -r FILE   reference genome sequence" << std::endl <<
          " -a FILE   similar segments" << std::endl <<
          " -s FILE   target genome sequence" << std::endl <<
          " -o FILE   output GFF/GTF file" << std::endl <<
          " -n        genome sequence alignment approach"
          "           `sliding` (sliding window) or `band` ( default: sliding)" << std::endl <<
          " -w INT    sequence alignment window width (default: 60)" << std::endl <<
          " -sl       run in slow model (default false)" << std::endl <<
          " -l INT    longest transcript to align. (default: 50000)"<< std::endl <<
          " -m INT    minimum intron size" << std::endl << std::endl;

    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-a") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") ){
        std::string gffFilePath = inputParser.getCmdOption("-i");
        std::string databaseFastaFilePath = inputParser.getCmdOption("-r");
        std::string queryFastaFilePath = inputParser.getCmdOption("-s");
        std::string nucmerFilePath=inputParser.getCmdOption("-a");
        std::string outPutFilePath=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
            if( minIntron < 5 ){
                std::cerr << "the intron size should be 5 at minimum" << std::endl;
                exit(1);
            }
        }else{
            minIntron=5;
        }

        bool slowMode = false;
        if ( inputParser.cmdOptionExists("-sl") ){
            slowMode = true;
        }
        int lengthThreadhold;
        if( inputParser.cmdOptionExists("-l") ){
            lengthThreadhold = std::stoi( inputParser.getCmdOption("-l") );
        }else{
            lengthThreadhold=50000;
        }
        int windowWidth = 60;
        if( inputParser.cmdOptionExists("-w") ){
            windowWidth = std::stoi( inputParser.getCmdOption("-w") );
        }
        int alignmentApproach = 1;
        if( inputParser.cmdOptionExists("-n") ){
            if( inputParser.getCmdOption("-n").compare("sliding") == 0 ){
                alignmentApproach = 0;
            }else if ( inputParser.getCmdOption("-n").compare("band") == 0 ){
                alignmentApproach = 1;
            }else{
                std::cout << "invalid sequence alignment approach parameter" << std::endl;
                std::exit(1024);
            }
        }
        TransferAllExonWithNucmerResult( gffFilePath, databaseFastaFilePath, queryFastaFilePath, nucmerFilePath, parameters, outPutFilePath, minIntron, slowMode, windowWidth, lengthThreadhold, alignmentApproach );
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int spliceAlignmentToGff( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" spltogff -i inputGffFile -r inputGenome -a similar segments -s new genome sequence -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   reference GFF/GTF file" << std::endl <<
          " -r FILE   reference genome sequence" << std::endl <<
          " -a FILE   sam file" << std::endl <<
          " -s FILE   target genome sequence" << std::endl <<
          " -o FILE   output GFF/GTF file" << std::endl <<
          " -g INT    output tag, should be 0, 1 or 2 (default 1)" << std::endl <<
          "           0 prefer to output the ZDP realignment result" << std::endl <<
          "           1 prefer output the standard alignment result" << std::endl <<
          "           2 prefer output the longer result" << std::endl <<
          " -n        genome sequence alignment approach" << std::endl <<
          "           `sliding` (sliding window) or `band` ( default: band)" << std::endl <<
          " -w INT    sequence alignment band/window width (default: 60)" << std::endl <<
          " -l INT    longest transcript to align. default(50000)"<<std::endl <<
          " -m INT    minimum intron size" << std::endl << std::endl;

    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-a") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") ){
        std::string gffFilePath = inputParser.getCmdOption("-i");
        std::string databaseFastaFilePath = inputParser.getCmdOption("-r");
        std::string queryFastaFilePath = inputParser.getCmdOption("-s");
        std::string nucmerFilePath=inputParser.getCmdOption("-a");
        std::string outPutFilePath=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
            if( minIntron < 5 ){
                std::cerr << "the intron size should be 5 at minimum" << std::endl;
                exit(1);
            }
        }else{
            minIntron=5;
        }

        int lengthThreadhold;
        if( inputParser.cmdOptionExists("-l") ){
            lengthThreadhold = std::stoi( inputParser.getCmdOption("-l") );
        }else{
            lengthThreadhold=50000;
        }
        int windowWidth = 60;
        if( inputParser.cmdOptionExists("-w") ){
            windowWidth = std::stoi( inputParser.getCmdOption("-w") );
        }
        int outputTag=1;
        if( inputParser.cmdOptionExists("-g") ){
            outputTag = std::stoi( inputParser.getCmdOption("-g") );
        }
        if( outputTag!=0 && outputTag!=1 && outputTag!=2 ){
            std::cerr << "output tag should be 0, 1 or 2" << std::endl;
            return 1;
        }

        int alignmentApproach = 1;
        if( inputParser.cmdOptionExists("-n") ){
            if( inputParser.getCmdOption("-n").compare("sliding") == 0 ){
                alignmentApproach = 0;
            }else if ( inputParser.getCmdOption("-n").compare("band") == 0 ){
                alignmentApproach = 1;
            }else{
                std::cout << "invalid sequence alignment approach parameter" << std::endl;
                std::exit(1024);
            }
        }

        // outputTag, 0 prefer to output the ZDP realignment result
        // 1 prefer output the standard alignment result
        // 2 prefer output the longer result
        TransferAllExonWithSpliceAlignmentResult( gffFilePath, databaseFastaFilePath, queryFastaFilePath, nucmerFilePath,
                parameters, outPutFilePath, minIntron, windowWidth, lengthThreadhold, outputTag, alignmentApproach);
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int PurifyGff( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" purifygff -i inputGffFile -s inputGenome -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   GFF/GTF file" << std::endl <<
          " -s FILE   target genome sequence" << std::endl <<
          " -o FILE   output GFF/GTF file" << std::endl <<
          " -x INT    minimum gene length" << std::endl <<
          " -m INT    minimum intron size" << std::endl << std::endl;

    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") ){
        std::string gffFilePath = inputParser.getCmdOption("-i");
        std::string queryFastaFilePath = inputParser.getCmdOption("-s");
        std::string outPutFilePath=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        int minGene;
        if( inputParser.cmdOptionExists("-x") ){
            minGene = std::stoi( inputParser.getCmdOption("-x") );
        }else{
            minGene=9;
        }
        if( minGene < 9 ){
            std::cerr << "the gene size should be 9 at minimum" << std::endl;
            exit(1);
        }

        denovoGffLiftOverToUniqGffOutput(gffFilePath, outPutFilePath, queryFastaFilePath, minIntron, minGene, parameters);

        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int outPutORFConserveredTranscripts( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" orf -i inputGffFile -s inputGenome -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   GFF/GTF file" << std::endl <<
          " -s FILE   genome sequence" << std::endl <<
          " -o FILE   output GFF/GTF file" << std::endl <<
          " -m INT    minimum intron size" << std::endl << std::endl;

    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") ){
        std::string gffFilePath = inputParser.getCmdOption("-i");
        std::string genomeFile = inputParser.getCmdOption("-s");
        std::string outPutFilePath=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }

        outPutORFConserveredTranscripts( genomeFile, gffFilePath,
                                         outPutFilePath ,  minIntron,
                                         parameters );

        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int syntenicSingleCopy( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" sinsyn -i referenceGffFile -s inputGenome -a inputGffFile -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h          produce help message" << std::endl <<
          " -i FILE     reference GFF/GTF file" << std::endl <<
          //" -r FILE     reference genome sequence" << std::endl <<
          " -s FILE     target genome sequence" << std::endl <<
          " -a FILE     target GFF/GTF file" << std::endl <<
          " -o FILE     output GFF file" << std::endl << std::endl <<
          " ADVANCED PARAMETERS" << std::endl <<
          " -m INT      minimum intron size" << std::endl <<
          " -d          keep tandem duplication (default false) " << std::endl <<
          " -on         only output syntenic bolck genes (default false) " << std::endl <<
          " -rt DOUBLE  reverse syntenic block threads hold (12.0)" << std::endl <<
          " -rs DOUBLE  reverse syntenic match score (3.0)" << std::endl <<
          " -rp DOUBLE  reverse syntenic mismatch penalty (-4.0)" << std::endl <<
          " -ss DOUBLE  score for gene located in syntenic region (1.0)" << std::endl <<
          " -so DOUBLE  score for gene with ORF conserved (1.5)" << std::endl <<
          " -dl DOUBLE  length ratio to drop a gene transformation (0.2)" << std::endl;
    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") && inputParser.cmdOptionExists("-a") ){

        std::string referenceGffFile = inputParser.getCmdOption("-i");
//        std::string databaseFastaFilePath = inputParser.getCmdOption("-r");
        std::string queryGenomeFile = inputParser.getCmdOption("-s");
        std::string queryNewGffFile=inputParser.getCmdOption("-a");
        std::string outputGffFile=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        int minGene;
        if( inputParser.cmdOptionExists("-x") ){
            minGene = std::stoi( inputParser.getCmdOption("-x") );
        }else{
            minGene=9;
        }
        if( minGene < 9 ){
            std::cerr << "the gene size should be 9 at minimum" << std::endl;
            exit(1);
        }

        bool keepTandemDuplication=false;
        if( inputParser.cmdOptionExists("-d")){
            keepTandemDuplication = true;
        }
        double scoreThreshold=12;
        if( inputParser.cmdOptionExists("-rt") ){
            scoreThreshold = std::stod( inputParser.getCmdOption("-rt") );
        }
        double score = 3.0;
        if( inputParser.cmdOptionExists("-rs") ){
            score = std::stod( inputParser.getCmdOption("-rs") );
        }
        double penalty = -4.0;
        if( inputParser.cmdOptionExists("-rp") ){
            penalty = std::stod( inputParser.getCmdOption("-rp") );
        }

        double syntenicScore=1.0;
        if( inputParser.cmdOptionExists("-ss") ){
            syntenicScore = std::stod( inputParser.getCmdOption("-ss") );
        }

        double orfScore=1.5;
        if( inputParser.cmdOptionExists("-so") ){
            orfScore = std::stod( inputParser.getCmdOption("-so") );
        }
        double dropLengthThredshold=0.2;
        if( inputParser.cmdOptionExists("-dl") ){
            dropLengthThredshold = std::stod( inputParser.getCmdOption("-dl") );
        }
        bool onlySyntenic = false;
        if( inputParser.cmdOptionExists("-on") ){
            onlySyntenic=true;
        }
        generateLongestOutput( referenceGffFile,  queryNewGffFile, queryGenomeFile, outputGffFile,  minIntron,
                               score, penalty,  scoreThreshold,  keepTandemDuplication, parameters, syntenicScore,
                               orfScore, dropLengthThredshold, onlySyntenic);

        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

int syntenicSingleCopy2( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" sinsyn2 -i referenceGffFile -s inputGenome -a inputGffFile -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h          produce help message" << std::endl <<
          " -i FILE     reference GFF/GTF file" << std::endl <<
          //" -r FILE     reference genome sequence" << std::endl <<
          " -s FILE     target genome sequence" << std::endl <<
          " -a FILE     target GFF/GTF file" << std::endl <<
          " -o FILE     output GFF file" << std::endl << std::endl <<
          " ADVANCED PARAMETERS" << std::endl <<
          " -m INT      minimum intron size" << std::endl <<
          " -d          keep tandem duplication (default false) " << std::endl <<
          " -on         only output syntenic bolck genes (default false) " << std::endl <<
          " -r          whether sort output with coordinate (default: false, only make sense when -on set as true)"<<std::endl <<
          " -md INT     maximum distance of syntenic block genes (default 20) " << std::endl <<
          " -ms DOUBLE  minimum syntenic block score (default 6.0) " << std::endl <<
          " -op DOUBLE  open gap penalty (default -0.1) " << std::endl <<
          " -ep DOUBLE  extend gap penalty (default -0.05) " << std::endl <<
          " -ss DOUBLE  score for gene located in syntenic region (1.0)" << std::endl <<
          " -so DOUBLE  score for gene with ORF conserved (1.5)" << std::endl <<
          " -dl DOUBLE  length ratio to drop a gene transformation (0.2)" << std::endl;
    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") && inputParser.cmdOptionExists("-a") ){

        std::string referenceGffFile = inputParser.getCmdOption("-i");
//        std::string databaseFastaFilePath = inputParser.getCmdOption("-r");
        std::string queryGenomeFile = inputParser.getCmdOption("-s");
        std::string queryNewGffFile=inputParser.getCmdOption("-a");
        std::string outputGffFile=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        int minGene;
        if( inputParser.cmdOptionExists("-x") ){
            minGene = std::stoi( inputParser.getCmdOption("-x") );
        }else{
            minGene=9;
        }
        if( minGene < 9 ){
            std::cerr << "the gene size should be 9 at minimum" << std::endl;
            exit(1);
        }
        bool keepTandemDuplication=false;
        if( inputParser.cmdOptionExists("-d")){
            keepTandemDuplication = true;
        }
        double syntenicScore=1.0;
        if( inputParser.cmdOptionExists("-ss") ){
            syntenicScore = std::stod( inputParser.getCmdOption("-ss") );
        }

        double orfScore=1.5;
        if( inputParser.cmdOptionExists("-so") ){
            orfScore = std::stod( inputParser.getCmdOption("-so") );
        }
        double dropLengthThredshold=0.2;
        if( inputParser.cmdOptionExists("-dl") ){
            dropLengthThredshold = std::stod( inputParser.getCmdOption("-dl") );
        }

        int MAX_DIST_BETWEEN_MATCHES=20;
        if( inputParser.cmdOptionExists("-md") ){
            MAX_DIST_BETWEEN_MATCHES = std::stoi( inputParser.getCmdOption("-md") );
        }
        int BP_GAP_SIZE=1; // this parameter maybe is not necessary

        double INDEL_SCORE=-0.05;
        if( inputParser.cmdOptionExists("-op") ){
            INDEL_SCORE = std::stod( inputParser.getCmdOption("-op") );
        }
        double GAP_OPEN_PENALTY=-0.1;
        if( inputParser.cmdOptionExists("-ep") ){
            GAP_OPEN_PENALTY = std::stod( inputParser.getCmdOption("-ep") );
        }
        double MIN_ALIGNMENT_SCORE = 6.0;
        if( inputParser.cmdOptionExists("-ms") ){
            MIN_ALIGNMENT_SCORE = std::stod( inputParser.getCmdOption("-ms") );
        }
        bool onlySyntenic = false;
        if( inputParser.cmdOptionExists("-on") ){
            onlySyntenic=true;
        }
        bool  sortOutPutGffBycoordinate=false;
        if( inputParser.cmdOptionExists("-r") ){
            sortOutPutGffBycoordinate=true;
        }
        generateDagChainerOutput( referenceGffFile, queryNewGffFile, queryGenomeFile, outputGffFile, minIntron, keepTandemDuplication,
                                  parameters, syntenicScore, orfScore, dropLengthThredshold, MAX_DIST_BETWEEN_MATCHES /*max gap in the term of number of genes*/,
                                  BP_GAP_SIZE, INDEL_SCORE, GAP_OPEN_PENALTY, MIN_ALIGNMENT_SCORE, onlySyntenic, sortOutPutGffBycoordinate);



        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}


int syntenicSingleCopy3( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" quotasyn -i referenceGffFile -s inputGenome -a inputGffFile -o output GFF/GTF file " << std::endl<<
          "Options" << std::endl <<
          " -h          produce help message" << std::endl <<
          " -i  FILE    reference GFF/GTF file" << std::endl <<
          " -s  FILE    target genome sequence" << std::endl <<
          " -a  FILE    target GFF/GTF file" << std::endl <<
          " -o  FILE    output GFF file" << std::endl << std::endl <<
          " ADVANCED PARAMETERS" << std::endl <<
          " -m  INT     minimum intron size" << std::endl <<
          " -mq INT     maximum gene copies in the query " << std::endl <<
          " -d          keep tandem duplication (default false) " << std::endl <<
          " -on         only output syntenic bolck genes (default false) " << std::endl <<
          " -r          whether sort output with coordinate (default: false, only make sense when -on set as true)"<<std::endl <<
          " -md INT     maximum distance of syntenic block genes (default 20) " << std::endl <<
          " -ms DOUBLE  minimum syntenic block score (default 6.0) " << std::endl <<
          " -op DOUBLE  open gap penalty (default -0.1) " << std::endl <<
          " -ep DOUBLE  extend gap penalty (default -0.05) " << std::endl <<
          " -ss DOUBLE  score for gene located in syntenic region (1.0)" << std::endl <<
          " -so DOUBLE  score for gene with ORF conserved (1.5)" << std::endl <<
          " -dl DOUBLE  length ratio to drop a gene transformation (0.2)" << std::endl;
    InputParser inputParser (argc, argv);
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
    }else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-s") && inputParser.cmdOptionExists("-o") && inputParser.cmdOptionExists("-a") ){

        std::string referenceGffFile = inputParser.getCmdOption("-i");
        std::string queryGenomeFile = inputParser.getCmdOption("-s");
        std::string queryNewGffFile=inputParser.getCmdOption("-a");
        std::string outputGffFile=inputParser.getCmdOption("-o");

        int minIntron;
        if( inputParser.cmdOptionExists("-m") ){
            minIntron = std::stoi( inputParser.getCmdOption("-m") );
        }else{
            minIntron=5;
        }
        if( minIntron < 5 ){
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        int minGene;
        if( inputParser.cmdOptionExists("-x") ){
            minGene = std::stoi( inputParser.getCmdOption("-x") );
        }else{
            minGene=9;
        }
        if( minGene < 9 ){
            std::cerr << "the gene size should be 9 at minimum" << std::endl;
            exit(1);
        }
        bool keepTandemDuplication=false;
        if( inputParser.cmdOptionExists("-d")){
            keepTandemDuplication = true;
        }
        double syntenicScore=1.0;
        if( inputParser.cmdOptionExists("-ss") ){
            syntenicScore = std::stod( inputParser.getCmdOption("-ss") );
        }

        double orfScore=1.5;
        if( inputParser.cmdOptionExists("-so") ){
            orfScore = std::stod( inputParser.getCmdOption("-so") );
        }
        double dropLengthThredshold=0.2;
        if( inputParser.cmdOptionExists("-dl") ){
            dropLengthThredshold = std::stod( inputParser.getCmdOption("-dl") );
        }

        double INDEL_SCORE=-0.05;
        if( inputParser.cmdOptionExists("-op") ){
            INDEL_SCORE = std::stod( inputParser.getCmdOption("-op") );
        }
        double GAP_OPEN_PENALTY=-0.1;
        if( inputParser.cmdOptionExists("-ep") ){
            GAP_OPEN_PENALTY = std::stod( inputParser.getCmdOption("-ep") );
        }
        double MIN_ALIGNMENT_SCORE = 6;
        if( inputParser.cmdOptionExists("-ms") ){
            MIN_ALIGNMENT_SCORE = std::stod( inputParser.getCmdOption("-ms") );
        }
        int refMaximumTimes=1;

        int queryMaximumTimes=1;
        if( inputParser.cmdOptionExists("-mq") ){
            queryMaximumTimes = std::stoi( inputParser.getCmdOption("-mq") );
        }
        bool onlySyntenic = false;
        if( inputParser.cmdOptionExists("-on") ){
            onlySyntenic=true;
        }
        bool  sortOutPutGffBycoordinate=false;
        if( inputParser.cmdOptionExists("-r") ){
            sortOutPutGffBycoordinate=true;
        }
        int MAX_DIST_BETWEEN_MATCHES=20;
        if( inputParser.cmdOptionExists("-md") ){
            MAX_DIST_BETWEEN_MATCHES = std::stoi( inputParser.getCmdOption("-md") );
        }
        generateLongestQuotaOutput( referenceGffFile, queryNewGffFile, queryGenomeFile, outputGffFile, minIntron, keepTandemDuplication,
                                    parameters, syntenicScore, orfScore, dropLengthThredshold, INDEL_SCORE, GAP_OPEN_PENALTY, MIN_ALIGNMENT_SCORE, MAX_DIST_BETWEEN_MATCHES,
                                    refMaximumTimes, queryMaximumTimes, onlySyntenic, sortOutPutGffBycoordinate );

        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}


int DenoveAssemblyVariantCalling( int argc, char** argv, std::map<std::string, std::string>& parameters ) {
    std::stringstream usage;
    usage << "Usage: " << softwareName
          << " varcall -i refGffFile -r refGenome  -t targetGff -s targetGenome -o output GFF/GTF file " << std::endl <<
          "Options" << std::endl <<
          " -h        produce help message" << std::endl <<
          " -i FILE   reference GFF/GTF file" << std::endl <<
          " -r FILE   reference genome sequence" << std::endl <<
          " -t FILE   target GFF/GTF file" << std::endl <<
          " -s FILE   target genome sequence" << std::endl <<
          " -o FILE   output file" << std::endl <<
          " -x INT    minimum gene length" << std::endl <<
          " -w INT    sequence alignment window width (default: 60)" << std::endl <<
          " -m INT    minimum intron size" << std::endl << std::endl;

    InputParser inputParser(argc, argv);
    if (inputParser.cmdOptionExists("-h") || inputParser.cmdOptionExists("--help")) {
        std::cerr << usage.str();
    } else if (inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") &&
               inputParser.cmdOptionExists("-t") && inputParser.cmdOptionExists("-s") &&
               inputParser.cmdOptionExists("-o")) {
        std::string refGffFilePath = inputParser.getCmdOption("-i");
        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
        std::string targetGffFilePath = inputParser.getCmdOption("-t");
        std::string targetGenomeSequence = inputParser.getCmdOption("-s");
        std::string outPutFilePath = inputParser.getCmdOption("-o");

        int minIntron;
        if (inputParser.cmdOptionExists("-m")) {
            minIntron = std::stoi(inputParser.getCmdOption("-m"));
        } else {
            minIntron = 5;
        }
        if (minIntron < 5) {
            std::cerr << "the intron size should be 5 at minimum" << std::endl;
            exit(1);
        }
        int minGene;
        if( inputParser.cmdOptionExists("-x") ){
            minGene = std::stoi( inputParser.getCmdOption("-x") );
        }else{
            minGene=9;
        }
        if( minGene < 9 ){
            std::cerr << "the gene size should be 9 at minimum" << std::endl;
            exit(1);
        }
        size_t widownWidth = 60;
        if( inputParser.cmdOptionExists("-w") ){
            widownWidth = std::stoi( inputParser.getCmdOption("-w") );
        }
        deNovoGenomeVariantCalling(refGffFilePath, referenceGenomeSequence, targetGffFilePath, targetGenomeSequence,
                                   minIntron, minGene, parameters, widownWidth, outPutFilePath);
        return 0;
    }else{
        std::cerr << usage.str();
    }
    return 0;
}

//
//int annotationLiftOver( int argc, char** argv ){
//    std::stringstream usage;
//    usage <<  "Usage:    * annotationLiftOver -i inputGffFile -r referenceGenomeSequence -v variants -o outputGffFile" << std::endl <<
//        "   -h        produce help message" << std::endl <<
//        "   -i        the input GFF/GTF file of reference line/accession" << std::endl <<
//        "   -r        reference genome in fasta format" << std::endl <<
//        "   -v        variant calling result in vcf/sdi format" << std::endl <<
//        "   -o        the output GFF/GTF file of target line/accession" << std::endl <<
//        "   -prefix   prefix for fasta records" << std::endl <<
//        "   -g        default: (.+?)\\\\.   it works for parese the TAIR10 annotation" << std::endl <<
//        "             regex to parser the parent gene id from transcript id"  << std::endl <<
//        "   -t        (int) number of threads, default: 4 "<< std::endl <<
//        "   -l        (int) if sequence is longer than this threshold would be aligned, for RAM and time saving porpose  "<< std::endl;
//    InputParser inputParser (argc, argv);
//    std::string vcfFix = "";
//    if( inputParser.cmdOptionExists("-prefix")){
//        vcfFix = inputParser.getCmdOption("-prefix");
//    }
//    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
//        std::cerr << usage.str();
//        return 1;
//    } else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") ){
//        std::string inputGffFile = inputParser.getCmdOption("-i");
//        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
//        std::string variants = inputParser.getCmdOption("-v");
//        std::string outputGffFile = inputParser.getCmdOption("-o");
//        std::string regexG;
//        if( inputParser.cmdOptionExists("-g") ){
//            regexG = inputParser.getCmdOption("-g");
//        }else{
//            regexG = "(.+?)\\.";
//        }
//        int threads;
//        if( inputParser.cmdOptionExists("-t") ){
//            threads = std::stoi( inputParser.getCmdOption("-t") );
//        }else{
//            threads = 4;
//        }
//        int lengthThread;
//        if( inputParser.cmdOptionExists("-l") ){
//            lengthThread = std::stoi( inputParser.getCmdOption("-l") );
//        }else{
//            lengthThread=100000;
//        }
//        myReAnnotationLiftoverSingleLine( referenceGenomeSequence, inputGffFile, variants,
//                                          outputGffFile, threads, regexG, lengthThread, vcfFix );
//        return 0;
//    } else{
//        std::cerr << usage.str();
//        return 1;
//    }
//    return 1;
//}
//
//int annotationLiftOverAndOrth( int argc, char** argv ){
//    std::stringstream usage;
//    usage <<  "Usage:    * annotationLiftOverAndOrth -i inputGffFile -r referenceGenomeSequence -v variants -o outputGffFile" << std::endl <<
//          "   -h        produce help message" << std::endl <<
//          "   -i        the input GFF/GTF file of reference line/accession" << std::endl <<
//          "   -r        reference genome in fasta format" << std::endl <<
//          "   -v        variant calling result in vcf/sdi format" << std::endl <<
//          "   -o        the output GFF/GTF file of target line/accession" << std::endl <<
//          "   -x        default: ([\\s\\S]*)Parent=([\\s\\S]*?)[;,][\\s\\S]*$  it works for parese the TAIR10 annotation" << std::endl<<
//          "                 regex to parser the structure of CDS elements and parent transcript in GFF/GTF file" << std::endl <<
//          "   -prefix   prefix for fasta records" << std::endl <<
//          "   -g        default: (.+?)\\\\.   it works for parese the TAIR10 annotation" << std::endl <<
//          "             regex to parser the parent gene id from transcript id"  << std::endl <<
//          "   -t        (int) number of threads, default: 4 "<< std::endl <<
//          "   -l        (int) if sequence is longer than this threshold would be aligned, for RAM and time saving porpose  "<< std::endl;
//    InputParser inputParser (argc, argv);
//    std::string vcfFix = "";
//    if( inputParser.cmdOptionExists("-prefix")){
//        vcfFix = inputParser.getCmdOption("-prefix");
//    }
//    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
//        std::cerr << usage.str();
//        return 1;
//    } else if( inputParser.cmdOptionExists("-i") && inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") ){
//        std::string inputGffFile = inputParser.getCmdOption("-i");
//        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
//        std::string variants = inputParser.getCmdOption("-v");
//        std::string outputGffFile = inputParser.getCmdOption("-o");
//        std::string regex;
//        if( inputParser.cmdOptionExists("-x")  ){
//            regex = inputParser.getCmdOption("-x");
//        }else{
//            regex = "([\\s\\S]*)Parent=([\\s\\S]*?)[;,][\\s\\S]*$";
//        }
//        std::string regexG;
//        if( inputParser.cmdOptionExists("-g") ){
//            regexG = inputParser.getCmdOption("-g");
//        }else{
//            regexG = "(.+?)\\.";
//        }
//        int threads;
//        if( inputParser.cmdOptionExists("-t") ){
//            threads = std::stoi( inputParser.getCmdOption("-t") );
//        }else{
//            threads = 4;
//        }
//        int lengthThread;
//        if( inputParser.cmdOptionExists("-l") ){
//            lengthThread = std::stoi( inputParser.getCmdOption("-l") );
//        }else{
//            lengthThread=30000;
//        }
//        myReAnnotationLiftoverAndOrthologous( referenceGenomeSequence, inputGffFile, variants,
//                                          outputGffFile, regex, threads, regexG, lengthThread, vcfFix);
//        return 0;
//    } else{
//        std::cerr << usage.str();
//        return 1;
//    }
//    return 1;
//}


int myCountNumberOfTwoneighborSNP( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" countNumberOfTwoneighborSNP -v variants" << std::endl <<
        "Options" << std::endl <<
        "   -h        produce help message" << std::endl <<
        "   -r        (string) reference genome in fasta format" << std::endl <<
        "   -v        variant calling result in vcf/sdi format" << std::endl <<
        "   -l        (int) number of bases in which range there should be no variants for double SNP" << std::endl <<
        "   -o        prefix of output file" <<std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
        return 1;
    } else if(inputParser.cmdOptionExists("-r") &&  inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") ) {
        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
        std::string variants = inputParser.getCmdOption("-v");
        std::string outputPrefix = inputParser.getCmdOption("-o");
        int rangeLength;
        if( inputParser.cmdOptionExists("-l") ){
            rangeLength = std::stoi( inputParser.getCmdOption("-l") );
        }else{
            rangeLength = 3;
        }
        countNumberOfTwoneighborSNP(variants, outputPrefix, rangeLength, vcfFix, parameters, referenceGenomeSequence);
        return 0;
    }else {
        std::cerr << usage.str();
        return 0;
    }
}


int mycountNumberSNPAndIndel( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" countNumberSNPAndIndel -v variants" << std::endl <<
          "Options" << std::endl <<
          "   -h        produce help message" << std::endl <<
          "   -r        (string) reference genome in fasta format" << std::endl <<
          "   -v        variant calling result in vcf/sdi format" << std::endl <<
          "   -l        (int) number of bases in which range there should be no INDEL" << std::endl <<
          "   -o        prefix of output file" <<std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if(inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
        return 1;
    } else if( inputParser.cmdOptionExists("-r") &&  inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") ) {
        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
        std::string variants = inputParser.getCmdOption("-v");
        std::string outputPrefix = inputParser.getCmdOption("-o");
        int rangeLength;
        if( inputParser.cmdOptionExists("-l") ){
            rangeLength = std::stoi( inputParser.getCmdOption("-l") );
        }else{
            rangeLength = 3;
        }
        countNumberSNPAndIndel(variants, outputPrefix, rangeLength, vcfFix, parameters, referenceGenomeSequence);
        return 0;
    }else {
        std::cerr << usage.str();
        return 0;
    }
}

int myGenerateRandomSdi( int argc, char** argv, std::map<std::string, std::string>& parameters ){
    std::stringstream usage;
    usage <<  "Usage: "<<softwareName<<" generateRandomSdi -v variants" << std::endl <<
          "Options" << std::endl <<
          "   -h        produce help message" << std::endl <<
          "   -r        (string) reference genome in fasta format" << std::endl <<
          "   -v        variant calling result in vcf/sdi format" << std::endl <<
          "   -o        prefix of output file" <<std::endl;
    InputParser inputParser (argc, argv);
    std::string vcfFix = "";
    if( inputParser.cmdOptionExists("-h") ||inputParser.cmdOptionExists("--help")  ){
        std::cerr << usage.str();
        return 1;
    } else if( inputParser.cmdOptionExists("-r") && inputParser.cmdOptionExists("-v") && inputParser.cmdOptionExists("-o") && inputParser.cmdOptionExists("-r") ) {
        std::string referenceGenomeSequence = inputParser.getCmdOption("-r");
        std::string variants = inputParser.getCmdOption("-v");
        std::string outputPrefix = inputParser.getCmdOption("-o");
        generateRandomSdi(variants, outputPrefix, vcfFix, referenceGenomeSequence);
        return 0;
    }else {
        std::cerr << usage.str();
        return 0;
    }
}
