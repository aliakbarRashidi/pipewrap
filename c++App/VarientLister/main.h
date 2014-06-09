#ifndef MAIN_H
#define MAIN_H
#include <QCoreApplication>
#include <QCommandLineParser>
#include <fstream>
#include <string>

using namespace std;
static const long double Ten = 10.0;
static const long double One = 1.0;
static const long double Zero = 0.0;

void configCommandLineParser(QCommandLineParser& parser);
void runProgram(QCommandLineParser& parser);
bool compareBases(char c1, char c2);
long double phred2prob(int phred);

//http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
string getFileContents(const char *filename);

int n_C_r(int n,int r);
int factorial(int N);
long double log_n_C_r(unsigned int n, unsigned int r);
long double log_fac(unsigned int N);
long double log_sfac(unsigned int N, unsigned int NpK);

//command line options
//input Bam file
const QCommandLineOption bamFilename(
            QStringList() << "i" << "in-Bam",
            "Input BAM file to be searched for pSNPs",
            "filename",
            "stampy_sorted.bam");

//input template fasta file
const QCommandLineOption templateFilename(
            QStringList() << "t" << "template",
           "Input template fasta file",
           "filename",
            "Sc8915bp_cs.fasta");
const QCommandLineOption freqPartitionFilename(
            QStringList() << "q" << "frequency",
           "File with freqyency of pSNP bins",
           "filename",
            "");
//per read output filename
const QCommandLineOption readCsvOutfile(
            QStringList() << "r" << "readout",
            "Output filename for Read centric output data",
            "filename",
            "read_data.csv");
//per loci output filename
const QCommandLineOption lociCsvOutfile(
            QStringList() << "l" << "lociout",
            "Output filename for loci centric output data",
            "filename",
            "loci_data.csv");
const QCommandLineOption errorThreshold(
            QStringList() << "e" << "error-threshold",
            "Ingnore a read if more than percentage of reads are errors",
            "percentage",
            "30");
//output p-values
const QCommandLineOption fisherFilename(
            QStringList() << "f" << "fisher",
            "Output filename for fisher exact test p_values",
            "filename",
            "fisher.pvalues.csv");
const QCommandLineOption bionomialFilename(
            QStringList() << "b" << "bionomial",
            "Output filename for bionomial p_values",
            "filename",
            "bionomial.pvalues.csv");
const QCommandLineOption poissonFilename(
            QStringList() << "p" << "poisson",
            "Output filename for poission p_values",
            "filename",
            "poission.pvalues.csv");
const QCommandLineOption poissonBinomialFilename(
            QStringList() << "n" << "poisson-bionomial",
            "Output filename for poission bionomial p_values",
            "filename",
            "poissionBiomomial.pvalues.csv");
const QCommandLineOption knownFrequencyFilename(
            QStringList() << "k" << "known-frequency",
            "Output filename for known freqyency p_values",
            "filename",
            "knwonFrequency.pvalues.csv");



























#endif // MAIN_H
