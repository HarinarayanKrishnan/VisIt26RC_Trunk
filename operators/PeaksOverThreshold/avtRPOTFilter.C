#include <avtParallel.h>
#include <avtRPOTFilter.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <limits>
#include <vtkRInterface.h>
#include <vtkRectilinearGrid.h>
#include <DebugStream.h>

#include <avtCallback.h>
#include <avtDatabase.h>
#include <avtDatabaseMetaData.h>
#include <InvalidFilesException.h>

#ifdef PARALLEL
  #include <mpi.h>
#endif

using namespace std;

// ****************************************************************************
// Method:  avtRPOTFilter::avtRPOTFilter
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

avtRPOTFilter::avtRPOTFilter()
{
    initialized = false;
    outDS = NULL;
    numTuples = 0;
    nodeCenteredData = false;
    numYears = 0;
    numBins = 0;
}

// ****************************************************************************
// Method:  avtRPOTFilter::~avtRPOTFilter
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

avtRPOTFilter::~avtRPOTFilter()
{
}

// ****************************************************************************
// Method:  avtRPOTFilter::FilterSupportsTimeParallelization
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

bool
avtRPOTFilter::FilterSupportsTimeParallelization()
{
    return true;
}

// ****************************************************************************
// Method:  avtRPOTFilter::DataCanBeParallelizedOverTime
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

bool
avtRPOTFilter::DataCanBeParallelizedOverTime()
{
    return true;
}

// ****************************************************************************
// Method:  avtRPOTFilter::GetMonthFromDay
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

int
avtRPOTFilter::GetMonthFromDay(int t)
{
    int year = GetYearFromDay(t);
    int dayMonth[12] = {31,59,90,120,151,181,213,244,274,304,334,365}; //no leap year
    int dayInYear = t % 365;

    int month = -1;
    for (int i = 0; i < 12; i++)
        if (dayInYear <= dayMonth[i])
            return i;
    
    EXCEPTION1(ImproperUseException, "Date out of range.");
}

// ****************************************************************************
// Method:  avtRPOTFilter::GetYearFromDay
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

int
avtRPOTFilter::GetYearFromDay(int t)
{
    return t/365;
}

// ****************************************************************************
// Method:  avtRPOTFilter::GetSeasonFromDay
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

int
avtRPOTFilter::GetSeasonFromDay(int t)
{
    int month = GetMonthFromDay(t);
    if (month == 11 || month == 0 || month == 1) // WINTER
        return 0;
    else if (month == 2 || month == 3 || month == 4) //SPRING
        return 1;
    else if (month == 5 || month == 6 || month == 7) //SUMMER
        return 2;
    else if (month == 8 || month == 9 || month == 10) //FALL
        return 3;
    
    EXCEPTION1(ImproperUseException, "Date out of range.");
}

// ****************************************************************************
// Method:  avtRPOTFilter::GetIndexFromDay
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

int
avtRPOTFilter::GetIndexFromDay(int t)
{
    if (atts.GetAggregation() == PeaksOverThresholdAttributes::ANNUAL)
        return 0;
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
        return GetMonthFromDay(t);
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::SEASONAL)
        return GetSeasonFromDay(t);
}

// ****************************************************************************
// Method:  avtRPOTFilter::CalculateThreshold
//
// Programmer:  Dave Pugmire
// Creation:    May 22, 2012
//
// ****************************************************************************

float
avtRPOTFilter::CalculateThreshold(int loc, int arr)
{
    int nVals = values[loc][arr].size();
    if (nVals == 0)
        return 0.0f;
    
    vtkDoubleArray *inData = vtkDoubleArray::New();
    inData->SetNumberOfComponents(1);
    inData->SetNumberOfTuples(nVals);
    for (int i = 0; i < nVals; i++)
        inData->SetValue(i, values[loc][arr][i].val);

    vtkRInterface *RI = vtkRInterface::New();
    RI->AssignVTKDataArrayToRVariable(inData, "inData");
    RI->EvalRscript(CreateQuantileCommand("q", "inData", arr).c_str());
    vtkDoubleArray *Q = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("q"));

    float threshold = 0.0f;
    if (Q)
        threshold = (float)Q->GetComponent(0,0);

    inData->Delete();
    RI->Delete();
    return threshold;
}


