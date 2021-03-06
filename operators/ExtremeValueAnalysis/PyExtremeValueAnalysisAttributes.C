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

#include <PyExtremeValueAnalysisAttributes.h>
#include <ObserverToCallback.h>
#include <stdio.h>
#include <snprintf.h>

// ****************************************************************************
// Module: PyExtremeValueAnalysisAttributes
//
// Purpose: 
//   Attributes for ExtremeValueAnalysis operator
//
// Note:       Autogenerated by xml2python. Do not modify by hand!
//
// Programmer: xml2python
// Creation:   omitted
//
// ****************************************************************************

//
// This struct contains the Python type information and a ExtremeValueAnalysisAttributes.
//
struct ExtremeValueAnalysisAttributesObject
{
    PyObject_HEAD
    ExtremeValueAnalysisAttributes *data;
    bool        owns;
    PyObject   *parent;
};

//
// Internal prototypes
//
static PyObject *NewExtremeValueAnalysisAttributes(int);

std::string
PyExtremeValueAnalysisAttributes_ToString(const ExtremeValueAnalysisAttributes *atts, const char *prefix)
{
    std::string str; 
    char tmpStr[1000]; 

    const char *aggregation_names = "ANNUAL, MONTHLY, SEASONAL";
    switch (atts->GetAggregation())
    {
      case ExtremeValueAnalysisAttributes::ANNUAL:
          SNPRINTF(tmpStr, 1000, "%saggregation = %sANNUAL  # %s\n", prefix, prefix, aggregation_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::MONTHLY:
          SNPRINTF(tmpStr, 1000, "%saggregation = %sMONTHLY  # %s\n", prefix, prefix, aggregation_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::SEASONAL:
          SNPRINTF(tmpStr, 1000, "%saggregation = %sSEASONAL  # %s\n", prefix, prefix, aggregation_names);
          str += tmpStr;
          break;
      default:
          break;
    }

    const char *displayMonth_names = "January, February, March, April, May, "
        "June, July, August, September, "
        "October, November, December";
    switch (atts->GetDisplayMonth())
    {
      case ExtremeValueAnalysisAttributes::January:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sJanuary  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::February:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sFebruary  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::March:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sMarch  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::April:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sApril  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::May:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sMay  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::June:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sJune  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::July:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sJuly  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::August:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sAugust  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::September:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sSeptember  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::October:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sOctober  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::November:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sNovember  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::December:
          SNPRINTF(tmpStr, 1000, "%sdisplayMonth = %sDecember  # %s\n", prefix, prefix, displayMonth_names);
          str += tmpStr;
          break;
      default:
          break;
    }

    const char *displaySeason_names = "WINTER, SPRING, SUMMER, FALL";
    switch (atts->GetDisplaySeason())
    {
      case ExtremeValueAnalysisAttributes::WINTER:
          SNPRINTF(tmpStr, 1000, "%sdisplaySeason = %sWINTER  # %s\n", prefix, prefix, displaySeason_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::SPRING:
          SNPRINTF(tmpStr, 1000, "%sdisplaySeason = %sSPRING  # %s\n", prefix, prefix, displaySeason_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::SUMMER:
          SNPRINTF(tmpStr, 1000, "%sdisplaySeason = %sSUMMER  # %s\n", prefix, prefix, displaySeason_names);
          str += tmpStr;
          break;
      case ExtremeValueAnalysisAttributes::FALL:
          SNPRINTF(tmpStr, 1000, "%sdisplaySeason = %sFALL  # %s\n", prefix, prefix, displaySeason_names);
          str += tmpStr;
          break;
      default:
          break;
    }

    if(atts->GetDumpData())
        SNPRINTF(tmpStr, 1000, "%sdumpData = 1\n", prefix);
    else
        SNPRINTF(tmpStr, 1000, "%sdumpData = 0\n", prefix);
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%sdataScaling = %g\n", prefix, atts->GetDataScaling());
    str += tmpStr;
    return str;
}

static PyObject *
ExtremeValueAnalysisAttributes_Notify(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;
    obj->data->Notify();
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_SetAggregation(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the aggregation in the object.
    if(ival >= 0 && ival < 3)
        obj->data->SetAggregation(ExtremeValueAnalysisAttributes::AggregationType(ival));
    else
    {
        fprintf(stderr, "An invalid aggregation value was given. "
                        "Valid values are in the range of [0,2]. "
                        "You can also use the following names: "
                        "ANNUAL, MONTHLY, SEASONAL.");
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_GetAggregation(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetAggregation()));
    return retval;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_SetDisplayMonth(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the displayMonth in the object.
    if(ival >= 0 && ival < 12)
        obj->data->SetDisplayMonth(ExtremeValueAnalysisAttributes::MonthType(ival));
    else
    {
        fprintf(stderr, "An invalid displayMonth value was given. "
                        "Valid values are in the range of [0,11]. "
                        "You can also use the following names: "
                        "January, February, March, April, May, "
                        "June, July, August, September, "
                        "October, November, December.");
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_GetDisplayMonth(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetDisplayMonth()));
    return retval;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_SetDisplaySeason(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the displaySeason in the object.
    if(ival >= 0 && ival < 4)
        obj->data->SetDisplaySeason(ExtremeValueAnalysisAttributes::SeasonType(ival));
    else
    {
        fprintf(stderr, "An invalid displaySeason value was given. "
                        "Valid values are in the range of [0,3]. "
                        "You can also use the following names: "
                        "WINTER, SPRING, SUMMER, FALL.");
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_GetDisplaySeason(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetDisplaySeason()));
    return retval;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_SetDumpData(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the dumpData in the object.
    obj->data->SetDumpData(ival != 0);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_GetDumpData(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(obj->data->GetDumpData()?1L:0L);
    return retval;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_SetDataScaling(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the dataScaling in the object.
    obj->data->SetDataScaling(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExtremeValueAnalysisAttributes_GetDataScaling(PyObject *self, PyObject *args)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetDataScaling());
    return retval;
}



PyMethodDef PyExtremeValueAnalysisAttributes_methods[EXTREMEVALUEANALYSISATTRIBUTES_NMETH] = {
    {"Notify", ExtremeValueAnalysisAttributes_Notify, METH_VARARGS},
    {"SetAggregation", ExtremeValueAnalysisAttributes_SetAggregation, METH_VARARGS},
    {"GetAggregation", ExtremeValueAnalysisAttributes_GetAggregation, METH_VARARGS},
    {"SetDisplayMonth", ExtremeValueAnalysisAttributes_SetDisplayMonth, METH_VARARGS},
    {"GetDisplayMonth", ExtremeValueAnalysisAttributes_GetDisplayMonth, METH_VARARGS},
    {"SetDisplaySeason", ExtremeValueAnalysisAttributes_SetDisplaySeason, METH_VARARGS},
    {"GetDisplaySeason", ExtremeValueAnalysisAttributes_GetDisplaySeason, METH_VARARGS},
    {"SetDumpData", ExtremeValueAnalysisAttributes_SetDumpData, METH_VARARGS},
    {"GetDumpData", ExtremeValueAnalysisAttributes_GetDumpData, METH_VARARGS},
    {"SetDataScaling", ExtremeValueAnalysisAttributes_SetDataScaling, METH_VARARGS},
    {"GetDataScaling", ExtremeValueAnalysisAttributes_GetDataScaling, METH_VARARGS},
    {NULL, NULL}
};

//
// Type functions
//

static void
ExtremeValueAnalysisAttributes_dealloc(PyObject *v)
{
   ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)v;
   if(obj->parent != 0)
       Py_DECREF(obj->parent);
   if(obj->owns)
       delete obj->data;
}

static int
ExtremeValueAnalysisAttributes_compare(PyObject *v, PyObject *w)
{
    ExtremeValueAnalysisAttributes *a = ((ExtremeValueAnalysisAttributesObject *)v)->data;
    ExtremeValueAnalysisAttributes *b = ((ExtremeValueAnalysisAttributesObject *)w)->data;
    return (*a == *b) ? 0 : -1;
}

PyObject *
PyExtremeValueAnalysisAttributes_getattr(PyObject *self, char *name)
{
    if(strcmp(name, "aggregation") == 0)
        return ExtremeValueAnalysisAttributes_GetAggregation(self, NULL);
    if(strcmp(name, "ANNUAL") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::ANNUAL));
    if(strcmp(name, "MONTHLY") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::MONTHLY));
    if(strcmp(name, "SEASONAL") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::SEASONAL));

    if(strcmp(name, "displayMonth") == 0)
        return ExtremeValueAnalysisAttributes_GetDisplayMonth(self, NULL);
    if(strcmp(name, "January") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::January));
    if(strcmp(name, "February") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::February));
    if(strcmp(name, "March") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::March));
    if(strcmp(name, "April") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::April));
    if(strcmp(name, "May") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::May));
    if(strcmp(name, "June") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::June));
    if(strcmp(name, "July") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::July));
    if(strcmp(name, "August") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::August));
    if(strcmp(name, "September") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::September));
    if(strcmp(name, "October") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::October));
    if(strcmp(name, "November") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::November));
    if(strcmp(name, "December") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::December));

    if(strcmp(name, "displaySeason") == 0)
        return ExtremeValueAnalysisAttributes_GetDisplaySeason(self, NULL);
    if(strcmp(name, "WINTER") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::WINTER));
    if(strcmp(name, "SPRING") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::SPRING));
    if(strcmp(name, "SUMMER") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::SUMMER));
    if(strcmp(name, "FALL") == 0)
        return PyInt_FromLong(long(ExtremeValueAnalysisAttributes::FALL));

    if(strcmp(name, "dumpData") == 0)
        return ExtremeValueAnalysisAttributes_GetDumpData(self, NULL);
    if(strcmp(name, "dataScaling") == 0)
        return ExtremeValueAnalysisAttributes_GetDataScaling(self, NULL);

    return Py_FindMethod(PyExtremeValueAnalysisAttributes_methods, self, name);
}

