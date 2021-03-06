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

// ****************************************************************************
//  Modifications:
//
//    Hank Childs, Thu Jun  8 16:45:00 PDT 2006
//    Add copyright string.
//
//    Brad Whitlock, Thu May  8 12:05:45 PDT 2008
//    Qt 4. Use QTextStream.
//
//    Tom Fogal, Wed Aug  4 10:50:37 MDT 2010
//    Fix argument parsing.
//
// ****************************************************************************

#include <iterator>
#include <string>
#include <vector>
#include "XMLParser.h"

// Prototypes
void CallGenerator(const QString &docType, Attribute *att, Plugin *plugin, const QString & file);
void ProcessFile(QString file);

// ***************************************************************************
//  Function: Die
//  Purpose: 
//   When a fatal error occurs we need to flush cErr & cOut manually before 
//   calling exit().
//
// ***************************************************************************
void
Die(int err_code = -1)
{
    cOut.flush();
    cErr.flush();
    exit(err_code);
}


// ***************************************************************************
//  Function: PrintUsage
//
//  Modifications:
//    Kathleen Bonnell, Thu Apr 17 09:55:07 PDT 2008
//    Remove -version6 (no longer supported), add -version8.
//
// ***************************************************************************
void
PrintUsage(const char *prog)
{
    cErr << "usage: "<<prog<<" [options] <file.xml>" << Endl;
    cErr << "    options:" << Endl;
    cErr << "        -clobber       overwrite old files if possible" << Endl;
    cErr << "        -noprint       no debug output" << Endl;
    cErr << "        -public        (xml2makefile/xml2cmake only) install publicly" 
         << Endl;
    cErr << "        -private       (xml2makefile/xml2cmake only) install privately" 
         << Endl;
    cErr << "        -outputtoinputdir  store results in same location as "
         << ".xml file" << Endl;
#ifdef EXTRA_PRINTUSAGE
    ExtraPrintUsage();
#endif
}

class ErrorHandler : public QXmlErrorHandler
{
public:
    ErrorHandler() : QXmlErrorHandler(), stream(stderr)
    {
    }
    virtual ~ErrorHandler()
    {
    }

    bool error(const QXmlParseException & exception)
    {
        stream << "Error: " << exception.message() << "\n";
        stream << "Line: "   << exception.lineNumber() << "\n";
        stream << "Column: " << exception.columnNumber() << "\n";
        return true;
    }
    bool warning(const QXmlParseException & exception)
    {
        stream << "Warning: " << exception.message() << "\n";
        stream << "Line: "   << exception.lineNumber() << "\n";
        stream << "Column: " << exception.columnNumber() << "\n";
        return true;
    }
    bool fatalError(const QXmlParseException & exception)
    {
        stream << "Fatal error: " << exception.message() << "\n";
        stream << "Line: "   << exception.lineNumber() << "\n";
        stream << "Column: " << exception.columnNumber() << "\n";
        return true;
    }
    virtual QString errorString() const
    {
        return "No error string defined....";
    }

    QTextStream stream;
};

// ****************************************************************************
//  Function:  Open
//
//  Purpose:
//    Open a file, checking for errors.
//
//  Arguments:
//    file           the ofstream to use for opening the file
//    name_nopath    the filename, without path, of the file to open
//
//  Modifications:
//    Jeremy Meredith, Wed Jun  6 12:37:34 EDT 2007
//    Added support for outputing files to the input directory.
//
//    Brad Whitlock, Wed May  7 16:39:34 PDT 2008
//    Qt 4.
//
//    Jeremy Meredith, Fri Jul 22 13:33:49 EDT 2011
//    Delete the QFile if we never call open on it; trying to use it anyway
//    will cause a segfault.
//
// ****************************************************************************

QFile *
Open(const QString &name_withoutpath)
{
    QString name;
    if (outputtoinputdir)
        name = currentInputDir + name_withoutpath;
    else
        name = name_withoutpath;

    bool alreadyexists = false;
    QFile *file = new QFile(name);
    if (clobber)
        file->open(QIODevice::WriteOnly | QIODevice::Text);
    else
    {
        if (!file->exists())
        {
            file->open(QIODevice::WriteOnly | QIODevice::Text);
        }
        else
        {
            alreadyexists = true;
        }
    }
    if (alreadyexists || !file)
    {
        cErr << "Warning: Could not create file '"
             << name.toStdString().c_str()
             << "' for writing." << Endl;
        if (!clobber)
        {
            cErr << "Info: If you wish to overwrite file '"
                 << name.toStdString().c_str()
                 <<"'," << Endl;
            cErr << "Info: you might want to give the -clobber flag." << Endl;
        }
        if (file)
            delete file;
        file = NULL;
    }
    return file;
}