// ****************************************************************************
// Method:  avtRPOTFilter::ExecutionSuccessful
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

bool
avtRPOTFilter::ExecutionSuccessful()
{
    return true;
}

// ****************************************************************************
// Method:  avtRPOTFilter::CreateQuantileCommand
//
// Programmer:  Dave Pugmire
// Creation:    May 22, 2012
//
// ****************************************************************************

string
avtRPOTFilter::CreateQuantileCommand(const char *var, const char *in, int aggregationIdx)
{
    float percentile = 0.0;
    
    if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
        percentile = atts.GetMonthlyPercentile()[aggregationIdx];
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::SEASONAL)
        percentile = atts.GetSeasonalPercentile()[aggregationIdx];
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::ANNUAL)
        percentile = atts.GetAnnualPercentile();

    char str[128];
    sprintf(str, "%s = quantile(%s, %f)\n", var, in, percentile);
    string cmd = str;
    return cmd;
}


// ****************************************************************************
// Method:  avtRPOTFilter::GetDumpFileName
//
// Programmer:  Dave Pugmire
// Creation:    May 22, 2012
//
// ****************************************************************************

string
avtRPOTFilter::GetDumpFileName(int idx, int yr, int var)
{
    string nm;
    char str[128];
    
    char *varNm;
    if (var == 0)
        varNm = "returnValue";
    else if (var == 1)
        varNm = "se_returnValue";
    else if (var == 2)
        varNm = "paramValues";
    else if (var == 3)
        varNm = "se_paramValues";
      
    nm = "POT";
    if (atts.GetComputeCovariates() && yr != -1)
    {
        if (atts.GetComputeRVDifferences() && yr >= atts.GetCovariateReturnYears().size())
            sprintf(str, ".RVDiff");
        else
            sprintf(str, ".%d", atts.GetCovariateReturnYears()[yr]);
        nm += str;
    }
    
    if (atts.GetAggregation() == PeaksOverThresholdAttributes::ANNUAL)
        sprintf(str, ".annual_%s.txt", varNm);
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
    {
        const char *m[12] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
        sprintf(str, ".%s_%s.txt", m[idx], varNm);
    }
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::SEASONAL)
    {
        const char *s[4] = {"WINTER", "SPRING", "SUMMER", "FALL"};
        sprintf(str, ".%s_%s.txt", s[idx], varNm);
    }
    
    nm += str;
    return nm;
}

// ****************************************************************************
// Method:  avtRPOTFilter::Initialize
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

void
avtRPOTFilter::Initialize()
{
    if (initialized)
        return;

    int nleaves;
    vtkDataSet **leaves = GetInputDataTree()->GetAllLeaves(nleaves);

    if (nleaves != 1)
    {
        EXCEPTION1(ImproperUseException, "Multi-domain not supported yet.");
    }

    vtkDataSet *inDS = leaves[0];
    outDS = (vtkDataSet *)(inDS->NewInstance());
    outDS->CopyStructure(inDS);

    nodeCenteredData = (GetInput()->GetInfo().GetAttributes().GetCentering() == AVT_NODECENT);
    if (nodeCenteredData)
        numTuples = inDS->GetPointData()->GetScalars()->GetNumberOfTuples();
    else
        numTuples = inDS->GetCellData()->GetScalars()->GetNumberOfTuples();
    
    //cout<<"Initialize: numTuples= "<<numTuples<<endl;
    int t0 = GetStartTime();
    int t1 = GetEndTime();
    numTimes = t1-t0 + 1;
    numYears = numTimes/365;
    if (atts.GetDataAnalysisYearRangeEnabled())
    {
        numYears = atts.GetDataAnalysisYearRange()[1]-atts.GetDataAnalysisYearRange()[0]+1;
        numTimes = numYears*365;
    }
    //cout<<"numTimes = "<<numTimes<<" : numYears = "<<numYears<<endl;


    //How to compute maxes.
    //Monthly maxes.
    if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
        numBins = 12;
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::SEASONAL)
        numBins = 4;
    else if (atts.GetAggregation() == PeaksOverThresholdAttributes::ANNUAL)
        numBins = 1;
    
    values.resize(numTuples);
    for (int i = 0; i < numTuples; i++)
        values[i].resize(numBins);

    //cout<<"values:"<<values.size()<<" x "<<values[0].size()<<" x "<<values[0][0].size()<<endl;
    //cout<<"values["<<numTuples<<"]["<<numBins<<"][numTimes]"<<endl;
    
    idx0 = 0;
    idxN = numTuples;
    
