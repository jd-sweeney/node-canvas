#include "Backend.h"
#include <string>

Backend::Backend(std::string name, double width, double height)
  : name(name)
  , width(width)
  , height(height)
{}

Backend::~Backend()
{
  this->destroySurface();
}

void Backend::init(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  double width  = 0;
  double height = 0;
  if (info[0]->IsNumber()) width  = Nan::To<double>(info[0]).FromMaybe(0);
  if (info[1]->IsNumber()) height = Nan::To<double>(info[1]).FromMaybe(0);

  Backend *backend = construct(width, height);

  backend->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void Backend::setCanvas(Canvas* _canvas)
{
  this->canvas = _canvas;
}


cairo_surface_t* Backend::recreateSurface()
{
  this->destroySurface();

  return this->createSurface();
}

DLL_PUBLIC cairo_surface_t* Backend::getSurface() {
  if (!surface) createSurface();
  return surface;
}

void Backend::destroySurface()
{
  if(this->surface)
  {
    cairo_surface_destroy(this->surface);
    this->surface = NULL;
  }
}


std::string Backend::getName()
{
  return name;
}

double Backend::getWidth()
{
  return this->width;
}
void Backend::setWidth(double width_)
{
  this->width = width_;
  this->recreateSurface();
}

double Backend::getHeight()
{
  return this->height;
}
void Backend::setHeight(double height_)
{
  this->height = height_;
  this->recreateSurface();
}

bool Backend::isSurfaceValid(){
  bool hadSurface = surface != NULL;
  bool isValid = true;

  cairo_status_t status = cairo_surface_status(getSurface());

  if (status != CAIRO_STATUS_SUCCESS) {
    error = cairo_status_to_string(status);
    isValid = false;
  }

  if (!hadSurface)
    destroySurface();

  return isValid;
}


BackendOperationNotAvailable::BackendOperationNotAvailable(Backend* backend,
  std::string operation_name)
  : backend(backend)
  , operation_name(operation_name)
{};

BackendOperationNotAvailable::~BackendOperationNotAvailable() throw() {};

const char* BackendOperationNotAvailable::what() const throw()
{
  std::string msg = "operation " + this->operation_name +
    " not supported by backend " + backend->getName();

  return msg.c_str();
};