// ****************************************************************************
//  Function:  FileContentsChecksum 
//
//  Purpose:   Return a checksum of a file's contents given its pathname
//
//  Arguments:
//    name           the pathname
//
//  Programmer:  Mark C. Miller 
//  Creation:    April 9, 2008 
//
//  Modifications:
//    Brad Whitlock, Thu May  8 13:40:53 PDT 2008
//    Qt 4. Use QFile..
//
// ****************************************************************************

bool
FileContentsChecksum(const QString &name, unsigned int *sum)
{
    QFile *file = new QFile(name);

    if (!file->open(QIODevice::ReadOnly))
    {
        delete file;
        return false;
    }

    // compute the checksum
    char buf[80];
    *sum = 0;
    while (!file->atEnd())
    {
        qint64 len = file->read(buf, 80);
        *sum = BJHash::Hash((unsigned char *) buf,
                            (unsigned int)len, *sum);
    }

    file->close();
    delete file;

    return true;
}

// ****************************************************************************
//  Function: CloseHeader
//
//  Purpose:  Close an open header file ensuring it is overwritten ONLY when
//            it has in fact actually changed.
//
//  Arguments:
//    file           the header file stream to close
//    pre_name_withoutpath the (pre) of the file without pathname
//
//  Programmer:  Mark C. Miller 
//  Creation:    April 9, 2008 
//
//  Modifications:
//    Brad Whitlock, Thu May  8 13:48:35 PDT 2008
//    Use QFile, QTextStream.
//
// ****************************************************************************

void
CloseHeader(QTextStream &file, const QString &pre_name_withoutpath)
{
    // close the file
    file.device()->close();
    delete file.device();

    QString pre_name;
    if (outputtoinputdir)
        pre_name = currentInputDir + pre_name_withoutpath;
    else
        pre_name = pre_name_withoutpath;

    // create the real target file name
    QString post_name = pre_name.right(pre_name.length() - preHeaderLeader.length());

    unsigned int pre_cksum;
    FileContentsChecksum(pre_name, &pre_cksum);
    unsigned int post_cksum;
    bool havePostFile = FileContentsChecksum(post_name, &post_cksum);

    if (havePostFile)
    {
        if (post_cksum == pre_cksum)
        {
            // Since the new header file is the same as the old, don't
            // touch the old and remove the new (pre) one.
            QFile(pre_name).remove();
            cOut << "Note: Header file \"" << post_name << "\" did NOT change." << Endl;
        }
        else
        {
            // Since the new headeer file is different from the old,
            // remove the old one and rename the new one.
            QFile(post_name).remove();
            QFile(pre_name).rename(post_name);
            cOut << "Note: Header file \"" << post_name << "\" changed." << Endl;
        }
    }
    else
    {
        QFile(pre_name).rename(post_name);
    }
}

