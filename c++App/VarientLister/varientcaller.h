#ifndef VARIENTCALLER_H
#define VARIENTCALLER_H
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <api/BamReader.h>
#include <api/BamAlignment.h>

using namespace std;
using namespace BamTools;

class VarientCaller
{
public:
    VarientCaller(const string& in_file,
                  const string& t_filename,                  
                  int ethreshold,
                  ostream &out_stream = cout);
    void callVarants();

private:
    void basesFromFasta();
    char visBase(char bamChar);
    std::vector<unsigned int> getCoverage();
    double pvalueBionomial(int N, int K, double p);
    double pvaluePoisson(int N, int K, double p);
    double pvalueBionomialPoisson(int t_pos, int N, int K);

    BamReader bam_reader;
    string t;
    ostream& out;
    int error_threshold;
    std::vector<unsigned int> coverage;
    std::vector<unsigned int> ave_quality;

};

#endif // VARIENTCALLER_H