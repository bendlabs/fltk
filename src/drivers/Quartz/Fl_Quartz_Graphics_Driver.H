//
// Definition of Apple Quartz graphics driver
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2022 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

/**
 \file Fl_Quartz_Graphics_Driver.H
 \brief Definition of Apple Quartz graphics driver.
 */

#ifndef FL_QUARTZ_GRAPHICS_DRIVER_H
#define FL_QUARTZ_GRAPHICS_DRIVER_H

#include <FL/platform.H>
#include <FL/Fl_Graphics_Driver.H>
#include <ApplicationServices/ApplicationServices.h>

// condition for the ATSU API to be available at compile time
#if (    (!defined(__LP64__) || !__LP64__) \
      && MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_11 \
      && MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5 \
    )
#define HAS_ATSU 1
#else
#define HAS_ATSU 0
#endif

struct Fl_Fontdesc;
class Fl_Quartz_Font_Descriptor;

/**
 \brief The Mac OS X-specific graphics class.

 This class is implemented only on the Mac OS X platform.
 */
class Fl_Quartz_Graphics_Driver : public Fl_Graphics_Driver {
  friend class Fl_Cocoa_Printer_Driver;
  friend class Fl_Quartz_Font_Descriptor;
protected:
  CGContextRef gc_;
  bool high_resolution_;
  float quartz_line_width_;
  CGLineCap quartz_line_cap_;
  CGLineJoin quartz_line_join_;
  CGFloat *quartz_line_pattern;
  int quartz_line_pattern_size;
  virtual void cache_size(Fl_Image* img, int &width, int &height);
public:
  Fl_Quartz_Graphics_Driver();
  virtual int has_feature(driver_feature mask) { return mask & NATIVE; }
  virtual void gc(void *ctxt) { gc_ = (CGContextRef)ctxt; global_gc(); }
  virtual void *gc() {return gc_;}
  char can_do_alpha_blending();

  // --- bitmap stuff
  CGImageRef create_bitmask(int w, int h, const uchar *array); // NOT virtual
  virtual void delete_bitmask(fl_uintptr_t bm);
  void draw_pixmap(Fl_Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_bitmap(Fl_Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_rgb(Fl_RGB_Image *img, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
  void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3);
  void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
  void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1);
  void cache(Fl_Pixmap *img);
  void cache(Fl_Bitmap *img);
  void cache(Fl_RGB_Image *img);
  void uncache(Fl_RGB_Image *img, fl_uintptr_t &id_, fl_uintptr_t &mask_);
  void copy_offscreen(int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy);
  void draw_CGImage(CGImageRef cgimg, int x, int y, int w, int h, int srcx, int srcy, int sw, int sh);
  static CGRect fl_cgrectmake_cocoa(int x, int y, int w, int h);
  void add_rectangle_to_region(Fl_Region r, int x, int y, int w, int h);
  Fl_Region XRectangleRegion(int x, int y, int w, int h);
  void XDestroyRegion(Fl_Region r);
  void high_resolution(bool b) { high_resolution_ = b; }
protected:
  void point(int x, int y);
  void rect(int x, int y, int w, int h);
  void focus_rect(int x, int y, int w, int h);
  void rectf(int x, int y, int w, int h);
  void line(int x, int y, int x1, int y1);
  void line(int x, int y, int x1, int y1, int x2, int y2);
  void xyline(int x, int y, int x1);
  void xyline(int x, int y, int x1, int y2);
  void xyline(int x, int y, int x1, int y2, int x3);
  void yxline(int x, int y, int y1);
  void yxline(int x, int y, int y1, int x2);
  void yxline(int x, int y, int y1, int x2, int y3);
  void loop(int x0, int y0, int x1, int y1, int x2, int y2);
  void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
  void polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  // --- clipping
  void push_clip(int x, int y, int w, int h);
  int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
  int not_clipped(int x, int y, int w, int h);
  void restore_clip();
  void end_points();
  void end_line();
  void end_polygon();
  void end_complex_polygon();
  void circle(double x, double y, double r);
  void arc(int x, int y, int w, int h, double a1, double a2);
  void pie(int x, int y, int w, int h, double a1, double a2);
  void line_style(int style, int width=0, char* dashes=0);
  void color(Fl_Color c);
  void set_color(Fl_Color i, unsigned int c);
  Fl_Color color() { return color_; }
  void color(uchar r, uchar g, uchar b);
  void draw(const char *str, int n, int x, int y);
  void draw(const char *str, int n, float x, float y);
  void draw(int angle, const char *str, int n, int x, int y);
  double width(const UniChar* txt, int n);
  void rtl_draw(const char *str, int n, int x, int y);
  void font(Fl_Font face, Fl_Fontsize fsize);
  double width(const char *str, int n);
  double width(unsigned int c);
  int height();
  int descent();
  virtual bool high_resolution() { return high_resolution_; }
  virtual void global_gc();
  void quartz_restore_line_style();
  inline Fl_Quartz_Font_Descriptor *valid_font_descriptor();
  virtual const char* get_font_name(Fl_Font fnum, int* ap);
  virtual int get_font_sizes(Fl_Font fnum, int*& sizep);
  virtual const char *font_name(int num);
  virtual void font_name(int num, const char *name);
  Fl_Fontdesc* calc_fl_fonts(void);

  virtual void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
  virtual Fl_Font set_fonts(const char* xstarname);
  void set_fontname_in_fontdesc(Fl_Fontdesc *f);
  virtual void uncache_pixmap(fl_uintptr_t p);

#if HAS_ATSU && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
  enum {use_CoreText = 1, use_ATSU};
  static int CoreText_or_ATSU;
  static void init_CoreText_or_ATSU();
  void draw_CoreText(const char *str, int n, float x, float y);
  double width_CoreText(const UniChar* txt, int n);
  void descriptor_init_CoreText(const char* name, Fl_Fontsize Size, Fl_Quartz_Font_Descriptor *d);
  void text_extents_CoreText(const char*, int n, int& dx, int& dy, int& w, int& h);
  Fl_Font set_fonts_CoreText(const char* xstarname);

  void draw_ATSU(const char *str, int n, float x, float y);
  double width_ATSU(const UniChar* txt, int n);
  void descriptor_init_ATSU(const char* name, Fl_Fontsize Size, Fl_Quartz_Font_Descriptor *d);
  void text_extents_ATSU(const char*, int n, int& dx, int& dy, int& w, int& h);
  Fl_Font set_fonts_ATSU(const char* xstarname);

  // define 2 kinds of pointers to member function
  typedef void (Fl_Quartz_Graphics_Driver::*pter_to_draw_member)(const char *str, int n, float x, float y);
  typedef double (Fl_Quartz_Graphics_Driver::*pter_to_width_member)(const UniChar *str, int n);
  static pter_to_draw_member CoreText_or_ATSU_draw;
  static pter_to_width_member CoreText_or_ATSU_width;
#else
  void descriptor_init(const char* name, Fl_Fontsize Size, Fl_Quartz_Font_Descriptor *d);
#endif
  virtual void overlay_rect(int x, int y, int w , int h);
  virtual float override_scale();
  virtual void restore_scale(float);
  virtual void antialias(int state);
  virtual int antialias();
};

class Fl_Quartz_Printer_Graphics_Driver : public Fl_Quartz_Graphics_Driver {
public:
  virtual int has_feature(driver_feature mask) { return mask & (NATIVE | PRINTER); }
};

#endif // FL_QUARTZ_GRAPHICS_DRIVER_H