// ****************************************************************************
//  Function:  main
//
//  Purpose:
//    Parse an xml file and generate code if asked.
//
//  Arguments:
//    <file.xml> :     File name
//
//  Programmer:  Jeremy Meredith
//  Creation:    August 28, 2001
//
//  Modifications:
//    Jeremy Meredith, Fri Sep 28 13:25:08 PDT 2001
//    Added Common plugin info.
//
//    Jeremy Meredith, Wed Feb  6 16:53:39 PST 2002
//    Added code so it will not clobber existing files unless the 
//    "-clobber" flag is given, and so it will not print if
//    the "-noprint" option is given.
//
//    Brad Whitlock, Thu Feb 21 16:40:54 PST 2002
//    Added some more top level try/catch code.
//
//    Jeremy Meredith, Fri Apr 19 15:35:59 PDT 2002
//    Pulled printing code to a common place.  Added check to make sure
//    a file exists before trying to parse it.
//
//    Jeremy Meredith, Mon Jul 15 01:22:09 PDT 2002
//    Big enhancements so all our plugin makefile can be build automatically.
//
//    Jeremy Meredith, Fri Jul 19 17:35:32 PDT 2002
//    Fixed an if test that was missing braces.
//
//    Brad Whitlock, Thu Aug 8 17:54:21 PST 2002
//    I added conditionally compiled code to generate Java source code.
//
//    Jeremy Meredith, Tue Aug 27 14:32:40 PDT 2002
//    Added support for database plugins.
//
//    Brad Whitlock, Mon Nov 18 14:07:54 PST 2002
//    I renamed the Parser class to XMLParser to make it build on Windows. I
//    also changed some exception handling so giving non-plugin files to
//    xml2info and xml2avt does not crash.
//
//    Jeremy Meredith, Tue Sep 23 17:02:38 PDT 2003
//    Moved most of the functionality here into a separate function, and
//    allow for multiple XML files on the command line.
//
//    Jeremy Meredith, Wed Nov  5 13:28:03 PST 2003
//    Added avt files for databases.
//
//    Brad Whitlock, Tue Dec 16 11:06:47 PDT 2003
//    Added code to generate Windows project files.
//
//    Jeremy Meredith, Fri Mar 19 15:17:31 PST 2004
//    Added code to clear enums in between input files.
//
//    Jeremy Meredith, Tue Mar 30 10:16:00 PST 2004
//    I added support for database plugins with only a mdserver or engine
//    component.  This was critical for simulation support.
//
//    Brad Whitlock, Mon Aug 16 16:50:29 PST 2004
//    I added the -version7 flag.
//
//    Jeremy Meredith, Wed Aug 11 14:56:02 PDT 2004
//    I made the mdserver/engine only support be specified by the xml file.
//
//    Jeremy Meredith, Wed Aug 25 11:51:33 PDT 2004
//    Moved the engine-only concept into the XML file, not a main argument.
//
//    Jeremy Meredith, Tue Feb  8 08:54:08 PST 2005
//    Added a new file generated by xml2makefile.  This contains a reference
//    to the version of visit used to generate the plugin.  Really it only
//    uses visit-config.h, but xml2makefile ensures it picks up the
//    visit-config.h from the same version as the plugin tools used.
//
//    Brad Whitlock, Thu Mar 3 09:01:29 PDT 2005
//    I removed the code to write a version file since the version is now
//    stored in the common plugin info to make it easier for us on Windows.
//
//    Hank Childs, Tue May 24 09:41:53 PDT 2005
//    Added hasoptions.
//
//    Jeremy Meredith, Wed Jun  6 12:36:30 EDT 2007
//    Added -outputtoinputdir.
//    Check for any nonexistent files before starting the processing.
//
//    Kathleen Bonnell, Thu Apr 17 09:55:07 PDT 2008
//    Remove -version6 (no longer supported), add -version8 (coming soon).
//
//    Brad Whitlock, Thu Jan 29 16:36:08 PST 2009
//    I moved special argument handling into the HandleArgv function.
//
//    Brad Whitlock, Mon Dec 14 12:16:27 PDT 2009
//    I added -dv so we can have it behave differently when installed vs
//    not installed.
//
// ****************************************************************************

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        PrintUsage(argv[0]);
        exit(1);
    }

#ifdef HANDLE_ARGV
    HandleArgv(argc, argv);
#endif

    for (int i=1; i<argc; i++)
    {
        if (strcmp(argv[i], "-noprint") == 0)
        {
            print = false;
            argc--;
            for (int j=i; j<argc; j++)
                argv[j] = argv[j+1];
            i--;
        }
        else if (strcmp(argv[i], "-clobber") == 0)
        {
            clobber = true;
            argc--;
            for (int j=i; j<argc; j++)
                argv[j] = argv[j+1];
            i--;
        }
        else if (strcmp(argv[i], "-public") == 0)
        {
            installpublic = true;
            argc--;
            for (int j=i; j<argc; j++)
                argv[j] = argv[j+1];
            i--;
        }
        else if (strcmp(argv[i], "-private") == 0)
        {
            installprivate = true;
            argc--;
            for (int j=i; j<argc; j++)
                argv[j] = argv[j+1];
            i--;
        }
        else if (strcmp(argv[i], "-outputtoinputdir") == 0)
        {
            outputtoinputdir = true;
            argc--;
            for (int j=i; j<argc; j++)
                argv[j] = argv[j+1];
            i--;
        }
        else if (strcmp(argv[i], "-dv") == 0)
        {
            using_dev = true;
            argc--;
            for (int j=i; j<argc; j++)
                argv[j] = argv[j+1];
            i--;
        }
    }

    if (installpublic && installprivate)
    {
        cErr << "Cannot specify -public and -private at the same time!\n";
        PrintUsage(argv[0]);
        exit(1);
    }

    std::vector<std::string> files;
    std::back_insert_iterator<std::vector<std::string> > fi(files);
    for (int f = 1 ; f < argc ; f++)
    {
        if(QFile(argv[f]).exists())
        {
            *fi = std::string(argv[f]);
        }
        else if(argv[f][0] != '-') // don't warn for arguments.
        {
            cErr << "File '" << argv[1] << "' doesn't exist!\n";
        }
    }

    for(std::vector<std::string>::const_iterator f = files.begin();
        f != files.end(); ++f)
    {
        ProcessFile(QString::fromStdString(*f));
    }
    files.clear();

    return 0;
}

