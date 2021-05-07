#ifndef SERVICE_FUNCTIONS_H
#define SERVICE_FUNCTIONS_H

#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QLibrary>
#include <QFileDialog>
#include <QSettings>

#include "tiffio2.h"

#define N(a) (sizeof(a) / sizeof (a[0]))
#define TIFFTAG_INCIDENTPOWER     65000

static const TIFFFieldInfo xtiffFieldInfo[] = {
        { TIFFTAG_INCIDENTPOWER,  1, 1, TIFF_DOUBLE,  FIELD_CUSTOM, 0, 0, const_cast<char*>("LaserIncidentPower") },};

class myTIFF: public QObject

{
    Q_OBJECT

public:
    myTIFF(QString name);

    TIFFExtendProc parent_extender = NULL;  // In case we want a chain of extensions
    void registerCustomTIFFTags(TIFF *tif);
    void augment_libtiff_with_custom_tags();

    ushort * openFile(const char *  path, uint * width, uint * height);

    void writeFile(const char * path, ushort *data, uint  width, uint  height);

    uint getWidth(const char *path);
    uint getHeight(const char *path);

    ushort * data;

    typedef TIFF* (*TIFFOpen)(const char*, const char*);
    TIFFOpen myTIFFOpen;

    typedef int (*TIFFGetField)(TIFF*, ttag_t, ...);
    TIFFGetField myTIFFGetField;

    typedef int (*TIFFReadScanline)(TIFF*, tdata_t, uint32, tsample_t);
    TIFFReadScanline myTIFFReadScanline;

    typedef tdata_t (*_TIFFmalloc)(tsize_t);
    _TIFFmalloc my_TIFFmalloc;

    typedef tsize_t (*TIFFScanlineSize)(TIFF*);
    TIFFScanlineSize myTIFFScanlineSize;

    typedef int (*TIFFWriteScanline)(TIFF*, tdata_t, uint32, tsample_t);
    TIFFWriteScanline myTIFFWriteScanline;

    typedef uint32 (*TIFFDefaultStripSize)(TIFF*, uint32);
    TIFFDefaultStripSize myTIFFDefaultStripSize;

    typedef int (*TIFFSetField)(TIFF*, ttag_t, ...);
    TIFFSetField myTIFFSetField;

    typedef void (*TIFFClose)(TIFF*);
    TIFFClose myTIFFClose;

    typedef void (*TIFFSetupStrips)(TIFF*);
    TIFFSetupStrips myTIFFSetupStrips;

    typedef void (*TIFFMergeFieldInfo)(TIFF*, const TIFFFieldInfo[], int);
    TIFFMergeFieldInfo myTIFFMergeFieldInfo;

    typedef TIFFExtendProc (*TIFFSetTagExtender)(TIFFExtendProc);
    TIFFSetTagExtender myTIFFSetTagExtender;

protected:
    QString nameOfClass;

};

#endif // SERVICE_FUNCTIONS_H
