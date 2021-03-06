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

#include <PyExportDBAttributes.h>
#include <ObserverToCallback.h>
#include <stdio.h>
#include <snprintf.h>
#include <PyDBOptionsAttributes.h>

// ****************************************************************************
// Module: PyExportDBAttributes
//
// Purpose: 
//   The attributes for export a database
//
// Note:       Gened by xml2python, but modified to hide db_type_fullname.
//
// Programmer: xml2python
// Creation:   Fri Aug 8 12:04:04 PDT 2008
// 
// Modifications:  
//   Cyrus Harrison, Mon Oct 13 10:03:31 PDT 2008
//   Public visibility of db_type_fullname causes confusion (it is an internal
//   visit param, not something the user should set) This file was edited to
//   remove it from the attributes string rep and logging, but maintain backwards 
//   compatibility. 
//   
// ****************************************************************************

//
// This struct contains the Python type information and a ExportDBAttributes.
//
struct ExportDBAttributesObject
{
    PyObject_HEAD
    ExportDBAttributes *data;
    bool        owns;
    PyObject   *parent;
};

//
// Internal prototypes
//
static PyObject *NewExportDBAttributes(int);

std::string
PyExportDBAttributes_ToString(const ExportDBAttributes *atts, const char *prefix)
{
    std::string str; 
    char tmpStr[1000]; 

    SNPRINTF(tmpStr, 1000, "%sdb_type = \"%s\"\n", prefix, atts->GetDb_type().c_str());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%sfilename = \"%s\"\n", prefix, atts->GetFilename().c_str());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%sdirname = \"%s\"\n", prefix, atts->GetDirname().c_str());
    str += tmpStr;
    {   const stringVector &variables = atts->GetVariables();
        SNPRINTF(tmpStr, 1000, "%svariables = (", prefix);
        str += tmpStr;
        for(size_t i = 0; i < variables.size(); ++i)
        {
            SNPRINTF(tmpStr, 1000, "\"%s\"", variables[i].c_str());
            str += tmpStr;
            if(i < variables.size() - 1)
            {
                SNPRINTF(tmpStr, 1000, ", ");
                str += tmpStr;
            }
        }
        SNPRINTF(tmpStr, 1000, ")\n");
        str += tmpStr;
    }
    { // new scope
        std::string objPrefix(prefix);
        objPrefix += "opts.";
        str += PyDBOptionsAttributes_ToString(&atts->GetOpts(), objPrefix.c_str());
    }
    return str;
}