// ****************************************************************************
//
//    Kathleen Bonnell, Thu Jul  5 17:58:22 PDT 2007 
//    For GeneratePython, if plugin, call plugin->WriteHeader instead of
//    attribute->WriteHeader.  There is a flag that needs to be turned off
//    for plugins.  The plugin->WriteHeader will turn off the flag then call
//    the WriteHeader for the atts.
//    
//    Mark C. Miller, Mon Apr 14 15:41:21 PDT 2008
//    Made it re-write header file only when header file has changed
//
//    Kathleen Bonnell, Thu Apr 17 09:55:07 PDT 2008
//    Added code to retrieve the full path for this executable when
//    generating project files on windows. 
//
//    Brad Whitlock, Thu May  8 11:53:18 PDT 2008
//    Qt 4. Use QTextStream.
//
//    Cyrus Harrison, Wed Oct  1 10:34:01 PDT 2008
//    Use "Die" on fatal error.
//
//    Brad Whitlock, Thu Jan 29 16:24:59 PST 2009
//    I moved all of the code into routines called CallGenerator.
//
// ****************************************************************************

void
ProcessFile(QString file)
{
    QString    docType;
    Plugin    *plugin    = NULL;
    Attribute *attribute = NULL;

    EnumType::enums.clear();

    currentInputDir = "";
    int lastslash = file.lastIndexOf("/");
    if (lastslash < 0)
        lastslash = file.lastIndexOf("\\");
    if (lastslash >= 0)
        currentInputDir = file.left(lastslash+1);

    FieldFactory  *fieldFactory = new FieldFactory;
    XMLParser     parser(fieldFactory, file);
    try
    {
        QFile             xmlFile(file);
        QXmlInputSource   source(&xmlFile);
        QXmlSimpleReader  reader;
        ErrorHandler      errorhandler;
        
        reader.setFeature(
           "http://trolltech.com/xml/features/report-whitespace-only-CharData",
           false);
        reader.setContentHandler(&parser);
        reader.setErrorHandler(&errorhandler);
        reader.parse(source);

        docType   = parser.docType;
        plugin    = parser.plugin;

        if (docType == "Attribute")
            attribute = parser.attribute;
        else if(plugin != NULL)
            attribute = plugin->atts;
    }
    catch (const char *s)
    {
        cErr << "ERROR: " << s << Endl;
        Die();
    }
    catch (const QString &s)
    {
        cErr << "ERROR: " << s.toStdString().c_str() << Endl;
        Die();
    }

    if (docType.isNull())
    {
        cErr << "Error in parsing " << file.toStdString().c_str() << Endl;
        Die();
    }

    if (print)
    {
        cOut << "--------------------------------------------------------------"
             << "---" << Endl;
        cOut << "               Parsed document of type " << docType << Endl;
        cOut << "--------------------------------------------------------------"
             << "---" << Endl;
        cOut << Endl;
    }

    // test mode
    try
    {
        if (print)
        {
            for (size_t i=0; i<EnumType::enums.size(); i++)
            {
                EnumType::enums[i]->Print(cOut);
                cOut << Endl;
            }
            if (docType == "Plugin")
            {
                if(plugin != NULL)
                    plugin->Print(cOut);
            }
            else if (docType == "Attribute")
            {
                if(attribute != NULL)
                    attribute->Print(cOut);
            }
            else
            {
                cErr << "Document type "
                     << docType
                     << "not supported" << Endl;
                Die();
            }
            cOut << Endl;
        }

        CallGenerator(docType, attribute, plugin, file);

        cOut.flush();
        cErr.flush();

        delete attribute;
        delete plugin;
    }
    catch (const char *s)
    {
        cErr << "ERROR: " << s << Endl;
        Die();
    }
    catch (const QString &s)
    {
        cErr << "ERROR: " << s << Endl;
        Die();
    }
}
