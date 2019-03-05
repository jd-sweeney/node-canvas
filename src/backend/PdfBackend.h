#pragma once

#include "Backend.h"
#include "../closure.h"
#include <v8.h>

class PdfBackend : public Backend
{
  private:
    cairo_surface_t* createSurface();
    cairo_surface_t* recreateSurface();

  public:
    PdfSvgClosure* _closure = NULL;
    inline PdfSvgClosure* closure() { return _closure; }

    PdfBackend(double width, double height);
    ~PdfBackend();
    static Backend *construct(double width, double height);

    static Nan::Persistent<v8::FunctionTemplate> constructor;
    static void Initialize(v8::Handle<v8::Object> target);
    static NAN_METHOD(New);
};