#ifdef PARALLEL
    int rank = PAR_Rank();
    int nProcs = PAR_Size();

    int nSamplesPerProc = (numTuples / nProcs);
    int oneExtraUntil = (numTuples % nProcs);
    if (rank < oneExtraUntil)
    {
        idx0 = (rank)*(nSamplesPerProc+1);
        idxN = (rank+1)*(nSamplesPerProc+1);
    }
    else
    {
        idx0 = (rank)*(nSamplesPerProc) + oneExtraUntil;
        idxN = (rank+1)*(nSamplesPerProc) + oneExtraUntil;
    }
    debug1<<"I have: ["<<idx0<<" "<<idxN<<"]"<<endl;
#endif
    
    initialized = true;
    delete [] leaves;
}


// ****************************************************************************
// Method:  avtRPOTFilter::PreExecute
//
// Programmer:  Dave Pugmire
// Creation:    March 16, 2012
//
// ****************************************************************************

void
avtRPOTFilter::PreExecute()
{
    avtDatasetToDatasetFilter::PreExecute();
}

// ****************************************************************************
// Method:  avtRPOTFilter::PostExecute
//
// Programmer:  Dave Pugmire
// Creation:    March 16, 2012
//
// ****************************************************************************

void
avtRPOTFilter::PostExecute()
{
    avtDatasetToDatasetFilter::PostExecute();
}

// ****************************************************************************
// Method:  avtRPOTFilter::Execute
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// ****************************************************************************

void
avtRPOTFilter::Execute()
{
    debug1<<"avtRPOTFilter::Execute() time= "<<currentTime<<endl;

    Initialize();
    
    if (atts.GetDataAnalysisYearRangeEnabled())
    {
        int currYear = atts.GetDataYearBegin()+(currentTime/365);
        if (currYear < atts.GetDataAnalysisYearRange()[0] ||
            currYear > atts.GetDataAnalysisYearRange()[1])
        {
            //cout<<"Skipping "<<currentTime<<" : year= "<<currYear<<endl;
            return;
        }
    }
    
    int nleaves;
    vtkDataSet **leaves = GetInputDataTree()->GetAllLeaves(nleaves);
    vtkDataSet *ds = leaves[0];
    vtkFloatArray *scalars = NULL;

    if (nodeCenteredData)
        scalars = (vtkFloatArray *)ds->GetPointData()->GetScalars();
    else
        scalars = (vtkFloatArray *)ds->GetCellData()->GetScalars();
    float *vals = (float *) scalars->GetVoidPointer(0);

    int nTuples = scalars->GetNumberOfTuples();
    int index = GetIndexFromDay(currentTime);

    //cout<<"processing "<<currentTime<<" sv = "<<scalingVal<<" index= "<<index<<endl;
    float scaling = atts.GetDataScaling(), cutoff = atts.GetCutoff();
    for (int i = 0; i < nTuples; i++)
    {
        float v = vals[i]*scaling;
        if (v > cutoff)
            values[i][index].push_back(sample(v,currentTime));
    }
}

// ****************************************************************************
// Method:  avtRPOTFilter::CreateFinalOutput
//
// Programmer:  Dave Pugmire
// Creation:    February  7, 2012
//
// Modifications:
//
//   Dave Pugmire, Wed Apr 11 17:07:26 EDT 2012
//   Remove the processing of locations in rounds. No longer needed.
//
// ****************************************************************************