static PyObject *
ExportDBAttributes_Notify(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;
    obj->data->Notify();
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_SetDb_type(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;

    char *str;
    if(!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    // Set the db_type in the object.
    obj->data->SetDb_type(std::string(str));

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_GetDb_type(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;
    PyObject *retval = PyString_FromString(obj->data->GetDb_type().c_str());
    return retval;
}

/*static*/ PyObject *
ExportDBAttributes_SetDb_type_fullname(PyObject *self, PyObject *args)
{
    // Do not change the value, but keep for backward compat.
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_GetDb_type_fullname(PyObject *self, PyObject *args)
{
    // Do not access, but keep for backward compat.
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_SetFilename(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;

    char *str;
    if(!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    // Set the filename in the object.
    obj->data->SetFilename(std::string(str));

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_GetFilename(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;
    PyObject *retval = PyString_FromString(obj->data->GetFilename().c_str());
    return retval;
}

/*static*/ PyObject *
ExportDBAttributes_SetDirname(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;

    char *str;
    if(!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    // Set the dirname in the object.
    obj->data->SetDirname(std::string(str));

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_GetDirname(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;
    PyObject *retval = PyString_FromString(obj->data->GetDirname().c_str());
    return retval;
}

/*static*/ PyObject *
ExportDBAttributes_SetVariables(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;

    stringVector  &vec = obj->data->GetVariables();
    PyObject     *tuple;
    if(!PyArg_ParseTuple(args, "O", &tuple))
        return NULL;

    if(PyTuple_Check(tuple))
    {
        vec.resize(PyTuple_Size(tuple));
        for(int i = 0; i < PyTuple_Size(tuple); ++i)
        {
            PyObject *item = PyTuple_GET_ITEM(tuple, i);
            if(PyString_Check(item))
                vec[i] = std::string(PyString_AS_STRING(item));
            else
                vec[i] = std::string("");
        }
    }
    else if(PyString_Check(tuple))
    {
        vec.resize(1);
        vec[0] = std::string(PyString_AS_STRING(tuple));
    }
    else
        return NULL;

    // Mark the variables in the object as modified.
    obj->data->SelectVariables();

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_GetVariables(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;
    // Allocate a tuple the with enough entries to hold the variables.
    const stringVector &variables = obj->data->GetVariables();
    PyObject *retval = PyTuple_New(variables.size());
    for(size_t i = 0; i < variables.size(); ++i)
        PyTuple_SET_ITEM(retval, i, PyString_FromString(variables[i].c_str()));
    return retval;
}

/*static*/ PyObject *
ExportDBAttributes_SetOpts(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;

    PyObject *newValue = NULL;
    if(!PyArg_ParseTuple(args, "O", &newValue))
        return NULL;
    if(!PyDBOptionsAttributes_Check(newValue))
    {
        fprintf(stderr, "The opts field can only be set with DBOptionsAttributes objects.\n");
        return NULL;
    }

    obj->data->SetOpts(*PyDBOptionsAttributes_FromPyObject(newValue));

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ExportDBAttributes_GetOpts(PyObject *self, PyObject *args)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)self;
    // Since the new object will point to data owned by this object,
    // we need to increment the reference count.
    Py_INCREF(self);

    PyObject *retval = PyDBOptionsAttributes_Wrap(&obj->data->GetOpts());
    // Set the object's parent so the reference to the parent can be decref'd
    // when the child goes out of scope.
    PyDBOptionsAttributes_SetParent(retval, self);

    return retval;
}



static struct PyMethodDef ExportDBAttributes_methods[] = {
    {"Notify", ExportDBAttributes_Notify, METH_VARARGS},
    {"SetDb_type", ExportDBAttributes_SetDb_type, METH_VARARGS},
    {"GetDb_type", ExportDBAttributes_GetDb_type, METH_VARARGS},
    {"SetDb_type_fullname", ExportDBAttributes_SetDb_type_fullname, METH_VARARGS},
    {"GetDb_type_fullname", ExportDBAttributes_GetDb_type_fullname, METH_VARARGS},
    {"SetFilename", ExportDBAttributes_SetFilename, METH_VARARGS},
    {"GetFilename", ExportDBAttributes_GetFilename, METH_VARARGS},
    {"SetDirname", ExportDBAttributes_SetDirname, METH_VARARGS},
    {"GetDirname", ExportDBAttributes_GetDirname, METH_VARARGS},
    {"SetVariables", ExportDBAttributes_SetVariables, METH_VARARGS},
    {"GetVariables", ExportDBAttributes_GetVariables, METH_VARARGS},
    {"SetOpts", ExportDBAttributes_SetOpts, METH_VARARGS},
    {"GetOpts", ExportDBAttributes_GetOpts, METH_VARARGS},
    {NULL, NULL}
};

//
// Type functions
//

static void
ExportDBAttributes_dealloc(PyObject *v)
{
   ExportDBAttributesObject *obj = (ExportDBAttributesObject *)v;
   if(obj->parent != 0)
       Py_DECREF(obj->parent);
   if(obj->owns)
       delete obj->data;
}

static int
ExportDBAttributes_compare(PyObject *v, PyObject *w)
{
    ExportDBAttributes *a = ((ExportDBAttributesObject *)v)->data;
    ExportDBAttributes *b = ((ExportDBAttributesObject *)w)->data;
    return (*a == *b) ? 0 : -1;
}

PyObject *
ExportDBAttributes_getattr(PyObject *self, char *name)
{
    if(strcmp(name, "db_type") == 0)
        return ExportDBAttributes_GetDb_type(self, NULL);
    if(strcmp(name, "db_type_fullname") == 0)
        return ExportDBAttributes_GetDb_type_fullname(self, NULL);
    if(strcmp(name, "filename") == 0)
        return ExportDBAttributes_GetFilename(self, NULL);
    if(strcmp(name, "dirname") == 0)
        return ExportDBAttributes_GetDirname(self, NULL);
    if(strcmp(name, "variables") == 0)
        return ExportDBAttributes_GetVariables(self, NULL);
    if(strcmp(name, "opts") == 0)
        return ExportDBAttributes_GetOpts(self, NULL);

    return Py_FindMethod(ExportDBAttributes_methods, self, name);
}

static int
ExportDBAttributes_setattr(PyObject *self, char *name, PyObject *args)
{
    // Create a tuple to contain the arguments since all of the Set
    // functions expect a tuple.
    PyObject *tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, args);
    Py_INCREF(args);
    PyObject *obj = NULL;

    if(strcmp(name, "db_type") == 0)
        obj = ExportDBAttributes_SetDb_type(self, tuple);
    else if(strcmp(name, "db_type_fullname") == 0)
        obj = ExportDBAttributes_SetDb_type_fullname(self, tuple);
    else if(strcmp(name, "filename") == 0)
        obj = ExportDBAttributes_SetFilename(self, tuple);
    else if(strcmp(name, "dirname") == 0)
        obj = ExportDBAttributes_SetDirname(self, tuple);
    else if(strcmp(name, "variables") == 0)
        obj = ExportDBAttributes_SetVariables(self, tuple);
    else if(strcmp(name, "opts") == 0)
        obj = ExportDBAttributes_SetOpts(self, tuple);

    if(obj != NULL)
        Py_DECREF(obj);

    Py_DECREF(tuple);
    return (obj != NULL) ? 0 : -1;
}

static int
ExportDBAttributes_print(PyObject *v, FILE *fp, int flags)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)v;
    fprintf(fp, "%s", PyExportDBAttributes_ToString(obj->data, "").c_str());
    return 0;
}

PyObject *
ExportDBAttributes_str(PyObject *v)
{
    ExportDBAttributesObject *obj = (ExportDBAttributesObject *)v;
    return PyString_FromString(PyExportDBAttributes_ToString(obj->data,"").c_str());
}

//
// The doc string for the class.
//
#if PY_MAJOR_VERSION > 2 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION >= 5)
static const char *ExportDBAttributes_Purpose = "The attributes for export a database";
#else
static char *ExportDBAttributes_Purpose = "The attributes for export a database";
#endif

//
// The type description structure
//
static PyTypeObject ExportDBAttributesType =
{
    //
    // Type header
    //
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                   // ob_size
    "ExportDBAttributes",                    // tp_name
    sizeof(ExportDBAttributesObject),        // tp_basicsize
    0,                                   // tp_itemsize
    //
    // Standard methods
    //
    (destructor)ExportDBAttributes_dealloc,  // tp_dealloc
    (printfunc)ExportDBAttributes_print,     // tp_print
    (getattrfunc)ExportDBAttributes_getattr, // tp_getattr
    (setattrfunc)ExportDBAttributes_setattr, // tp_setattr
    (cmpfunc)ExportDBAttributes_compare,     // tp_compare
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
    (reprfunc)ExportDBAttributes_str,        // tp_str
    0,                                   // tp_getattro
    0,                                   // tp_setattro
    0,                                   // tp_as_buffer
    Py_TPFLAGS_CHECKTYPES,               // tp_flags
    ExportDBAttributes_Purpose,              // tp_doc
    0,                                   // tp_traverse
    0,                                   // tp_clear
    0,                                   // tp_richcompare
    0                                    // tp_weaklistoffset
};

//
// Helper functions for object allocation.
//

static ExportDBAttributes *defaultAtts = 0;
static ExportDBAttributes *currentAtts = 0;

static PyObject *
NewExportDBAttributes(int useCurrent)
{
    ExportDBAttributesObject *newObject;
    newObject = PyObject_NEW(ExportDBAttributesObject, &ExportDBAttributesType);
    if(newObject == NULL)
        return NULL;
    if(useCurrent && currentAtts != 0)
        newObject->data = new ExportDBAttributes(*currentAtts);
    else if(defaultAtts != 0)
        newObject->data = new ExportDBAttributes(*defaultAtts);
    else
        newObject->data = new ExportDBAttributes;
    newObject->owns = true;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

static PyObject *
WrapExportDBAttributes(const ExportDBAttributes *attr)
{
    ExportDBAttributesObject *newObject;
    newObject = PyObject_NEW(ExportDBAttributesObject, &ExportDBAttributesType);
    if(newObject == NULL)
        return NULL;
    newObject->data = (ExportDBAttributes *)attr;
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
ExportDBAttributes_new(PyObject *self, PyObject *args)
{
    int useCurrent = 0;
    if (!PyArg_ParseTuple(args, "i", &useCurrent))
    {
        if (!PyArg_ParseTuple(args, ""))
            return NULL;
        else
            PyErr_Clear();
    }

    return (PyObject *)NewExportDBAttributes(useCurrent);
}

//
// Plugin method table. These methods are added to the visitmodule's methods.
//
static PyMethodDef ExportDBAttributesMethods[] = {
    {"ExportDBAttributes", ExportDBAttributes_new, METH_VARARGS},
    {NULL,      NULL}        /* Sentinel */
};

static Observer *ExportDBAttributesObserver = 0;

std::string
PyExportDBAttributes_GetLogString()
{
    std::string s("ExportDBAtts = ExportDBAttributes()\n");
    if(currentAtts != 0)
        s += PyExportDBAttributes_ToString(currentAtts, "ExportDBAtts.");
    return s;
}

static void
PyExportDBAttributes_CallLogRoutine(Subject *subj, void *data)
{
    ExportDBAttributes *atts = (ExportDBAttributes *)subj;
    typedef void (*logCallback)(const std::string &);
    logCallback cb = (logCallback)data;

    if(cb != 0)
    {
        std::string s("ExportDBAtts = ExportDBAttributes()\n");
        s += PyExportDBAttributes_ToString(currentAtts, "ExportDBAtts.");
        cb(s);
    }
}

void
PyExportDBAttributes_StartUp(ExportDBAttributes *subj, void *data)
{
    if(subj == 0)
        return;

    currentAtts = subj;
    PyExportDBAttributes_SetDefaults(subj);

    //
    // Create the observer that will be notified when the attributes change.
    //
    if(ExportDBAttributesObserver == 0)
    {
        ExportDBAttributesObserver = new ObserverToCallback(subj,
            PyExportDBAttributes_CallLogRoutine, (void *)data);
    }

}

void
PyExportDBAttributes_CloseDown()
{
    delete defaultAtts;
    defaultAtts = 0;
    delete ExportDBAttributesObserver;
    ExportDBAttributesObserver = 0;
}

PyMethodDef *
PyExportDBAttributes_GetMethodTable(int *nMethods)
{
    *nMethods = 1;
    return ExportDBAttributesMethods;
}

bool
PyExportDBAttributes_Check(PyObject *obj)
{
    return (obj->ob_type == &ExportDBAttributesType);
}

ExportDBAttributes *
PyExportDBAttributes_FromPyObject(PyObject *obj)
{
    ExportDBAttributesObject *obj2 = (ExportDBAttributesObject *)obj;
    return obj2->data;
}

PyObject *
PyExportDBAttributes_New()
{
    return NewExportDBAttributes(0);
}

PyObject *
PyExportDBAttributes_Wrap(const ExportDBAttributes *attr)
{
    return WrapExportDBAttributes(attr);
}

void
PyExportDBAttributes_SetParent(PyObject *obj, PyObject *parent)
{
    ExportDBAttributesObject *obj2 = (ExportDBAttributesObject *)obj;
    obj2->parent = parent;
}

void
PyExportDBAttributes_SetDefaults(const ExportDBAttributes *atts)
{
    if(defaultAtts)
        delete defaultAtts;

    defaultAtts = new ExportDBAttributes(*atts);
}

