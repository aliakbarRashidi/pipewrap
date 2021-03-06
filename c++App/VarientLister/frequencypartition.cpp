#include <algorithm>
#include <fstream>
#include <list>
#include <cmath>
#include "frequencypartition.h"
#include "main.h"

FrequencyPartition::FrequencyPartition(const string& filename,
                                       const string& numBins)
    : dataFile(filename)
    , numBins(std::stoi(numBins))
    //, totalBaseReads(std::stoi(totalBaseReads))
{
}

void FrequencyPartition::setParmeters(long long tReads,
                                      long long tVarients)
{

    totalReads = tReads;
    //totalPsnps = tVarients -(long long) tReads*(1 - exp(Ten, -avePhred/Ten));
    totalPsnps = tVarients;

    if (0 == totalReads) return;
    if (0 == totalPsnps) totalPsnps = tVarients /2;
    ifstream fin(dataFile.c_str(), ios_base::in);
    //if (fin.is_open())
    if (false)
    {
            list<int> binFreq;
            list<double> binProportion;
            while (!fin.eof())
            {
                int inBin = 0;
                double inProportion = 0.0;

                fin >> inBin >> inProportion;
                binFreq.push_back(inBin);
                binProportion.push_back(inProportion);
            };
            int totalBaseReads = binFreq.size();
            pMatch.push_back(1);
            ratioPartitions.push_back((totalBaseReads*totalReads)/totalPsnps);

            for (int i = 1 ; i<totalBaseReads ; i++ )
            {
                pMatch.push_back((long double)i/(long double)totalBaseReads);
                ratioPartitions.push_back(1);
            }
    }
    else
    {
        //const int NumPartitions = 100;
        pMatch.push_back(1);
        ratioPartitions.push_back((numBins *totalReads)/totalPsnps);

        for (unsigned int i = 1 ; i<numBins ; i++ )
        {
            pMatch.push_back((long double)i/(long double)numBins);
            ratioPartitions.push_back(1);
        }
    }
}

long double FrequencyPartition::getPartionFreq(int par,
                           int coverage,
                           std::vector<unsigned int>& phreds)
{
    long double lpf = logPartionRatio(par, coverage, phreds);
    long double normFactor = logNormlisingFactor(coverage, phreds);
    if (isinf(normFactor))
    {
        return 0.0;
    }
    else
    {
        lpf -= logNormlisingFactor(coverage, phreds);
        return pow(Ten, lpf);
    }
}

long double FrequencyPartition::logPartionRatio(int partition,
                                        int coverage,
                                        std::vector<unsigned int> &phreds)
{
    long double lmatch = ((long double)(coverage-phreds.size()))
                                    *log10(pMatch[partition]);
    long double lvar = 0.0;
    long double denom = 0.0;
    for (unsigned int i = 0 ; i < phreds.size() ; i++)
    {
        lvar += log10(One-pMatch[partition] + phred2prob(phreds[i]));
        denom += log10(1+phred2prob(phreds[i]));
    }
    return lmatch+lvar-denom + log10((long double)ratioPartitions[partition]);
}

long double FrequencyPartition::logNormlisingFactor(int coverage,
                                std::vector<unsigned int> &phreds)
{
    long double norm = 0.0;
    for (unsigned int par = 0 ; par < ratioPartitions.size() ; par++)
    {
        norm += pow(Ten,logPartionRatio(par, coverage, phreds));
    }
    return log10(norm);
}















































