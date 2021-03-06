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

#include <PyBoxAttributes.h>
#include <ObserverToCallback.h>
#include <stdio.h>
#include <snprintf.h>

// ****************************************************************************
// Module: PyBoxAttributes
//
// Purpose: 
//   This class contains attributes for the box operator.
//
// Note:       Autogenerated by xml2python. Do not modify by hand!
//
// Programmer: xml2python
// Creation:   omitted
//
// ****************************************************************************

//
// This struct contains the Python type information and a BoxAttributes.
//
struct BoxAttributesObject
{
    PyObject_HEAD
    BoxAttributes *data;
    bool        owns;
    PyObject   *parent;
};

//
// Internal prototypes
//
static PyObject *NewBoxAttributes(int);

std::string
PyBoxAttributes_ToString(const BoxAttributes *atts, const char *prefix)
{
    std::string str; 
    char tmpStr[1000]; 

    const char *amount_names = "Some, All";
    switch (atts->GetAmount())
    {
      case BoxAttributes::Some:
          SNPRINTF(tmpStr, 1000, "%samount = %sSome  # %s\n", prefix, prefix, amount_names);
          str += tmpStr;
          break;
      case BoxAttributes::All:
          SNPRINTF(tmpStr, 1000, "%samount = %sAll  # %s\n", prefix, prefix, amount_names);
          str += tmpStr;
          break;
      default:
          break;
    }

    SNPRINTF(tmpStr, 1000, "%sminx = %g\n", prefix, atts->GetMinx());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%smaxx = %g\n", prefix, atts->GetMaxx());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%sminy = %g\n", prefix, atts->GetMiny());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%smaxy = %g\n", prefix, atts->GetMaxy());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%sminz = %g\n", prefix, atts->GetMinz());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%smaxz = %g\n", prefix, atts->GetMaxz());
    str += tmpStr;
    return str;
}

