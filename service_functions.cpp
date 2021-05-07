#include "service_functions.h"

myTIFF::myTIFF(QString name)
{
        //////////////// Для открытия tiff файлов///////////////////////////////////////////////////////
        nameOfClass = name;
        QLibrary lib2("libTIFF64.dll");
        if(lib2.load())
            qDebug() << QString("myTIFF::%1::Библиотека загружена").arg(nameOfClass);
        else
            //qDebug() << QString("myTIFF::%1::Библиотека не загружена").arg(nameOfClass);
            qDebug() << QString("myTIFF::%1::Библиотека не загружена | %2").arg(lib2.errorString(), QDir::currentPath());

        myTIFFOpen = (TIFFOpen) QLibrary::resolve("libTIFF64.dll", "TIFFOpen");
        myTIFFClose = (TIFFClose) QLibrary::resolve("libTIFF64.dll", "TIFFClose");
        myTIFFGetField = (TIFFGetField) QLibrary::resolve("libTIFF64.dll", "TIFFGetField");
        myTIFFReadScanline = (TIFFReadScanline) QLibrary::resolve("libTIFF64.dll", "TIFFReadScanline");
        my_TIFFmalloc = (_TIFFmalloc) QLibrary::resolve("libTIFF64.dll", "_TIFFmalloc");
        myTIFFScanlineSize = (TIFFScanlineSize) QLibrary::resolve("libTIFF64.dll", "TIFFScanlineSize");

        myTIFFWriteScanline = (TIFFWriteScanline) QLibrary::resolve("libTIFF64.dll", "TIFFWriteScanline");
        myTIFFDefaultStripSize = (TIFFDefaultStripSize) QLibrary::resolve("libTIFF64.dll", "TIFFDefaultStripSize");
        myTIFFSetField = (TIFFSetField) QLibrary::resolve("libTIFF64.dll", "TIFFSetField");  
        myTIFFSetupStrips = (TIFFClose) QLibrary::resolve("libTIFF64.dll", "TIFFSetupStrips");

        myTIFFMergeFieldInfo = (TIFFMergeFieldInfo) QLibrary::resolve("libTIFF64.dll", "TIFFMergeFieldInfo");
        myTIFFSetTagExtender = (TIFFSetTagExtender) QLibrary::resolve("libTIFF64.dll", "TIFFSetTagExtender");

        data = new ushort[4000*4000];
}

ushort * myTIFF::openFile(const char * path, uint * width, uint * height)
{
    augment_libtiff_with_custom_tags();
    TIFF * img = myTIFFOpen(path, "r");

    if (img)
    {
        uint32 w, h;

        myTIFFGetField(img, TIFFTAG_IMAGEWIDTH, &w);
        myTIFFGetField(img, TIFFTAG_IMAGELENGTH, &h);

        uint32 imagelength;
        tdata_t buf;
        uint32 row;
        delete[] data;
        data = new ushort[w*h];
        memcpy(width, (uint *)&w, SIZEOF_INT);
        memcpy(height, (uint *)&h, SIZEOF_INT);
        tsize_t size = myTIFFScanlineSize(img);
        buf = my_TIFFmalloc(size);
        myTIFFGetField(img, TIFFTAG_IMAGELENGTH, &imagelength);

        for (uint32 i=0;i<h;i++)
        {
            row = i;
            myTIFFReadScanline(img, buf, row, 0);
            ushort * pix = (ushort*) buf;

            memcpy((data+i*w),pix,size);
        }
        myTIFFClose(img);
        return data;
    }
    else
    {
        qDebug() << path;
        return NULL;
    }
}

void myTIFF::registerCustomTIFFTags(TIFF *tif)
{
    /* Install the extended Tag field info */
    /*int error = */myTIFFMergeFieldInfo(tif, xtiffFieldInfo, N(xtiffFieldInfo));

    if (parent_extender)
        (*parent_extender)(tif);
}

void myTIFF::augment_libtiff_with_custom_tags() {
    static bool first_time = true;
    if (!first_time) return;
    first_time = false;
    myTIFFSetTagExtender(parent_extender);

}

void myTIFF::writeFile(const char * path, ushort * data, uint  width, uint  height)
{
    qDebug() << "path" << path << width << height;
    augment_libtiff_with_custom_tags();
    TIFF *out = myTIFFOpen(path, "w");
    if (out)
    {
        uint32 imagelength, imagewidth;
        imagewidth = width;
        imagelength = height;

        myTIFFSetField(out, TIFFTAG_IMAGELENGTH, imagelength);
        myTIFFSetField(out, TIFFTAG_IMAGEWIDTH,  imagewidth);
        myTIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
        myTIFFSetField(out, TIFFTAG_PHOTOMETRIC, 1);
        myTIFFSetField(out, TIFFTAG_COMPRESSION, 1);
        myTIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 16);
        myTIFFSetField(out, TIFFTAG_ROWSPERSTRIP, myTIFFDefaultStripSize(out, imagelength));

        myTIFFSetupStrips(out);

        for (uint32 i = 0; i < height; i++)
        {
            myTIFFWriteScanline(out, &data[i*width], i, 0);
        }

        myTIFFClose(out);
    }
    else
    {
        qDebug() << QString("myTIFF::%1::writeFile ERROR!").arg(nameOfClass) << path;
    }
}


uint myTIFF::getWidth(const char *path)
{
    TIFF * img = myTIFFOpen(path, "r");
    uint w;
    if (img)
    {
        myTIFFGetField(img, TIFFTAG_IMAGEWIDTH, &w);
    }
    myTIFFClose(img);
    return w;
}

uint myTIFF::getHeight(const char *path)
{
    TIFF * img = myTIFFOpen(path, "r");
    uint h;
    if (img)
    {
        myTIFFGetField(img, TIFFTAG_IMAGELENGTH, &h);
        myTIFFClose(img);
    }
    else
    {
        qDebug() << QString("myTIFF::%1::getWidth ERROR!").arg(nameOfClass) << path;
        return 0;
    }

    return h;
}
