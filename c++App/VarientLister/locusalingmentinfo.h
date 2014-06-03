#ifndef LOCUSINFO_H
#define LOCUSINFO_H
#include <vector>
#include <string>

using namespace std;

//Adenine
//Cytosine
//Guanine
//Thymine

class LocusInfo
{
public:
    LocusInfo(char newbase);
    void inc_calls(char base, int phred);
    void populate();

    char getBase() const;
    int getCoverage() const;
    int getAvePhred() const;
    long double getPVBionomial() const;
    long double getPVPoission() const;
    long double getPVBionomialPiossion() const;
    long double getPVFisher() const;
    char bestbaseEx();
    int countBestEx();
private:



    char base;
    int coverage = 0;
    int ave_phred = 0;
    int total_phred = 0;
    int a_calls = 0;
    int c_calls = 0;
    int g_calls = 0;
    int t_calls = 0;
    int bad_calls = 0;
    //std::vector<unsigned int> base_qualities;
    //std::vector<char> base_calls;
    //std::vector<string> read_id;

    long double p_value_bionomial = 0;
    long double p_value_poission = 0;
    long double p_value_b_poisson = 0;
    long double p_value_fisher = 0;

    //std::vector<unsigned int> DeletionQV;
    //std::vector<unsigned int> InsertionQV;
    //std::vector<unsigned int> MergeQV;
    //std::vector<unsigned int> SubstitutionQV;
};

static long double pBionomial(unsigned int N,
                              unsigned int K, 
                              unsigned int ave_phred);
static long double pPoisson(unsigned int N, 
                            unsigned int K, 
                            unsigned int ave_phred);
static long double pFisher(unsigned int N,
                            unsigned int K,
                            unsigned int ave_phred);
#endif // LOCUSINFO_H