void
avtRPOTFilter::CreateFinalOutput()
{
    avtCallback::ResetTimeout(0);

    //Exchange data....
#ifdef PARALLEL
    float *tmp = new float[numTimes];
    float *res = new float[numTimes];
    int *flags = new int[numTimes];
    int *flagsRes = new int[numTimes];
    
    for (int i=0; i<numTuples; i++)
    {
        for (int t=0; t<numTimes; t++)
        {
            tmp[t] = 0.0;//-numeric_limits<float>::min();
            flags[t] = 0;
        }
        for (int b = 0; b < numBins; b++)
        {
            int nt = values[i][b].size();
            for (int t=0; t<nt; t++)
            {
                //if(PAR_Rank()== 0 ) cout<<values[i][b][t].time<<endl;
                tmp[values[i][b][t].time] = values[i][b][t].val;
                flags[values[i][b][t].time] = 1;
            }
        }
        MPI_Allreduce(tmp, res, numTimes, MPI_FLOAT, MPI_SUM, VISIT_MPI_COMM);
        MPI_Allreduce(flags, flagsRes, numTimes, MPI_INT, MPI_SUM, VISIT_MPI_COMM);

        for (int b = 0; b < numBins; b++)
            values[i][b].resize(0);

        //See if it's one of my locations.
        if (i >= idx0 && i < idxN)
        {
            for (int t=0; t<numTimes; t++)
            {
                if (flagsRes[t])
                {
                    int b = GetIndexFromDay(t);
                    values[i][b].push_back(sample(res[t], t));
                }
            }
        }
    }
    
    delete [] tmp;
    delete [] res;
    delete [] flags;
    delete [] flagsRes;
#endif

    vtkRInterface *RI = vtkRInterface::New();
    char fileLoad[1024];
    sprintf(fileLoad,
            "source('%s/auxil.r')\n"   \
            "source('%s/pp.fit2.r')\n" \
            "source('%s/potVisit.r')\n",
            codeDir.c_str(),
            codeDir.c_str(),
            codeDir.c_str());
    
    char potCmd[1024];
            
    vector<float> thresholds(numBins);
    
    int nValsPerOut = 1;
    if (atts.GetComputeCovariates())
    {
        nValsPerOut *= atts.GetCovariateReturnYears().size();
        if (atts.GetComputeRVDifferences())
            nValsPerOut++;
    }
    
    vector<vector<vector<float> > > outputs_rv, outputs_serv;
    outputs_rv.resize(nValsPerOut);
    outputs_serv.resize(nValsPerOut);
    for (int n = 0; n < nValsPerOut; n++)
    {
        outputs_rv[n].resize(numBins);
        outputs_serv[n].resize(numBins);
        for (int b = 0; b < numBins; b++)
        {
            outputs_rv[n][b].resize(numTuples);
            outputs_serv[n][b].resize(numTuples);
        }
    }

    vector<vector<vector<float> > > outputs_mle, outputs_semle;
    if (atts.GetComputeParamValues())
    {
        outputs_mle.resize(4);
        outputs_semle.resize(4);
        for (int n = 0; n < 4; n++)
        {
            outputs_mle[n].resize(numBins);
            outputs_semle[n].resize(numBins);
            for (int b = 0; b < numBins; b++)
            {
                outputs_mle[n][b].resize(numTuples);
                outputs_semle[n][b].resize(numTuples);
            }
        }
    }

    
    for (int i = idx0; i < idxN; i++)
    {
        //DebugData(i, "./dumps/x0");
        for (int j = 0; j < numBins; j++)
            thresholds[j] = CalculateThreshold(i, j);

        int numExceedences = 0;
        for (int b = 0; b < numBins; b++)
        {
            int nt = values[i][b].size();
            for (int t = 0; t < nt; t++)
                if (values[i][b][t].val > thresholds[b])
                    numExceedences++;
        }
        //cout<<"numExceedences: "<<numExceedences<<endl;
        vtkDoubleArray *exceedences = vtkDoubleArray::New();
        vtkIntArray *dayIndices = vtkIntArray::New();
        vtkIntArray *monthIndices = vtkIntArray::New();
        vtkIntArray *yearIndices = vtkIntArray::New();
        exceedences->SetNumberOfComponents(1);
        exceedences->SetNumberOfTuples(numExceedences);
        dayIndices->SetNumberOfComponents(1);
        dayIndices->SetNumberOfTuples(numExceedences);
        monthIndices->SetNumberOfComponents(1);
        monthIndices->SetNumberOfTuples(numExceedences);
        yearIndices->SetNumberOfComponents(1);
        yearIndices->SetNumberOfTuples(numExceedences);
        
        int idx = 0;
        for (int b = 0; b < numBins; b++)
        {
            int nt = values[i][b].size();
            for (int t = 0; t < nt; t++)
                if (values[i][b][t].val > thresholds[b])
                {
                    exceedences->SetValue(idx, values[i][b][t].val);
                    dayIndices->SetValue(idx, values[i][b][t].time+1);
                    monthIndices->SetValue(idx, GetMonthFromDay(values[i][b][t].time)+1);
                    yearIndices->SetValue(idx, GetYearFromDay(values[i][b][t].time)+1);
                 
                    idx++;
                    if (idx == numExceedences)
                        break;
                }
        }
        RI->AssignVTKDataArrayToRVariable(exceedences, "exceedences");
        RI->AssignVTKDataArrayToRVariable(dayIndices, "dayIndices");
        RI->AssignVTKDataArrayToRVariable(monthIndices, "monthIndices");
        string threshStr, aggrStr;
        if (atts.GetAggregation() == PeaksOverThresholdAttributes::ANNUAL)
        {
            aggrStr = "'annual'";
            char str[32];
            sprintf(str, "%f", thresholds[0]);
            threshStr = str;
        }
        else if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
        {
            aggrStr = "'monthly'";
            threshStr = "thresholds";
            vtkDoubleArray *thresh = vtkDoubleArray::New();
            thresh->SetNumberOfComponents(1);
            thresh->SetNumberOfTuples(12);
            for (int t = 0; t < 12; t++)
                thresh->SetValue(t, thresholds[t]);
            RI->AssignVTKDataArrayToRVariable(thresh, "thresholds");
        }
        else if (atts.GetAggregation() == PeaksOverThresholdAttributes::SEASONAL)
        {
            aggrStr = "'seasonal'";
            threshStr = "thresholds";
            vtkDoubleArray *thresh = vtkDoubleArray::New();
            thresh->SetNumberOfComponents(1);
            thresh->SetNumberOfTuples(4);
            for (int t = 0; t < 4; t++)
                thresh->SetValue(t, thresholds[t]);
            RI->AssignVTKDataArrayToRVariable(thresh, "thresholds");
        }
        
        int nCovariates = 0;
        string covarNm = "NULL", covByYear = "NULL", newDataStr = "NULL", rvDiffStr = "NULL";
        string useLocModelStr = "NULL", useShapeModelStr = "NULL", useScaleModelStr = "NULL";
        string returnParamsStr = "FALSE";

        if (atts.GetComputeParamValues())
            returnParamsStr = "TRUE";
        
        if (atts.GetComputeCovariates())
        {
            nCovariates = 1;
            covarNm = "cov";
            covByYear = "covByYear";

            vtkIntArray *covByYearArr = vtkIntArray::New();
            covByYearArr->SetNumberOfComponents(1);
            covByYearArr->SetNumberOfTuples(numYears);

            for (int y = 0; y < numYears; y++)
                covByYearArr->SetValue(y, y+1);

            RI->AssignVTKDataArrayToRVariable(yearIndices, covarNm.c_str());
            RI->AssignVTKDataArrayToRVariable(covByYearArr, covByYear.c_str());
            covByYearArr->Delete();
            
            newDataStr = "newData";
            vtkIntArray *newData = vtkIntArray::New();
            newData->SetNumberOfComponents(1);
            newData->SetNumberOfTuples(atts.GetCovariateReturnYears().size());
            for (int y = 0; y < atts.GetCovariateReturnYears().size(); y++)
            {
                newData->SetValue(y, atts.GetCovariateReturnYears()[y]-atts.GetDataYearBegin() +1);
            }
            RI->AssignVTKDataArrayToRVariable(newData, newDataStr.c_str());
            newData->Delete();

            if (atts.GetComputeRVDifferences())
            {
                rvDiffStr = "rvDiff";
                vtkIntArray *rvDiff = vtkIntArray::New();
                rvDiff->SetNumberOfComponents(1);
                rvDiff->SetNumberOfTuples(2);
                rvDiff->SetValue(0, atts.GetRvDifferences()[0]-atts.GetDataYearBegin() +1);
                rvDiff->SetValue(1, atts.GetRvDifferences()[1]-atts.GetDataYearBegin() +1);
                RI->AssignVTKDataArrayToRVariable(rvDiff, rvDiffStr.c_str());
                rvDiff->Delete();
            }
            if (atts.GetCovariateModelLocation())
                useLocModelStr = "1";
            if (atts.GetCovariateModelShape())
                useShapeModelStr = "1";
            if (atts.GetCovariateModelScale())
                useScaleModelStr = "1";
        }
        string dumpStr = "";
        if (0)
        {
            char tmp[128];
            sprintf(tmp, "save.image(file='tmp_%d.RData')\n", PAR_Rank());
            dumpStr = tmp;
        }
        string outputStr = "rv = output$returnValue\n";
        outputStr += "se_rv = output$se.returnValue\n";
        if (atts.GetComputeCovariates() && atts.GetComputeRVDifferences())
        {
            outputStr += "rvDiff = output$returnValueDiff;\n";
            outputStr += "se_rvDiff = output$se.returnValueDiff;\n";
        }
        if (atts.GetComputeParamValues())
        {
            outputStr += "mle = output$mle;\n";
            outputStr += "se_mle = output$se.mle;\n";
        }

        sprintf(potCmd,
                "require(ismev)\n" \
                "output = potFit(data = exceedences, day = dayIndices, month = monthIndices, "\
                "nYears=%d, threshold=%s, aggregation=%s, " \
                "nCovariates=%d, covariates=%s, covariatesByYear=%s, locationModel=%s, scaleModel=%s, shapeModel=%s,"\
                "newData=%s, rvDifference=%s," \
                "returnParams=%s"\
                ")\n"\
                "%s" \
                "%s",
                numYears, threshStr.c_str(), aggrStr.c_str(),
                nCovariates, covarNm.c_str(), covByYear.c_str(),
                useLocModelStr.c_str(), useShapeModelStr.c_str(), useScaleModelStr.c_str(),
                newDataStr.c_str(), rvDiffStr.c_str(),
                returnParamsStr.c_str(),
                dumpStr.c_str(),
                outputStr.c_str());
        
        string command = fileLoad;
        command += potCmd;
        //cout<<command<<endl;
        RI->EvalRscript(command.c_str());

        vtkDoubleArray *out_rv = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("rv"));
        vtkDoubleArray *out_serv = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("se_rv"));
        //cout<<"valsPerLoc: "<<nValsPerOut<<endl;
        //cout<<"out_rv  : "<<out_rv->GetNumberOfComponents()<<" x "<<out_rv->GetNumberOfTuples()<<endl;
        //cout<<"out_serv: "<<out_serv->GetNumberOfComponents()<<" x "<<out_serv->GetNumberOfTuples()<<endl;

        int N = nValsPerOut;
        if (atts.GetComputeRVDifferences())
            N--;
        
        for (int b = 0; b < numBins; b++)
        {
            for (int n = 0; n < N; n++)
            {
                outputs_rv[n][b][i] = (float)out_rv->GetComponent(b,n);
                outputs_serv[n][b][i] = (float)out_serv->GetComponent(b,n);
            }
        }
        
        vtkDoubleArray *out_rvDiff = NULL, *out_servDiff = NULL;
        if (atts.GetComputeCovariates() && atts.GetComputeRVDifferences())
        {
            out_rvDiff = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("rvDiff"));
            out_servDiff = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("se_rvDiff"));
            //cout<<"out_rvDiff  : "<<out_rvDiff->GetNumberOfComponents()<<" x "<<out_rvDiff->GetNumberOfTuples()<<endl;
            //cout<<"out_servDiff: "<<out_servDiff->GetNumberOfComponents()<<" x "<<out_servDiff->GetNumberOfTuples()<<endl;
            int N = nValsPerOut-1;
            for (int n = nValsPerOut-1; n < nValsPerOut; n++)
            {
                for (int b = 0; b < numBins; b++)
                {
                    outputs_rv[n][b][i] = (float)out_rvDiff->GetComponent(b,0);
                    outputs_serv[n][b][i] = (float)out_servDiff->GetComponent(b,0);
                }
            }
        }
        if (atts.GetComputeParamValues())
        {
            vtkDoubleArray *out_mle, *out_semle;
            out_mle = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("mle"));
            out_semle = vtkDoubleArray::SafeDownCast(RI->AssignRVariableToVTKDataArray("se_mle"));
            for (int n = 0; n < 4; n++)
            {
                for (int b = 0; b < numBins; b++)
                {
                    outputs_mle[n][b][i] = (float)out_mle->GetComponent(b,n);
                    outputs_semle[n][b][i] = (float)out_semle->GetComponent(b,n);
                }
            }
        }
        
        exceedences->Delete();
        dayIndices->Delete();
        monthIndices->Delete();
        yearIndices->Delete();
    }