int
PyExtremeValueAnalysisAttributes_setattr(PyObject *self, char *name, PyObject *args)
{
    // Create a tuple to contain the arguments since all of the Set
    // functions expect a tuple.
    PyObject *tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, args);
    Py_INCREF(args);
    PyObject *obj = NULL;

    if(strcmp(name, "aggregation") == 0)
        obj = ExtremeValueAnalysisAttributes_SetAggregation(self, tuple);
    else if(strcmp(name, "displayMonth") == 0)
        obj = ExtremeValueAnalysisAttributes_SetDisplayMonth(self, tuple);
    else if(strcmp(name, "displaySeason") == 0)
        obj = ExtremeValueAnalysisAttributes_SetDisplaySeason(self, tuple);
    else if(strcmp(name, "dumpData") == 0)
        obj = ExtremeValueAnalysisAttributes_SetDumpData(self, tuple);
    else if(strcmp(name, "dataScaling") == 0)
        obj = ExtremeValueAnalysisAttributes_SetDataScaling(self, tuple);

    if(obj != NULL)
        Py_DECREF(obj);

    Py_DECREF(tuple);
    if( obj == NULL)
        PyErr_Format(PyExc_RuntimeError, "Unable to set unknown attribute: '%s'", name);
    return (obj != NULL) ? 0 : -1;
}

