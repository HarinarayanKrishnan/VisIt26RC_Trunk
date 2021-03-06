/*****************************************************************************
*
* Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#ifndef PEAKSOVERTHRESHOLDATTRIBUTES_H
#define PEAKSOVERTHRESHOLDATTRIBUTES_H
#include <string>
#include <AttributeSubject.h>


// ****************************************************************************
// Class: PeaksOverThresholdAttributes
//
// Purpose:
//    Attributes for PeaksOverThreshold operator
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class PeaksOverThresholdAttributes : public AttributeSubject
{
public:
    enum AggregationType
    {
        ANNUAL,
        SEASONAL,
        MONTHLY
    };
    enum SeasonType
    {
        WINTER,
        SPRING,
        SUMMER,
        FALL
    };
    enum MonthType
    {
        JAN,
        FEB,
        MAR,
        APR,
        MAY,
        JUN,
        JUL,
        AUG,
        SEP,
        OCT,
        NOV,
        DEC
    };

    // These constructors are for objects of this class
    PeaksOverThresholdAttributes();
    PeaksOverThresholdAttributes(const PeaksOverThresholdAttributes &obj);
protected:
    // These constructors are for objects derived from this class
    PeaksOverThresholdAttributes(private_tmfs_t tmfs);
    PeaksOverThresholdAttributes(const PeaksOverThresholdAttributes &obj, private_tmfs_t tmfs);
public:
    virtual ~PeaksOverThresholdAttributes();

    virtual PeaksOverThresholdAttributes& operator = (const PeaksOverThresholdAttributes &obj);
    virtual bool operator == (const PeaksOverThresholdAttributes &obj) const;
    virtual bool operator != (const PeaksOverThresholdAttributes &obj) const;
private:
    void Init();
    void Copy(const PeaksOverThresholdAttributes &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectDataAnalysisYearRange();
    void SelectSeasonalPercentile();
    void SelectMonthlyPercentile();
    void SelectCovariateReturnYears();
    void SelectRvDifferences();

    // Property setting methods
    void SetDataYearBegin(int dataYearBegin_);
    void SetDataAnalysisYearRangeEnabled(bool dataAnalysisYearRangeEnabled_);
    void SetDataAnalysisYearRange(const int *dataAnalysisYearRange_);
    void SetAggregation(AggregationType aggregation_);
    void SetAnnualPercentile(double annualPercentile_);
    void SetSeasonalPercentile(const double *seasonalPercentile_);
    void SetMonthlyPercentile(const double *monthlyPercentile_);
    void SetDisplaySeason(SeasonType displaySeason_);
    void SetDisplayMonth(MonthType displayMonth_);
    void SetCutoff(float cutoff_);
    void SetComputeParamValues(bool computeParamValues_);
    void SetComputeCovariates(bool computeCovariates_);
    void SetCovariateReturnYears(const intVector &covariateReturnYears_);
    void SetCovariateModelLocation(bool covariateModelLocation_);
    void SetCovariateModelShape(bool covariateModelShape_);
    void SetCovariateModelScale(bool covariateModelScale_);
    void SetComputeRVDifferences(bool computeRVDifferences_);
    void SetRvDifferences(const int *rvDifferences_);
    void SetDataScaling(double dataScaling_);
    void SetDumpData(bool dumpData_);

    // Property getting methods
    int             GetDataYearBegin() const;
    bool            GetDataAnalysisYearRangeEnabled() const;
    const int       *GetDataAnalysisYearRange() const;
          int       *GetDataAnalysisYearRange();
    AggregationType GetAggregation() const;
    double          GetAnnualPercentile() const;
    const double    *GetSeasonalPercentile() const;
          double    *GetSeasonalPercentile();
    const double    *GetMonthlyPercentile() const;
          double    *GetMonthlyPercentile();
    SeasonType      GetDisplaySeason() const;
    MonthType       GetDisplayMonth() const;
    float           GetCutoff() const;
    bool            GetComputeParamValues() const;
    bool            GetComputeCovariates() const;
    const intVector &GetCovariateReturnYears() const;
          intVector &GetCovariateReturnYears();
    bool            GetCovariateModelLocation() const;
    bool            GetCovariateModelShape() const;
    bool            GetCovariateModelScale() const;
    bool            GetComputeRVDifferences() const;
    const int       *GetRvDifferences() const;
          int       *GetRvDifferences();
    double          GetDataScaling() const;
    bool            GetDumpData() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string AggregationType_ToString(AggregationType);
    static bool AggregationType_FromString(const std::string &, AggregationType &);
protected:
    static std::string AggregationType_ToString(int);
public:
    static std::string SeasonType_ToString(SeasonType);
    static bool SeasonType_FromString(const std::string &, SeasonType &);
protected:
    static std::string SeasonType_ToString(int);
public:
    static std::string MonthType_ToString(MonthType);
    static bool MonthType_FromString(const std::string &, MonthType &);
protected:
    static std::string MonthType_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;


    // IDs that can be used to identify fields in case statements
    enum {
        ID_dataYearBegin = 0,
        ID_dataAnalysisYearRangeEnabled,
        ID_dataAnalysisYearRange,
        ID_aggregation,
        ID_annualPercentile,
        ID_seasonalPercentile,
        ID_monthlyPercentile,
        ID_displaySeason,
        ID_displayMonth,
        ID_cutoff,
        ID_computeParamValues,
        ID_computeCovariates,
        ID_covariateReturnYears,
        ID_covariateModelLocation,
        ID_covariateModelShape,
        ID_covariateModelScale,
        ID_computeRVDifferences,
        ID_rvDifferences,
        ID_dataScaling,
        ID_dumpData,
        ID__LAST
    };

private:
    int       dataYearBegin;
    bool      dataAnalysisYearRangeEnabled;
    int       dataAnalysisYearRange[2];
    int       aggregation;
    double    annualPercentile;
    double    seasonalPercentile[4];
    double    monthlyPercentile[12];
    int       displaySeason;
    int       displayMonth;
    float     cutoff;
    bool      computeParamValues;
    bool      computeCovariates;
    intVector covariateReturnYears;
    bool      covariateModelLocation;
    bool      covariateModelShape;
    bool      covariateModelScale;
    bool      computeRVDifferences;
    int       rvDifferences[2];
    double    dataScaling;
    bool      dumpData;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define PEAKSOVERTHRESHOLDATTRIBUTES_TMFS "ibIidDDiifbbi*bbbbIdb"

#endif