static PyObject *
BoxAttributes_Notify(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    obj->data->Notify();
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_SetAmount(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the amount in the object.
    if(ival >= 0 && ival < 2)
        obj->data->SetAmount(BoxAttributes::Amount(ival));
    else
    {
        fprintf(stderr, "An invalid amount value was given. "
                        "Valid values are in the range of [0,1]. "
                        "You can also use the following names: "
                        "Some, All.");
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetAmount(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetAmount()));
    return retval;
}

/*static*/ PyObject *
BoxAttributes_SetMinx(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the minx in the object.
    obj->data->SetMinx(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetMinx(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetMinx());
    return retval;
}

/*static*/ PyObject *
BoxAttributes_SetMaxx(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the maxx in the object.
    obj->data->SetMaxx(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetMaxx(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetMaxx());
    return retval;
}

/*static*/ PyObject *
BoxAttributes_SetMiny(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the miny in the object.
    obj->data->SetMiny(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetMiny(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetMiny());
    return retval;
}

/*static*/ PyObject *
BoxAttributes_SetMaxy(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the maxy in the object.
    obj->data->SetMaxy(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetMaxy(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetMaxy());
    return retval;
}

/*static*/ PyObject *
BoxAttributes_SetMinz(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the minz in the object.
    obj->data->SetMinz(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetMinz(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetMinz());
    return retval;
}

/*static*/ PyObject *
BoxAttributes_SetMaxz(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the maxz in the object.
    obj->data->SetMaxz(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
BoxAttributes_GetMaxz(PyObject *self, PyObject *args)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetMaxz());
    return retval;
}



PyMethodDef PyBoxAttributes_methods[BOXATTRIBUTES_NMETH] = {
    {"Notify", BoxAttributes_Notify, METH_VARARGS},
    {"SetAmount", BoxAttributes_SetAmount, METH_VARARGS},
    {"GetAmount", BoxAttributes_GetAmount, METH_VARARGS},
    {"SetMinx", BoxAttributes_SetMinx, METH_VARARGS},
    {"GetMinx", BoxAttributes_GetMinx, METH_VARARGS},
    {"SetMaxx", BoxAttributes_SetMaxx, METH_VARARGS},
    {"GetMaxx", BoxAttributes_GetMaxx, METH_VARARGS},
    {"SetMiny", BoxAttributes_SetMiny, METH_VARARGS},
    {"GetMiny", BoxAttributes_GetMiny, METH_VARARGS},
    {"SetMaxy", BoxAttributes_SetMaxy, METH_VARARGS},
    {"GetMaxy", BoxAttributes_GetMaxy, METH_VARARGS},
    {"SetMinz", BoxAttributes_SetMinz, METH_VARARGS},
    {"GetMinz", BoxAttributes_GetMinz, METH_VARARGS},
    {"SetMaxz", BoxAttributes_SetMaxz, METH_VARARGS},
    {"GetMaxz", BoxAttributes_GetMaxz, METH_VARARGS},
    {NULL, NULL}
};

//
// Type functions
//

static void
BoxAttributes_dealloc(PyObject *v)
{
   BoxAttributesObject *obj = (BoxAttributesObject *)v;
   if(obj->parent != 0)
       Py_DECREF(obj->parent);
   if(obj->owns)
       delete obj->data;
}

static int
BoxAttributes_compare(PyObject *v, PyObject *w)
{
    BoxAttributes *a = ((BoxAttributesObject *)v)->data;
    BoxAttributes *b = ((BoxAttributesObject *)w)->data;
    return (*a == *b) ? 0 : -1;
}

PyObject *
PyBoxAttributes_getattr(PyObject *self, char *name)
{
    if(strcmp(name, "amount") == 0)
        return BoxAttributes_GetAmount(self, NULL);
    if(strcmp(name, "Some") == 0)
        return PyInt_FromLong(long(BoxAttributes::Some));
    if(strcmp(name, "All") == 0)
        return PyInt_FromLong(long(BoxAttributes::All));

    if(strcmp(name, "minx") == 0)
        return BoxAttributes_GetMinx(self, NULL);
    if(strcmp(name, "maxx") == 0)
        return BoxAttributes_GetMaxx(self, NULL);
    if(strcmp(name, "miny") == 0)
        return BoxAttributes_GetMiny(self, NULL);
    if(strcmp(name, "maxy") == 0)
        return BoxAttributes_GetMaxy(self, NULL);
    if(strcmp(name, "minz") == 0)
        return BoxAttributes_GetMinz(self, NULL);
    if(strcmp(name, "maxz") == 0)
        return BoxAttributes_GetMaxz(self, NULL);

    return Py_FindMethod(PyBoxAttributes_methods, self, name);
}

int
PyBoxAttributes_setattr(PyObject *self, char *name, PyObject *args)
{
    // Create a tuple to contain the arguments since all of the Set
    // functions expect a tuple.
    PyObject *tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, args);
    Py_INCREF(args);
    PyObject *obj = NULL;

    if(strcmp(name, "amount") == 0)
        obj = BoxAttributes_SetAmount(self, tuple);
    else if(strcmp(name, "minx") == 0)
        obj = BoxAttributes_SetMinx(self, tuple);
    else if(strcmp(name, "maxx") == 0)
        obj = BoxAttributes_SetMaxx(self, tuple);
    else if(strcmp(name, "miny") == 0)
        obj = BoxAttributes_SetMiny(self, tuple);
    else if(strcmp(name, "maxy") == 0)
        obj = BoxAttributes_SetMaxy(self, tuple);
    else if(strcmp(name, "minz") == 0)
        obj = BoxAttributes_SetMinz(self, tuple);
    else if(strcmp(name, "maxz") == 0)
        obj = BoxAttributes_SetMaxz(self, tuple);

    if(obj != NULL)
        Py_DECREF(obj);

    Py_DECREF(tuple);
    if( obj == NULL)
        PyErr_Format(PyExc_RuntimeError, "Unable to set unknown attribute: '%s'", name);
    return (obj != NULL) ? 0 : -1;
}

static int
BoxAttributes_print(PyObject *v, FILE *fp, int flags)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)v;
    fprintf(fp, "%s", PyBoxAttributes_ToString(obj->data, "").c_str());
    return 0;
}

PyObject *
BoxAttributes_str(PyObject *v)
{
    BoxAttributesObject *obj = (BoxAttributesObject *)v;
    return PyString_FromString(PyBoxAttributes_ToString(obj->data,"").c_str());
}

//
// The doc string for the class.
//
#if PY_MAJOR_VERSION > 2 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION >= 5)
static const char *BoxAttributes_Purpose = "This class contains attributes for the box operator.";
#else
static char *BoxAttributes_Purpose = "This class contains attributes for the box operator.";
#endif

//
// The type description structure
//
static PyTypeObject BoxAttributesType =
{
    //
    // Type header
    //
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                   // ob_size
    "BoxAttributes",                    // tp_name
    sizeof(BoxAttributesObject),        // tp_basicsize
    0,                                   // tp_itemsize
    //
    // Standard methods
    //
    (destructor)BoxAttributes_dealloc,  // tp_dealloc
    (printfunc)BoxAttributes_print,     // tp_print
    (getattrfunc)PyBoxAttributes_getattr, // tp_getattr
    (setattrfunc)PyBoxAttributes_setattr, // tp_setattr
    (cmpfunc)BoxAttributes_compare,     // tp_compare
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
    (reprfunc)BoxAttributes_str,        // tp_str
    0,                                   // tp_getattro
    0,                                   // tp_setattro
    0,                                   // tp_as_buffer
    Py_TPFLAGS_CHECKTYPES,               // tp_flags
    BoxAttributes_Purpose,              // tp_doc
    0,                                   // tp_traverse
    0,                                   // tp_clear
    0,                                   // tp_richcompare
    0                                    // tp_weaklistoffset
};

//
// Helper functions for object allocation.
//

static BoxAttributes *defaultAtts = 0;
static BoxAttributes *currentAtts = 0;

static PyObject *
NewBoxAttributes(int useCurrent)
{
    BoxAttributesObject *newObject;
    newObject = PyObject_NEW(BoxAttributesObject, &BoxAttributesType);
    if(newObject == NULL)
        return NULL;
    if(useCurrent && currentAtts != 0)
        newObject->data = new BoxAttributes(*currentAtts);
    else if(defaultAtts != 0)
        newObject->data = new BoxAttributes(*defaultAtts);
    else
        newObject->data = new BoxAttributes;
    newObject->owns = true;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

static PyObject *
WrapBoxAttributes(const BoxAttributes *attr)
{
    BoxAttributesObject *newObject;
    newObject = PyObject_NEW(BoxAttributesObject, &BoxAttributesType);
    if(newObject == NULL)
        return NULL;
    newObject->data = (BoxAttributes *)attr;
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
BoxAttributes_new(PyObject *self, PyObject *args)
{
    int useCurrent = 0;
    if (!PyArg_ParseTuple(args, "i", &useCurrent))
    {
        if (!PyArg_ParseTuple(args, ""))
            return NULL;
        else
            PyErr_Clear();
    }

    return (PyObject *)NewBoxAttributes(useCurrent);
}

//
// Plugin method table. These methods are added to the visitmodule's methods.
//
static PyMethodDef BoxAttributesMethods[] = {
    {"BoxAttributes", BoxAttributes_new, METH_VARARGS},
    {NULL,      NULL}        /* Sentinel */
};

static Observer *BoxAttributesObserver = 0;

std::string
PyBoxAttributes_GetLogString()
{
    std::string s("BoxAtts = BoxAttributes()\n");
    if(currentAtts != 0)
        s += PyBoxAttributes_ToString(currentAtts, "BoxAtts.");
    return s;
}

static void
PyBoxAttributes_CallLogRoutine(Subject *subj, void *data)
{
    BoxAttributes *atts = (BoxAttributes *)subj;
    typedef void (*logCallback)(const std::string &);
    logCallback cb = (logCallback)data;

    if(cb != 0)
    {
        std::string s("BoxAtts = BoxAttributes()\n");
        s += PyBoxAttributes_ToString(currentAtts, "BoxAtts.");
        cb(s);
    }
}

void
PyBoxAttributes_StartUp(BoxAttributes *subj, void *data)
{
    if(subj == 0)
        return;

    currentAtts = subj;
    PyBoxAttributes_SetDefaults(subj);

    //
    // Create the observer that will be notified when the attributes change.
    //
    if(BoxAttributesObserver == 0)
    {
        BoxAttributesObserver = new ObserverToCallback(subj,
            PyBoxAttributes_CallLogRoutine, (void *)data);
    }

}

void
PyBoxAttributes_CloseDown()
{
    delete defaultAtts;
    defaultAtts = 0;
    delete BoxAttributesObserver;
    BoxAttributesObserver = 0;
}

PyMethodDef *
PyBoxAttributes_GetMethodTable(int *nMethods)
{
    *nMethods = 1;
    return BoxAttributesMethods;
}

bool
PyBoxAttributes_Check(PyObject *obj)
{
    return (obj->ob_type == &BoxAttributesType);
}

BoxAttributes *
PyBoxAttributes_FromPyObject(PyObject *obj)
{
    BoxAttributesObject *obj2 = (BoxAttributesObject *)obj;
    return obj2->data;
}

PyObject *
PyBoxAttributes_New()
{
    return NewBoxAttributes(0);
}

PyObject *
PyBoxAttributes_Wrap(const BoxAttributes *attr)
{
    return WrapBoxAttributes(attr);
}

void
PyBoxAttributes_SetParent(PyObject *obj, PyObject *parent)
{
    BoxAttributesObject *obj2 = (BoxAttributesObject *)obj;
    obj2->parent = parent;
}

void
PyBoxAttributes_SetDefaults(const BoxAttributes *atts)
{
    if(defaultAtts)
        delete defaultAtts;

    defaultAtts = new BoxAttributes(*atts);
}