#if PARALLEL
    float *in0 = new float[numTuples], *in1 = new float[numTuples];
    float *sum0 = new float[numTuples], *sum1 = new float[numTuples];
    int N = outputs_rv.size();
    
    for (int n = 0; n < N; n++)
        in0[n] = in1[n] = sum0[n] = sum1[n] = 0.0;
    for (int n = 0; n < N; n++)
    {
        for (int b = 0; b<numBins; b++)
        {
            for (int i=0; i<numTuples; i++)
            {
                if (i >= idx0 && i < idxN)
                {
                    in0[i] = outputs_rv[n][b][i];
                    in1[i] = outputs_serv[n][b][i];
                }
                else
                    in0[i] = in1[i] = 0.0f;
            }
            SumFloatArray(in0, sum0, numTuples);
            SumFloatArray(in1, sum1, numTuples);
            if (PAR_Rank() == 0)
            {
                for (int i=0; i<numTuples; i++)
                {
                    outputs_rv[n][b][i] = sum0[i];
                    outputs_serv[n][b][i] = sum1[i];
                }
            }
        }
    }
    if (atts.GetComputeParamValues())
    {
        for (int n = 0; n < N; n++)
            in0[n] = in1[n] = sum0[n] = sum1[n] = 0.0;
        N = outputs_mle.size();
        for (int n = 0; n < N; n++)
        {
            for (int b = 0; b<numBins; b++)
            {
                for (int i=0; i<numTuples; i++)
                {
                    if (i >= idx0 && i < idxN)
                    {
                        in0[i] = outputs_mle[n][b][i];
                        in1[i] = outputs_semle[n][b][i];
                    }
                    else
                        in0[i] = in1[i] = 0.0f;
                }
                SumFloatArray(in0, sum0, numTuples);
                SumFloatArray(in1, sum1, numTuples);
                if (PAR_Rank() == 0)
                {
                    for (int i=0; i<numTuples; i++)
                    {
                        outputs_mle[n][b][i] = sum0[i];
                        outputs_semle[n][b][i] = sum1[i];
                    }
                }
            }
        }
    }

    delete [] in0;
    delete [] in1;
    delete [] sum0;
    delete [] sum1;