static int
ExtremeValueAnalysisAttributes_print(PyObject *v, FILE *fp, int flags)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)v;
    fprintf(fp, "%s", PyExtremeValueAnalysisAttributes_ToString(obj->data, "").c_str());
    return 0;
}

PyObject *
ExtremeValueAnalysisAttributes_str(PyObject *v)
{
    ExtremeValueAnalysisAttributesObject *obj = (ExtremeValueAnalysisAttributesObject *)v;
    return PyString_FromString(PyExtremeValueAnalysisAttributes_ToString(obj->data,"").c_str());
}

//
// The doc string for the class.
//
#if PY_MAJOR_VERSION > 2 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION >= 5)
static const char *ExtremeValueAnalysisAttributes_Purpose = "Attributes for ExtremeValueAnalysis operator";
#else
static char *ExtremeValueAnalysisAttributes_Purpose = "Attributes for ExtremeValueAnalysis operator";
#endif

//
// The type description structure
//
static PyTypeObject ExtremeValueAnalysisAttributesType =
{
    //
    // Type header
    //
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                   // ob_size
    "ExtremeValueAnalysisAttributes",                    // tp_name
    sizeof(ExtremeValueAnalysisAttributesObject),        // tp_basicsize
    0,                                   // tp_itemsize
    //
    // Standard methods
    //
    (destructor)ExtremeValueAnalysisAttributes_dealloc,  // tp_dealloc
    (printfunc)ExtremeValueAnalysisAttributes_print,     // tp_print
    (getattrfunc)PyExtremeValueAnalysisAttributes_getattr, // tp_getattr
    (setattrfunc)PyExtremeValueAnalysisAttributes_setattr, // tp_setattr
    (cmpfunc)ExtremeValueAnalysisAttributes_compare,     // tp_compare
    (reprfunc)0,                         // tp_repr
    //
    // Type categories
    //
    0,                                   // tp_as_number
    0,                                   // tp_as_sequence
    0,                                   // tp_as_mapping
    //
    // More methods
    //
    0,                                   // tp_hash
    0,                                   // tp_call
    (reprfunc)ExtremeValueAnalysisAttributes_str,        // tp_str
    0,                                   // tp_getattro
    0,                                   // tp_setattro
    0,                                   // tp_as_buffer
    Py_TPFLAGS_CHECKTYPES,               // tp_flags
    ExtremeValueAnalysisAttributes_Purpose,              // tp_doc
    0,                                   // tp_traverse
    0,                                   // tp_clear
    0,                                   // tp_richcompare
    0                                    // tp_weaklistoffset
};

