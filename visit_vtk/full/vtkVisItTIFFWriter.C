/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkVisItTIFFWriter.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include <vtkVisItTIFFWriter.h>

#include <vtkErrorCode.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtk_tiff.h>

vtkCxxRevisionMacro(vtkVisItTIFFWriter, "$Revision: 1.30 $");
vtkStandardNewMacro(vtkVisItTIFFWriter);

//----------------------------------------------------------------------------
vtkVisItTIFFWriter::vtkVisItTIFFWriter() 
{
  this->TIFFPtr = 0;
  this->Compression = vtkVisItTIFFWriter::PackBits;
};


class vtkVisItTIFFWriterIO
{
public:
  // Writing file no reading
  static tsize_t TIFFRead(thandle_t, tdata_t, tsize_t) { return 0; }

  // Write data
  static tsize_t TIFFWrite(thandle_t fd, tdata_t buf, tsize_t size) 
    {
    ofstream *out = reinterpret_cast<ofstream *>(fd);
    out->write(static_cast<char *>(buf), size);
    return out->fail() ? static_cast<tsize_t>(0) : size;
    }

  static toff_t TIFFSeek(thandle_t fd, toff_t off, int whence) 
    {
    ofstream *out = reinterpret_cast<ofstream *>(fd);
    switch (whence) 
      {
      case SEEK_SET:
        out->seekp(off, ios::beg);
        break;
      case SEEK_END:
        out->seekp(off, ios::end);
        break;
      case SEEK_CUR:
        out->seekp(off, ios::cur);
        break;
      default:
        return out->tellp();
      }
    return out->tellp();
    }

  // File will be closed by the superclass
  static int TIFFClose(thandle_t) { return 1; }

  static toff_t TIFFSize(thandle_t fd) 
    {
    ofstream *out = reinterpret_cast<ofstream *>(fd);
    out->seekp(0, ios::end);
    return out->tellp();
    }

  static int TIFFMapFile(thandle_t, tdata_t*, toff_t*) { return (0); }
  static void TIFFUnmapFile(thandle_t, tdata_t, toff_t) {}
};

//----------------------------------------------------------------------------
void vtkVisItTIFFWriter::WriteFileHeader(ofstream *file, vtkImageData *data)
{
  int dims[3];
  int width, height;
  data->GetDimensions(dims);
  int scomponents = data->GetNumberOfScalarComponents();
  int stype = data->GetScalarType();
  double resolution = -1;
  uint32 rowsperstrip = (uint32) -1;

  int min0, min1, max0, max1, min2, max2;


  int bps;
  switch (stype)
    {
  case VTK_CHAR:
  case VTK_SIGNED_CHAR:
  case VTK_UNSIGNED_CHAR:
    bps = 8;
    break;
  case VTK_SHORT:
  case VTK_UNSIGNED_SHORT:
    bps = 16;
    break;
  case VTK_FLOAT:
    bps = 32;
    break;
  default:
    vtkErrorMacro(<< "Unsupported data type: " << data->GetScalarTypeAsString());
    this->SetErrorCode(vtkErrorCode::FileFormatError);
    return;
    }

  int predictor = 0;

  // Find the length of the rows to write.
  data->GetWholeExtent(min0, max0, min1, max1, min2, max2);
  width = (max0 - min0 + 1);
  height = (max1 - min1 + 1);

  TIFF* tif = TIFFClientOpen(this->GetFileName(), "w",
    (thandle_t) file,
    reinterpret_cast<TIFFReadWriteProc>(vtkVisItTIFFWriterIO::TIFFRead), 
    reinterpret_cast<TIFFReadWriteProc>(vtkVisItTIFFWriterIO::TIFFWrite),
    reinterpret_cast<TIFFSeekProc>(vtkVisItTIFFWriterIO::TIFFSeek),
    reinterpret_cast<TIFFCloseProc>(vtkVisItTIFFWriterIO::TIFFClose), 
    reinterpret_cast<TIFFSizeProc>(vtkVisItTIFFWriterIO::TIFFSize),
    reinterpret_cast<TIFFMapFileProc>(vtkVisItTIFFWriterIO::TIFFMapFile), 
    reinterpret_cast<TIFFUnmapFileProc>(vtkVisItTIFFWriterIO::TIFFUnmapFile));

  if ( !tif )
    {
    this->TIFFPtr = 0;
    return;
    }
  this->TIFFPtr = tif;

  uint32 w = width;
  uint32 h = height;
  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
  TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, scomponents);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps); // Fix for stype
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  if (stype == VTK_FLOAT)
    {
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    }

  if ( scomponents > 3 )
    {
    // if number of scalar components is greater than 3, that means we assume
    // there is alpha.
    uint16 extra_samples = scomponents-3;
    uint16 *sample_info = new uint16[scomponents-3];
    sample_info[0]=EXTRASAMPLE_ASSOCALPHA;
    int cc;
    for ( cc = 1; cc < scomponents-3; cc ++ )
      {
      sample_info[cc] = EXTRASAMPLE_UNSPECIFIED;
      }
    TIFFSetField(tif,TIFFTAG_EXTRASAMPLES,extra_samples,
      sample_info);
    delete [] sample_info;
    }

  int compression = COMPRESSION_PACKBITS;
  switch ( this->Compression )
    {
  case vtkVisItTIFFWriter::PackBits: compression = COMPRESSION_PACKBITS; break;
  case vtkVisItTIFFWriter::JPEG:     compression = COMPRESSION_JPEG; break;
  case vtkVisItTIFFWriter::Deflate:  compression = COMPRESSION_DEFLATE; break;
  case vtkVisItTIFFWriter::LZW:      compression = COMPRESSION_LZW; break;
  default: compression = COMPRESSION_NONE;
    }
  //compression = COMPRESSION_JPEG;
  TIFFSetField(tif, TIFFTAG_COMPRESSION, compression); // Fix for compression
  uint16 photometric = 
      (stype == VTK_FLOAT ? PHOTOMETRIC_MINISBLACK : PHOTOMETRIC_RGB);
  if ( compression == COMPRESSION_JPEG )
    {
    TIFFSetField(tif, TIFFTAG_JPEGQUALITY, 75); // Parameter
    TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
    photometric = PHOTOMETRIC_YCBCR;
    }
  else if ( compression == COMPRESSION_LZW )
    {
    predictor = 2;
    TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
    vtkErrorMacro("LZW compression is patented outside US so it is disabled");
    }
  else if ( compression == COMPRESSION_DEFLATE )
    {
    predictor = 2;
    TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
    }

  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric); // Fix for scomponents
  TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP,
    TIFFDefaultStripSize(tif, rowsperstrip));
  if (resolution > 0) 
    {
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, resolution);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, resolution);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    }
}