#endif
    if (PAR_Rank() == 0)
    {
        int displayIdx = 0;
        if (atts.GetAggregation() == PeaksOverThresholdAttributes::ANNUAL)
            displayIdx = 0;
        else if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
            displayIdx = (int)atts.GetDisplayMonth() - (int)(PeaksOverThresholdAttributes::JAN);
        else if (atts.GetAggregation() == PeaksOverThresholdAttributes::MONTHLY)
            displayIdx = (int)atts.GetDisplaySeason() - (int)(PeaksOverThresholdAttributes::WINTER);
        
        //cout<<"Create output.....("<<numOutputComponents<<" x "<<numTuples<<")"<<endl;
        vtkFloatArray *outVar = vtkFloatArray::New();
        outVar->SetNumberOfComponents(1);
        outVar->SetNumberOfTuples(numTuples);
        int idx = 0;
        for (int i = 0; i < numTuples; i++, idx++)
        {
            outVar->SetValue(i, outputs_rv[0][displayIdx][i]);
        }
    
        if (nodeCenteredData)
            outDS->GetPointData()->SetScalars(outVar);
        else
            outDS->GetPointData()->SetScalars(outVar);
        
        outVar->Delete();

        avtDataTree_p outputTree = new avtDataTree(outDS, 0);
        SetOutputDataTree(outputTree);
        outDS->Delete();

        //cout<<"outputs: "<<outputs_rv.size()<<" "<<outputs_rv[0].size()<<" "<<outputs_rv[0][0].size()<<endl;
        if (atts.GetDumpData())
        {
            int N = outputs_rv.size();
            for (int b = 0; b < numBins; b++)
            {
                for (int n = 0; n < N; n++)
                {
                    string nm0 = GetDumpFileName(b, n, 0);
                    string nm1 = GetDumpFileName(b, n, 1);
                    ofstream ofile0(nm0.c_str());
                    ofstream ofile1(nm1.c_str());
                    //cout<<nm0<<": "<<N<<" x "<<numTuples<<endl;
                    //cout<<nm1<<": "<<N<<" x "<<numTuples<<endl;
                    for (int i = 0; i < numTuples; i++)
                    {
                        ofile0<<outputs_rv[n][b][i]<<" ";
                        ofile1<<outputs_serv[n][b][i]<<" ";
                    }
                    ofile0<<endl;
                    ofile1<<endl;
                }
            }
            if (atts.GetComputeParamValues())
            {
                int N = outputs_mle.size();
                for (int b = 0; b < numBins; b++)
                {
                    string nm0 = GetDumpFileName(b, -1, 2);
                    string nm1 = GetDumpFileName(b, -1, 3);
                    ofstream ofile0(nm0.c_str());
                    ofstream ofile1(nm1.c_str());
                    //cout<<nm0<<": "<<N<<" x "<<numTuples<<endl;
                    //cout<<nm1<<": "<<N<<" x "<<numTuples<<endl;
                    for (int i = 0; i < numTuples; i++)
                    {
                        for (int n = 0; n < N; n++)
                        {
                            //ofile0<<outputs_mle[n][b][i]<<" ";
                            //ofile1<<outputs_semle[n][b][i]<<" ";
                            ofile0<<outputs_mle[n][b][i]<<endl;
                            ofile1<<outputs_semle[n][b][i]<<endl;
                        }
                    }
                    ofile0<<endl;
                    ofile1<<endl;
                }
            }
        }
    }
    else
        SetOutputDataTree(new avtDataTree());

    avtCallback::ResetTimeout(5*60);
}


void
avtRPOTFilter::DebugData(int loc, std::string nm)
{
    char filename[128];
    sprintf(filename, "%s_r%02d_%04d.txt", nm.c_str(), PAR_Rank(), loc);
    
    ofstream ofile(filename);
    for (int b = 0; b < numBins; b++)
    {
        ofile<<"["<<b<<"] ("<<values[loc][b].size()<<") : ";
        for (int t = 0; t < values[loc][b].size(); t++)
            ofile<<values[loc][b][t].val<<" ";    
        ofile<<endl;
    }
}