//
// Helper functions for object allocation.
//

static ExtremeValueAnalysisAttributes *defaultAtts = 0;
static ExtremeValueAnalysisAttributes *currentAtts = 0;

static PyObject *
NewExtremeValueAnalysisAttributes(int useCurrent)
{
    ExtremeValueAnalysisAttributesObject *newObject;
    newObject = PyObject_NEW(ExtremeValueAnalysisAttributesObject, &ExtremeValueAnalysisAttributesType);
    if(newObject == NULL)
        return NULL;
    if(useCurrent && currentAtts != 0)
        newObject->data = new ExtremeValueAnalysisAttributes(*currentAtts);
    else if(defaultAtts != 0)
        newObject->data = new ExtremeValueAnalysisAttributes(*defaultAtts);
    else
        newObject->data = new ExtremeValueAnalysisAttributes;
    newObject->owns = true;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

static PyObject *
WrapExtremeValueAnalysisAttributes(const ExtremeValueAnalysisAttributes *attr)
{
    ExtremeValueAnalysisAttributesObject *newObject;
    newObject = PyObject_NEW(ExtremeValueAnalysisAttributesObject, &ExtremeValueAnalysisAttributesType);
    if(newObject == NULL)
        return NULL;
    newObject->data = (ExtremeValueAnalysisAttributes *)attr;
    newObject->owns = false;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

///////////////////////////////////////////////////////////////////////////////
//
// Interface that is exposed to the VisIt module.
//
///////////////////////////////////////////////////////////////////////////////

PyObject *
ExtremeValueAnalysisAttributes_new(PyObject *self, PyObject *args)
{
    int useCurrent = 0;
    if (!PyArg_ParseTuple(args, "i", &useCurrent))
    {
        if (!PyArg_ParseTuple(args, ""))
            return NULL;
        else
            PyErr_Clear();
    }

    return (PyObject *)NewExtremeValueAnalysisAttributes(useCurrent);
}

//
// Plugin method table. These methods are added to the visitmodule's methods.
//
static PyMethodDef ExtremeValueAnalysisAttributesMethods[] = {
    {"ExtremeValueAnalysisAttributes", ExtremeValueAnalysisAttributes_new, METH_VARARGS},
    {NULL,      NULL}        /* Sentinel */
};

static Observer *ExtremeValueAnalysisAttributesObserver = 0;

std::string
PyExtremeValueAnalysisAttributes_GetLogString()
{
    std::string s("ExtremeValueAnalysisAtts = ExtremeValueAnalysisAttributes()\n");
    if(currentAtts != 0)
        s += PyExtremeValueAnalysisAttributes_ToString(currentAtts, "ExtremeValueAnalysisAtts.");
    return s;
}

static void
PyExtremeValueAnalysisAttributes_CallLogRoutine(Subject *subj, void *data)
{
    ExtremeValueAnalysisAttributes *atts = (ExtremeValueAnalysisAttributes *)subj;
    typedef void (*logCallback)(const std::string &);
    logCallback cb = (logCallback)data;

    if(cb != 0)
    {
        std::string s("ExtremeValueAnalysisAtts = ExtremeValueAnalysisAttributes()\n");
        s += PyExtremeValueAnalysisAttributes_ToString(currentAtts, "ExtremeValueAnalysisAtts.");
        cb(s);
    }
}

void
PyExtremeValueAnalysisAttributes_StartUp(ExtremeValueAnalysisAttributes *subj, void *data)
{
    if(subj == 0)
        return;

    currentAtts = subj;
    PyExtremeValueAnalysisAttributes_SetDefaults(subj);

    //
    // Create the observer that will be notified when the attributes change.
    //
    if(ExtremeValueAnalysisAttributesObserver == 0)
    {
        ExtremeValueAnalysisAttributesObserver = new ObserverToCallback(subj,
            PyExtremeValueAnalysisAttributes_CallLogRoutine, (void *)data);
    }

}

void
PyExtremeValueAnalysisAttributes_CloseDown()
{
    delete defaultAtts;
    defaultAtts = 0;
    delete ExtremeValueAnalysisAttributesObserver;
    ExtremeValueAnalysisAttributesObserver = 0;
}

PyMethodDef *
PyExtremeValueAnalysisAttributes_GetMethodTable(int *nMethods)
{
    *nMethods = 1;
    return ExtremeValueAnalysisAttributesMethods;
}

bool
PyExtremeValueAnalysisAttributes_Check(PyObject *obj)
{
    return (obj->ob_type == &ExtremeValueAnalysisAttributesType);
}

ExtremeValueAnalysisAttributes *
PyExtremeValueAnalysisAttributes_FromPyObject(PyObject *obj)
{
    ExtremeValueAnalysisAttributesObject *obj2 = (ExtremeValueAnalysisAttributesObject *)obj;
    return obj2->data;
}

PyObject *
PyExtremeValueAnalysisAttributes_New()
{
    return NewExtremeValueAnalysisAttributes(0);
}

PyObject *
PyExtremeValueAnalysisAttributes_Wrap(const ExtremeValueAnalysisAttributes *attr)
{
    return WrapExtremeValueAnalysisAttributes(attr);
}

void
PyExtremeValueAnalysisAttributes_SetParent(PyObject *obj, PyObject *parent)
{
    ExtremeValueAnalysisAttributesObject *obj2 = (ExtremeValueAnalysisAttributesObject *)obj;
    obj2->parent = parent;
}

void
PyExtremeValueAnalysisAttributes_SetDefaults(const ExtremeValueAnalysisAttributes *atts)
{
    if(defaultAtts)
        delete defaultAtts;

    defaultAtts = new ExtremeValueAnalysisAttributes(*atts);
}