//----------------------------------------------------------------------------
void vtkVisItTIFFWriter::WriteFile(ofstream *, vtkImageData *data,
  int extent[6])
{
  int idx1, idx2;
  void *ptr;

  // Make sure we actually have data.
  if ( !data->GetPointData()->GetScalars())
    {
    vtkErrorMacro(<< "Could not get data from input.");
    return;
    }

  TIFF* tif = reinterpret_cast<TIFF*>(this->TIFFPtr);
  if ( !tif )
    {
    vtkErrorMacro("Problem writting trailer.");
    this->SetErrorCode(vtkErrorCode::FileFormatError);
    return;
    }

  // take into consideration the scalar type
  if (data->GetScalarType() != VTK_UNSIGNED_CHAR
   && data->GetScalarType() != VTK_UNSIGNED_SHORT
   && data->GetScalarType() != VTK_FLOAT)
    {
    vtkErrorMacro("TIFFWriter only accepts unsigned char/short or float scalars!");
    return; 
    }

  int row = 0;
  for (idx2 = extent[4]; idx2 <= extent[5]; ++idx2)
    {
    for (idx1 = extent[3]; idx1 >= extent[2]; idx1--)
      {
      ptr = data->GetScalarPointer(extent[0], idx1, idx2);
      if ( TIFFWriteScanline(tif, static_cast<unsigned char*>(ptr), row, 0) < 0)
        {
        this->SetErrorCode(vtkErrorCode::OutOfDiskSpaceError);
        break;
        }
      row ++;
      }
    }
}

//----------------------------------------------------------------------------
void vtkVisItTIFFWriter::WriteFileTrailer(ofstream *, vtkImageData *)
{
  TIFF* tif = reinterpret_cast<TIFF*>(this->TIFFPtr);
  if ( !tif )
    {
    vtkErrorMacro("Problem writting trailer.");
    this->SetErrorCode(vtkErrorCode::FileFormatError);
    }
  TIFFClose(tif);
  this->TIFFPtr = 0;
}
//----------------------------------------------------------------------------
void vtkVisItTIFFWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Compression: ";
  if ( this->Compression == vtkVisItTIFFWriter::PackBits )
    {
    os << "Pack Bits\n";
    }
  else if ( this->Compression == vtkVisItTIFFWriter::JPEG )
    {
    os << "JPEG\n";
    }
  else if ( this->Compression == vtkVisItTIFFWriter::Deflate )
    {
    os << "Deflate\n";
    }
  else if ( this->Compression == vtkVisItTIFFWriter::LZW )
    {
    os << "LZW\n";
    }
  else //if ( this->Compression == vtkVisItTIFFWriter::NoCompression )
    {
    os << "No Compression\n";
    }
}
